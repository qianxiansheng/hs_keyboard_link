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

        auto window = ImGui::GetCurrentWindow();
        auto& dc = window->DC;
        
        ImVec2 cursor = dc.CursorPos;
        kbv_x = cursor.x;
        kbv_y = cursor.y;
        ImVec2 region = ImGui::GetContentRegionAvail();

        printf("%f %f | %f %f\n", mouse_pos.x, mouse_pos.y, cursor.x, cursor.y);

        viewport = ImRect(cursor, region);

        ImGui::Image((ImTextureID)texColorBuffer, 
            ImVec2(kbv_width, kbv_height), 
            ImVec2(0, 1), ImVec2(1, 0));

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
