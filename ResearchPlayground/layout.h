#ifndef __KL_LAYOUT_H__
#define __KL_LAYOUT_H__

#include "pch.h"
#include <string>

#include <iostream>
#include <unordered_map>
#include <mutex>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "imgui/extensions/ImFileDialog.h"

enum KLLayoutType
{
	KL_LAYOUT_SETTING,
	KL_LAYOUT_ASSIGNMENT,
	KL_LAYOUT_LIGHT,
	KL_LAYOUT_MACRO,
};


typedef void (*imgui_draw_window_cb)(bool* p_open);



struct KLWindowInfo
{
	std::string name;	//unique
	bool open;
	imgui_draw_window_cb draw;

	ImGuiID id;
	ImGuiWindow* pwin;

	KLWindowInfo(std::string&& name, imgui_draw_window_cb draw)
	{
		this->name = name;
		this->open = false;
		this->draw = draw;

		ImGuiWindow* win = ImGui::FindWindowByName(name.c_str());
		if (win)
		{
			this->pwin = win;
			this->id = win->ID;
		}
	}

	bool operator==(const KLWindowInfo& b)
	{
		return strcmp(this->name.c_str(), b.name.c_str()) == 0;
	}
};

struct KLWindowLayout
{
	KLLayoutType type;
	std::vector<std::string> wnames;
};

class KLWindowLayoutManager
{
public:
	std::unordered_map<KLLayoutType, KLWindowLayout> layouts;
	std::vector<KLWindowInfo> window_table;

	bool isLayoutSwitch = false;

	void KLRegisterLayout(KLLayoutType type, KLWindowInfo& winInfo)
	{
		auto it0 = std::find(window_table.begin(), window_table.end(), winInfo);
		if (it0 == window_table.end())
			window_table.emplace_back(winInfo);

		auto it1 = layouts.find(type);
		if (it1 == layouts.end())
		{
			KLWindowLayout newLayout;
			newLayout.type = type;
			layouts[type] = newLayout;
		}

		auto it2 = std::find(layouts[type].wnames.begin(), layouts[type].wnames.end(), winInfo.name);
		if (it2 == layouts[type].wnames.end())
			layouts[type].wnames.push_back(winInfo.name);
	}

	void SetLayoutType(KLLayoutType type)
	{
		if (type == this->m_CurrentType)
			return;

		this->m_CurrentType = type;
		this->isLayoutSwitch = true;
	}
	KLLayoutType GetLayoutType()
	{
		return this->m_CurrentType;
	}

	static KLWindowLayoutManager* GetInstance()
	{
		std::call_once(s_OnceFlag, []() {
			s_Instance = new KLWindowLayoutManager();
			});
		return s_Instance;
	}
private:
	KLWindowLayoutManager()
		: m_CurrentType(KL_LAYOUT_SETTING)
	{
	}

private:
	KLLayoutType					m_CurrentType;
	static std::once_flag			s_OnceFlag;
	static KLWindowLayoutManager*	s_Instance;
};



class KLDialogManager
{
public:
	void ShowDialog()
	{

	}
private:
	char text[128];
	bool open;

public:
	static KLDialogManager* GetInstance()
	{
		std::call_once(s_OnceFlag, []() {
			s_Instance = new KLDialogManager();
			});
		return s_Instance;
	}
private:
	KLDialogManager();
private:
	static std::once_flag			s_OnceFlag;
	static KLDialogManager* s_Instance;
};

#endif