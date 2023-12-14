#include "menu.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"

#include "layout.h"
#include "resources.h"
#include "keylink.h"


#ifndef IMGUI_DEFINE_MATH_OPERATORS
static ImVec2 operator+(const ImVec2& a, const ImVec2& b) {
	return ImVec2(a.x + b.x, a.y + b.y);
}
static ImVec2 operator+(const ImVec2& a, const float& b) {
	return ImVec2(a.x + b, a.y + b);
}
static ImVec2 operator-(const ImVec2& a, const float& b) {
	return ImVec2(a.x - b, a.y - b);
}
#endif

bool MySelectImageBtn(ImTextureID defaultTexID, ImTextureID hoverTexID, ImVec2 size, bool selected)
{
	auto& io = ImGui::GetIO();

	ImVec2 cursor = ImGui::GetCursorScreenPos();
	ImRect bb(cursor, cursor + size);
	ImVec2 next_cursor = ImVec2(cursor.x, cursor.y + size.y);

	bool hover = bb.Contains(io.MousePos);
	if (hover)
		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

	ImTextureID texID = hover ? hoverTexID : defaultTexID;

	if (selected)
	{
		ImGui::Image(hoverTexID, size);
	}
	else
	{
		ImGui::Image(texID, size);
	}
	ImGui::SetCursorScreenPos(next_cursor);

	return hover && io.MouseClicked[0];
}

void ShowMenuWindow(bool* p_open)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin(WINNAME_MENU, p_open, window_flags)) {
		ImGui::End();
		ImGui::PopStyleVar();	//ImGuiStyleVar_WindowPadding
		return;
	}

	ImGui::GetCurrentWindow()->DockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

	auto imageManager = KLImageManager::GetInstance();
	auto layoutManager = KLWindowLayoutManager::GetInstance();


	auto& imgsetti1 = imageManager->GetImage("frame_setting_gray");
	auto& imgsetti2 = imageManager->GetImage("frame_setting_black");
	auto& imgkbord1 = imageManager->GetImage("frame_keyboard_gray");
	auto& imgkbord2 = imageManager->GetImage("frame_keyboard_blue");
	auto& imglight1 = imageManager->GetImage("frame_lighting_gray");
	auto& imglight2 = imageManager->GetImage("frame_lighting_pink");
	auto& imgmacro1 = imageManager->GetImage("frame_macro_gray");
	auto& imgmacro2 = imageManager->GetImage("frame_macro_orange");

	ImVec2 size((float)imgsetti1.width, (float)imgsetti1.height);

	static int selected = 0;

	if (MySelectImageBtn(imgsetti1.texID, imgsetti2.texID, size, selected == 0))
	{
		selected = 0;
		layoutManager->SetLayoutType(KL_LAYOUT_SETTING);
	}
	if (MySelectImageBtn(imgkbord1.texID, imgkbord2.texID, size, selected == 1))
	{
		selected = 1;
		layoutManager->SetLayoutType(KL_LAYOUT_ASSIGNMENT);

		UpdateWindowDockNodeHeight(WINNAME_FUNCTION, WINHEIGHT_FUNCTION_LIGHT_MODIFY);
		UpdateWindowDockNodeWidth(WINNAME_ASSIGN_CONFIG_MGR, WINWIDTH_LIGHT_CONFIG);
	}
	if (MySelectImageBtn(imglight1.texID, imglight2.texID, size, selected == 2))
	{
		selected = 2;
		layoutManager->SetLayoutType(KL_LAYOUT_LIGHT);

		UpdateWindowDockNodeHeight(WINNAME_LIGHT_MODIFY, WINHEIGHT_FUNCTION_LIGHT_MODIFY);
		UpdateWindowDockNodeWidth(WINNAME_LIGHT, WINWIDTH_LIGHT_CONFIG);
	}
	if (MySelectImageBtn(imgmacro1.texID, imgmacro2.texID, size, selected == 3))
	{
		selected = 3;
		layoutManager->SetLayoutType(KL_LAYOUT_MACRO);

		UpdateWindowDockNodeWidth(WINNAME_MACRO_CONFIG_MGR, WINWIDTH_LIGHT_CONFIG);
	}


	static int fps = 0;
	static int frame_cnt = 0;
	frame_cnt++;
	static long long time = 0;
	long long curr = utils::get_current_system_time_us();
	if (curr - time >= 1000000)
	{
		time = curr;
		fps = frame_cnt;
		frame_cnt = 0;
	}
	ImGui::Text("FPS:%d", fps);

	ImGui::End();
	ImGui::PopStyleVar();	//ImGuiStyleVar_WindowPadding
}