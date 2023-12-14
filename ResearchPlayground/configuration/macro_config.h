#ifndef __KL_MACRO_CONFIG_H__
#define __KL_MACRO_CONFIG_H__

#include "pch.h"
#include <iostream>
#include <mutex>
#include <vector>

#include "function/function.h"
#include "keyboard/keyboard.h"

enum KLFunctionID;

enum KLMacroActionType
{
    KL_MACRO_ACTYPE_DOWN,
    KL_MACRO_ACTYPE_UP,
};

struct KLMacroAction
{
    KLMacroActionType type;
    uint32_t delay;
    KLFunctionID fid;

	KLMacroAction(KLMacroActionType _type, uint16_t _delay, KLFunctionID _fid) 
		: type(_type), delay(_delay), fid(_fid) {};
};

struct KLMacroActionPairInfo
{
    int mFrameStart, mFrameEnd;
    KLFunctionID mFunctionID;
};

struct KLMacro
{
	int id;
	std::string name;
	std::vector<KLMacroActionPairInfo> pair_actions;
	std::vector<KLMacroAction> actions;
};

class KLMacroConfigManager
{
public:
	std::vector<KLMacro> m_ConfigList;
	uint32_t m_CurrentConfigIndex = 0;

	void AddConfig(KLMacro& config);
	void AddConfig(const char* name);

	bool IsConfigExists(const char* name);
	bool RemoveCurrentConfig();
	bool RemoveConfig(const char* name);

	void SaveConfig(const char* filename = MACROS_FILE_NAME);
	void LoadConfig(const char* filename = MACROS_FILE_NAME);

	void SetCurrentConfig(uint32_t i);
	KLMacro& GetCurrentConfig();
	KLMacro* GetConfigByID(uint8_t id);
private:
	uint8_t FindUnusedID();
public:
	static KLMacroConfigManager* GetInstance()
	{
		std::call_once(s_OnceFlag, []() {
			s_Instance = new KLMacroConfigManager();
			});
		return s_Instance;
	}
private:
	static std::once_flag        s_OnceFlag;
	static KLMacroConfigManager* s_Instance;
};

void SyncActionsFromPairInfo(KLMacro& macro);
void SyncActionsToPairInfo(KLMacro& macro);

void InitMacroConfigWindow();
void ShowMacroConfigManagerWindow(bool* p_open);

#endif