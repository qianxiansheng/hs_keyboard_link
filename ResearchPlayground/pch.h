#pragma once

#define APP_X				100
#define APP_Y				50
#define APP_WIDTH			1080
#define APP_HEIGHT			600
#define APP_TITLE           L"Keyboard Link"
#define EN_FONT				u8"DroidSans.ttf"
#define ZH_FONT				u8"C:\\Windows\\Fonts\\SIMYOU.TTF"
//#define DEFAULT_FONT		"DroidSans.ttf"

#define KL_KB_VIEW_WIDTH	800
#define KL_KB_VIEW_HEIGHT	600
#define KL_KB_VIEW_TEX_X    90.0f
#define KL_KB_VIEW_TEX_Y    180.0f
#define KL_KB_VIEW_TEX_W    620.0f
#define KL_KB_VIEW_TEX_H    220.0f

#define WINNAME_MENU "menu"
#define WINNAME_KEYBOARD "keyboard"
#define WINNAME_SEQUNCER "sequncer"
#define WINNAME_FUNCTION "function"
#define WINNAME_SETTINGS "settings"
#define WINNAME_STATEBAR "statebar"
#define WINNAME_LIGHT "light"
#define WINNAME_LIGHT_MODIFY "light_modify"
#define WINNAME_MACRO "macro"
#define WINNAME_ASSIGN_CONFIG_MGR "assign_config"
#define WINNAME_MACRO_CONFIG_MGR "macro_config"
#define WINNAME_MACRO_VIEW "macro_view"
#define DOCKSPACE_ID "MyDockSpace"

#define WINWIDTH_LIGHT_CONFIG (150.0f)
#define WINHEIGHT_FUNCTION_LIGHT_MODIFY ((APP_HEIGHT) / 2.0f)

#define KL_CONFIG_MAX_NAME_SIZE 32
#define KL_CONFIG_MAX_NUM 99

constexpr auto ASSIGNMENT_FILE_NAME = "assignment.xml";
constexpr auto MACROS_FILE_NAME = "macros.xml";
constexpr auto LANGUAGE_FILE_NAME = "language.xml";
constexpr auto SETTINGS_FILE_NAME = "settings.xml";

#define VOID_PTR_CAST(pint) reinterpret_cast<void*>(static_cast<uintptr_t>(pint))


/******************************************************************************************
 * Protocol base on IAP
 * 
 * 
 ******************************************************************************************/

#define DEVICE_APP_VID 0x36B0
#define DEVICE_APP_PID 0x3001
#define DEVICE_APP_RID 0x2F

#define IAP_HEADER_SIZE 128

 // 7 = 1 + 1 + 2 + 2 + 1
 // 1:header
 // 1:ctrl_1
 // 2:pack_ctrl 
 // 2:length
 // 1:bcc
#define IAP_PRO_TRANSFER_ATT_SIZE 7
// 5 = 1 + 2 + 2
// 1:cmd
// 2:length
// 2:crc
#define IAP_PRO_BUSINESS_ATT_SIZE 5
// 7 = 1 + 1 + 1 + 2 + 2
// 1:cmd
// 1:errCode
// 1:rspCmd
// 2:length
// 2:crc
#define IAP_PRO_BUSINESS_ACK_ATT_SIZE 7
// 6 = 2 + 4
// 2:blockIdx
// 4:offset
#define IAP_PRO_BUSINESS_FLASH_WRITE_ATT_SIZE 6
// 6 = 4 + 2
// 4:offset
// 2:size
#define IAP_PRO_BUSINESS_READ_WRITE_ATT_SIZE 6

#define IAP_USB_UID_EP_MPS              (64)
#define IAP_USB_HID_REPORT_SIZE         (64)
#define IAP_USB_HID_REPORT_ID           (0x2F)
#define IAP_USB_HID_REPORT_CONTENT_SIZE (IAP_USB_HID_REPORT_SIZE - 1)   //ReportID:1
#define IAP_PRO_TRANSFER_SIZE           (IAP_USB_HID_REPORT_CONTENT_SIZE)
#define IAP_PRO_TRANSFER_PAYLOAD_SIZE   (IAP_PRO_TRANSFER_SIZE - IAP_PRO_TRANSFER_ATT_SIZE)

#define IAP_PRO_TRANSFER_HEADER 0xAA

#define IAP_PRO_MAX_BLOCK_SIZE 0x2000
#define IAP_PRO_BUSINESS_FLASH_WRITE_PAYLOAD_MAX_SIZE   (IAP_PRO_MAX_BLOCK_SIZE + IAP_PRO_BUSINESS_FLASH_WRITE_ATT_SIZE)
#define IAP_PRO_BUSINESS_FLASH_WRITE_BUF_MAX_SIZE       (IAP_PRO_BUSINESS_FLASH_WRITE_PAYLOAD_MAX_SIZE + IAP_PRO_BUSINESS_ATT_SIZE)
#define IAP_PRO_BUSINESS_PAYLOAD_MAX_SIZE               (IAP_PRO_BUSINESS_FLASH_WRITE_PAYLOAD_MAX_SIZE)

#define IAP_PRO_BUSINESS_FLASH_READ_PAYLOAD_MAX_SIZE    (IAP_PRO_MAX_BLOCK_SIZE + IAP_PRO_BUSINESS_READ_WRITE_ATT_SIZE)
#define IAP_PRO_BUSINESS_FLASH_READ_BUF_MAX_SIZE        (IAP_PRO_BUSINESS_FLASH_READ_PAYLOAD_MAX_SIZE + IAP_PRO_BUSINESS_ACK_ATT_SIZE)
#define IAP_PRO_BUSINESS_ACK_PAYLOAD_MAX_SIZE           (IAP_PRO_BUSINESS_FLASH_READ_PAYLOAD_MAX_SIZE)
#define IAP_PRO_BUSINESS_ACK_MAX_SIZE                   (IAP_PRO_BUSINESS_ACK_PAYLOAD_MAX_SIZE + IAP_PRO_BUSINESS_ACK_ATT_SIZE)

#define IAP_PACK_ERROR "PACK ERROR"

#define DRV_PING_TIMEOUT 1000
/******************************************************************************************/


#define ImGuiDCXAxisAlign(v) ImGui::SetCursorPos(ImVec2((v), ImGui::GetCursorPos().y))
#define ImGuiDCYMargin(v) ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y + (v)))
#define DPI(v) ((v) * dpiScale())


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
