#include "svgloader.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "libxml/parser.h"

#define CDT_USE_64_BIT_INDEX_TYPE
#include "CDT.h"

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

            if (cmd == 'z' || cmd == 'Z')
                break;
        }

        if (*p == '\0' )
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
    std::vector<SVGPathCmd>  output;
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
            //printf("%c %s\n", cmd, pData + a);
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
                //printf("%c %s\n", cmd, pData + a);
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

    output.bb = parseSvgViewBox((char*)data[0].c_str());

    for (uint32_t i = 1; i < data.size(); ++i)
    {
        output.paths.push_back(parseSvgPathData((char*)data[i].c_str()));
    }
    return output;
}

KLVectorGraph ConvertSvgData(SVGData svgData)
{
    auto drawList = new ImDrawList(nullptr);

    ImVec2 pos;
    ImVec2 c1;
    ImVec2 c2;
    ImVec2 c3;

    int num_segments = 6;

    std::vector<CDT::V2d<float>> vertices;
    CDT::EdgeVec edges;

    for (uint32_t i = 0; i < svgData.paths.size(); ++i)
    {
        auto& path = svgData.paths[i];

        for (uint32_t j = 0; j < path.size(); ++j)
        {
            auto& cmd = path[j];

            switch (cmd.cmd)
            {
            case 'm':
            {
                pos = pos + ImVec2(cmd.argv[0], cmd.argv[1]);
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

        r.trangles.push_back({ p1, p2, p3 });
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