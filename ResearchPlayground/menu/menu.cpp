#include "menu.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"

#include "layout.h"
#include "resource.h"

void ShowMenuWindow(bool* p_open)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin(WINNAME_MENU, p_open, window_flags)) {
		ImGui::End();
		ImGui::PopStyleVar();	//ImGuiStyleVar_WindowPadding
		return;
	}
	auto imageManager = KLImageManager::GetInstance();
	auto layoutManager = KLWindowLayoutManager::GetInstance();

	static int selected = -1;
		
	auto& imgSetting = imageManager->GetImage("frame_setting");
	if (ImGui::ImageButton(imgSetting.texID, ImVec2(imgSetting.width, imgSetting.height))) 
	{
		layoutManager->SetLayoutType(KL_LAYOUT_SETTING);
	}

	auto& imgKB = imageManager->GetImage("frame_keyboard");
	if (ImGui::ImageButton(imgKB.texID, ImVec2(imgKB.width, imgKB.height))) 
	{
		layoutManager->SetLayoutType(KL_LAYOUT_ASSIGNMENT);
	}

	auto& imgLight = imageManager->GetImage("frame_lighting");
	if (ImGui::ImageButton(imgLight.texID, ImVec2(imgLight.width, imgLight.height))) 
	{
		layoutManager->SetLayoutType(KL_LAYOUT_LIGHT);
	}

	auto& imgMacro = imageManager->GetImage("frame_macro");
	if (ImGui::ImageButton(imgMacro.texID, ImVec2(imgMacro.width, imgMacro.height))) 
	{
		layoutManager->SetLayoutType(KL_LAYOUT_MACRO);
	}

	ImGui::End();
	ImGui::PopStyleVar();	//ImGuiStyleVar_WindowPadding
}