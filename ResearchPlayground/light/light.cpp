#include "light.h"

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"

const char* lights[] = {
    "����-1",
    "����-2",
    "����-3",
    "����-4",
};

void ShowLightWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin(WINNAME_LIGHT, p_open, window_flags)) {
		ImGui::End();
		return;
	}

	ImGui::Text(utils::gbk_to_utf8("��Ч����").c_str());


    static int selected = -1;
    for (int n = 0; n < 4; n++)
    {
        if (ImGui::Selectable(utils::gbk_to_utf8(lights[n]).c_str(), selected == n))
            selected = n;
    }

	ImGui::End();
}
