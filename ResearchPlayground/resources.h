#pragma once

#include "pch.h"

#include <iostream>
#include <mutex>
#include <unordered_map>

#include "util/utils.h"

#include "imgui/imgui.h"
#include "svgloader.h"

struct KLImageDefine
{
    char file[64];
    char name[32];
};

struct KLImage
{
    std::string name;
    int width;
    int height;
    int channels;
    ImTextureID texID;
};

class KLImageManager
{
public:
    std::unordered_map<std::string, KLImage> images;
    std::unordered_map<std::string, KLVectorGraph> vectorGraphs;

    KLImage& GetImage(std::string name)
    {
        return images[name];
    }

    void DrawGraph(ImDrawList* drawList, std::string name, ImVec2 pos, ImVec2 size, ImU32 color);

    static KLImageManager* GetInstance()
    {
        std::call_once(s_OnceFlag, []() {
            s_Instance = new KLImageManager();
            });
        return s_Instance;
    }

    ~KLImageManager() {

    }
private:
    KLImageManager();
private:
    static std::once_flag  s_OnceFlag;
    static KLImageManager* s_Instance;
};
