#ifndef __KL_SVG_LOADER_H__
#define __KL_SVG_LOADER_H__

#include <iostream>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

std::vector<std::string> getSvgData(const char* filename);


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

SVGPathData parseSvgPathData(char* data);
SVGData parseSvgData(const char* filename);

KLVectorGraph ConvertSvgData(SVGData svgData);
KLVectorGraph KLCreateVectorGraph(const char* filename);
void DrawListDrawVectorGraph(ImDrawList* drawList, KLVectorGraph& graph, ImVec2 pos = ImVec2(0.0f, 0.0f), ImVec2 size = ImVec2(32.0f, 32.0f), ImU32 color = 0xFF000000);

#endif