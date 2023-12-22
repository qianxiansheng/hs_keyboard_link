#include "light.h"

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"

#include "keyboard/keyboard.h"

#include "language.h"
#include <unordered_map>

constexpr auto LIGHT_CUSTOMIZE = 17;

std::unordered_map<KEY_MapId_t, bool> customize_table;

static int selected = -1;

bool IsSelectCustomize()
{
    return selected == LIGHT_CUSTOMIZE;
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

#define LIGHT_SELECTABLE(kll_key, idx) if (ImGui::Selectable(KLLABLEB((kll_key), "LIGHT_SEL"), selected == (idx))) {selected = (idx);}
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_1 , 1);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_2 , 2);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_3 , 3);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_4 , 4);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_5 , 5);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_6 , 6);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_7 , 7);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_8 , 8);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_9 , 9);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_10, 10);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_11, 11);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_12, 12);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_13, 13);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_14, 14);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_15, 15);
    LIGHT_SELECTABLE(KLL_KEY_LIGHT_16, 16);
#undef LIGHT_SELECTABLE

    if (ImGui::Selectable("Customize##LIGHT_SEL", selected == LIGHT_CUSTOMIZE)) {
        selected = LIGHT_CUSTOMIZE;
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


    static int brightness = 0;
    ImGui::SliderInt("##BRIGHTNESS", &brightness, 0, 5);

    if (selected == LIGHT_CUSTOMIZE)
    {
        ImGui::Text("Customize");
    }

    ImGui::End();
}
