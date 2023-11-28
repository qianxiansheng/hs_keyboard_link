#pragma once

#include <iostream>
#include <mutex>
#include <unordered_map>

#include "util/utils.h"
#include "stb_image.h"

#include "imgui/imgui.h"
#include "glad/glad/glad.h"

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

    KLImage& GetImage(std::string name)
    {
        return images[name];
    }

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
