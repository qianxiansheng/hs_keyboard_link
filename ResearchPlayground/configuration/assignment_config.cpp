#include "assignment_config.h"

#include <iostream>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "util/utils.h"

#include "function/function.h"
#include "resources.h"
#include "language.h"
#include "keylink.h"


void InitAssignmentConfigManagerWindow()
{
	auto configManager = KLFunctionConfigManager::GetInstance();

	configManager->LoadConfig();

	if (configManager->m_ConfigList.size() == 0)
	{
		KLFunctionConfig config;
		config.name = "default";
		configManager->AddConfig(config);
		configManager->SetCurrentConfig(0);
	}
}

static bool MyButton(const char* vgname, ImVec2 size)
{
	auto imageManager = KLImageManager::GetInstance();
	auto& io = ImGui::GetIO();
	auto drawList = ImGui::GetWindowDrawList();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImRect rect(pos, ImVec2(pos.x + size.x, pos.y + size.y));

	bool clicked = false;

	ImGui::ItemSize(size, 0.0f);
	if (rect.Contains(io.MousePos))
	{
		if (io.MouseClicked[0])
			clicked = true;

		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		if (io.MouseDown[0])
		{
			imageManager->DrawGraph(drawList, vgname, pos, size, 0xFF00007F);
		}
		else
		{
			imageManager->DrawGraph(drawList, vgname, pos, size, 0xFF0000FF);
		}
	}
	else
	{
		imageManager->DrawGraph(drawList, vgname, pos, size, 0xFFdbdbdb);
	}
	return clicked;
}

void ShowAssignmentConfigManagerWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin(WINNAME_ASSIGN_CONFIG_MGR, p_open, window_flags)) {
		ImGui::End();
		return;
	}
	ImGui::GetCurrentWindow()->DockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

	auto configManager = KLFunctionConfigManager::GetInstance();
	auto imageManager = KLImageManager::GetInstance();

	ImVec2 size(DPI(16.0f), DPI(16.0f));

	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	static bool show_new = false;
	static bool show_rename = false;
	static bool show_del_confirm = false;
	static bool show_msg_1 = false;
	static bool show_msg_2 = false;
	static bool show_msg_3 = false;
	static char data[KL_CONFIG_MAX_NAME_SIZE] = "";

	if (MyButton("btn_new", size)) {
		int n = -1;
		char buf[KL_CONFIG_MAX_NAME_SIZE] = "";
		for (int i = 0; i < KL_CONFIG_MAX_NUM; ++i) {
			sprintf(buf, "New Config %d", i);
			if (!configManager->IsConfigExists(buf)) {
				n = i;
				break;
			}
		}
		strcpy(data, buf);
		show_new = true;
	}
	ImGui::SameLine();
	if (MyButton("btn_del", size)) {
		if (configManager->m_ConfigList.size() <= 1) {
			show_msg_3 = true;
		} else {
			show_del_confirm = true;
		}
	}
	if (utils::ConfirmEx(&show_del_confirm,
		KLLABLEB(KLL_KEY_DIALOG_DEL_CONFIG, "DLG_DEL_CONFIG"),
		KLLABLEA(KLL_KEY_DIALOG_DEL_CONFIG_CONFIRM))) {
		configManager->RemoveCurrentConfig();
		configManager->SetCurrentConfig(0);
	}

	ImGui::SameLine();

	if (MyButton("btn_edit", size)) {
		strcpy(data, configManager->GetCurrentConfig().name.c_str());
		show_rename = true;
	}
	if (utils::Prompt(&show_new, KLLABLEB(KLL_KEY_DIALOG_NEW_CONFIG, "DLG_NEW_CONFIG"), "", data, KL_CONFIG_MAX_NAME_SIZE)) {
		if (configManager->IsConfigExists(data)) {
			show_msg_1 = true;
		} else {
			configManager->AddConfig(data);
		}
	}
	if (utils::Prompt(&show_rename, KLLABLEB(KLL_KEY_DIALOG_REN_CONFIG, "DLG_REN_CONFIG"), "", data, KL_CONFIG_MAX_NAME_SIZE)) {
		if (configManager->GetCurrentConfig().name != data && configManager->IsConfigExists(data)) {
			show_msg_2 = true;
		} else {
			configManager->GetCurrentConfig().name = data;
		}
	}

	utils::AlertEx(&show_msg_1, KLLABLEB(KLL_KEY_DIALOG_MSG, "CONFIG_MSG_1"), KLLABLEA(KLL_KEY_DIALOG_NEW_ERROR_CONFIG_EXISTSED));
	utils::AlertEx(&show_msg_2, KLLABLEB(KLL_KEY_DIALOG_MSG, "CONFIG_MSG_2"), KLLABLEA(KLL_KEY_DIALOG_REN_ERROR_CONFIG_EXISTSED));
	utils::AlertEx(&show_msg_3, KLLABLEB(KLL_KEY_DIALOG_MSG, "CONFIG_MSG_3"), KLLABLEA(KLL_KEY_DIALOG_DEL_ERROR_KEEP_AT_LEAST_ONE));

	ImGui::SameLine();
	if (MyButton("btn_export", size))
	{

	}
	ImGui::SameLine();
	if (MyButton("btn_import", size))
	{

	}

	static int selected = 0;
	for (int i = 0; i < configManager->m_ConfigList.size(); ++i)
	{
		char label[KL_CONFIG_MAX_NAME_SIZE + 5] = "";
		sprintf(label, "%s##%d", configManager->m_ConfigList[i].name.c_str(), i);
		if (ImGui::Selectable(label, selected == i))
		{
			selected = i;

			configManager->SetCurrentConfig(i);
		}
	}


	static int frame_cnt = 0;
	if (frame_cnt++ >= 500)
	{
		configManager->SaveConfig();
		frame_cnt = 0;
	}
	
	ImGui::End();
}

