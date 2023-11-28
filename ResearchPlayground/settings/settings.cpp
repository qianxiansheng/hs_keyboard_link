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

	char xinghao[] = "--";
	char softVersion[] = "--";
	char hardVersion[] = "--";

	ImGui::SeparatorText(utils::gbk_to_utf8("�豸��Ϣ").c_str());
	ImGui::Text(utils::gbk_to_utf8("��Ʒ�ͺţ�%s").c_str(), xinghao);
	ImGui::Text(utils::gbk_to_utf8("����汾��%s").c_str(), softVersion);
	ImGui::Text(utils::gbk_to_utf8("Ӳ���汾��%s").c_str(), hardVersion);


	ImGui::SeparatorText(utils::gbk_to_utf8("����").c_str());

	static int item_current_2 = 0;
	ImGui::Combo("##language", &item_current_2, "Chinese\0English\0\0");

	static int idle_time = 0;
	ImGui::SeparatorText(utils::gbk_to_utf8("����").c_str());
	ImGui::SliderInt("##IDLE_TIME", &idle_time, 1, 20);

	ImGui::SeparatorText(utils::gbk_to_utf8("�ָ���������").c_str());
	ImGui::Button(utils::gbk_to_utf8("�ָ���������").c_str());

	ImGui::End();
}
