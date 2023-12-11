#include "function.h"

#include <unordered_map>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include "keyboard/keyboard.h"
#include "resources.h"
#include "configuration/kl_persistence.h"

#include "keylink.h"

// initial static member
KLFunctionConfigManager* KLFunctionConfigManager::s_Instance = nullptr;
std::once_flag KLFunctionConfigManager::s_OnceFlag;

void KLFunctionConfigManager::AddConfig(KLFunctionConfig& config)
{
	m_ConfigList.push_back(std::move(config));
}
void KLFunctionConfigManager::AddConfig(const char* name)
{
	KLFunctionConfig config(name);
	//InitDefaultConfig(config);
	m_ConfigList.push_back(config);
}
bool KLFunctionConfigManager::IsConfigExists(const char* name)
{
	for (auto& config : m_ConfigList)
		if (config.name == name)
			return true;
	return false;
}

bool KLFunctionConfigManager::RemoveCurrentConfig()
{
	return RemoveConfig(GetCurrentConfig().name.c_str());
}

bool KLFunctionConfigManager::RemoveConfig(const char* name)
{
	for (auto it = m_ConfigList.begin(); it != m_ConfigList.end(); ++it) {
		if (it->name == name) {
			m_ConfigList.erase(it);
			break;
		}
	}
	return true;
}


void KLFunctionConfigManager::SaveConfig(const char* filename)
{
	AssignmentConfigWrite(filename);
}
void KLFunctionConfigManager::LoadConfig(const char* filename)
{
	AssignmentConfigRead(filename);
}

KLFunctionLayout::KLFunctionLayout(KLFunctionID id, const char* name, ImVec4 param, KLFunctionLayoutFlags flagbits)
{
	this->id = id;
	strcpy(this->name, name);
	this->x = param.x;
	this->y = param.y;
	this->w = param.z;
	this->h = param.w;
	this->flagbits = flagbits;
}

std::unordered_map<KLFunctionID, KLFunction> function_map = {
	{KLF_A         , KLFunction(KLF_A         , "A"            , KEY_TYPE_GENERAL, HID_KEYB_A                    )},
	{KLF_B         , KLFunction(KLF_B         , "B"            , KEY_TYPE_GENERAL, HID_KEYB_B                    )},
	{KLF_C         , KLFunction(KLF_C         , "C"            , KEY_TYPE_GENERAL, HID_KEYB_C                    )},
	{KLF_D         , KLFunction(KLF_D         , "D"            , KEY_TYPE_GENERAL, HID_KEYB_D                    )},
	{KLF_E         , KLFunction(KLF_E         , "E"            , KEY_TYPE_GENERAL, HID_KEYB_E                    )},
	{KLF_F         , KLFunction(KLF_F         , "F"            , KEY_TYPE_GENERAL, HID_KEYB_F                    )},
	{KLF_G         , KLFunction(KLF_G         , "G"            , KEY_TYPE_GENERAL, HID_KEYB_G                    )},
	{KLF_H         , KLFunction(KLF_H         , "H"            , KEY_TYPE_GENERAL, HID_KEYB_H                    )},
	{KLF_I         , KLFunction(KLF_I         , "I"            , KEY_TYPE_GENERAL, HID_KEYB_I                    )},
	{KLF_J         , KLFunction(KLF_J         , "J"            , KEY_TYPE_GENERAL, HID_KEYB_J                    )},
	{KLF_K         , KLFunction(KLF_K         , "K"            , KEY_TYPE_GENERAL, HID_KEYB_K                    )},
	{KLF_L         , KLFunction(KLF_L         , "L"            , KEY_TYPE_GENERAL, HID_KEYB_L                    )},
	{KLF_M         , KLFunction(KLF_M         , "M"            , KEY_TYPE_GENERAL, HID_KEYB_M                    )},
	{KLF_N         , KLFunction(KLF_N         , "N"            , KEY_TYPE_GENERAL, HID_KEYB_N                    )},
	{KLF_O         , KLFunction(KLF_O         , "O"            , KEY_TYPE_GENERAL, HID_KEYB_O                    )},
	{KLF_P         , KLFunction(KLF_P         , "P"            , KEY_TYPE_GENERAL, HID_KEYB_P                    )},
	{KLF_Q         , KLFunction(KLF_Q         , "Q"            , KEY_TYPE_GENERAL, HID_KEYB_Q                    )},
	{KLF_R         , KLFunction(KLF_R         , "R"            , KEY_TYPE_GENERAL, HID_KEYB_R                    )},
	{KLF_S         , KLFunction(KLF_S         , "S"            , KEY_TYPE_GENERAL, HID_KEYB_S                    )},
	{KLF_T         , KLFunction(KLF_T         , "T"            , KEY_TYPE_GENERAL, HID_KEYB_T                    )},
	{KLF_U         , KLFunction(KLF_U         , "U"            , KEY_TYPE_GENERAL, HID_KEYB_U                    )},
	{KLF_V         , KLFunction(KLF_V         , "V"            , KEY_TYPE_GENERAL, HID_KEYB_V                    )},
	{KLF_W         , KLFunction(KLF_W         , "W"            , KEY_TYPE_GENERAL, HID_KEYB_W                    )},
	{KLF_X         , KLFunction(KLF_X         , "X"            , KEY_TYPE_GENERAL, HID_KEYB_X                    )},
	{KLF_Y         , KLFunction(KLF_Y         , "Y"            , KEY_TYPE_GENERAL, HID_KEYB_Y                    )},
	{KLF_Z         , KLFunction(KLF_Z         , "Z"            , KEY_TYPE_GENERAL, HID_KEYB_Z                    )},
	{KLF_1         , KLFunction(KLF_1         , "1"            , KEY_TYPE_GENERAL, HID_KEYB_1                    )},
	{KLF_2         , KLFunction(KLF_2         , "2"            , KEY_TYPE_GENERAL, HID_KEYB_2                    )},
	{KLF_3         , KLFunction(KLF_3         , "3"            , KEY_TYPE_GENERAL, HID_KEYB_3                    )},
	{KLF_4         , KLFunction(KLF_4         , "4"            , KEY_TYPE_GENERAL, HID_KEYB_4                    )},
	{KLF_5         , KLFunction(KLF_5         , "5"            , KEY_TYPE_GENERAL, HID_KEYB_5                    )},
	{KLF_6         , KLFunction(KLF_6         , "6"            , KEY_TYPE_GENERAL, HID_KEYB_6                    )},
	{KLF_7         , KLFunction(KLF_7         , "7"            , KEY_TYPE_GENERAL, HID_KEYB_7                    )},
	{KLF_8         , KLFunction(KLF_8         , "8"            , KEY_TYPE_GENERAL, HID_KEYB_8                    )},
	{KLF_9         , KLFunction(KLF_9         , "9"            , KEY_TYPE_GENERAL, HID_KEYB_9                    )},
	{KLF_0         , KLFunction(KLF_0         , "0"            , KEY_TYPE_GENERAL, HID_KEYB_0                    )},
	{KLF_ENTER     , KLFunction(KLF_ENTER     , "ENTER"        , KEY_TYPE_GENERAL, HID_KEYB_ENTER                )},
	{KLF_ESC       , KLFunction(KLF_ESC       , "ESC"          , KEY_TYPE_GENERAL, HID_KEYB_ESCAPE               )},
	{KLF_BS        , KLFunction(KLF_BS        , "BACKSPACE"    , KEY_TYPE_GENERAL, HID_KEYB_BACKSPACE            )},
	{KLF_TAB       , KLFunction(KLF_TAB       , "TAB"          , KEY_TYPE_GENERAL, HID_KEYB_TAB                  )},
	{KLF_SP        , KLFunction(KLF_SP        , "SPACE"        , KEY_TYPE_GENERAL, HID_KEYB_SPACEBAR             )},
	{KLF_UDS       , KLFunction(KLF_UDS       , "UNDERSCORE"   , KEY_TYPE_GENERAL, HID_KEYB_UNDERSCORE           )},
	{KLF_PLUS      , KLFunction(KLF_PLUS      , "PLUS"         , KEY_TYPE_GENERAL, HID_KEYB_PLUS                 )},
	{KLF_OB        , KLFunction(KLF_OB        , "OPEN_BRACKET" , KEY_TYPE_GENERAL, HID_KEYB_OPEN_BRACKET         )},
	{KLF_CB        , KLFunction(KLF_CB        , "CLOSE_BRACKET", KEY_TYPE_GENERAL, HID_KEYB_CLOSE_BRACKET        )},
	{KLF_BSL       , KLFunction(KLF_BSL       , "BACKSLASH"    , KEY_TYPE_GENERAL, HID_KEYB_BACKSLASH            )},
	{KLF_ASH       , KLFunction(KLF_ASH       , "ASH"          , KEY_TYPE_GENERAL, HID_KEYB_ASH                  )},
	{KLF_COLON     , KLFunction(KLF_COLON     , "COLON"        , KEY_TYPE_GENERAL, HID_KEYB_COLON                )},
	{KLF_QUOTE     , KLFunction(KLF_QUOTE     , "QUOTE"        , KEY_TYPE_GENERAL, HID_KEYB_QUOTE                )},
	{KLF_TILDE     , KLFunction(KLF_TILDE     , "TILDE"        , KEY_TYPE_GENERAL, HID_KEYB_TILDE                )},
	{KLF_COMM      , KLFunction(KLF_COMM      , "COMMA"        , KEY_TYPE_GENERAL, HID_KEYB_COMMA                )},
	{KLF_DOT       , KLFunction(KLF_DOT       , "DOT"          , KEY_TYPE_GENERAL, HID_KEYB_DOT                  )},
	{KLF_SLASH     , KLFunction(KLF_SLASH     , "SLASH"        , KEY_TYPE_GENERAL, HID_KEYB_SLASH                )},
	{KLF_CAPS      , KLFunction(KLF_CAPS      , "CAPS_LOCK"    , KEY_TYPE_GENERAL, HID_KEYB_CAPS_LOCK            )},
	{KLF_F1        , KLFunction(KLF_F1        , "F1"           , KEY_TYPE_GENERAL, HID_KEYB_F1                   )},
	{KLF_F2        , KLFunction(KLF_F2        , "F2"           , KEY_TYPE_GENERAL, HID_KEYB_F2                   )},
	{KLF_F3        , KLFunction(KLF_F3        , "F3"           , KEY_TYPE_GENERAL, HID_KEYB_F3                   )},
	{KLF_F4        , KLFunction(KLF_F4        , "F4"           , KEY_TYPE_GENERAL, HID_KEYB_F4                   )},
	{KLF_F5        , KLFunction(KLF_F5        , "F5"           , KEY_TYPE_GENERAL, HID_KEYB_F5                   )},
	{KLF_F6        , KLFunction(KLF_F6        , "F6"           , KEY_TYPE_GENERAL, HID_KEYB_F6                   )},
	{KLF_F7        , KLFunction(KLF_F7        , "F7"           , KEY_TYPE_GENERAL, HID_KEYB_F7                   )},
	{KLF_F8        , KLFunction(KLF_F8        , "F8"           , KEY_TYPE_GENERAL, HID_KEYB_F8                   )},
	{KLF_F9        , KLFunction(KLF_F9        , "F9"           , KEY_TYPE_GENERAL, HID_KEYB_F9                   )},
	{KLF_F10       , KLFunction(KLF_F10       , "F10"          , KEY_TYPE_GENERAL, HID_KEYB_F10                  )},
	{KLF_F11       , KLFunction(KLF_F11       , "F11"          , KEY_TYPE_GENERAL, HID_KEYB_F11                  )},
	{KLF_F12       , KLFunction(KLF_F12       , "F12"          , KEY_TYPE_GENERAL, HID_KEYB_F12                  )},
	{KLF_PRINT     , KLFunction(KLF_PRINT     , "PRINTSCREEN"  , KEY_TYPE_GENERAL, HID_KEYB_PRINTSCREEN          )},
	{KLF_SCL       , KLFunction(KLF_SCL       , "SCROLL_LOCK"  , KEY_TYPE_GENERAL, HID_KEYB_SCROLL_LOCK          )},
	{KLF_PAUSE     , KLFunction(KLF_PAUSE     , "PAUSE"        , KEY_TYPE_GENERAL, HID_KEYB_PAUSE                )},
	{KLF_INS       , KLFunction(KLF_INS       , "INSART"       , KEY_TYPE_GENERAL, HID_KEYB_INSERT               )},
	{KLF_HOME      , KLFunction(KLF_HOME      , "HOME"         , KEY_TYPE_GENERAL, HID_KEYB_HOME                 )},
	{KLF_PAGEUP    , KLFunction(KLF_PAGEUP    , "PAGEUP"       , KEY_TYPE_GENERAL, HID_KEYB_PAGEUP               )},
	{KLF_DEL       , KLFunction(KLF_DEL       , "DELETE"       , KEY_TYPE_GENERAL, HID_KEYB_DELETE               )},
	{KLF_END       , KLFunction(KLF_END       , "END"          , KEY_TYPE_GENERAL, HID_KEYB_END                  )},
	{KLF_PAGEDOWN  , KLFunction(KLF_PAGEDOWN  , "PAGEDOWN "    , KEY_TYPE_GENERAL, HID_KEYB_PAGEDOWN             )},
	{KLF_RIGHT     , KLFunction(KLF_RIGHT     , "RIGHT"        , KEY_TYPE_GENERAL, HID_KEYB_RIGHT                )},
	{KLF_LEFT      , KLFunction(KLF_LEFT      , "LEFT"         , KEY_TYPE_GENERAL, HID_KEYB_LEFT                 )},
	{KLF_DOWN      , KLFunction(KLF_DOWN      , "DOWN"         , KEY_TYPE_GENERAL, HID_KEYB_DOWN                 )},
	{KLF_UP        , KLFunction(KLF_UP        , "UP"           , KEY_TYPE_GENERAL, HID_KEYB_UP                   )},
	{KLF_KP_NUM    , KLFunction(KLF_KP_NUM    , "KP_NUM"       , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_NUM_LOCK      )},
	{KLF_KP_DIV    , KLFunction(KLF_KP_DIV    , "KP_DIV"       , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_DIVIDE        )},
	{KLF_KP_AT     , KLFunction(KLF_KP_AT     , "KP_AT"        , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_AT            )},
	{KLF_KP_MULT   , KLFunction(KLF_KP_MULT   , "KP_MULTIPLY"  , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_MULTIPLY      )},
	{KLF_KP_MINUS  , KLFunction(KLF_KP_MINUS  , "KP_MINUS"     , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_MINUS         )},
	{KLF_KP_PLUS   , KLFunction(KLF_KP_PLUS   , "KP_PLUS"      , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_PLUS          )},
	{KLF_KP_ENTER  , KLFunction(KLF_KP_ENTER  , "KP_ENTER"     , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_ENTER         )},
	{KLF_KP_1      , KLFunction(KLF_KP_1      , "KP_1"         , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_1             )},
	{KLF_KP_2      , KLFunction(KLF_KP_2      , "KP_2"         , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_2             )},
	{KLF_KP_3      , KLFunction(KLF_KP_3      , "KP_3"         , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_3             )},
	{KLF_KP_4      , KLFunction(KLF_KP_4      , "KP_4"         , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_4             )},
	{KLF_KP_5      , KLFunction(KLF_KP_5      , "KP_5"         , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_5             )},
	{KLF_KP_6      , KLFunction(KLF_KP_6      , "KP_6"         , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_6             )},
	{KLF_KP_7      , KLFunction(KLF_KP_7      , "KP_7"         , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_7             )},
	{KLF_KP_8      , KLFunction(KLF_KP_8      , "KP_8"         , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_8             )},
	{KLF_KP_9      , KLFunction(KLF_KP_9      , "KP_9"         , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_9             )},
	{KLF_KP_0      , KLFunction(KLF_KP_0      , "KP_0"         , KEY_TYPE_GENERAL, HID_KEYB_KEYPAD_0             )},
	{KLF_KP_DEL    , KLFunction(KLF_KP_DEL    , "KP_DELETE"    , KEY_TYPE_GENERAL, 99                            )},//HID_KEYB_KEYPAD_DELETE
	{KLF_CTRL_L    , KLFunction(KLF_CTRL_L    , "LEFT_CTRL"    , KEY_TYPE_MODIFIER, HID_KEYB_MODIFIER_LEFT_CTRL  )},
	{KLF_SHIFT_L   , KLFunction(KLF_SHIFT_L   , "LEFT_SHIFT"   , KEY_TYPE_MODIFIER, HID_KEYB_MODIFIER_LEFT_SHIFT )},
	{KLF_ALT_L     , KLFunction(KLF_ALT_L     , "LEFT_ALT"     , KEY_TYPE_MODIFIER, HID_KEYB_MODIFIER_LEFT_ALT   )},
	{KLF_WIN_L     , KLFunction(KLF_WIN_L     , "LEFT_UI"      , KEY_TYPE_MODIFIER, HID_KEYB_MODIFIER_LEFT_UI    )},
	{KLF_CTRL_R    , KLFunction(KLF_CTRL_R    , "RIGHT_CTRL"   , KEY_TYPE_MODIFIER, HID_KEYB_MODIFIER_RIGHT_CTRL )},
	{KLF_SHIFT_R   , KLFunction(KLF_SHIFT_R   , "RIGHT_SHIFT"  , KEY_TYPE_MODIFIER, HID_KEYB_MODIFIER_RIGHT_SHIFT)},
	{KLF_ALT_R     , KLFunction(KLF_ALT_R     , "RIGHT_ALT"    , KEY_TYPE_MODIFIER, HID_KEYB_MODIFIER_RIGHT_ALT  )},
	{KLF_WIN_R     , KLFunction(KLF_WIN_R     , "RIGHT_WIN"    , KEY_TYPE_MODIFIER, HID_KEYB_MODIFIER_RIGHT_UI   )},
	{KLF_FN        , KLFunction(KLF_FN        , "FN"           , KEY_TYPE_VENDOR_DEFINE,    HID_KEYB_VENDOR_FN   )},

	{KLF_MEDIA_PLAY             , KLFunction(KLF_MEDIA_PLAY             , "PLAY"           , HID_MEDIA_PLAY               )},
	{KLF_MEDIA_TERMINATE        , KLFunction(KLF_MEDIA_TERMINATE        , "TERMINATE"      , HID_MEDIA_TERMINATE          )},
	{KLF_MEDIA_BACKWARD         , KLFunction(KLF_MEDIA_BACKWARD         , "BACKWARD"       , HID_MEDIA_BACKWARD           )},
	{KLF_MEDIA_FORWARD          , KLFunction(KLF_MEDIA_FORWARD          , "FORWARD"        , HID_MEDIA_FORWARD            )},
	{KLF_MEDIA_VOLUME_INC       , KLFunction(KLF_MEDIA_VOLUME_INC       , "VOLUME_INC"     , HID_MEDIA_VOLUME_INC         )},
	{KLF_MEDIA_VOLUME_DEC       , KLFunction(KLF_MEDIA_VOLUME_DEC       , "VOLUME_DEC"     , HID_MEDIA_VOLUME_DEC         )},
	{KLF_MEDIA_MUTE             , KLFunction(KLF_MEDIA_MUTE             , "MUTE"           , HID_MEDIA_MUTE               )},
	{KLF_MEDIA_HOME             , KLFunction(KLF_MEDIA_HOME             , "HOME"           , HID_MEDIA_HOME               )},
	{KLF_MEDIA_CALC             , KLFunction(KLF_MEDIA_CALC             , "CALC"           , HID_MEDIA_CALC               )},
	{KLF_MEDIA_EMAIL            , KLFunction(KLF_MEDIA_EMAIL            , "EMAIL"          , HID_MEDIA_EMAIL              )},
	{KLF_MEDIA_COMPUTER         , KLFunction(KLF_MEDIA_COMPUTER         , "COMPUTER"       , HID_MEDIA_COMPUTER           )},
	{KLF_MEDIA_COLLECT			, KLFunction(KLF_MEDIA_COLLECT          , "COLLECT"        , HID_MEDIA_COLLECT            )},
	{KLF_MEDIA_BRIGHTNESS_INC   , KLFunction(KLF_MEDIA_BRIGHTNESS_INC   , "BRIGHTNESS_INC" , HID_MEDIA_BRIGHTNESS_INC     )},
	{KLF_MEDIA_BRIGHTNESS_DEC   , KLFunction(KLF_MEDIA_BRIGHTNESS_DEC   , "BRIGHTNESS_DEC" , HID_MEDIA_BRIGHTNESS_DEC     )},

	{KLF_MEDIA_PREV			    , KLFunction(KLF_MEDIA_PREV             , "PREV"           , HID_MEDIA_PREV)},
	{KLF_MEDIA_NEXT             , KLFunction(KLF_MEDIA_NEXT             , "NEXT"           , HID_MEDIA_NEXT)},
	{KLF_MEDIA_FULLSCREEN       , KLFunction(KLF_MEDIA_FULLSCREEN       , "FULLSCREEN"     , HID_MEDIA_FULLSCREEN)},

};

KEY_MapId_t mid;
KLFunctionID fid;

std::unordered_map<KEY_MapId_t, KLFunctionID> default_map_function_map = {
	{KM_A              ,	KLF_A		},
	{KM_B              ,	KLF_B		},
	{KM_C              ,	KLF_C		},
	{KM_D              ,	KLF_D		},
	{KM_E              ,	KLF_E		},
	{KM_F              ,	KLF_F		},
	{KM_G              ,	KLF_G		},
	{KM_H              ,	KLF_H		},
	{KM_I              ,	KLF_I		},
	{KM_J              ,	KLF_J		},
	{KM_K              ,	KLF_K		},
	{KM_L              ,	KLF_L		},
	{KM_M              ,	KLF_M		},
	{KM_N              ,	KLF_N		},
	{KM_O              ,	KLF_O		},
	{KM_P              ,	KLF_P		},
	{KM_Q              ,	KLF_Q		},
	{KM_R              ,	KLF_R		},
	{KM_S              ,	KLF_S		},
	{KM_T              ,	KLF_T		},
	{KM_U              ,	KLF_U		},
	{KM_V              ,	KLF_V		},
	{KM_W              ,	KLF_W		},
	{KM_X              ,	KLF_X		},
	{KM_Y              ,	KLF_Y		},
	{KM_Z              ,	KLF_Z		},
	{KM_1              ,	KLF_1		},
	{KM_2              ,	KLF_2		},
	{KM_3              ,	KLF_3		},
	{KM_4              ,	KLF_4		},
	{KM_5              ,	KLF_5		},
	{KM_6              ,	KLF_6		},
	{KM_7              ,	KLF_7		},
	{KM_8              ,	KLF_8		},
	{KM_9              ,	KLF_9		},
	{KM_0              ,	KLF_0		},
	{KM_ENTER          ,	KLF_ENTER	},
	{KM_ESC            ,	KLF_ESC		},
	{KM_BS             ,	KLF_BS		},
	{KM_TAB            ,	KLF_TAB		},
	{KM_SP             ,	KLF_SP		},
	{KM_UDS            ,	KLF_UDS		},
	{KM_PLUS           ,	KLF_PLUS	},
	{KM_OB             ,	KLF_OB		},
	{KM_CB             ,	KLF_CB		},
	{KM_BSL            ,	KLF_BSL		},
	{KM_ASH            ,	KLF_ASH		},
	{KM_COLON          ,	KLF_COLON	},
	{KM_QUOTE          ,	KLF_QUOTE	},
	{KM_TILDE          ,	KLF_TILDE	},
	{KM_COMM           ,	KLF_COMM	},
	{KM_DOT            ,	KLF_DOT		},
	{KM_SLASH          ,	KLF_SLASH	},
	{KM_CAPS           ,	KLF_CAPS	},
	{KM_F1             ,	KLF_F1		},
	{KM_F2             ,	KLF_F2		},
	{KM_F3             ,	KLF_F3		},
	{KM_F4             ,	KLF_F4		},
	{KM_F5             ,	KLF_F5		},
	{KM_F6             ,	KLF_F6		},
	{KM_F7             ,	KLF_F7		},
	{KM_F8             ,	KLF_F8		},
	{KM_F9             ,	KLF_F9		},
	{KM_F10            ,	KLF_F10		},
	{KM_F11            ,	KLF_F11		},
	{KM_F12            ,	KLF_F12		},
	{KM_PRINT          ,	KLF_PRINT	},
	{KM_SCL            ,	KLF_SCL		},
	{KM_PAUSE          ,	KLF_PAUSE	},
	{KM_INS            ,	KLF_INS		},
	{KM_HOME           ,	KLF_HOME	},
	{KM_PAGEUP         ,	KLF_PAGEUP	},
	{KM_DEL            ,	KLF_DEL		},
	{KM_END            ,	KLF_END		},
	{KM_PAGEDOWN       ,	KLF_PAGEDOWN},
	{KM_RIGHT          ,	KLF_RIGHT	},
	{KM_LEFT           ,	KLF_LEFT	},
	{KM_DOWN           ,	KLF_DOWN	},
	{KM_UP             ,	KLF_UP		},
	{KM_KP_NUM         ,	KLF_KP_NUM	},
	{KM_KP_DIV         ,	KLF_KP_DIV	},
	{KM_KP_AT          ,	KLF_KP_AT	},
	{KM_KP_MULT        ,	KLF_KP_MULT	},
	{KM_KP_MINUS       ,	KLF_KP_MINUS},
	{KM_KP_PLUS        ,	KLF_KP_PLUS	},
	{KM_KP_ENTER       ,	KLF_KP_ENTER},
	{KM_KP_1           ,	KLF_KP_1	},
	{KM_KP_2           ,	KLF_KP_2	},
	{KM_KP_3           ,	KLF_KP_3	},
	{KM_KP_4           ,	KLF_KP_4	},
	{KM_KP_5           ,	KLF_KP_5	},
	{KM_KP_6           ,	KLF_KP_6	},
	{KM_KP_7           ,	KLF_KP_7	},
	{KM_KP_8           ,	KLF_KP_8	},
	{KM_KP_9           ,	KLF_KP_9	},
	{KM_KP_0           ,	KLF_KP_0	},
	{KM_KP_DEL         ,	KLF_KP_DEL	},

	{KM_CTRL_L         ,	KLF_CTRL_L	},
	{KM_SHIFT_L        ,	KLF_SHIFT_L	},
	{KM_ALT_L          ,	KLF_ALT_L	},
	{KM_WIN_L          ,	KLF_WIN_L	},
	{KM_CTRL_R         ,	KLF_CTRL_R	},
	{KM_SHIFT_R        ,	KLF_SHIFT_R	},
	{KM_ALT_R          ,	KLF_ALT_R	},
	{KM_WIN_R          ,	KLF_WIN_R	},

	{KM_FN             ,	KLF_FN		},
};

KLFunctionLayout functionLayout[] = {
	
	KLFunctionLayout(KLF_MEDIA_PLAY            , "func_play"                , {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_BACKWARD        , "func_backward"            , {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_FORWARD         , "func_forward"             , {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_TERMINATE       , "func_terminate"           , {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_PREV			   , "func_prev"                , {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_NEXT			   , "func_next"                , {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),

	KLFunctionLayout(KLF_MEDIA_VOLUME_INC	   , "func_volume_increment"	, {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_VOLUME_DEC	   , "func_volume_decrement"	, {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_MUTE			   , "func_mute"                , {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_HOME			   , "func_home"                , {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_CALC			   , "func_calc"                , {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_EMAIL		   , "func_email"               , {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_COMPUTER		   , "func_computer"            , {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_COLLECT		   , "func_collect"	            , {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_BRIGHTNESS_INC  , "func_brightness_increment", {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_BRIGHTNESS_DEC  , "func_brightness_decrement", {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),
	KLFunctionLayout(KLF_MEDIA_FULLSCREEN	   , "func_full_screen"         , {0.0f, 0.0f, 1.0f, 1.0f} , KL_FUNC_FLAG_IMAGE),


	KLFunctionLayout(KLF_ESC     , "Esc"         , {0.0f, 0.5f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_F1      , "F1"          , {1.0f, 0.0f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_F2      , "F2"          ),
	KLFunctionLayout(KLF_F3      , "F3"          ),
	KLFunctionLayout(KLF_F4      , "F4"          ),
	KLFunctionLayout(KLF_F5      , "F5"          , {0.5f, 0.0f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_F6      , "F6"          ),
	KLFunctionLayout(KLF_F7      , "F7"          ),
	KLFunctionLayout(KLF_F8      , "F8"          ),
	KLFunctionLayout(KLF_F9      , "F9"          , {0.5f, 0.0f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_F10     , "F10"         ),
	KLFunctionLayout(KLF_F11     , "F11"         ),
	KLFunctionLayout(KLF_F12     , "F12"         ),
	KLFunctionLayout(KLF_PRINT   , "PtrSc"       , {0.25f, 0.0f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_SCL     , "Scroll\nLock"),
	KLFunctionLayout(KLF_PAUSE   , "Pause\nBreak"),
	
	KLFunctionLayout(KLF_TILDE   , "~\n`"        , {0.0f, 0.5f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_1       , "!\n1"        ),
	KLFunctionLayout(KLF_2       , "@\n2"        ),
	KLFunctionLayout(KLF_3       , "#\n3"        ),
	KLFunctionLayout(KLF_4       , "$\n4"        ),
	KLFunctionLayout(KLF_5       , "%\n5"        ),
	KLFunctionLayout(KLF_6       , "^\n6"        ),
	KLFunctionLayout(KLF_7       , "&\n7"        ),
	KLFunctionLayout(KLF_8       , "*\n8"        ),
	KLFunctionLayout(KLF_9       , "(\n9"        ),
	KLFunctionLayout(KLF_0       , ")\n0"        ),
	KLFunctionLayout(KLF_UDS     , "_\n-"        ),
	KLFunctionLayout(KLF_PLUS    , "+\n="        ),
	KLFunctionLayout(KLF_BS      , "Backspae"    , {0.0f, 0.0f, 2.0f, 1.0f}),
	
	KLFunctionLayout(KLF_INS     , "Insert"      , {0.25f, 0.0f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_HOME    , "Home"        ),
	KLFunctionLayout(KLF_PAGEUP  , "PgUp"        ),
	
	KLFunctionLayout(KLF_KP_NUM  , "Num\nLock"   , {0.25f, 0.0f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_KP_DIV  , "/"           ),
	KLFunctionLayout(KLF_KP_MULT , "*"           ),
	KLFunctionLayout(KLF_KP_MINUS, "-"           ),
	
	KLFunctionLayout(KLF_TAB     , "Tab"         , {0.0f, 0.0f, 1.5f, 1.0f}),
	KLFunctionLayout(KLF_Q       , "Q"           ),
	KLFunctionLayout(KLF_W       , "W"           ),
	KLFunctionLayout(KLF_E       , "E"           ),
	KLFunctionLayout(KLF_R       , "R"           ),
	KLFunctionLayout(KLF_T       , "T"           ),
	KLFunctionLayout(KLF_Y       , "Y"           ),
	KLFunctionLayout(KLF_U       , "U"           ),
	KLFunctionLayout(KLF_I       , "I"           ),
	KLFunctionLayout(KLF_O       , "O"           ),
	KLFunctionLayout(KLF_P       , "P"           ),
	KLFunctionLayout(KLF_OB      , "{\n["        ),
	KLFunctionLayout(KLF_CB      , "}\n]"        ),
	KLFunctionLayout(KLF_BSL     , "|\n\\"       , {0.0f, 0.0f, 1.5f, 1.0f}),
	
	KLFunctionLayout(KLF_DEL     , "Delete"      , {0.25f, 0.0f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_END     , "End"         ),
	KLFunctionLayout(KLF_PAGEDOWN, "PgDn"        ),
	
	KLFunctionLayout(KLF_KP_7    , "7"           , {0.25f, 0.0f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_KP_8    , "8"           ),
	KLFunctionLayout(KLF_KP_9    , "9"           ),
	KLFunctionLayout(KLF_KP_PLUS , "+"           , {0.0f, 0.0f, 1.0f, 2.0f}),
	
	KLFunctionLayout(KLF_CAPS    , "Caps Lock"   , {0.0f, 0.0f, 1.75f, 1.0f}),
	KLFunctionLayout(KLF_A       , "A"           ),
	KLFunctionLayout(KLF_S       , "S"           ),
	KLFunctionLayout(KLF_D       , "D"           ),
	KLFunctionLayout(KLF_F       , "F"           ),
	KLFunctionLayout(KLF_G       , "G"           ),
	KLFunctionLayout(KLF_H       , "H"           ),
	KLFunctionLayout(KLF_J       , "J"           ),
	KLFunctionLayout(KLF_K       , "K"           ),
	KLFunctionLayout(KLF_L       , "L"           ),
	KLFunctionLayout(KLF_COLON   , ":\n;"        ),
	KLFunctionLayout(KLF_QUOTE   , "\"\n'"       ),
	KLFunctionLayout(KLF_ENTER   , "Enter"       , {0.0f, 0.0f, 2.25f, 1.0f}),
	
	KLFunctionLayout(KLF_KP_4    , "4"           , {3.5f, 0.0f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_KP_5    , "5"           ),
	KLFunctionLayout(KLF_KP_6    , "6"           ),

	KLFunctionLayout(KLF_SHIFT_L , "SHIFT"       , {0.0f, 0.0f, 2.25f, 1.0f}),
	KLFunctionLayout(KLF_Z       , "Z"           ),
	KLFunctionLayout(KLF_X       , "X"           ),
	KLFunctionLayout(KLF_C       , "C"           ),
	KLFunctionLayout(KLF_V       , "V"           ),
	KLFunctionLayout(KLF_B       , "B"           ),
	KLFunctionLayout(KLF_N       , "N"           ),
	KLFunctionLayout(KLF_M       , "M"           ),
	KLFunctionLayout(KLF_COMM    , "<\n,"        ),
	KLFunctionLayout(KLF_DOT     , ">\n."        ),
	KLFunctionLayout(KLF_SLASH   , "?\n/"        ),
	KLFunctionLayout(KLF_SHIFT_R , "SHIFT"       , {0.0f, 0.0f, 2.75f, 1.0f}),
	KLFunctionLayout(KLF_UP      , "↑"           , {1.25f, 0.0f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_KP_1    , "1"           , {1.25f, 0.0f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_KP_2    , "2"           ),
	KLFunctionLayout(KLF_KP_3    , "3"           ),
	KLFunctionLayout(KLF_KP_ENTER, "Enter"       , {0.0f, 0.0f, 1.0f, 2.0f}),
	
	KLFunctionLayout(KLF_CTRL_L  , "Ctrl"        , {0.0f, 0.0f, 1.25f, 1.0f}),
	KLFunctionLayout(KLF_WIN_L   , "WIN"         , {0.0f, 0.0f, 1.25f, 1.0f}),
	KLFunctionLayout(KLF_ALT_L   , "ALT"         , {0.0f, 0.0f, 1.25f, 1.0f}),
	KLFunctionLayout(KLF_SP      , ""            , {0.0f, 0.0f, 6.25f, 1.0f}),
	KLFunctionLayout(KLF_ALT_R   , "ALT"         , {0.0f, 0.0f, 1.25f, 1.0f}),
	KLFunctionLayout(KLF_WIN_R   , "WIN"         , {0.0f, 0.0f, 1.25f, 1.0f}),
	KLFunctionLayout(KLF_FN      , "FN"          , {0.0f, 0.0f, 1.25f, 1.0f}),
	KLFunctionLayout(KLF_CTRL_R  , "CTRL"        , {0.0f, 0.0f, 1.25f, 1.0f}),
	
	KLFunctionLayout(KLF_LEFT    , "←"           , {0.25f, 0.0f, 1.0f, 1.0f}),
	KLFunctionLayout(KLF_DOWN    , "↓"           ),
	KLFunctionLayout(KLF_RIGHT   , "→"           ),
	KLFunctionLayout(KLF_KP_0    , "0"           , {0.25f, 0.0f, 2.0f, 1.0f}),
	KLFunctionLayout(KLF_KP_DEL  , ".\nDel"      ),


};
uint32_t functionLayoutSize = sizeof(functionLayout) / sizeof(functionLayout[0]);


KLFunction& FindFunctionByFunctionID(KLFunctionID id)
{
	return function_map[id];
}

KLFunctionID FindFunctionIDByMapID(KEY_MapId_t id)
{
	return default_map_function_map[id];
}

KLFunction& FindDefaultFunctionByMapID(KEY_MapId_t id)
{
	return FindFunctionByFunctionID(FindFunctionIDByMapID(id));
}

KLFunction& FindCurrentConfigFunctionByMapID(KEY_MapId_t mid)
{

	auto configManager = KLFunctionConfigManager::GetInstance();

	auto& layer = configManager->GetCurrentConfig().layers[configManager->m_CurrentLayerType];

	auto it = layer.find(mid);
	if (it == layer.end())
	{
		return function_map[default_map_function_map[mid]];
	}
	else
	{
		return it->second;
	}
}


void InitDefaultConfig(KLFunctionConfig& temp_config)
{
	for (uint8_t row = 0; row < KEY_ROW_NUM; ++row) {
		for (uint8_t col = 0; col < KB_COL_NUM; ++col) {

			KEY_MapId_t mapId = (KEY_MapId_t)KEY_GetMapIdByRowAndCol(row, col);

			KLFunctionID fid = FindFunctionIDByMapID(mapId);

			KLFunction function = FindFunctionByFunctionID(fid);

			for (uint8_t i = 0; i < KL_LAYER_TYPE_MAXIMUM; ++i)
			{
				temp_config.layers[i][mapId] = function;
			}
		}
	}
}

void DrawFunctionLayout()
{
	auto imageManager = KLImageManager::GetInstance();

	ImVec2 startPoint = ImGui::GetCursorScreenPos();
	ImVec2 pos = startPoint;
	auto io = ImGui::GetIO();

	ImDrawList* dl = ImGui::GetWindowDrawList();

	const float d = 32.0f * dpiScale();
	const float padding = d / 10.0f;
	const float rouding = 0.0f;

	ImGui::SetWindowFontScale(1.0f);

	for (uint32_t i = 0; i < functionLayoutSize; ++i)
	{
		pos.x += d * functionLayout[i].x;
		pos.y += d * functionLayout[i].y;

		ImRect keyRect(pos, ImVec2(pos.x + d * functionLayout[i].w, pos.y + d * functionLayout[i].h));

		bool hover = false;
		bool mouse_down = false;

		if (keyRect.Contains(io.MousePos))
		{
			hover = true;

			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

			mouse_down = io.MouseDown[0];
		}

		if (functionLayout[i].flagbits & KL_FUNC_FLAG_IMAGE)
		{
			auto& image = imageManager->GetImage(functionLayout[i].name);

			dl->AddRectFilled(keyRect.Min, keyRect.Max, hover ? 0xFFFF0000 : 0xFFFFFFFF, rouding);
			dl->AddImage(image.texID, keyRect.Min, keyRect.Max);
		}
		else
		{
			dl->AddRectFilled(keyRect.Min, keyRect.Max, hover ? 0xFFFF0000 : 0xFFFFFFFF, rouding);
			dl->AddText(ImVec2(pos.x + padding, pos.y + padding), 0xFF000000, utils::gbk_to_utf8(functionLayout[i].name).c_str());
		}
		if (mouse_down)
		{
			auto manager = KLFunctionConfigManager::GetInstance();

			auto& functions = manager->GetCurrentConfig().layers[manager->m_CurrentLayerType];

			auto kmId = KeyboardGetActiveMapID();
			if (kmId != KM_NONE)
			{
				functions[kmId] = FindFunctionByFunctionID(functionLayout[i].id);
			}
		}

		pos.x += d * functionLayout[i].w;

		/* break line */
		if (functionLayout[i].id == KLF_PAUSE ||
			functionLayout[i].id == KLF_KP_MINUS ||
			functionLayout[i].id == KLF_KP_PLUS ||
			functionLayout[i].id == KLF_KP_6 ||
			functionLayout[i].id == KLF_KP_ENTER ||
			functionLayout[i].id == KLF_KP_DEL ||
			functionLayout[i].id == KLF_MEDIA_FULLSCREEN)
		{
			pos.x = startPoint.x;
			pos.y += d;
		}
	}
	
	pos.y += 8.0f;

	ImGui::SetCursorScreenPos(pos);
	ImGui::SetWindowFontScale(1);
}


// 要和HID 值对应上
void ShowFunctionWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin(WINNAME_FUNCTION, p_open, window_flags)) {
		ImGui::End();
		return;
	}
	ImGui::GetCurrentWindow()->DockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

	DrawFunctionLayout();


	ImGui::End();
}