#include "macro_config.h"

#include <iostream>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "util/utils.h"

#include "resources.h"

#include "sequencer/ingseq.h"
#include "kl_persistence.h"
#include "language.h"
#include "keylink.h"

// initial static member
KLMacroConfigManager* KLMacroConfigManager::s_Instance = nullptr;
std::once_flag KLMacroConfigManager::s_OnceFlag;

void KLMacroConfigManager::AddConfig(KLMacro& config)
{
	m_ConfigList.push_back(std::move(config));
}

uint8_t KLMacroConfigManager::FindUnusedID()
{
	uint8_t id = std::numeric_limits<uint8_t>::max();
	for (uint8_t i = 0; i < std::numeric_limits<uint8_t>::max(); ++i)
	{
		bool find = false;
		for (const auto& config : m_ConfigList)
		{
			if (i == config.id)
			{
				find = true;
				break;
			}
		}
		if (!find)
		{
			id = i;
			break;
		}
	}
	return id;
}
void KLMacroConfigManager::AddConfig(const char* name)
{
	KLMacro config;
	config.id = FindUnusedID();
	config.name = name;
	m_ConfigList.push_back(config);
}
bool KLMacroConfigManager::IsConfigExists(const char* name)
{
	for (auto& config : m_ConfigList)
		if (config.name == name)
			return true;
	return false;
}

bool KLMacroConfigManager::RemoveCurrentConfig()
{
	return RemoveConfig(GetCurrentConfig().name.c_str());
}

bool KLMacroConfigManager::RemoveConfig(const char* name)
{
	for (auto it = m_ConfigList.begin(); it != m_ConfigList.end(); ++it) {
		if (it->name == name) {
			m_ConfigList.erase(it);
			break;
		}
	}
	return true;
}


void KLMacroConfigManager::SaveConfig(const char* filename)
{
	MacroConfigWrite(utils::getFileAbsolutePath(filename).c_str());
}
void KLMacroConfigManager::LoadConfig(const char* filename)
{
	MacroConfigRead(utils::getFileAbsolutePath(filename).c_str());
}

void KLMacroConfigManager::SetCurrentConfig(uint32_t i)
{
	m_CurrentConfigIndex = i;
}
KLMacro& KLMacroConfigManager::GetCurrentConfig()
{
	return m_ConfigList[m_CurrentConfigIndex];
}
KLMacro* KLMacroConfigManager::GetConfigByID(uint8_t id)
{
	for (auto it = m_ConfigList.begin(); it != m_ConfigList.end(); ++it)
	{
		if (it->id == id)
			return &*it;
	}
	return NULL;
}

void SyncActionsFromPairInfo(KLMacro& macro)
{
	macro.actions.clear();
	if (macro.pair_actions.size() == 0)
		return;

	std::vector<KLMacroAction> actions;

	for (uint32_t i = 0; i < macro.pair_actions.size(); ++i)
	{
		auto& pair_action = macro.pair_actions[i];
		
		actions.push_back(KLMacroAction(KL_MACRO_ACTYPE_DOWN, pair_action.mFrameStart, pair_action.mFunctionID));
		actions.push_back(KLMacroAction(KL_MACRO_ACTYPE_UP  , pair_action.mFrameEnd  , pair_action.mFunctionID));
	}

	std::sort(actions.begin(), actions.end(), [](const KLMacroAction& a, const KLMacroAction& b) {
		return a.delay < b.delay;
	});

	uint32_t time = actions[0].delay;

	for (auto& action : actions) {
		action.delay -= time;
		time += action.delay;
	}

	macro.actions = actions;
}

void SyncActionsToPairInfo(KLMacro& macro)
{

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

void InitMacroConfigWindow()
{
	auto configManager = KLMacroConfigManager::GetInstance();

	configManager->LoadConfig();

	for (auto& macro : configManager->m_ConfigList) {
		SyncActionsFromPairInfo(macro);
	}

	if (configManager->m_ConfigList.size() == 0)
	{
		KLMacro macro;
		macro.name = "default";
		configManager->AddConfig(macro);
		configManager->SetCurrentConfig(0);
	}
}

void ShowMacroConfigManagerWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin(WINNAME_MACRO_CONFIG_MGR, p_open, window_flags)) {
		ImGui::End();
		return;
	}

	auto dockNode = ImGui::GetCurrentWindow()->DockNode;
	if (dockNode) dockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

	auto configManager = KLMacroConfigManager::GetInstance();
	auto imageManager = KLImageManager::GetInstance();

	ImVec2 size(DPI(16.0f), DPI(16.0f));

	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	ImGui::Text(KLLABLEA(KLL_KEY_MACRO_MANAGER));
	ImGui::Separator();

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
		}
		else {
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
		}
		else {
			configManager->AddConfig(data);
		}
	}
	if (utils::Prompt(&show_rename, KLLABLEB(KLL_KEY_DIALOG_REN_CONFIG, "DLG_REN_CONFIG"), "", data, KL_CONFIG_MAX_NAME_SIZE)) {
		if (configManager->GetCurrentConfig().name != data && configManager->IsConfigExists(data)) {
			show_msg_2 = true;
		}
		else {
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
			ReloadMacroConfig(configManager->GetCurrentConfig());
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


