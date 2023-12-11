#include "settings.h"

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"

#include "language.h"
#include "settings/program_setting.h"
#include "configuration/kl_persistence.h"


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
bool global_setting_auto_run = false;
bool global_setting_x_system_tray = false;
bool global_setting_enable_sleep = false;


void ToggleButton(const char* str_id, bool* v)
{
	ImVec2 p = ImGui::GetCursorScreenPos();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	float height = ImGui::GetFrameHeight();
	float width = height * 1.55f;
	float radius = height * 0.50f;

	if (ImGui::InvisibleButton(str_id, ImVec2(width, height)))
		*v = !*v;
	ImU32 col_bg;
	if (ImGui::IsItemHovered())
		col_bg = *v ? IM_COL32(145 + 20, 211, 68 + 20, 255) : IM_COL32(218 - 20, 218 - 20, 218 - 20, 255);
	else
		col_bg = *v ? IM_COL32(145, 211, 68, 255) : IM_COL32(218, 218, 218, 255);

	draw_list->AddRectFilled(p, ImVec2(p.x + width, p.y + height), col_bg, height * 0.5f);
	draw_list->AddCircleFilled(ImVec2(*v ? (p.x + width - radius) : (p.x + radius), p.y + radius), radius - 1.5f, IM_COL32(255, 255, 255, 255));
}

void InitSettingsWindow()
{
	global_setting_auto_run = AutoRunRegisterCheck();

	SettingsRead(SETTINGS_FILE_NAME);
}

void ShowSettingsWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin(WINNAME_SETTINGS, p_open, window_flags)) {
		ImGui::End();
		return;
	}
	ImGui::GetCurrentWindow()->DockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;


	/* Global Setting Device Info */
	ImGui::SeparatorText(KLLABLEA(KLL_KEY_DEVICE_INFO));
	char device_model[] = "-";
	char chip_model[] = "-";
	char hard_version[] = "-";
	char soft_version[] = "-";
	float c1 = 150.0f;

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


	/* Global Setting Language */
	ImGui::SeparatorText(KLLABLEA(KLL_KEY_LANGUAGE));
	static int item_current_2 = KLLanguageManager::GetInstance()->GetLanguage();
	static int prev_item = 0;
	ImGui::Combo("##LANGUAGE", &item_current_2, u8"¼òÌåÖÐÎÄ\0English\0\0");
	if (prev_item != item_current_2)
		KLLanguageManager::GetInstance()->SetLanguage((KLLangType)item_current_2);
	prev_item = item_current_2;

	/* Global Setting Auto Run */
	ImGui::SeparatorText(KLLABLEA(KLL_KEY_STARTUP));
	static bool autoRunPrev = global_setting_auto_run;
	static bool autoRunCurr = global_setting_auto_run;
	ToggleButton("##TOGGLE_AUTO_RUN", &autoRunCurr);
	if (autoRunPrev != autoRunCurr)
	{
		global_setting_auto_run = autoRunPrev = autoRunCurr;
		if (global_setting_auto_run) {
			AutoRunRegisterSet();
		} else {
			AutoRunRegisterDel();
		}
	}
	ImGui::SameLine();
	ImGui::Text(KLLABLEA(KLL_KEY_AUTO_RUN));

	/* Global Setting Exit */
	ImGui::SeparatorText(KLLABLEA(KLL_KEY_EXIT));
	ImGui::Text(KLLABLEA(KLL_KEY_WHEN_CLICK_X));
	if (ImGui::RadioButton(KLLABLEB(KLL_KEY_DIRECT_EXIT, "RADIO_DIRECT_EXIT"), !global_setting_x_system_tray)) global_setting_x_system_tray = false;
	if (ImGui::RadioButton(KLLABLEB(KLL_KEY_RESIDES_SYSTEM_TRAY, "RADIO_SYSTEM_TRAY"), global_setting_x_system_tray)) global_setting_x_system_tray = true;


	/* Global Setting Sleepxit */
	ImGui::SeparatorText(KLLABLEA(KLL_KEY_SLEEP));
	static bool enableSleepPrev = global_setting_enable_sleep;
	static bool enableSleepCurr = global_setting_enable_sleep;
	ToggleButton("##TOGGLE_ENABLE_SLEEP", &enableSleepCurr);
	if (enableSleepPrev != enableSleepCurr)
	{
		global_setting_enable_sleep = enableSleepPrev = enableSleepCurr;
		if (global_setting_enable_sleep) {
			/* Enable Sleep Mode */
		} else {
			/* Disable Sleep Mode */
		}
	}
	ImGui::SliderInt("##IDLE_TIME", &global_setting_idle_time_idx, 0, 9, "");
	ImGui::SameLine();
	ImGui::Text("%d %s", idleTime[global_setting_idle_time_idx].num, idleTime[global_setting_idle_time_idx].unit == 0 ? KLLABLEA(KLL_KEY_SECOND) : KLLABLEA(KLL_KEY_MINUTE));
	ImGui::SameLine();
	utils::HelpMarker(KLLABLEA(KLL_KEY_TOOLTIP_SLEEP));

	/* Global Setting Restore Factory Settings */
	ImGui::SeparatorText(KLLABLEA(KLL_KEY_RESET));
	ImGui::Button(KLLABLEB(KLL_KEY_RESET_BTN, "RESET_BTN"));
	ImGui::SameLine();
	utils::HelpMarker(KLLABLEA(KLL_KEY_RESTORE_FACTORY_SETTINGS));


	static int frame_cnt = 0;
	if (frame_cnt++ >= 500)
	{
		SettingsWrite(SETTINGS_FILE_NAME);

		frame_cnt = 0;
	}

	ImGui::End();
}
