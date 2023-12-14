#include "statusbar.h"

#include <iostream>
#include <mutex>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "language.h"

#include "protocol/iap.h"

#include "ui_thread.h"
#include "keylink.h"

char label_message[];

KLDeviceManager* KLDeviceManager::s_Instance = nullptr;
std::once_flag KLDeviceManager::s_OnceFlag;


void KLDeviceManager::SetCurrentDevice(int i) {
	this->m_CurrentDeviceIndex = i;
}
const HIDDevice& KLDeviceManager::GetCurrentDevice() const {
	return m_Devices[m_CurrentDeviceIndex];
}
void KLDeviceManager::AddDevice(HIDDevice& device)
{
	m_Devices.push_back(device);
}
void KLDeviceManager::RemoveDevice(const HIDDevice& device)
{
	for (std::vector<HIDDevice>::iterator it = this->m_Devices.begin();
			it != this->m_Devices.end(); ++it)
	{
		if (it->phandle == device.phandle)
		{
			this->m_Devices.erase(it);
			this->m_CurrentDeviceIndex = 0;
			break;
		}
	}
}
bool KLDeviceManager::HasDevice() const
{
	return m_Devices.size() > 0;
}
KLDeviceManager::~KLDeviceManager()
{
	for (auto& dev : m_Devices) {
		CloseHIDInterface(dev);
	}
}

void ShowStatusBarWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
	if (!ImGui::Begin(WINNAME_STATEBAR, p_open, window_flags)) {
		ImGui::End();
		return;
	}

	auto dockNode = ImGui::GetCurrentWindow()->DockNode;
	if (dockNode) dockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

	auto deviceManager = KLDeviceManager::GetInstance();

	{
		if (deviceManager->HasDevice()) 
		{
			ImGui::Text("%s %s", KLLABLEA(KLL_KEY_DEVICE_CONNECTED), "");
		}
		else
		{
			static char dotbuf[7] = "";
			static int dotcnt = 0;
			static int framecnt = 0;
			if (framecnt++ >= 30)
			{
				if (dotcnt == 6)
				{
					dotcnt = 0;
					strcpy(dotbuf, "");
				}
				else
				{
					dotbuf[dotcnt] = '.';
					dotcnt++;
					dotbuf[dotcnt] = '\0';
				}
				framecnt = 0;
			}
			ImGui::Text("%s%s", KLLABLEA(KLL_KEY_WAIT_FOR_CONNECTION), dotbuf);
		}
	}
	ImGui::SameLine();
	ImGuiDCXAxisAlign(DPI(300.0f));
	{
		ImGui::Text(label_message);
	}
	ImGui::SameLine();
	ImGuiDCXAxisAlign(DPI(600.0f));
	{
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
	}

	ImGui::End();
}