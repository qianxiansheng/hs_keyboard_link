#include "settings.h"

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"

#include "language.h"


#define ImGuiDCXAxisAlign(v) ImGui::SetCursorPos(ImVec2((v), ImGui::GetCursorPos().y))
#define ImGuiDCYMargin(v) ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + (v)))

IdelTime idleTime[] = {
	{30, 0},
	{ 1, 1},
	{90, 0},
	{ 2, 1},
	{ 3, 1},
	{ 4, 1},
	{ 5, 1},
	{10, 1},
	{15, 1},
	{20, 1},
};

int global_setting_idle_time_idx = 0;

void ShowSettingsWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin(WINNAME_SETTINGS, p_open, window_flags)) {
		ImGui::End();
		return;
	}
	ImGui::GetCurrentWindow()->DockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

	char device_model[] = "-";
	char chip_model[] = "-";
	char hard_version[] = "-";
	char soft_version[] = "-";

	float c1 = 150.0f;

	ImGui::SeparatorText(KLLABLEA(KLL_KEY_DEVICE_INFO));
	ImGui::Text(KLLABLEA(KLL_KEY_DEVICE_MODEL));
	ImGui::SameLine();
	ImGuiDCXAxisAlign(c1);
	ImGui::Text("%s", device_model);

	ImGui::Text(KLLABLEA(KLL_KEY_CHIP_MODEL));
	ImGui::SameLine();
	ImGuiDCXAxisAlign(c1);
	ImGui::Text("%s", chip_model);

	ImGui::Text(KLLABLEA(KLL_KEY_HARDWARE_VERSION));
	ImGui::SameLine();
	ImGuiDCXAxisAlign(c1);
	ImGui::Text("%s", hard_version);

	ImGui::Text(KLLABLEA(KLL_KEY_SOFTWARE_VERSION));
	ImGui::SameLine();
	ImGuiDCXAxisAlign(c1);
	ImGui::Text("%s", soft_version);


	ImGui::SeparatorText(KLLABLEA(KLL_KEY_LANGUAGE));

	static int item_current_2 = 0;
	static int prev_item = 0;
	ImGui::Combo("##LANGUAGE", &item_current_2, u8"¼òÌåÖÐÎÄ\0English\0\0");
	if (prev_item != item_current_2)
		KLLanguageManager::GetInstance()->SetLanguage((KLLangType)item_current_2);
	prev_item = item_current_2;

	ImGui::SeparatorText(KLLABLEA(KLL_KEY_STARTUP));
	ImGui::SeparatorText(KLLABLEA(KLL_KEY_EXIT));
	

	ImGui::SeparatorText(KLLABLEA(KLL_KEY_SLEEP));
	ImGui::SliderInt("##IDLE_TIME", &global_setting_idle_time_idx, 1, 9, "");
	ImGui::SameLine();
	ImGui::Text("%d %s", idleTime[global_setting_idle_time_idx].num, idleTime[global_setting_idle_time_idx].unit == 0 ? KLLABLEA(KLL_KEY_SECOND) : KLLABLEA(KLL_KEY_MINUTE));
	ImGui::SameLine();
	utils::HelpMarker(KLLABLEA(KLL_KEY_TOOLTIP_SLEEP));


	ImGui::SeparatorText(KLLABLEA(KLL_KEY_RESET));
	ImGui::Button(KLLABLEB(KLL_KEY_RESET_BTN, "RESET_BTN"));
	ImGui::SameLine();
	utils::HelpMarker(KLLABLEA(KLL_KEY_RESTORE_FACTORY_SETTINGS));

	ImGui::End();
}
