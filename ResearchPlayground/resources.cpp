#include "resource.h"

#include "glad/glad/glad.h"
#include "stb_image.h"

KLImageDefine image_resources_define[] = {
    {"image/func_fastbackward.png"    , "func_backward"        },
    {"image/func_fastforward.png"     , "func_forward"         },
    {"image/func_full_screen.png"     , "func_full_screen"     },
    {"image/func_mute.png"            , "func_mute"            },
    {"image/func_next.png"            , "func_next"            },
    {"image/func_play.png"            , "func_play"            },
    {"image/func_prev.png"            , "func_prev"            },
    {"image/func_terminate.png"       , "func_terminate"       },
    {"image/func_voiume_increment.png", "func_voiume_increment"},
    {"image/func_volume_decrement.png", "func_volume_decrement"},

    {"image/btn_del_blue.png"         , "btn_del_blue"         },
    {"image/btn_del_gray.png"         , "btn_del_gray"         },
    {"image/btn_edit_blue.png"        , "btn_edit_blue"        },
    {"image/btn_edit_gray.png"        , "btn_edit_gray"        },
    {"image/btn_export_blue.png"      , "btn_export_blue"      },
    {"image/btn_export_gray.png"      , "btn_export_gray"      },
    {"image/btn_import_black.png"     , "btn_import_black"     },
    {"image/btn_import_blue.png"      , "btn_import_blue"      },
    {"image/btn_import_gray.png"      , "btn_import_gray"      },
    {"image/btn_new_blue.png"         , "btn_new_blue"         },
    {"image/btn_new_gray.png"         , "btn_new_gray"         },
    {"image/btn_reset.png"            , "btn_reset"            },
    {"image/btn_reset_blue.png"       , "btn_reset_blue"       },
    {"image/btn_reset_gray.png"       , "btn_reset_gray"       },
    {"image/btn_save.png"             , "btn_save"             },
    {"image/btn_save_blue.png"        , "btn_save_blue"        },
    {"image/btn_save_gray.png"        , "btn_save_gray"        },

    {"image/frame_keyboard.png"       , "frame_keyboard"       },
    {"image/frame_keyboard_blue.png"  , "frame_keyboard_blue"  },
    {"image/frame_keyboard_gray.png"  , "frame_keyboard_gray"  },
    {"image/frame_lighting.png"       , "frame_lighting"       },
    {"image/frame_lighting_gray.png"  , "frame_lighting_gray"  },
    {"image/frame_lighting_pink.png"  , "frame_lighting_pink"  },
    {"image/frame_macro.png"          , "frame_macro"          },
    {"image/frame_macro_gray.png"     , "frame_macro_gray"     },
    {"image/frame_macro_orange.png"   , "frame_macro_orange"   },
    {"image/frame_setting_gray.png"   , "frame_setting_gray"   },
    {"image/frame_setting_black.png"  , "frame_setting_black"  },
};
int image_resources_define_size = sizeof(image_resources_define) / sizeof(image_resources_define[0]);


KLImageDefine vector_graph_resources_define[] = {
    {"image/svg/btn_del.svg"       , "btn_del"        },
    {"image/svg/btn_new.svg"       , "btn_new"        },
    {"image/svg/btn_edit.svg"      , "btn_edit"       },
    {"image/svg/btn_import.svg"    , "btn_import"     },
    {"image/svg/btn_export.svg"    , "btn_export"     },
    {"image/svg/frame_setting.svg" , "frame_setting"  },
 // {"image/svg/frame_keyboard.svg", "frame_keyboard" },
 // {"image/svg/frame_light.svg"   , "frame_light"    },
 // {"image/svg/frame_macro.svg"   , "frame_macro"    },
};
int vector_graph_resoures_define_size = sizeof(vector_graph_resources_define) / sizeof(vector_graph_resources_define[0]);


// initial static member
KLImageManager* KLImageManager::s_Instance = nullptr;
std::once_flag KLImageManager::s_OnceFlag;

KLImageManager::KLImageManager() {
    for (int i = 0; i < vector_graph_resoures_define_size; ++i)
    {
        vectorGraphs[vector_graph_resources_define[i].name] = KLCreateVectorGraph(vector_graph_resources_define[i].file);
    }
    for (int i = 0; i < image_resources_define_size; ++i)
    {
        int width, height, channels;
        unsigned char* data = utils::load_image(
            image_resources_define[i].file,
            &width,
            &height,
            &channels,
            STBI_rgb_alpha
        );
        assert(data);

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        utils::free_image(data);

        KLImage& image = images[image_resources_define[i].name];
        image.name = image_resources_define[i].name;
        image.width = width;
        image.height = height;
        image.channels = channels;
        image.texID = (ImTextureID)textureID;
    }
}

void KLImageManager::DrawGraph(ImDrawList* drawList, std::string name, ImVec2 pos, ImVec2 size, ImU32 color)
{
    auto it = vectorGraphs.find(name);
    if (it == vectorGraphs.end())
        return;
    DrawListDrawVectorGraph(drawList, it->second, pos, size, color);
}

