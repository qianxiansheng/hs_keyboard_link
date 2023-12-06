#include "program_setting.h"

#include <iostream>
#include <Windows.h>

#define REGISTRY_PATH "Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define REGISTRY_NAME "IngchipsKBLink"

bool AutoRunRegisterCheck()
{
	// ��ȡ��ǰ��ִ���ļ���·��
	char buffer1[MAX_PATH];
	char buffer2[MAX_PATH];
	GetModuleFileNameA(NULL, buffer1, MAX_PATH);

	HKEY hKey;
	LONG result = RegOpenKeyExA(HKEY_CURRENT_USER, REGISTRY_PATH, 0, KEY_READ, &hKey);

	bool isRegiestered = false;

	if (result == ERROR_SUCCESS) {
		// ����Ƿ����ָ����ע�����
		DWORD dataType;
		DWORD dataSize;
		result = RegQueryValueExA(hKey, REGISTRY_NAME, nullptr, &dataType, nullptr, &dataSize);

		if (result == ERROR_SUCCESS) {

			result = RegQueryValueExA(hKey, REGISTRY_NAME, nullptr, nullptr, reinterpret_cast<LPBYTE>(buffer2), &dataSize);
			
			if (result == ERROR_SUCCESS) {
				std::cout << "��ѯ����ֵΪ: " << buffer2 << std::endl;
			
				if (strcmp(buffer1, buffer2) == 0)
				{
					std::cout << "��ע�Ὺ���Զ�����" << std::endl;
					isRegiestered = true;
				}
				else
				{
					std::cout << "·����ƥ�䣬��Ҫ����ע��" << std::endl;
				}
			}
			else {
				std::cerr << "�޷���ѯע���ֵ" << std::endl;
			}
		}
		else {
			std::cout << "����δע�Ὺ���Զ�����" << std::endl;
		}

		// �ر�ע�����
		RegCloseKey(hKey);
	}
	else {
		std::cerr << "�޷���ע�����" << std::endl;
	}

	return isRegiestered;
}

int AutoRunRegisterDel()
{
	// ��ע�����
	HKEY hKey;
	if (RegOpenKeyExA(HKEY_CURRENT_USER, REGISTRY_PATH, 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
		// ɾ��ע�����
		if (RegDeleteValueA(hKey, REGISTRY_NAME) == ERROR_SUCCESS) {
			std::cout << "�����������ѳɹ�ȡ��" << std::endl;
		}
		else {
			std::cerr << "ȡ������������ʱ���ִ���" << std::endl;
		}

		// �ر�ע�����
		RegCloseKey(hKey);
	}
	else {
		std::cerr << "�޷���ע�����" << std::endl;
	}

	return 0;
}

int AutoRunRegisterSet()
{
	// ��ȡ��ǰ��ִ���ļ���·��
	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	// ����ע���·��

	// ���ÿ����Զ�����
	HKEY hKey;
	LONG result = RegCreateKeyExA(HKEY_CURRENT_USER, REGISTRY_PATH, 0, NULL, 0, KEY_SET_VALUE, NULL, &hKey, NULL);
	if (result == ERROR_SUCCESS) {
		result = RegSetValueExA(hKey, REGISTRY_NAME, 0, REG_SZ, (BYTE*)buffer, (DWORD)(strlen(buffer) * sizeof(char)));
		RegCloseKey(hKey);

		if (result == ERROR_SUCCESS) {
			std::wcout << L"����������Ϊ�����Զ�������" << std::endl;
		}
		else {
			std::wcerr << L"���ÿ����Զ�����ʧ�ܣ������룺" << result << std::endl;
		}
	}
	else {
		std::wcerr << L"��ע����ʧ�ܣ������룺" << result << std::endl;
	}

	return 0;
}
