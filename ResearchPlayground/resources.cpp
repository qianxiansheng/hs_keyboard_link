#include "resource.h"



KLImageDefine image_resources_define[] = {
    {"image/frame_keyboard.png"       , "frame_keyboard"       },
    {"image/frame_lighting.png"       , "frame_lighting"       },
    {"image/frame_macro.png"          , "frame_macro"          },
    {"image/frame_setting.png"        , "frame_setting"        },
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
    {"image/btn_reset.png"            , "reset"                },
    {"image/btn_save.png"             , "save"                 },
};
int image_resources_define_size = sizeof(image_resources_define) / sizeof(image_resources_define[0]);


// initial static member
KLImageManager* KLImageManager::s_Instance = nullptr;
std::once_flag KLImageManager::s_OnceFlag;

KLImageManager::KLImageManager() {
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
