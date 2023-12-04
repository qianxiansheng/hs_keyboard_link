#include "ingseq.h"

#include <unordered_map>
#include <Windows.h>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"
#include "function/function.h"

#include "configuration/macro_config.h"

#include "gui_win32_gl3.h"

int MySequence::GetFrameMin() const {
	return mFrameMin;
}
int MySequence::GetFrameMax() const {
	return mFrameMax;
}
int MySequence::GetItemCount() const { return (int)myItems.size(); }

int MySequence::GetItemTypeCount() const { return 0; }
const char* MySequence::GetItemTypeName(int typeIndex) const { return ""; }
const char* MySequence::GetItemLabel(int index) const
{
	static char tmps[16];
	snprintf(tmps, 16, "%s", myItems[index].mLabel);
	return tmps;
}

void MySequence::Get(int index, int** start, int** end, int* type, unsigned int* color)
{
	MySequenceItem& item = myItems[index];
	if (color)
		*color = 0xFFAA8080; // same color for everyone, return color based on type
	if (start)
		*start = &item.mFrameStart;
	if (end)
		*end = &item.mFrameEnd;
	if (type)
		*type = item.mFunction.id;
}
void MySequence::Add(int type) {  };
void MySequence::Del(int index) { myItems.erase(myItems.begin() + index); }
void MySequence::Duplicate(int index) { myItems.push_back(myItems[index]); }

size_t MySequence::GetCustomHeight(int index) { return myItems[index].mExpanded ? 300 : 0; }

// my datas
MySequence::MySequence() : mFrameMin(0), mFrameMax(0) {}

void MySequence::DoubleClick(int index) {
	if (myItems[index].mExpanded)
	{
		myItems[index].mExpanded = false;
		return;
	}
	for (auto& item : myItems)
		item.mExpanded = false;
	myItems[index].mExpanded = !myItems[index].mExpanded;
}

void MySequence::CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect)
{
	static const char* labels[] = { "Translation", "Rotation" , "Scale" };

	draw_list->PushClipRect(legendClippingRect.Min, legendClippingRect.Max, true);
	for (int i = 0; i < 3; i++)
	{
		ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y + i * 14.f);
		ImVec2 ptb(legendRect.Max.x, legendRect.Min.y + (i + 1) * 14.f);
	}
	draw_list->PopClipRect();

	ImGui::SetCursorScreenPos(rc.Min);
}

void MySequence::CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect)
{
}


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

	int64_t timePoint = utils::get_current_system_time_ms() - timeBase;
	if (function.id == KLF_NONE)
		return;
	auto it = key_down_stack.find(function.id);
	if (it != key_down_stack.end())
	{
		auto a = it->second;
		a.mFrameEnd = timePoint;
		mySequence.myItems.push_back(a);

		key_down_stack.erase(it);
	}
}




void ShowINGSequencerWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin(WINNAME_SEQUNCER, p_open, window_flags)) {
		ImGui::End();
		return;
	}

	auto configManager = KLMacroConfigManager::GetInstance();

	if (!IsRecording()) {
		if (ImGui::Button(u8"开始录制")) {
			BeginRecord();
		}
	}
	else
	{
		if (ImGui::Button(u8"结束录制")) {
			EndRecord();
		}
	}

	ImGui::SameLine();

	if (ImGui::Button(u8"保存"))
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
	EnableKeyHook(true);
	ImSequencer::Sequencer(&mySequence, &expanded, &selectedEntry, &firstFrame, ImSequencer::SEQUENCER_EDIT_STARTEND | ImSequencer::SEQUENCER_ADD | ImSequencer::SEQUENCER_DEL | ImSequencer::SEQUENCER_COPYPASTE | ImSequencer::SEQUENCER_CHANGE_FRAME);



	ImGui::End();
}