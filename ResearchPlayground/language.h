#ifndef __KL_LANGUAGE_H__
#define __KL_LANGUAGE_H__

#include "pch.h"

#include <string>
#include <mutex>
#include <unordered_map>

enum KLLangType
{
	KLL_TYPE_SIMPLE_CHINESE,
	KLL_TYPE_ENGLISH,

	KLL_TYPE_MAXIMUM,
};

enum KLLangTextKey
{
	KLL_KEY_NONE,

	KLL_KEY_GLOBAL = 1,
	KLL_KEY_DEVICE_INFO,
	KLL_KEY_LANGUAGE,
	KLL_KEY_STARTUP,
	KLL_KEY_EXIT,
	KLL_KEY_SLEEP,
	KLL_KEY_RESET,
	KLL_KEY_DEVICE_MODEL,
	KLL_KEY_CHIP_MODEL,
	KLL_KEY_HARDWARE_VERSION,
	KLL_KEY_SOFTWARE_VERSION,
	KLL_KEY_AUTO_RUN,
	KLL_KEY_WHEN_CLICK_X,
	KLL_KEY_DIRECT_EXIT,
	KLL_KEY_RESIDES_SYSTEM_TRAY,
	KLL_KEY_MINUTE,
	KLL_KEY_SECOND,
	KLL_KEY_TOOLTIP_SLEEP,
	KLL_KEY_RESET_BTN,
	KLL_KEY_RESTORE_FACTORY_SETTINGS,
	KLL_KEY_KEY_ASSIGNMENT,
	KLL_KEY_LIGHT_EFFECT,
	KLL_KEY_EFFECT,
	KLL_KEY_MACRO_EDIT,
	KLL_KEY_PROFILE,
	KLL_KEY_IMPORT,
	KLL_KEY_EXPORT,
	KLL_KEY_RENAME,
	KLL_KEY_DELETE,
	KLL_KEY_NEW,
	KLL_KEY_PREVIEW,

	KLL_KEY_MAXIMUM,
};

class KLLanguageManager
{
public:
	KLLangType currentType;
	std::unordered_map<KLLangType, std::unordered_map<KLLangTextKey, std::string>> map;

	void LoadConfig(const char* filename = LANGUAGE_FILE_NAME);

	void SetLanguage(KLLangType type)
	{
		currentType = type;
	}
	KLLangType GetLanguage()
	{
		return currentType;
	}

	std::string Text(KLLangTextKey key)
	{
		return map[currentType][key];
	}

public:
	static KLLanguageManager* GetInstance()
	{
		std::call_once(s_OnceFlag, []() {
			s_Instance = new KLLanguageManager();
			});
		return s_Instance;
	}
private:
	KLLanguageManager() : currentType(KLL_TYPE_SIMPLE_CHINESE) {}
private:
	static std::once_flag     s_OnceFlag;
	static KLLanguageManager* s_Instance;
};


#define KLLABLEB(key, id) (KLLanguageManager::GetInstance()->Text(key) + "##" + (id)).c_str()
#define KLLABLEA(key) (KLLanguageManager::GetInstance()->Text(key)).c_str()

#endif