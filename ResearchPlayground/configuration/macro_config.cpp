#include "macro_config.h"

#include <iostream>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "util/utils.h"

#include "resource.h"

#include "sequencer/ingseq.h"

// initial static member
KLMacroConfigManager* KLMacroConfigManager::s_Instance = nullptr;
std::once_flag KLMacroConfigManager::s_OnceFlag;

void KLMacroConfigManager::AddConfig(KLMacro& config)
{
	m_ConfigList.push_back(std::move(config));
}
void KLMacroConfigManager::AddConfig(const char* name)
{
	KLMacro config;
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


void SyncActionsFromPairInfo(KLMacro& macro)
{
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

	KLMacro macro;
	macro.name = "default";
	configManager->AddConfig(macro);
	configManager->SetCurrentConfig(0);
}

void ShowMacroConfigManagerWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin(WINNAME_MACRO_CONFIG_MGR, p_open, window_flags)) {
		ImGui::End();
		return;
	}
	ImGui::GetCurrentWindow()->DockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

	auto configManager = KLMacroConfigManager::GetInstance();
	auto imageManager = KLImageManager::GetInstance();

	ImVec2 size(16.0f, 16.0f);

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
		}
		else {
			show_del_confirm = true;
		}
	}
	if (utils::ConfirmEx(&show_del_confirm, u8"删除配置", u8"是否确认删除")) {
		configManager->SetCurrentConfig(0);
		for (auto it = configManager->m_ConfigList.begin(); it != configManager->m_ConfigList.end(); ++it) {
			if (it->name == configManager->GetCurrentConfig().name) {
				configManager->m_ConfigList.erase(it);
				break;
			}
		}
	}

	ImGui::SameLine();

	if (MyButton("btn_edit", size)) {
		strcpy(data, configManager->GetCurrentConfig().name.c_str());
		show_rename = true;
	}
	if (utils::Prompt(&show_new, u8"新建配置", u8"", data, KL_CONFIG_MAX_NAME_SIZE)) {
		if (configManager->IsConfigExists(data)) {
			show_msg_1 = true;
		}
		else {
			configManager->AddConfig(data);
		}
	}
	if (utils::Prompt(&show_rename, u8"重命名", u8"", data, KL_CONFIG_MAX_NAME_SIZE)) {
		if (configManager->GetCurrentConfig().name != data && configManager->IsConfigExists(data)) {
			show_msg_2 = true;
		}
		else {
			configManager->GetCurrentConfig().name = data;
		}
	}

	utils::AlertEx(&show_msg_1, u8"消息##CONFIG_MSG_1", u8"创建失败，配置名已存在");
	utils::AlertEx(&show_msg_2, u8"消息##CONFIG_MSG_2", u8"重命名失败，配置名已存在");
	utils::AlertEx(&show_msg_3, u8"消息##CONFIG_MSG_3", u8"请至少保留一个配置");

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

	ImGui::End();
}


