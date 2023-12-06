#include "program_setting.h"

#include <iostream>
#include <Windows.h>

#define REGISTRY_PATH "Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define REGISTRY_NAME "IngchipsKBLink"

bool AutoRunRegisterCheck()
{
	// 获取当前可执行文件的路径
	char buffer1[MAX_PATH];
	char buffer2[MAX_PATH];
	GetModuleFileNameA(NULL, buffer1, MAX_PATH);

	HKEY hKey;
	LONG result = RegOpenKeyExA(HKEY_CURRENT_USER, REGISTRY_PATH, 0, KEY_READ, &hKey);

	bool isRegiestered = false;

	if (result == ERROR_SUCCESS) {
		// 检查是否存在指定的注册表项
		DWORD dataType;
		DWORD dataSize;
		result = RegQueryValueExA(hKey, REGISTRY_NAME, nullptr, &dataType, nullptr, &dataSize);

		if (result == ERROR_SUCCESS) {

			result = RegQueryValueExA(hKey, REGISTRY_NAME, nullptr, nullptr, reinterpret_cast<LPBYTE>(buffer2), &dataSize);
			
			if (result == ERROR_SUCCESS) {
				std::cout << "查询到的值为: " << buffer2 << std::endl;
			
				if (strcmp(buffer1, buffer2) == 0)
				{
					std::cout << "已注册开机自动运行" << std::endl;
					isRegiestered = true;
				}
				else
				{
					std::cout << "路径不匹配，需要重新注册" << std::endl;
				}
			}
			else {
				std::cerr << "无法查询注册表值" << std::endl;
			}
		}
		else {
			std::cout << "程序未注册开机自动启动" << std::endl;
		}

		// 关闭注册表项
		RegCloseKey(hKey);
	}
	else {
		std::cerr << "无法打开注册表项" << std::endl;
	}

	return isRegiestered;
}

int AutoRunRegisterDel()
{
	// 打开注册表项
	HKEY hKey;
	if (RegOpenKeyExA(HKEY_CURRENT_USER, REGISTRY_PATH, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
		// 删除注册表项
		if (RegDeleteValueA(hKey, REGISTRY_NAME) == ERROR_SUCCESS) {
			std::cout << "开机启动项已成功取消" << std::endl;
		}
		else {
			std::cerr << "取消开机启动项时出现错误" << std::endl;
		}

		// 关闭注册表项
		RegCloseKey(hKey);
	}
	else {
		std::cerr << "无法打开注册表项" << std::endl;
	}

	return 0;
}

int AutoRunRegisterSet()
{
	// 获取当前可执行文件的路径
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	// 构建注册表路径

	// 设置开机自动启动
	HKEY hKey;
	LONG result = RegCreateKeyExA(HKEY_CURRENT_USER, REGISTRY_PATH, 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL);
	if (result == ERROR_SUCCESS) {
		result = RegSetValueExA(hKey, REGISTRY_NAME, 0, REG_SZ, (BYTE*)buffer, (DWORD)(strlen(buffer) * sizeof(char)));
		RegCloseKey(hKey);

		if (result == ERROR_SUCCESS) {
			std::wcout << L"程序已设置为开机自动启动。" << std::endl;
		}
		else {
			std::wcerr << L"设置开机自动启动失败，错误码：" << result << std::endl;
		}
	}
	else {
		std::wcerr << L"打开注册表键失败，错误码：" << result << std::endl;
	}

	return 0;
}
