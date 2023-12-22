#include <iostream>
#include <vector>
#include <functional>
#include <string>

#include <Windows.h>
#include "pch.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "util/utils.h"
#include "util/thread_pool.h"
#include "protocol/usb.h"
#include "ui_thread.h"

#include "layout.h"

#include "keyboard/keyboard.h"
#include "sequencer/ingseq.h"
#include "menu/menu.h"
#include "function/function.h"
#include "light/light.h"
#include "settings/settings.h"
#include "configuration/assignment_config.h"
#include "configuration/macro_config.h"
#include "sequencer/macro_view.h"
#include "statusbar.h"

#include "language.h"

#include "keylink.h"

#include "protocol/iap.h"

#define IMIDTEXT(name, i) ((std::string(name) + std::to_string(i)).c_str())

static char imgui_ini_name[128] = "";

static bool show_root_window = true;
static bool opt_fullscreen = true;
static bool opt_showdemowindow = false;

extern bool alert_flag;
extern char alert_message[];
extern char label_message[];

extern HWND hWnd;

static std::atomic_bool app_exit(false);

static bool AppIsRunning()
{
	return !app_exit.load(std::memory_order_acquire);
}

/******************************************************************************************
 * Application thread pool & Event loop
 *
 *
 ******************************************************************************************/
ThreadPool* pool;


// Search Device
static void SearchDevice(uint16_t vid, uint16_t pid, uint8_t rid, uint32_t milliseconds, HIDDevice* dev)
{
	auto overTime = std::chrono::system_clock::now() +
		std::chrono::milliseconds(milliseconds);

	while (AppIsRunning() && std::chrono::system_clock::now() < overTime)
	{
		if (OpenHIDInterface(vid, pid, rid, dev) == HID_FIND_SUCCESS)
		{
			printf("report ID:%02X\n", dev->reportId);
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}
static void SearchDeviceBootOrAPP(uint16_t bvid, uint16_t bpid, uint16_t avid, uint16_t apid, uint32_t milliseconds, HIDDevice* dev)
{
	auto overTime = std::chrono::system_clock::now() +
		std::chrono::milliseconds(milliseconds);

	while (AppIsRunning() && std::chrono::system_clock::now() < overTime)
	{
		if (OpenHIDInterface(bvid, bpid, dev) == HID_FIND_SUCCESS)
		{
			printf("report ID:%02X\n", dev->reportId);
			break;
		}
		if (OpenHIDInterface(avid, apid, dev) == HID_FIND_SUCCESS)
		{
			printf("report ID:%02X\n", dev->reportId);
			break;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}



extern MySequence mySequence;
extern bool global_setting_x_system_tray;
extern bool global_setting_first_boot;

const ImWchar* GetGlyphRangesChineseFullAndDirection()
{
	static const ImWchar ranges[] =
	{
		0x0020, 0x00FF, // Basic Latin + Latin Supplement
		0x2000, 0x206F, // General Punctuationv
		0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
		0x31F0, 0x31FF, // Katakana Phonetic Extensions
		0xFF00, 0xFFEF, // Half-width characters
		0xFFFD, 0xFFFD, // Invalid
		0x2190, 0x2193, // Direction
		0x4e00, 0x9FAF, // CJK Ideograms
		0,
	};
	return &ranges[0];
}

void BuildImGuiFont()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	// 添加第二个字体，设置优先级为1
	ImFontConfig config;
	config.MergeMode = false;
	config.PixelSnapH = true;
	config.OversampleH = 3;
	config.OversampleV = 1;
	io.Fonts->AddFontFromFileTTF(utils::getFileAbsolutePath(EN_FONT).c_str(), 13.0f, &config, NULL);

	// 添加第三个字体，设置优先级为2
	config.MergeMode = true;
	config.PixelSnapH = true;
	config.OversampleH = 2;
	config.OversampleV = 2;
	io.Fonts->AddFontFromFileTTF(ZH_FONT, 13.0f, &config, GetGlyphRangesChineseFullAndDirection());

	io.Fonts->Build();
}

void BuildLayoutManager()
{
	KLWindowInfo menu_win_info(WINNAME_MENU, ShowMenuWindow);
	KLWindowInfo kbod_win_info(WINNAME_KEYBOARD, ShowKeyboardWindow);
	KLWindowInfo func_win_info(WINNAME_FUNCTION, ShowFunctionWindow);
	KLWindowInfo sett_win_info(WINNAME_SETTINGS, ShowSettingsWindow);
	KLWindowInfo stat_win_info(WINNAME_STATEBAR, ShowStatusBarWindow);
	KLWindowInfo light_win_info(WINNAME_LIGHT, ShowLightWindow);
	KLWindowInfo light_modeify_win_info(WINNAME_LIGHT_MODIFY, ShowLightModifyWindow);
	KLWindowInfo assign_cfg_mgr_win_info(WINNAME_ASSIGN_CONFIG_MGR, ShowAssignmentConfigManagerWindow);
	KLWindowInfo macro_cfg_mgr_win_info(WINNAME_MACRO_CONFIG_MGR, ShowMacroConfigManagerWindow);
	KLWindowInfo macro_view_win_info(WINNAME_MACRO_VIEW, ShowMacroViewWindow);

	auto layoutManager = KLWindowLayoutManager::GetInstance();

	// Global settings
	layoutManager->KLRegisterLayout(KL_LAYOUT_SETTING, menu_win_info);
	layoutManager->KLRegisterLayout(KL_LAYOUT_SETTING, sett_win_info);
	layoutManager->KLRegisterLayout(KL_LAYOUT_SETTING, stat_win_info);
	

	// key assignment
	layoutManager->KLRegisterLayout(KL_LAYOUT_ASSIGNMENT, menu_win_info);
	layoutManager->KLRegisterLayout(KL_LAYOUT_ASSIGNMENT, kbod_win_info);
	layoutManager->KLRegisterLayout(KL_LAYOUT_ASSIGNMENT, func_win_info);
	layoutManager->KLRegisterLayout(KL_LAYOUT_ASSIGNMENT, assign_cfg_mgr_win_info);
	layoutManager->KLRegisterLayout(KL_LAYOUT_ASSIGNMENT, stat_win_info);

	// light
	layoutManager->KLRegisterLayout(KL_LAYOUT_LIGHT, menu_win_info);
	layoutManager->KLRegisterLayout(KL_LAYOUT_LIGHT, kbod_win_info);
	layoutManager->KLRegisterLayout(KL_LAYOUT_LIGHT, light_win_info);
	layoutManager->KLRegisterLayout(KL_LAYOUT_LIGHT, light_modeify_win_info);
	layoutManager->KLRegisterLayout(KL_LAYOUT_LIGHT, stat_win_info);

	// macro
	layoutManager->KLRegisterLayout(KL_LAYOUT_MACRO, menu_win_info);
	layoutManager->KLRegisterLayout(KL_LAYOUT_MACRO, macro_view_win_info);
	layoutManager->KLRegisterLayout(KL_LAYOUT_MACRO, macro_cfg_mgr_win_info);
	layoutManager->KLRegisterLayout(KL_LAYOUT_MACRO, stat_win_info);
}

void StartMonitorDeviceConnectivity(const HIDDevice& hidDevice);
void StartScanDevice();

void MonitorDeviceConnectivityTask(const HIDDevice& hidDevice)
{
	while (AppIsRunning())
	{
		/* Send ping cmd */
		if (!drv_ping(hidDevice))
		{
			showLabel("The device has been lost.");

			KLDeviceManager::GetInstance()->RemoveDevice(hidDevice);

			std::this_thread::sleep_for(std::chrono::milliseconds(1000));

			StartScanDevice();
			
			break;
		}

		/* Delay 2000ms */
		for (uint8_t i = 0; i < 20; ++i)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			if (!AppIsRunning())
				return;
		}
	}
}

void ScanDeviceTask()
{
	HIDDevice hid = { 0 };
	SearchDevice(DEVICE_APP_VID, DEVICE_APP_PID, DEVICE_APP_RID, 1800000, &hid);

	if (hid.phandle == NULL)
	{
		showAlert_("Open Device Failed!");
	}
	else
	{
		showLabel("Open Success! APP");

		auto deviceManager = KLDeviceManager::GetInstance();
		deviceManager->AddDevice(hid);
		deviceManager->SetCurrentDevice(0);		//TODO

		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		StartMonitorDeviceConnectivity(hid);
	}
}

void StartMonitorDeviceConnectivity(const HIDDevice& hidDevice)
{
	pool->enqueue(MonitorDeviceConnectivityTask, hidDevice);
}

void StartScanDevice()
{
	pool->enqueue(ScanDeviceTask);
}

bool main_init(int argc, char* argv[])
{
	// Enable Dock
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	//style.Colors[ImGuiCol_MenuBarBg] = ImVec4();

	// ImGui ini
	strcpy(imgui_ini_name, utils::getFileAbsolutePath("imgui.ini").c_str());
	io.IniFilename = imgui_ini_name;

	// Theme
	ImGui::StyleColorsLight();

	// Thread pool
	pool = new ThreadPool(10);

	// UI Thread
	InitUIThread();

	// Font
	BuildImGuiFont();

	// Layout Manager
	BuildLayoutManager();
	
	// Sequencer
	mySequence.mFrameMin = 0;
	mySequence.mFrameMax = 50000;

	// Window Init
	InitSettingsWindow();
	InitAssignmentConfigManagerWindow();
	InitMacroConfigWindow();
	KLLanguageManager::GetInstance()->LoadConfig();

	// HID
	HIDInit();

	// Start Search Device
	StartScanDevice();

	// Auto Hide
	if (global_setting_first_boot)
	{
		global_setting_first_boot = false;
	}
	else
	{
		ShowWindow(hWnd, SW_HIDE);
		HideApplication();
	}

    return true;
}

void main_shutdown(void)
{
	app_exit.store(true, std::memory_order_release);
	DestroyUIThread();
	HIDExit();

	delete pool;
}


#ifndef IMGUI_DEFINE_MATH_OPERATORS
static ImVec2 operator+(const ImVec2& a, const ImVec2& b) {
	return ImVec2(a.x + b.x, a.y + b.y);
}
static ImVec2 operator+(const ImVec2& a, const int b) {
	return ImVec2(a.x + b, a.y + b);
}
#endif

static void CustomizeTitle(int menuHeight)
{
	auto window = ImGui::GetCurrentWindow();
	auto rect = window->Rect();

	int itemHeight = menuHeight;
	int itemMargin = 1;
	int dragEdgeWidth = 3;
	int edgeWidth = 1;
	int triSize = 50;

	ImGuiIO& io = ImGui::GetIO();
	ImDrawList* draw_list = ImGui::GetForegroundDrawList();

	ImRect regionRect = window->Rect();
	ImRect titleRect(regionRect.Min, ImVec2(regionRect.Max.x, regionRect.Min.y + itemHeight));

	ImRect dragMoveRect(ImVec2(titleRect.Min.x, titleRect.Min.y + dragEdgeWidth), titleRect.Max);
	ImRect dragResizeVTRect(regionRect.Min, ImVec2(regionRect.Max.x, regionRect.Min.y + dragEdgeWidth));
	ImRect dragResizeVBRect(ImVec2(regionRect.Min.x, regionRect.Max.y - dragEdgeWidth), regionRect.Max);
	ImRect dragResizeHLRect(regionRect.Min, ImVec2(regionRect.Min.x + dragEdgeWidth, regionRect.Max.y));
	ImRect dragResizeHRRect(ImVec2(regionRect.Max.x - dragEdgeWidth, regionRect.Min.y), regionRect.Max);
	ImRect dragResizeTriRect(ImVec2(regionRect.Max.x - triSize, regionRect.Max.y - triSize), regionRect.Max);

	static ImVec2 prevMousePos;

	int minWidth = (int)DPI(150);
	int minHeight = (int)DPI(100);

	// Drag move
	static bool isDraging = false;
	if (dragMoveRect.Contains(io.MousePos))
	{
		if (io.MouseClicked[0])
		{
			isDraging = true;
			prevMousePos = io.MousePos;
			SetCapture(hWnd);
		}
		if (io.MouseDoubleClicked[0])
		{
			if (IsZoomed(hWnd))
				ShowWindow(hWnd, SW_RESTORE);
			else
				ShowWindow(hWnd, SW_MAXIMIZE);
		}
	}
	if (isDraging)
	{
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeAll);

		int deltaX = (int)(io.MousePos.x - prevMousePos.x);
		int deltaY = (int)(io.MousePos.y - prevMousePos.y);
		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);
		MoveWindow(hWnd,
			windowRect.left + deltaX, 
			windowRect.top + deltaY,
			windowRect.right - windowRect.left, 
			windowRect.bottom - windowRect.top, true);
		prevMousePos = io.MousePos;

		if (io.MouseReleased[0])
		{
			isDraging = false;
			ReleaseCapture();
		}
	}

	// Drag resize Horizontal Right
	static bool isDragingResizeHR = false;
	if (dragResizeHRRect.Contains(io.MousePos))
	{
		draw_list->AddRectFilled(dragResizeHRRect.Min, dragResizeHRRect.Max, 0x2FFFFF00);
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
		if (io.MouseClicked[0])
		{
			isDragingResizeHR = true;
			prevMousePos = io.MousePos;
		}
	}
	if (isDragingResizeHR)
	{
		draw_list->AddRectFilled(dragResizeHRRect.Min, dragResizeHRRect.Max, 0xFFFFFF00);
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
		int deltaX = (int)(io.MousePos.x - prevMousePos.x);
		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);

		int diff = (windowRect.right - windowRect.left + deltaX) - minWidth;

		if (diff < 0)
			deltaX -= diff;

		MoveWindow(hWnd,
			windowRect.left,
			windowRect.top,
			windowRect.right - windowRect.left + deltaX,
			windowRect.bottom - windowRect.top, TRUE);

		if (diff < 0)
			prevMousePos.x = (float)windowRect.right + deltaX;
		else
			prevMousePos = io.MousePos;

		if (io.MouseReleased[0])
			isDragingResizeHR = false;
	}

	// Drag resize Horizontal Left
	static bool isDragingResizeHL = false;
	if (dragResizeHLRect.Contains(io.MousePos))
	{
		draw_list->AddRectFilled(dragResizeHLRect.Min, dragResizeHLRect.Max, 0x2FFFFF00);
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
		if (io.MouseClicked[0])
		{
			isDragingResizeHL = true;
			prevMousePos = io.MousePos;
		}
	}
	if (isDragingResizeHL)
	{
		draw_list->AddRectFilled(dragResizeHLRect.Min, dragResizeHLRect.Max, 0xFFFFFF00);
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
		int deltaX = (int)(io.MousePos.x - prevMousePos.x);
		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);

		int diff = (windowRect.right - windowRect.left - deltaX) - minWidth;

		if (diff < 0)
			deltaX += diff;

		MoveWindow(hWnd,
			windowRect.left + deltaX,
			windowRect.top,
			windowRect.right - windowRect.left - deltaX,
			windowRect.bottom - windowRect.top, TRUE);

		if (diff < 0)
			prevMousePos.x = (float)windowRect.left + deltaX;
		else
			prevMousePos = io.MousePos;

		if (io.MouseReleased[0])
			isDragingResizeHL = false;
	}

	// Drag resize Vertical Bottom
	static bool isDragingResizeVB = false;
	if (dragResizeVBRect.Contains(io.MousePos))
	{
		draw_list->AddRectFilled(dragResizeVBRect.Min, dragResizeVBRect.Max, 0x2FFFFF00);
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
		if (io.MouseClicked[0])
		{
			isDragingResizeVB = true;
			prevMousePos = io.MousePos;
		}
	}
	if (isDragingResizeVB)
	{
		draw_list->AddRectFilled(dragResizeVBRect.Min, dragResizeVBRect.Max, 0xFFFFFF00);
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
		int deltaY = (int)(io.MousePos.y - prevMousePos.y);
		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);

		int diff = (windowRect.bottom - windowRect.top + deltaY) - minHeight;

		if (diff < 0)
			deltaY -= diff;

		MoveWindow(hWnd,
			windowRect.left,
			windowRect.top,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top + deltaY, TRUE);

		if (diff < 0)
			prevMousePos.y = (float)windowRect.bottom + deltaY;
		else
			prevMousePos = io.MousePos;

		if (io.MouseReleased[0])
			isDragingResizeVB = false;
	}

	// Drag resize Vertical Top
	static bool isDragingResizeVT = false;
	if (dragResizeVTRect.Contains(io.MousePos))
	{
		draw_list->AddRectFilled(dragResizeVTRect.Min, dragResizeVTRect.Max, 0x2FFFFF00);
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
		if (io.MouseClicked[0])
		{
			isDragingResizeVT = true;
			prevMousePos = io.MousePos;
		}
	}
	if (isDragingResizeVT)
	{
		draw_list->AddRectFilled(dragResizeVTRect.Min, dragResizeVTRect.Max, 0xFFFFFF00);
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
		int deltaY = (int)(io.MousePos.y - prevMousePos.y);
		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);

		int diff = (windowRect.bottom - windowRect.top - deltaY) - minHeight;

		if (diff < 0)
			deltaY += diff;

		MoveWindow(hWnd,
			windowRect.left,
			windowRect.top + deltaY,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top - deltaY, TRUE);

		if (diff < 0)
			prevMousePos.y = (float)windowRect.top + deltaY;
		else
			prevMousePos = io.MousePos;

		if (io.MouseReleased[0])
			isDragingResizeVT = false;
	}

	ImVec2 p1(dragResizeTriRect.Max.x, dragResizeTriRect.Min.y);
	ImVec2 p2(dragResizeTriRect.Max.x, dragResizeTriRect.Max.y);
	ImVec2 p3(dragResizeTriRect.Min.x, dragResizeTriRect.Max.y);
	static bool isDragingResizeTri = false;
	if (dragResizeTriRect.Contains(io.MousePos)) {
		draw_list->AddTriangleFilled(p1, p2, p3, 0x2FFFFF00);
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNWSE);
		if (io.MouseClicked[0])
		{
			isDragingResizeTri = true;
			prevMousePos = io.MousePos;
		}
	}
	if (isDragingResizeTri) {
		draw_list->AddTriangleFilled(p1, p2, p3, 0xFFFFFF00);
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNWSE);
		int deltaX = (int)(io.MousePos.x - prevMousePos.x);
		int deltaY = (int)(io.MousePos.y - prevMousePos.y);
		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);

		int diffX = (windowRect.right - windowRect.left + deltaX) - minWidth;
		int diffY = (windowRect.bottom - windowRect.top + deltaY) - minHeight;

		if (diffX < 0)
			deltaX -= diffX;
		if (diffY < 0)
			deltaY -= diffY;

		MoveWindow(hWnd,
			windowRect.left,
			windowRect.top,
			windowRect.right - windowRect.left + deltaX,
			windowRect.bottom - windowRect.top + deltaY, TRUE);

		if (diffX < 0)
			prevMousePos.x = (float)windowRect.right + deltaX;
		else
			prevMousePos.x = io.MousePos.x;

		if (diffY < 0)
			prevMousePos.y = (float)windowRect.bottom + deltaY;
		else
			prevMousePos.y = io.MousePos.y;

		if (io.MouseReleased[0])
			isDragingResizeTri = false;
	}


	// Draw button
	auto drawItem = [&](ImVec2&& pos, ImU32 col, ImU32 col2, ImU32 col3, bool* click_status, const std::function<void(void)>& func) {
		ImRect itemRect = ImRect(pos, pos + itemHeight);

		float radius = ((float)itemHeight / 2.0f) - itemMargin;

		if (itemRect.Contains(io.MousePos))
		{
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

			if (io.MouseDown[0])
				draw_list->AddCircleFilled(itemRect.Min + (itemHeight >> 1), radius, col3);
			else
				draw_list->AddCircleFilled(itemRect.Min + (itemHeight >> 1), radius, col2);

			if (io.MouseClicked[0]) *click_status = true;
			if (io.MouseReleased[0] && *click_status) func();
		}
		else
		{
			draw_list->AddCircleFilled(itemRect.Min + (itemHeight >> 1), radius, col);
		}
	};

	// Close button
	static bool btnMouseDown1 = false;
	drawItem(ImVec2(titleRect.Max.x - itemHeight, titleRect.Min.y), 0xFF0000FF, 0xFF0000BF, 0xFF00007F, &btnMouseDown1, [&](void) {
		if (global_setting_x_system_tray) {
			ShowWindow(hWnd, SW_HIDE);
			HideApplication();
		}
		else {
			PostQuitMessage(0);
		}
	});

	// Maximize button
	static bool btnMouseDown2 = false;
	drawItem(ImVec2(titleRect.Max.x - (2 * itemHeight), titleRect.Min.y), 0xFF00FFFF, 0xFF00BFBF, 0xFF007F7F, &btnMouseDown2, [&](void) {
		if (IsZoomed(hWnd))
			ShowWindow(hWnd, SW_RESTORE);
		else
			ShowWindow(hWnd, SW_MAXIMIZE);
	});

	// Minimize button
	static bool btnMouseDown3 = false;
	drawItem(ImVec2(titleRect.Max.x - (3 * itemHeight), titleRect.Min.y), 0xFF00FF00, 0xFF00BF00, 0xFF007F00, &btnMouseDown3, [&](void) {
		ShowWindow(hWnd, SW_MINIMIZE);
	});

	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4 menuBarBg = style.Colors[ImGuiCol_MenuBarBg];
	ImU32 color = ImGui::ColorConvertFloat4ToU32(menuBarBg);

	ImRect edgeVTRect(regionRect.Min, ImVec2(regionRect.Max.x, regionRect.Min.y + edgeWidth));
	ImRect edgeVBRect(ImVec2(regionRect.Min.x, regionRect.Max.y - edgeWidth), regionRect.Max);
	ImRect edgeHLRect(regionRect.Min, ImVec2(regionRect.Min.x + edgeWidth, regionRect.Max.y));
	ImRect edgeHRRect(ImVec2(regionRect.Max.x - edgeWidth, regionRect.Min.y), regionRect.Max);
	draw_list->AddRectFilled(edgeVTRect.Min, edgeVTRect.Max, color);
	draw_list->AddRectFilled(edgeVBRect.Min, edgeVBRect.Max, color);
	draw_list->AddRectFilled(edgeHLRect.Min, edgeHLRect.Max, color);
	draw_list->AddRectFilled(edgeHRRect.Min, edgeHRRect.Max, color);
}

static void UpdateMenuBarColor()
{
	static int frame_cnt = 0;
	static double H = 0;
	const double delta = 1.0 / 360.0;
	const double S = 0.5;
	const double V = 0.5;
	const int frame_threshold = 1;
	frame_cnt++;

	if (frame_cnt > frame_threshold)
	{
		frame_cnt = 0;
		ImVec4 color = {0, 0, 0, 1};
		ImGui::ColorConvertHSVtoRGB(H, S, V, color.x, color.y, color.z);
		ImGuiStyle& style = ImGui::GetStyle();
		style.Colors[ImGuiCol_MenuBarBg] = color;

		H += delta;
		if (H >= 1.0)
			H = 0.0;
	}
}

static void ShowRootWindowMenu()
{
	UpdateMenuBarColor();

	ImVec2 size;
	if (ImGui::BeginMenuBar()) {

		if (ImGui::BeginMenu("View")) {

			ImGui::MenuItem("Demo Window", NULL, &opt_showdemowindow);
			ImGui::Separator();
			//auto layoutManager = KLWindowLayoutManager::GetInstance();
			//auto& wtbl = layoutManager->window_table;
			//for (auto itw = wtbl.begin(); itw != wtbl.end(); ++itw)
			//{
			//	ImGui::MenuItem(itw->name.c_str(), NULL, &itw->open);
			//}
			ImGui::EndMenu();
		}

		size = ImGui::GetItemRectSize();

		ImGui::EndMenuBar();
	}
	CustomizeTitle(size.y);
}

static void ShowRootWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	if (opt_fullscreen) {
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	}
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("-", p_open, window_flags);

	if (opt_fullscreen) {
		ImGui::PopStyleVar();	// WindowBorderSize
		ImGui::PopStyleVar();	// WindowRounding
	}
	ImGui::PopStyleVar();	// WindowPadding

	ImGuiID dockspace_id = ImGui::GetID(DOCKSPACE_ID);
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_AutoHideTabBar | ImGuiDockNodeFlags_NoUndocking | ImGuiDockNodeFlags_NoResize);

	ShowRootWindowMenu();

	utils::AlertEx(&alert_flag, KLLABLEB(KLL_KEY_DIALOG_MSG, "UIMSG"), alert_message);

	ImGui::End();

	if (opt_showdemowindow) {
		ImGui::ShowDemoWindow(&opt_showdemowindow);
	}


	// 根据Layout显示窗口
	auto layoutManager = KLWindowLayoutManager::GetInstance();

	bool isLayoutSwitch = false;
	if (layoutManager->isLayoutSwitch)
	{
		layoutManager->isLayoutSwitch = false;
		isLayoutSwitch = true;
	}

	auto& wids = layoutManager->layouts[layoutManager->GetLayoutType()].wnames;
	auto& wtbl = layoutManager->window_table;

	for (auto itw = wtbl.begin(); itw != wtbl.end(); ++itw) {
		for (auto it = wids.begin(); it != wids.end(); ++it) {
			if (*it == itw->name) {
				itw->draw(&itw->open);
			}
		}
	}

	if (isLayoutSwitch)
	{
		if (KL_LAYOUT_ASSIGNMENT == layoutManager->GetLayoutType())
		{
			UpdateWindowDockNodeHeight(WINNAME_FUNCTION, WINHEIGHT_FUNCTION_LIGHT_MODIFY);
			UpdateWindowDockNodeWidth(WINNAME_ASSIGN_CONFIG_MGR, WINWIDTH_LIGHT_CONFIG);
		}
		else if (KL_LAYOUT_LIGHT == layoutManager->GetLayoutType())
		{
			UpdateWindowDockNodeHeight(WINNAME_LIGHT_MODIFY, WINHEIGHT_FUNCTION_LIGHT_MODIFY);
			UpdateWindowDockNodeWidth(WINNAME_LIGHT, WINWIDTH_LIGHT_CONFIG);
		}
		else if (KL_LAYOUT_MACRO == layoutManager->GetLayoutType())
		{
			UpdateWindowDockNodeWidth(WINNAME_MACRO_CONFIG_MGR, WINWIDTH_LIGHT_CONFIG);
		}
	}
}


int main_gui()
{
	ShowRootWindow(&show_root_window);

	return 0;
}
