#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include "../pch.h"
#include "keyboard/keyboard.h"

#include "imgui/imgui.h"

#include <unordered_map>
#include <mutex>

enum KLFunctionType
{
	KL_FUNC_TYPE_KB = 0,
	KL_FUNC_TYPE_MOUSE,
	KL_FUNC_TYPE_MEDIA,
	KL_FUNC_TYPE_MACRO,
	KL_FUNC_TYPE_CUSTM,
};

enum KLFunctionLoopType
{
	KL_FUNC_LOOPTYPE_RELEASE = 0,
	KL_FUNC_LOOPTYPE_DOWN,
	KL_FUNC_LOOPTYPE_COUNT,
};

enum KLFunctionLayerType
{
	KL_LAYER_TYPE_DEFAULT = 0,
	KL_LAYER_TYPE_FN1,
	KL_LAYER_TYPE_FN2,
	KL_LAYER_TYPE_MAXIMUM,
};

enum KLFunctionID
{
	KLF_NONE = 0,

	KLF_A,
	KLF_B,
	KLF_C,
	KLF_D,
	KLF_E,
	KLF_F,
	KLF_G,
	KLF_H,
	KLF_I,
	KLF_J,
	KLF_K,
	KLF_L,
	KLF_M,
	KLF_N,
	KLF_O,
	KLF_P,
	KLF_Q,
	KLF_R,
	KLF_S,
	KLF_T,
	KLF_U,
	KLF_V,
	KLF_W,
	KLF_X,
	KLF_Y,
	KLF_Z,
	KLF_1,
	KLF_2,
	KLF_3,
	KLF_4,
	KLF_5,
	KLF_6,
	KLF_7,
	KLF_8,
	KLF_9,
	KLF_0,
	KLF_ENTER,
	KLF_ESC,
	KLF_BS,
	KLF_TAB,
	KLF_SP,
	KLF_UDS,
	KLF_PLUS,
	KLF_OB,
	KLF_CB,
	KLF_BSL,
	KLF_ASH,
	KLF_COLON,
	KLF_QUOTE,
	KLF_TILDE,
	KLF_COMM,
	KLF_DOT,
	KLF_SLASH,
	KLF_CAPS,
	KLF_F1,
	KLF_F2,
	KLF_F3,
	KLF_F4,
	KLF_F5,
	KLF_F6,
	KLF_F7,
	KLF_F8,
	KLF_F9,
	KLF_F10,
	KLF_F11,
	KLF_F12,
	KLF_PRINT,
	KLF_SCL,
	KLF_PAUSE,
	KLF_INS,
	KLF_HOME,
	KLF_PAGEUP,
	KLF_DEL,
	KLF_END,
	KLF_PAGEDOWN,
	KLF_RIGHT,
	KLF_LEFT,
	KLF_DOWN,
	KLF_UP,
	KLF_KP_NUM,
	KLF_KP_DIV,
	KLF_KP_AT,
	KLF_KP_MULT,
	KLF_KP_MINUS,
	KLF_KP_PLUS,
	KLF_KP_ENTER,
	KLF_KP_1,
	KLF_KP_2,
	KLF_KP_3,
	KLF_KP_4,
	KLF_KP_5,
	KLF_KP_6,
	KLF_KP_7,
	KLF_KP_8,
	KLF_KP_9,
	KLF_KP_0,
	KLF_KP_DEL,

	KLF_CTRL_L,
	KLF_SHIFT_L,
	KLF_ALT_L,
	KLF_WIN_L,
	KLF_CTRL_R,
	KLF_SHIFT_R,
	KLF_ALT_R,
	KLF_WIN_R,

	KLF_FN,

	KLF_MEDIA_PLAY,
	KLF_MEDIA_FORWARD,
	KLF_MEDIA_BACKWARD,
	KLF_MEDIA_TERMINATE,
};

enum KLFunctionLayoutFlags
{
	KL_FUNC_FLAG_NONE        = 0x00,
	KL_FUNC_FLAG_IMAGE       = 0x01,
};

struct KLFunctionLayout
{
	char name[64];
	float x;
	float y;
	float w;
	float h;
	KLFunctionID id;

	KLFunctionLayoutFlags flagbits;

	KLFunctionLayout(KLFunctionID id, const char* name, ImVec4 param = ImVec4(0.0f, 0.0f, 1.0f, 1.0f), KLFunctionLayoutFlags flagbits = KL_FUNC_FLAG_NONE);
};

struct KLFunctionKB
{
	uint8_t subType;	// See BSP_HID_KB_Type_t.
	uint8_t __2;
	uint8_t hid;
};

struct KLFunctionMouse
{
	uint8_t __1;
	uint8_t __2;
	uint8_t __3;
};

struct KLFunctionMedia
{
	uint8_t __1;
	uint8_t __2;
	uint8_t hid;
};

struct KLFunctionMacro
{
	uint8_t loopType;	//See KLFunctionLoopType.
	uint8_t loopCount;
	uint8_t macroID;
};

struct KLFunctionCustm
{
	uint8_t __1;
	uint8_t __2;
	uint8_t __3;
};

struct KLFunction
{
	char name[16];
	KLFunctionID id;
	KLFunctionType type;
	union {
		KLFunctionKB kb;
		KLFunctionMouse mouse;
		KLFunctionMedia media;
		KLFunctionMacro macro;
		KLFunctionCustm custm;
	} payload;

	KLFunction() {}
	KLFunction(KLFunctionID id, const char* name, BSP_HID_KB_Type_t subType, uint8_t hid)
	{
		this->id = id;
		strcpy(this->name, name);
		this->type = KL_FUNC_TYPE_KB;
		this->payload.kb.subType = subType;
		this->payload.kb.hid = hid;
	}
	KLFunction(KLFunctionID id, const char* name, uint8_t hid)
	{
		this->id = id;
		strcpy(this->name, name);
		this->type = KL_FUNC_TYPE_MEDIA;
		this->payload.media.hid = hid;
	}
};

KLFunction FindFunctionByFunctionID(KLFunctionID id);
KLFunction FindFunctionByHID(uint16_t hid);
KLFunctionID FindFunctionIDByMapID(KEY_MapId_t id);


struct KLFunctionConfig
{
	std::string name;
	std::unordered_map<KEY_MapId_t, KLFunction> layers[KL_LAYER_TYPE_MAXIMUM];
};

class KLFunctionConfigManager
{
public:
	KLFunctionLayerType m_CurrentLayerType;
	KLFunctionConfig* m_CurrentConfig;
	std::vector<KLFunctionConfig> m_ConfigList;

	void AddConfig(KLFunctionConfig& config)
	{
		m_ConfigList.push_back(std::move(config));
	}

	void SetCurrentConfig(uint32_t i)
	{
		m_CurrentConfig = &m_ConfigList[i];
	}

	static KLFunctionConfigManager* GetInstance()
	{
		std::call_once(s_OnceFlag, []() {
			s_Instance = new KLFunctionConfigManager();
			});
		return s_Instance;
	}
private:
	KLFunctionConfigManager()
		: m_CurrentConfig(nullptr)
		, m_CurrentLayerType(KL_LAYER_TYPE_DEFAULT){}
private:
	static std::once_flag			s_OnceFlag;
	static KLFunctionConfigManager*	s_Instance;
};


void InitFunctionWindow();
void ShowFunctionWindow(bool* p_open);

#endif