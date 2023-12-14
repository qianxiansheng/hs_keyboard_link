#include "light.h"

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"

const char* lights[] = {
    u8"�沨����-1",
    u8"�沨����-2",
    u8"�沨����-3",
    u8"�沨����-4",
    u8"����ģʽ",
    u8"����ģʽ",
    u8"��������",
    u8"��ͷ����",
    u8"���ҹⲨ",
    u8"̤ѩ�޺�",
    u8"��ɫ�ٲ�",
    u8"�������",
    u8"б���仯",
    u8"һ������",
    u8"��Ӱ����",
    u8"������ɢ",
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

	ImGui::Text(u8"��Ч����");


    static int selected = -1;
    for (int n = 0; n < lightSize; n++)
    {
        if (ImGui::Selectable(lights[n], selected == n))
            selected = n;
    }

	ImGui::End();
}
