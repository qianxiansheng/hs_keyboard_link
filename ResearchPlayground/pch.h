#pragma once

#define APP_X				100
#define APP_Y				50
#define APP_WIDTH			1080
#define APP_HEIGHT			600
#define APP_TITLE           L"Keyboard Link"
#define DEFAULT_FONT		"c:\\Windows\\Fonts\\msyh.ttc"
//#define DEFAULT_FONT		"DroidSans.ttf"

#define KL_KB_VIEW_WIDTH 600
#define KL_KB_VIEW_HEIGHT 300

#define WINNAME_MENU "menu"
#define WINNAME_KEYBOARD "keyboard"
#define WINNAME_SEQUNCER "sequncer"
#define WINNAME_FUNCTION "function"
#define WINNAME_SETTINGS "settings"
#define WINNAME_LIGHT "light"
#define WINNAME_LIGHT_MODIFY "light_modify"
#define WINNAME_MACRO "macro"
#define WINNAME_ASSIGN_CONFIG_MGR "assign_config"
#define WINNAME_MACRO_CONFIG_MGR "macro_config"
#define WINNAME_MACRO_VIEW "macro_view"


#define KL_CONFIG_MAX_NAME_SIZE 32
#define KL_CONFIG_MAX_NUM 99

constexpr auto ASSIGNMENT_FILE_NAME = "assignment.xml";
constexpr auto MACROS_FILE_NAME = "macros.xml";
constexpr auto LANGUAGE_FILE_NAME = "language.xml";
constexpr auto SETTINGS_FILE_NAME = "settings.xml";

#define VOID_PTR_CAST(pint) reinterpret_cast<void*>(static_cast<uintptr_t>(pint))

#ifdef _WIN32
	//define something for Windows (32-bit and 64-bit, this part is common)
	#ifdef _WIN64
	   //define something for Windows (64-bit only)
	#else
	   //define something for Windows (32-bit only)
	#endif
#elif __APPLE__
	#if TARGET_IPHONE_SIMULATOR
	// iOS Simulator
	#elif TARGET_OS_IPHONE
	// iOS device
	#elif TARGET_OS_MAC
	// Other kinds of Mac OS
	#endif
#elif __ANDROID__
// android
#elif __linux__
// linux
#elif __unix__ // all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#   error "Unknown"
#endif

#ifdef _MSC_VER
#elif __GNUC__
#elif __BORLANDC__
#endif
