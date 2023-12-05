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

	ImGui::SeparatorText(u8"设备信息");
	ImGui::Text(u8"产品型号：%s", xinghao);
	ImGui::Text(u8"软件版本：%s", softVersion);
	ImGui::Text(u8"硬件版本：%s", hardVersion);


	ImGui::SeparatorText(u8"语言");

	static int item_current_2 = 0;
	ImGui::Combo("##LANGUAGE", &item_current_2, u8"简体中文\0English\0\0");

	static int idle_time = 0;
	ImGui::SeparatorText(u8"休眠");
	ImGui::SliderInt("##IDLE_TIME", &idle_time, 1, 20);

	ImGui::SeparatorText(u8"恢复出厂设置");
	ImGui::Button(u8"恢复出厂设置");

	ImGui::End();
}
