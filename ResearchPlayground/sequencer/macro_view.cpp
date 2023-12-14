#include "macro_view.h"

#include <iostream>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "util/utils.h"

#include "function/function.h"
#include "configuration/macro_config.h"
#include "resources.h"
#include "language.h"


void ShowMacroViewWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin(KLLABLEB(KLL_KEY_PREVIEW, WINNAME_MACRO_VIEW), p_open, window_flags)) {
		ImGui::End();
		return;
	}
	auto window = ImGui::GetCurrentWindow();
	auto dockNode = window->DockNode;
	if (dockNode) {
		dockNode->LocalFlags &= ~ImGuiDockNodeFlags_NoTabBar;
		dockNode->LocalFlags |= ImGuiDockNodeFlags_NoWindowMenuButton;
		dockNode->LocalFlags |= ImGuiDockNodeFlags_NoCloseButton;
	}

	auto configManager = KLMacroConfigManager::GetInstance();
	auto imageManager = KLImageManager::GetInstance();

	auto& config = configManager->GetCurrentConfig();

	const float TEXT_BASE_HEIGHT = ImGui::GetTextLineHeightWithSpacing();
	ImVec2 region = ImGui::GetContentRegionAvail();

	ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_SizingStretchProp;
	ImVec2 outer_size = ImVec2(0.0f, region.y);
	if (ImGui::BeginTable("##KLMACRO_VIEW", 4, flags, outer_size))
	{
		ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visibl
		ImGui::TableSetupColumn("No.");
		ImGui::TableSetupColumn("HID");
		ImGui::TableSetupColumn("Action");
		ImGui::TableSetupColumn("Delay(ms)");
		ImGui::TableHeadersRow();
		

		// Demonstrate using clipper for large vertical lists
		ImGuiListClipper clipper;
		clipper.Begin((int)config.actions.size());
		while (clipper.Step())
		{
			for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
			{
				auto& action = config.actions[row];
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%d", row);
				ImGui::TableSetColumnIndex(1);
				ImGui::Text(FindFunctionByFunctionID(action.fid).name);
				ImGui::TableSetColumnIndex(2);
				ImGui::Text(action.type == KL_MACRO_ACTYPE_UP ? "UP" : "DOWN");
				ImGui::TableSetColumnIndex(3);
				ImGui::Text("%d", action.delay);
			}
		}

		ImGui::EndTable();
	}

	ImGui::End();
}


