// Dear ImGui: standalone example application for Win32 + OpenGL 3

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

// This is provided for completeness, however it is strongly recommended you use OpenGL with SDL or GLFW.

#include <iostream>
#include <Windows.h>
#include <windowsx.h>

#include "glad/glad/glad.h"
#include "glm/glm.hpp"

#include "imgui/imgui_internal.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

#include <windows.h>
#include <tchar.h>


#define APP_TITLE L"泰拉瑞亚，除数为0"

// Data stored per platform window
struct WGL_WindowData { HDC hDC; };

// Data
static HGLRC            g_hRC;
static WGL_WindowData   g_MainWindow;
static int              g_Width;
static int              g_Height;

// Forward declarations of helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data);
void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data);
void ResetDeviceWGL();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Support function for multi-viewports
// Unlike most other backend combination, we need specific hooks to combine Win32+OpenGL.
// We could in theory decide to support Win32-specific code in OpenGL backend via e.g. an hypothetical ImGui_ImplOpenGL3_InitForRawWin32().
static void Hook_Renderer_CreateWindow(ImGuiViewport* viewport)
{
    assert(viewport->RendererUserData == NULL);

    WGL_WindowData* data = IM_NEW(WGL_WindowData);
    CreateDeviceWGL((HWND)viewport->PlatformHandle, data);
    viewport->RendererUserData = data;
}

static void Hook_Renderer_DestroyWindow(ImGuiViewport* viewport)
{
    if (viewport->RendererUserData != NULL)
    {
        WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData;
        CleanupDeviceWGL((HWND)viewport->PlatformHandle, data);
        IM_DELETE(data);
        viewport->RendererUserData = NULL;
    }
}

static void Hook_Platform_RenderWindow(ImGuiViewport* viewport, void*)
{
    // Activate the platform window DC in the OpenGL rendering context
    if (WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData)
        wglMakeCurrent(data->hDC, g_hRC);
}

static void Hook_Renderer_SwapBuffers(ImGuiViewport* viewport, void*)
{
    if (WGL_WindowData* data = (WGL_WindowData*)viewport->RendererUserData)
        ::SwapBuffers(data->hDC);
}


static glm::vec3 GetIntersectWithLineAndPlane(glm::vec3 point, glm::vec3 direct, glm::vec3 planeNormal, glm::vec3 planePoint)
{
    float d = glm::dot(planePoint - point, planeNormal) / glm::dot(direct, planeNormal);
    return d * direct + point;
}

bool isPointInsideTriangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P) {
    // Calculate vectors
    glm::vec3 AB = B - A;
    glm::vec3 BC = C - B;
    glm::vec3 CA = A - C;

    glm::vec3 AP = P - A;
    glm::vec3 BP = P - B;
    glm::vec3 CP = P - C;

    // Calculate cross products
    glm::vec3 cross_AB_AP = glm::cross(AB, AP);
    glm::vec3 cross_BC_BP = glm::cross(BC, BP);
    glm::vec3 cross_CA_CP = glm::cross(CA, CP);

    // Check if the vectors have the same direction
    if (glm::dot(cross_AB_AP, cross_BC_BP) >= 0 &&
        glm::dot(cross_BC_BP, cross_CA_CP) >= 0 &&
        glm::dot(cross_CA_CP, cross_AB_AP) >= 0) {
        return true;
    }
    else {
        return false;
    }
}

bool wmouse_in_quad(glm::vec3 mouse_pos, glm::vec3 camera_pos, float* quad)
{
    glm::vec3 dir = glm::normalize(mouse_pos - camera_pos);

    bool intri1 = false;
    bool intri2 = false;
    {
        glm::vec4 p1(quad[0], quad[1], quad[2], 1.0f);
        glm::vec4 p2(quad[3], quad[4], quad[5], 1.0f);
        glm::vec4 p3(quad[6], quad[7], quad[8], 1.0f);

        glm::vec3 d1 = p1 - p2;
        glm::vec3 d2 = p3 - p2;
        glm::vec3 n = glm::cross(d1, d2);
        n = glm::normalize(n);

        glm::vec3 a = GetIntersectWithLineAndPlane(camera_pos, dir, n, p1);

        intri1 = isPointInsideTriangle(p1, p2, p3, a);
    }
    {
        glm::vec4 p1(quad[6], quad[7], quad[8], 1.0f);
        glm::vec4 p2(quad[9], quad[10], quad[11], 1.0f);
        glm::vec4 p3(quad[0], quad[1], quad[2], 1.0f);

        glm::vec3 d1 = p1 - p2;
        glm::vec3 d2 = p3 - p2;
        glm::vec3 n = glm::cross(d1, d2);
        n = glm::normalize(n);

        glm::vec3 a = GetIntersectWithLineAndPlane(camera_pos, dir, n, p1);

        intri2 = isPointInsideTriangle(p1, p2, p3, a);
    }

    return intri1 || intri2;
}


HWND hwnd;

#include "model.h"

#include "learnopengl/shader_m.h"
#include "common/learnopengl/camera.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 500;
std::unique_ptr<Shader> shader;
// camera
Camera camera(glm::vec3(0, 0, 3.0f));
glm::vec2 mouse_pos(0.0f, 0.0f);
unsigned int VBO, cubeVAO;

ImRect viewport;


GLuint framebuffer;
GLuint texColorBuffer;
int kbv_width = 400;
int kbv_height = 300;

int kbv_x = 0;
int kbv_y = 0;

void init_scene()
{
    // build and compile shaders
    // -------------------------
    shader = std::make_unique<Shader>("3.2.blending.vs", "3.2.blending.fs");

    RenderModelInit();

    std::vector<float> vertices = RenderModelUpdate();

    // first, configure the cube's VAO (and VBO)
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STREAM_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // color attribute
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    // create framebuffer
    framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    texColorBuffer;
    glGenTextures(1, &texColorBuffer);
    glBindTexture(GL_TEXTURE_2D, texColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, kbv_width, kbv_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Attach it to currently bound framebuffer object
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, kbv_width, kbv_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}




void render_scene()
{
    // render
    // ------

    // draw objects
    shader->use();
    //glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)kbv_width / (float)kbv_height, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    //model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //model = glm::scale(model, glm::vec3(1.0f, -1.0f, 1.0f)); // a smaller cube
    shader->setMat4("model", model);
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    glm::mat4 modelViewMatrix = view * model;

    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    glm::vec3 mousePos_NDC(mouse_pos.x, kbv_height - mouse_pos.y, 0.1f);
    glm::vec3 mousePos_3DC = glm::unProject(
        mousePos_NDC,
        modelViewMatrix,
        projection,
        glm::vec4(viewport[0], viewport[1], viewport[2], viewport[3])
    );

    for (int i = 0; i < keyBtnViewSize; ++i)
    {
        if (wmouse_in_quad(mousePos_3DC, camera.Position, keyBtnViewList[i].face_vertex))
        {
            keyBtnViewList[i].hover = true;
        }
        else
        {
            keyBtnViewList[i].hover = false;
        }
    }

    // render the cube
    std::vector<float> vertices = RenderModelUpdate();
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), vertices.data());

    glBindVertexArray(cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 10);

}


#ifndef IMGUI_DEFINE_MATH_OPERATORS
static ImVec2 operator+(const ImVec2& a, const ImVec2& b) {
    return ImVec2(a.x + b.x, a.y + b.y);
}
static ImVec2 operator-(const ImVec2& a, const ImVec2& b) {
    return ImVec2(a.x - b.x, a.y - b.y);
}
static ImVec2 operator*(const ImVec2& a, const ImVec2& b) {
    return ImVec2(a.x * b.x, a.y * b.y);
}
static ImVec2 operator+(const ImVec2& a, const float& b) {
    return ImVec2(a.x + b, a.y + b);
}
static ImVec2 operator-(const ImVec2& a, const float& b) {
    return ImVec2(a.x - b, a.y - b);
}
static ImVec2 operator*(const ImVec2& a, const float& b) {
    return ImVec2(a.x * b, a.y * b);
}
#endif


#include <vector>
#include <libxml/parser.h>

std::vector<std::string> getSvgData(const char* filename)
{
    std::vector<std::string> result;

    xmlDocPtr doc;
    xmlNodePtr cur;

    doc = xmlParseFile(filename);

    if (doc == NULL) {
        fprintf(stderr, "Document not parsed successfully. \n");
        return result;
    }
    cur = xmlDocGetRootElement(doc);
    if (cur == NULL) {
        fprintf(stderr, "empty document\n");
        xmlFreeDoc(doc);
        return result;
    }
    if (xmlStrcmp(cur->name, (const xmlChar*)"svg")) {
        fprintf(stderr, "document of the wrong type, root node != story");
        xmlFreeDoc(doc);
        return result;
    }

    for (auto propertie = cur->properties; propertie != NULL; propertie = propertie->next)
    {
        if (xmlStrcmp(propertie->name, (const xmlChar*)"viewBox") == 0)
        {
            xmlChar* attr_value = xmlGetProp(cur, propertie->name);
            result.push_back((char*)attr_value);
        }
    }

    for (auto node = cur->children; node != NULL; node = node->next)
    {
        if (xmlStrcmp(node->name, (const xmlChar*)"path") == 0)
        {
            for (auto propertie = node->properties; propertie != NULL; propertie = propertie->next)
            {
                if (xmlStrcmp(propertie->name, (const xmlChar*)"d") == 0)
                {
                    xmlChar* attr_value = xmlGetProp(node, propertie->name);
                    result.push_back((char*)attr_value);
                }
            }
        }
    }


    xmlFreeDoc(doc);
    return result;
}


struct SVGPathCmd
{
    char cmd;
    float argv[6];
    int argc;
};

typedef std::vector<SVGPathCmd> SVGPathData;

struct SVGData
{
    ImRect bb;
    std::vector<SVGPathData> paths;
};

struct KLVectorGraphTriangle
{
    ImVec2 p1;
    ImVec2 p2;
    ImVec2 p3;
};

struct KLVectorGraph
{
    ImRect bb;
    std::vector<KLVectorGraphTriangle> trangles;
};

void parseSvgPathArgs(char cmd, char* args, SVGPathData& output)
{
    char buf[16] = { 0 };
    char* p = args;
    int i = 0;
    int max = 0;

    SVGPathCmd r;
    r.cmd = cmd;
    r.argc = 0;

    switch (cmd)
    {
    case 'M':
    case 'm':
    case 'L':
    case 'l':
        max = 2;
        break;
    case 'V':
    case 'v':
    case 'H':
    case 'h':
        max = 1;
        break;
    case 'C':
    case 'c':
        max = 6;
        break;
    case 'S':
    case 's':
    case 'Q':
    case 'q':
        max = 4;
        break;
    case 'T':
    case 't':
        max = 2;
        break;
    case 'A':
    case 'a':
        max = 7;
        break;
    case 'Z':
    case 'z':
        max = 0;
        break;
    }

    while (true)
    {

        if (*p == '\0' && i > 0)
        {
            buf[i] = '\0';
            i = 0;
            r.argv[r.argc++] = (float)atof(buf);
        }

        if (r.argc >= max)
        {
            output.push_back(r);
            r.argc = 0;

            if (cmd == 'Z' || cmd == 'z')
                break;
        }

        if (*p == '\0')
        {
            break;
        }

        if (((*p == '-') || (*p == ' ')) && i > 0)
        {
            buf[i] = '\0';
            i = 0;
            r.argv[r.argc++] = (float)atof(buf);
        }

        buf[i++] = *p;
        p++;
    }
}

ImRect parseSvgViewBox(char* viewBox)
{
    char* p = viewBox;
    int a = 0;
    int i = 0;
    float v[4] = { 0.0f };
    int j = 0;
    while (true)
    {
        if ((p[i] == ' ') || (p[i] == '\0'))
        {
            v[j++] = (float)atof(p + a);
            a = i + 1;
        }

        if (p[i] == '\0')
        {
            break;
        }
        i++;
    }
    
    return ImRect(ImVec2(v[0], v[1]), ImVec2(v[2], v[3]));
}

SVGPathData parseSvgPathData(char* data)
{
    SVGPathData output;
    char* pData = data;
    char* p = pData;

    assert(pData[0] == 'M');

    int i = 0;
    int a = -1;

    char cmd = 'Z';

    while (true)
    {
        if (*p == '\0')
        {
            printf("%c %s\n", cmd, pData + a);
            parseSvgPathArgs(cmd, pData + a, output);
            break;
        }

        switch (*p)
        {
        case 'M':
        case 'm':
        case 'L':
        case 'l':
        case 'V':
        case 'v':
        case 'H':
        case 'h':
        case 'C':
        case 'c':
        case 'S':
        case 's':
        case 'Q':
        case 'q':
        case 'T':
        case 't':
        case 'A':
        case 'a':
        case 'Z':
        case 'z':
            if (a == -1)
            {
                a = i + 1;

                cmd = *p;
            }
            else
            {
                char tmp = *p;

                *p = '\0';
                printf("%c %s\n", cmd, pData + a);
                parseSvgPathArgs(cmd, pData + a, output);

                cmd = tmp;
                
                a = i + 1;
            }
            break;
        default:
            assert(*p == ' ' || *p == '-' || *p == '.' || ('0' <= *p && *p <= '9'));
            break;
        }

        i++;
        p++;
    }
    return output;
}

SVGData parseSvgData(const char* filename)
{
    SVGData output;

    std::vector<std::string> data = getSvgData(filename);

    output.bb = parseSvgViewBox((char *)data[0].c_str());

    for (uint32_t i = 1; i < data.size(); ++i)
    {
        output.paths.push_back(parseSvgPathData((char*)data[i].c_str()));
    }
    return output;
}

#include "CDT.h"

KLVectorGraph ConvertSvgData(SVGData svgData)
{
    auto drawList = new ImDrawList(nullptr);
    
    ImVec2 pos;
    ImVec2 c1;
    ImVec2 c2;
    ImVec2 c3;

    int num_segments = 6;

    std::vector<CDT::V2d<float>> vertices;
    CDT::EdgeVec edges;//12 18 30 36 48 54

    for (uint32_t i = 0; i < svgData.paths.size(); ++i)
    {
        auto& path = svgData.paths[i];

        for (uint32_t j = 0; j < 54; ++j)
        {
            auto& cmd = path[j];

            switch (cmd.cmd)
            {
            case 'm':
            {
                pos = pos + ImVec2(cmd.argv[0], cmd.argv[1]);
                //if (j == 36)
                //    pos = ImVec2(665.52f, 674.5f);
                drawList->PathLineTo(pos);
                break;
            }
            case 'M':
            {
                pos = ImVec2(cmd.argv[0], cmd.argv[1]);
                drawList->PathLineTo(pos);
                break;
            }
            case 'l':
            {
                pos = pos + ImVec2(cmd.argv[0], cmd.argv[1]);
                drawList->PathLineTo(pos);
                break;
            }
            case 'L':
            {
                pos = ImVec2(cmd.argv[0], cmd.argv[1]);
                drawList->PathLineTo(pos);
                break;
            }
            case 'v':
            {
                pos = pos + ImVec2(0.0f, cmd.argv[0]);
                drawList->PathLineTo(pos);
                break;
            }
            case 'V':
            {
                pos.y = cmd.argv[0];
                drawList->PathLineTo(pos);
                break;
            }
            case 'h':
            {
                pos = pos + ImVec2(cmd.argv[0], 0.0f);
                drawList->PathLineTo(pos);
                break;
            }
            case 'H':
            {
                pos.x = cmd.argv[0];
                drawList->PathLineTo(pos);
                break;
            }
            case 'c':
            {
                c1 = pos + ImVec2(cmd.argv[0], cmd.argv[1]);
                c2 = pos + ImVec2(cmd.argv[2], cmd.argv[3]);
                c3 = pos + ImVec2(cmd.argv[4], cmd.argv[5]);
                drawList->PathBezierCubicCurveTo(c1, c2, c3, num_segments);
                pos = c3;
                break;
            }
            case 'C':
            {
                c1 = ImVec2(cmd.argv[0], cmd.argv[1]);
                c2 = ImVec2(cmd.argv[2], cmd.argv[3]);
                c3 = ImVec2(cmd.argv[4], cmd.argv[5]);
                drawList->PathBezierCubicCurveTo(c1, c2, c3, num_segments);
                pos = c3;
                break;
            }
            case 's':
            {
                c1 = pos - (c2 - pos);
                c2 = pos + ImVec2(cmd.argv[0], cmd.argv[1]);
                c3 = pos + ImVec2(cmd.argv[2], cmd.argv[3]);
                drawList->PathBezierCubicCurveTo(c1, c2, c3, num_segments);
                pos = c3;
                break;
            }
            case 'S':
            {
                c1 = pos - (c2 - pos);
                c2 = ImVec2(cmd.argv[0], cmd.argv[1]);
                c3 = ImVec2(cmd.argv[2], cmd.argv[3]);
                drawList->PathBezierCubicCurveTo(c1, c2, c3, num_segments);
                pos = c3;
                break;
            }
            case 'q':
            {
                c1 = pos + ImVec2(cmd.argv[0], cmd.argv[1]);
                c2 = pos + ImVec2(cmd.argv[2], cmd.argv[3]);
                drawList->PathBezierQuadraticCurveTo(c1, c2, num_segments);
                pos = c2;
                break;
            }
            case 'Q':
            {
                c1 = ImVec2(cmd.argv[0], cmd.argv[1]);
                c2 = ImVec2(cmd.argv[2], cmd.argv[3]);
                drawList->PathBezierQuadraticCurveTo(c1, c2, num_segments);
                pos = c2;
                break;
            }
            case 't':
            {
                c1 = pos - (c1 - pos);
                c2 = pos + ImVec2(cmd.argv[0], cmd.argv[1]);
                drawList->PathBezierQuadraticCurveTo(c1, c2, num_segments);
                pos = c2;
                break;
            }
            case 'T':
            {
                c1 = pos - (c1 - pos);
                c2 = ImVec2(cmd.argv[0], cmd.argv[1]);
                drawList->PathBezierQuadraticCurveTo(c1, c2, num_segments);
                pos = c2;
                break;
            }
            case 'a':
            case 'A':
                // TODO implement
                break;
            case 'z':
            case 'Z':
            {
                drawList->_Path;
                uint32_t offset = (uint32_t)vertices.size();
                uint32_t k;
                vertices.push_back({ drawList->_Path[0].x, drawList->_Path[0].y });
                for (k = 1; k < (uint32_t)drawList->_Path.Size; ++k)
                {
                    vertices.push_back({ drawList->_Path[k].x, drawList->_Path[k].y });
                    edges.push_back({ offset + k - 1, offset + k });
                }
                edges.push_back({ offset + k - 1, offset });

                drawList->PathClear();
                break;
            }
            }
        }

    }

    KLVectorGraph r;
    r.bb = svgData.bb;
    
    CDT::Triangulation<float> cdt;
    cdt.insertVertices(vertices);
    cdt.insertEdges(edges);
    cdt.eraseOuterTrianglesAndHoles();

    for (uint32_t i = 0; i < cdt.triangles.size(); ++i)
    {
        auto& tri = cdt.triangles[i].vertices;

        ImVec2 p1(cdt.vertices[tri[0]].x, cdt.vertices[tri[0]].y);
        ImVec2 p2(cdt.vertices[tri[1]].x, cdt.vertices[tri[1]].y);
        ImVec2 p3(cdt.vertices[tri[2]].x, cdt.vertices[tri[2]].y);

        r.trangles.push_back({p1, p2, p3});
    }
    
    delete drawList;
    
    return r;
}

KLVectorGraph KLCreateVectorGraph(const char* filename)
{
    return ConvertSvgData(parseSvgData(filename));
}

void DrawListDrawVectorGraph(ImDrawList* drawList, KLVectorGraph& graph, ImVec2 pos, ImVec2 size, ImU32 color)
{
    ImVec2 factor(
        size.x / (graph.bb.Max.x - graph.bb.Min.x), 
        size.y / (graph.bb.Max.y - graph.bb.Min.y));

    for (uint32_t i = 0; i < graph.trangles.size(); ++i)
    {
        auto& trangle = graph.trangles[i];

        drawList->Flags &= ~ImDrawListFlags_AntiAliasedFill;
        drawList->AddTriangleFilled(
            pos + trangle.p1 * factor, 
            pos + trangle.p2 * factor, 
            pos + trangle.p3 * factor, color);
    }
    drawList->Flags |= ImDrawListFlags_AntiAliasedFill;
}

KLVectorGraph vgraph;

void MyCanvasRender()
{
    auto window = ImGui::GetCurrentWindow();
    auto drawList = window->DrawList;


    ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
    ImVec2 canvas_size = ImGui::GetContentRegionAvail();

    ImVec2 offset = ImVec2(0.0f, 0.0f);
    float s = 0.5;
    ImU32 color = 0xFF00F0F0;
    float num_segments = 6;

    DrawListDrawVectorGraph(drawList, vgraph, ImVec2(300.0f, 100.0f), ImVec2(200.0f, 200.0), color);

    ImVec2 posa = canvas_pos + 50.0f;
    ImVec2 posb = posa + 50.0f;
    drawList->AddRectFilled(posa, posb, 0xFFFF0000);
}

// Main code
int main(int argc, char** argv)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_OWNDC, WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, NULL, NULL, L"ImGui Example2", NULL };
    ::RegisterClassExW(&wc);
    hwnd = ::CreateWindowW(
        wc.lpszClassName,
        APP_TITLE,
        WS_POPUP,
        100, 100, 800, 500, NULL, NULL, wc.hInstance, NULL);

    // Initialize OpenGL
    if (!CreateDeviceWGL(hwnd, &g_MainWindow))
    {
        CleanupDeviceWGL(hwnd, &g_MainWindow);
        ::DestroyWindow(hwnd);
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }
    wglMakeCurrent(g_MainWindow.hDC, g_hRC);
    gladLoadGL();

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;       // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;     // Enable Multi-Viewport / Platform Windows

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_InitForOpenGL(hwnd);
    ImGui_ImplOpenGL3_Init();

    //注册原始输入设备
    RAWINPUTDEVICE rawInputDevice[1];
    rawInputDevice[0].usUsagePage = 0x01;//设备类
    rawInputDevice[0].usUsage = 0x06;//设备类内的具体设备
    rawInputDevice[0].dwFlags = RIDEV_INPUTSINK;//意味着即使窗口失去焦点位置，仍然会一直接收输入消息
    rawInputDevice[0].hwndTarget = hwnd;
    if (RegisterRawInputDevices(rawInputDevice, 1, sizeof(rawInputDevice[0])) == FALSE)
    {
        printf("RegisterRawInputDevices failed");
    }

    // Win32+GL needs specific hooks for viewport, as there are specific things needed to tie Win32 and GL api.
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
        IM_ASSERT(platform_io.Renderer_CreateWindow == NULL);
        IM_ASSERT(platform_io.Renderer_DestroyWindow == NULL);
        IM_ASSERT(platform_io.Renderer_SwapBuffers == NULL);
        IM_ASSERT(platform_io.Platform_RenderWindow == NULL);
        platform_io.Renderer_CreateWindow = Hook_Renderer_CreateWindow;
        platform_io.Renderer_DestroyWindow = Hook_Renderer_DestroyWindow;
        platform_io.Renderer_SwapBuffers = Hook_Renderer_SwapBuffers;
        platform_io.Platform_RenderWindow = Hook_Platform_RenderWindow;
    }

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);

    init_scene();

    int dpi = GetDpiForWindow(hwnd);


    const char* filename = "../ResearchPlayground/image/svg/frame_keyboard.svg";
    vgraph = KLCreateVectorGraph(filename);

    

    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplWin32_NewFrame();

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glViewport(0.0f, 0.0f, kbv_width, kbv_height);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        render_scene();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0.0f, 0.0f, SCR_WIDTH, SCR_HEIGHT);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        ImGui::NewFrame();

        ImGui::Begin("first");
        ImGui::Button("Hello World!");

        MyCanvasRender();

        auto window = ImGui::GetCurrentWindow();
        auto& dc = window->DC;
        
        ImVec2 cursor = dc.CursorPos;
        kbv_x = cursor.x;
        kbv_y = cursor.y;
        ImVec2 region = ImGui::GetContentRegionAvail();

        //printf("%f %f | %f %f\n", mouse_pos.x, mouse_pos.y, cursor.x, cursor.y);

        viewport = ImRect(cursor, region);

        //ImGui::Image((ImTextureID)texColorBuffer, 
        //    ImVec2(kbv_width, kbv_height), 
        //    ImVec2(0, 1), ImVec2(1, 0));

        ImGui::End();

        // Rendering
        ImGui::Render();
        glViewport(0, 0, g_Width, g_Height);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        // Update and Render additional Platform Windows
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();

            // Restore the OpenGL rendering context to the main window DC, since platform windows might have changed it.
            wglMakeCurrent(g_MainWindow.hDC, g_hRC);
        }

        // Present
        ::SwapBuffers(g_MainWindow.hDC);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceWGL(hwnd, &g_MainWindow);
    wglDeleteContext(g_hRC);
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions
bool CreateDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    HDC hDc = ::GetDC(hWnd);
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;

    const int pf = ::ChoosePixelFormat(hDc, &pfd);
    if (pf == 0)
        return false;
    if (::SetPixelFormat(hDc, pf, &pfd) == FALSE)
        return false;
    ::ReleaseDC(hWnd, hDc);

    data->hDC = ::GetDC(hWnd);
    if (!g_hRC)
        g_hRC = wglCreateContext(data->hDC);
    return true;
}

void CleanupDeviceWGL(HWND hWnd, WGL_WindowData* data)
{
    wglMakeCurrent(NULL, NULL);
    ::ReleaseDC(hWnd, data->hDC);
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam != SIZE_MINIMIZED)
        {
            g_Width = LOWORD(lParam);
            g_Height = HIWORD(lParam);
        }
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break; 
    case WM_MOUSEMOVE:
    {
        // 提取鼠标坐标
        int xPos = GET_X_LPARAM(lParam);
        int yPos = GET_Y_LPARAM(lParam);

        mouse_pos.x = xPos - kbv_x + 100;
        mouse_pos.y = yPos - kbv_y + 100;
        return 0;
    }
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    case WM_INPUT:
    {
        return 0;
    }
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}
