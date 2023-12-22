#include "light.h"

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"

#include "keyboard/keyboard.h"

#include "language.h"
#include <unordered_map>
#include "keylink.h"

#include "ui_thread.h"
#include "statusbar.h"
#include "protocol/iap.h"

std::unordered_map<KEY_MapId_t, bool> customize_table;
static int light_mode = LIGHT_CUSTOMIZE;
static int brightness = 0;
static int speed = 0;

bool IsSelectCustomize()
{
    return light_mode == LIGHT_CUSTOMIZE;
}

void ShowLightWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
	if (!ImGui::Begin(WINNAME_LIGHT, p_open, window_flags)) {
		ImGui::End();
		return;
	}

    auto dockNode = ImGui::GetCurrentWindow()->DockNode;
    if (dockNode) dockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

	ImGui::Text(KLLABLEA(KLL_KEY_LIGHT_SETTINGS));
    ImGui::Separator();

#define LIGHT_SELECTABLE(kll_key, idx) if (ImGui::Selectable(KLLABLEB((kll_key), "LIGHT_SEL"), light_mode == (idx))) {light_mode = (idx);}
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_1 , 0x01);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_2 , 0x02);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_3 , 0x03);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_4 , 0x04);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_5 , 0x05);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_6 , 0x06);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_7 , 0x07);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_8 , 0x08);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_9 , 0x09);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_10, 0x0A);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_11, 0x0B);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_12, 0x0C);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_13, 0x0D);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_14, 0x0E);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_15, 0x0F);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_16, 0x10);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_17, 0x11);
#undef LIGHT_SELECTABLE

    if (ImGui::Selectable(KLLABLEB(KLL_KEY_CUSTOMIZE_LIGHT, "LIGHT_SEL"), light_mode == LIGHT_CUSTOMIZE)) {
        light_mode = LIGHT_CUSTOMIZE;
    }


	ImGui::End();
}

void ShowLightModifyWindow(bool* p_open)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
    if (!ImGui::Begin(WINNAME_LIGHT_MODIFY, p_open, window_flags)) {
        ImGui::End();
        return;
    }

    auto dockNode = ImGui::GetCurrentWindow()->DockNode;
    if (dockNode) dockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

    float al1 = DPI(100.0f);

    ImGui::Text(KLLABLEA(KLL_KEY_BRIGHTNESS));
    ImGui::SameLine();
    ImGuiDCXAxisAlign(al1);
    ImGui::SliderInt("##LIGHT_BRIGHTNESS", &brightness, 0, 4);

    ImGui::Text(KLLABLEA(KLL_KEY_SPEED));
    ImGui::SameLine();
    ImGuiDCXAxisAlign(al1);
    ImGui::SliderInt("##LIGHT_SPEED", &speed, 0, 4);

    if (ImGui::Button(KLLABLEB(KLL_KEY_SAVE, "APPLY_LIGHT_SETTINGS")))
    {

        auto deviceManager = KLDeviceManager::GetInstance();

        if (deviceManager->HasDevice())
        {
            /* send cmd */
            bool r = drv_set_light(deviceManager->GetCurrentDevice(), light_mode, brightness, speed);

            if (r)
            {
                showAlert_(KLLABLEA(KLL_KEY_SUCCESSFUL));
            }
            else
            {
                showAlert_(KLLABLEA(KLL_KEY_FAILED));
            }
        }
        else
        {
            showAlert_(KLLABLEA(KLL_KEY_DEVICE_NOT_CONNECTED));
        }
    }

    ImGui::End();
}
