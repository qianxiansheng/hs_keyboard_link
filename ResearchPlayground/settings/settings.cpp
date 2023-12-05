#include "settings.h"

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"

void ShowSettingsWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin(WINNAME_SETTINGS, p_open, window_flags)) {
		ImGui::End();
		return;
	}
	ImGui::GetCurrentWindow()->DockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

	char xinghao[] = "--";
	char softVersion[] = "--";
	char hardVersion[] = "--";

	ImGui::SeparatorText(u8"�豸��Ϣ");
	ImGui::Text(u8"��Ʒ�ͺţ�%s", xinghao);
	ImGui::Text(u8"����汾��%s", softVersion);
	ImGui::Text(u8"Ӳ���汾��%s", hardVersion);


	ImGui::SeparatorText(u8"����");

	static int item_current_2 = 0;
	ImGui::Combo("##LANGUAGE", &item_current_2, u8"��������\0English\0\0");

	static int idle_time = 0;
	ImGui::SeparatorText(u8"����");
	ImGui::SliderInt("##IDLE_TIME", &idle_time, 1, 20);

	ImGui::SeparatorText(u8"�ָ���������");
	ImGui::Button(u8"�ָ���������");

	ImGui::End();
}
