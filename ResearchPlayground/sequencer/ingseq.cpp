#include "ingseq.h"

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
