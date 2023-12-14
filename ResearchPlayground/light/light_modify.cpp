#include "light.h"

#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"

void ShowLightModifyWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
	if (!ImGui::Begin(WINNAME_LIGHT_MODIFY, p_open, window_flags)) {
		ImGui::End();
		return;
	}

	auto dockNode = ImGui::GetCurrentWindow()->DockNode;
	if (dockNode) dockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;



	ImGui::End();
}
