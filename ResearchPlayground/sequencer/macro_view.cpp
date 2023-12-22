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

#include "ingseq.h"
#include "keylink.h"


// sequence with default values
MySequence mySequence;
static int currentFrame = 100;
static bool expanded = true;
static int selectedEntry = -1;
static int firstFrame = 0;

static long long timeBase = 0;
static bool is1st = true;
static bool recording = false;


std::unordered_map<KLFunctionID, MySequence::MySequenceItem> key_down_stack;

void StartRecord()
{
	if (is1st) {
		timeBase = utils::get_current_system_time_ms();
		is1st = false;
	}
}


bool IsRecording()
{
	return recording;
}

void BeginRecord()
{
	mySequence.myItems.clear();
	is1st = true;
	recording = true;
}

void EndRecord()
{
	recording = false;
}

void UserInputKeyDown(KLFunction& function)
{
	if (!recording)
		return;

	StartRecord();
	int64_t timePoint = utils::get_current_system_time_ms() - timeBase;
	if (function.id == KLF_NONE)
		return;

	auto it = key_down_stack.find(function.id);
	if (it != key_down_stack.end())
		return;

	auto a = MySequence::MySequenceItem{ "", function, (int)timePoint, (int)timePoint + 1, false };
	strcpy(a.mLabel, function.name);
	key_down_stack[function.id] = a;
}

void UserInputKeyUp(KLFunction& function)
{
	if (!recording)
		return;

	int timePoint = (int)(utils::get_current_system_time_ms() - timeBase);
	if (function.id == KLF_NONE)
		return;
	auto it = key_down_stack.find(function.id);
	if (it != key_down_stack.end())
	{
		auto a = it->second;
		a.mFrameEnd = timePoint;
		mySequence.myItems.push_back(a);

		if (mySequence.mFrameMax < a.mFrameEnd)
		{
			mySequence.mFrameMax = a.mFrameEnd;
		}

		key_down_stack.erase(it);
	}
}

void ReloadMacroConfig(KLMacro& config)
{
	EnableKeyHook(false);
	EndRecord();

	mySequence.myItems.clear();

	for (uint32_t i = 0; i < config.pair_actions.size(); ++i)
	{
		auto& action_pair = config.pair_actions[i];
		auto& function = FindFunctionByFunctionID(action_pair.mFunctionID);
		MySequence::MySequenceItem item = {};
		strcpy(item.mLabel, function.name);
		item.mFunction = function;
		item.mFrameStart = action_pair.mFrameStart;
		item.mFrameEnd = action_pair.mFrameEnd;
		item.mExpanded = false;
		mySequence.myItems.push_back(item);
	}
}

#include "configuration/kl_persistence.h"


void ShowPreviewTab()
{
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

}

void ShowSequencerTab()
{

	auto configManager = KLMacroConfigManager::GetInstance();

	if (!IsRecording()) {
		if (ImGui::Button(KLLABLEB(KLL_KEY_SEQ_RECORD_START, "SEQ_RECORD_START"))) {
			EnableKeyHook(true);
			BeginRecord();
		}
	}
	else
	{
		if (ImGui::Button(KLLABLEB(KLL_KEY_SEQ_RECORD_STOP, "SEQ_RECORD_STOP"))) {
			EnableKeyHook(false);
			EndRecord();
		}
	}

	ImGui::SameLine();

	if (ImGui::Button(KLLABLEB(KLL_KEY_SEQ_RECORD_SAVE, "SEQ_RECORD_SAVE")))
	{
		auto& config = configManager->GetCurrentConfig();

		config.pair_actions.clear();

		for (uint32_t i = 0; i < mySequence.myItems.size(); ++i)
		{
			KLMacroActionPairInfo info = {};
			info.mFrameStart = mySequence.myItems[i].mFrameStart;
			info.mFrameEnd = mySequence.myItems[i].mFrameEnd;
			info.mFunctionID = mySequence.myItems[i].mFunction.id;

			config.pair_actions.push_back(info);
		}

		configManager->SaveConfig();

		SyncActionsFromPairInfo(config);
	}

	//ImGui::PushItemWidth(130);
	//ImGui::InputInt("Frame Min", &mySequence.mFrameMin);
	//ImGui::SameLine();
	//ImGui::InputInt("Frame ", &currentFrame);
	//ImGui::SameLine();
	//ImGui::InputInt("Frame Max", &mySequence.mFrameMax);
	//ImGui::PopItemWidth();
	//ImGui::InputInt("First Frame", &firstFrame);

	auto g = ImGui::GetCurrentContext();

	ImSequencer::Sequencer(&mySequence, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);

}

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
		dockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;
	}

	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MACRO_VIEW_TAB", tab_bar_flags))
	{
		if (ImGui::BeginTabItem(KLLABLEB(KLL_KEY_PREVIEW, "MACRO_VIEW_PREVIEW")))
		{
			ShowPreviewTab();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(KLLABLEB(KLL_KEY_RECORDER, "MACRO_VIEW_SEQUENCER")))
		{
			ShowSequencerTab();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}





	ImGui::End();
}


