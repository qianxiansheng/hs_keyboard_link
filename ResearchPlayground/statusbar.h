#ifndef __KL_STATUS_BAR_H__
#define __KL_STATUS_BAR_H__

#include "pch.h"

#include <iostream>
#include <vector>
#include <mutex>

#include "protocol/usb.h"

class KLDeviceManager
{
public:
	std::vector<HIDDevice> m_Devices;
	int m_CurrentDeviceIndex;

	void SetCurrentDevice(int i);
	const HIDDevice& GetCurrentDevice() const;
	void AddDevice(HIDDevice& device);
	void RemoveDevice(const HIDDevice& device);
	bool HasDevice() const;
public:
	static KLDeviceManager* GetInstance()
	{
		std::call_once(s_OnceFlag, []() {
			s_Instance = new KLDeviceManager();
			});
		return s_Instance;
	}
	~KLDeviceManager();
private:
	KLDeviceManager() {}
private:
	static std::once_flag   s_OnceFlag;
	static KLDeviceManager* s_Instance;
};

void ShowStatusBarWindow(bool* p_open);

#endif