#pragma once

#include "pch.h"

#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "extensions/ImSequencer.h"
#include "function/function.h"
#include "configuration/macro_config.h"

struct MySequence : public ImSequencer::SequenceInterface
{
	// interface with sequencer

	virtual int GetFrameMin() const;
	virtual int GetFrameMax() const;
	virtual int GetItemCount() const;

	virtual int GetItemTypeCount() const;
	virtual const char* GetItemTypeName(int typeIndex) const;
	virtual const char* GetItemLabel(int index) const;

	virtual void Get(int index, int** start, int** end, int* type, unsigned int* color);
	virtual void Add(int type);
	virtual void Del(int index);
	virtual void Duplicate(int index);

	virtual size_t GetCustomHeight(int index);

	// my datas
	MySequence();
	int mFrameMin, mFrameMax;
	struct MySequenceItem
	{
		char mLabel[16];
		KLFunction mFunction;
		int mFrameStart, mFrameEnd;
		bool mExpanded;
	};
	std::vector<MySequenceItem> myItems;

	virtual void DoubleClick(int index);

	virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect);

	virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect);
};

void ReloadMacroConfig(KLMacro& config);
void UserInputKeyDown(KLFunction& function);
void UserInputKeyUp(KLFunction& function);

