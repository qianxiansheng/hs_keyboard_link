#include "light.h"

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"

#include "language.h"

const char* lights[] = {
    KLLABLEB(KLL_KEY_LIGHT_1 , "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_2 , "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_3 , "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_4 , "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_5 , "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_6 , "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_7 , "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_8 , "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_9 , "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_10, "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_11, "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_12, "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_13, "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_14, "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_15, "LIGHT_SEL"),
    KLLABLEB(KLL_KEY_LIGHT_16, "LIGHT_SEL"),
};
int lightSize = sizeof(lights) / sizeof(lights[0]);

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


    static int selected = -1;
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



	ImGui::End();
}
