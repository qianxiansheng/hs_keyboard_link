#pragma once

#include "pch.h"


#include "glad/glad/glad.h"
#include "glm/glm.hpp"
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>


#define KEY_ROW_NUM             6
#define KB_COL_NUM              18

#define PADDING 0.1f
#define THINKNESS 0.1f
#define ACTIVE_COLOR glm::vec4(0.0f, 1.0f, 0.0f, 0.5f)
#define HOVER_COLOR glm::vec4(1.0f, 0.0f, 0.0f, 0.5f)
#define MODIFIED_COLOR glm::vec4(0.0f, 0.0f, 1.0f, 0.5f)
#define MODIFIED_ACTIVE_COLOR glm::vec4(1.0f, 0.0f, 1.0f, 0.5f)
#define DEFAULT_COLOR glm::vec4(0.7f, 0.7f, 0.7f, 0.5f)
#define BOARD_DEFAULT_COLOR glm::vec4(0.0f, 0.0f, 0.0f, 0.8f)

#define SECTOR 180.0f
#define KEY_DEPTH 90.0f

#define SL 180
#define MG 15

#define FL 90
#define KP 120
#define OM 60

#define LW (15 * SL + 14 * MG)
#define BACKSPACEW (LW - 13 * SL - 13 * MG)
#define TABW (SL + MG + SL / 2)
#define SLASHW (LW - 12 * SL - 13 * MG - TABW)

#define CAPSLOCKW (TABW + 2 * MG)
#define ENTERW (LW - 11 * SL - 12* MG - CAPSLOCKW)
#define LSHIFTW (CAPSLOCKW + MG + SL / 2)
#define RSHIFTW (LW - 11 * SL - 12* MG - LSHIFTW)

#define LCTRLW ((LSHIFTW + SL + SL / 2) / 3)
#define LWINW LCTRLW
#define LALTW LCTRLW
#define SPACEW (LW - LCTRLW - LWINW - LALTW - 5 * SL - 8 * MG)
#define RALTW SL
#define RFNW SL
#define RCTRLW SL

#define NUMPLUSH (2 * SL + MG)
#define NUMENTERH NUMPLUSH

#define BGC 0xFF007FFF


enum HSKeyFlagBit
{
	HSKEY_FLAG_BIT_NONE = 0x00000000,
	HSKEY_FLAG_BIT_SHIFT = 0x00000001,
	HSKEY_FLAG_BIT_MAX = 0xFFFFFFFF,
};
typedef int HSKeyFlagBits;


typedef enum
{
    // NO KEY.
    KM_NONE,

    // KEY_TYPE_GENERAL
    KM_A,
    KM_B,
    KM_C,
    KM_D,
    KM_E,
    KM_F,
    KM_G,
    KM_H,
    KM_I,
    KM_J,
    KM_K,
    KM_L,
    KM_M,
    KM_N,
    KM_O,
    KM_P,
    KM_Q,
    KM_R,
    KM_S,
    KM_T,
    KM_U,
    KM_V,
    KM_W,
    KM_X,
    KM_Y,
    KM_Z,
    KM_1,
    KM_2,
    KM_3,
    KM_4,
    KM_5,
    KM_6,
    KM_7,
    KM_8,
    KM_9,
    KM_0,
    KM_ENTER,
    KM_ESC,
    KM_BS, // BACKSPACE
    KM_TAB,
    KM_SP,
    KM_UDS,   // '-'
    KM_PLUS,  // '='
    KM_OB,    // '['
    KM_CB,    // ']'
    KM_BSL,   // '\'
    KM_ASH,   // '#'
    KM_COLON, // ':'
    KM_QUOTE, // '''
    KM_TILDE, // '~'
    KM_COMM,  // ','
    KM_DOT,   // '.'
    KM_SLASH, // '/'
    KM_CAPS,
    KM_F1,
    KM_F2,
    KM_F3,
    KM_F4,
    KM_F5,
    KM_F6,
    KM_F7,
    KM_F8,
    KM_F9,
    KM_F10,
    KM_F11,
    KM_F12,
    KM_PRINT, // PRINT SCREEN
    KM_SCL,   // SCROLL LOCK
    KM_PAUSE, // PAUSE
    KM_INS,   // INSERT
    KM_HOME,
    KM_PAGEUP,
    KM_DEL,
    KM_END,
    KM_PAGEDOWN,
    KM_RIGHT,
    KM_LEFT,
    KM_DOWN,
    KM_UP,
    KM_KP_NUM,   // NUM-LOCK
    KM_KP_DIV,   // NUM-/
    KM_KP_AT,    // NUM-@
    KM_KP_MULT,  // NUM-*
    KM_KP_MINUS, // NUM--
    KM_KP_PLUS,  // NUM-+
    KM_KP_ENTER, // NUM-ENTER
    KM_KP_1,
    KM_KP_2,
    KM_KP_3,
    KM_KP_4,
    KM_KP_5,
    KM_KP_6,
    KM_KP_7,
    KM_KP_8,
    KM_KP_9,
    KM_KP_0,
    KM_KP_DEL,

    // KEY_TYPE_MODIFIER
    KM_CTRL_L,
    KM_SHIFT_L,
    KM_ALT_L,
    KM_WIN_L,
    KM_CTRL_R,
    KM_SHIFT_R,
    KM_ALT_R,
    KM_WIN_R,

    // KEY_TYPE_VENDOR_DEFINE.
    KM_FN,

    KM_MAXIMUM,

} KEY_MapId_t;

typedef enum
{
    HID_KEYB_A = 4,
    HID_KEYB_B = 5,
    HID_KEYB_C = 6,
    HID_KEYB_D = 7,
    HID_KEYB_E = 8,
    HID_KEYB_F = 9,
    HID_KEYB_G = 10,
    HID_KEYB_H = 11,
    HID_KEYB_I = 12,
    HID_KEYB_J = 13,
    HID_KEYB_K = 14,
    HID_KEYB_L = 15,
    HID_KEYB_M = 16,
    HID_KEYB_N = 17,
    HID_KEYB_O = 18,
    HID_KEYB_P = 19,
    HID_KEYB_Q = 20,
    HID_KEYB_R = 21,
    HID_KEYB_S = 22,
    HID_KEYB_T = 23,
    HID_KEYB_U = 24,
    HID_KEYB_V = 25,
    HID_KEYB_W = 26,
    HID_KEYB_X = 27,
    HID_KEYB_Y = 28,
    HID_KEYB_Z = 29,
    HID_KEYB_1 = 30,
    HID_KEYB_2 = 31,
    HID_KEYB_3 = 32,
    HID_KEYB_4 = 33,
    HID_KEYB_5 = 34,
    HID_KEYB_6 = 35,
    HID_KEYB_7 = 36,
    HID_KEYB_8 = 37,
    HID_KEYB_9 = 38,
    HID_KEYB_0 = 39,
    HID_KEYB_ENTER = 40,
    HID_KEYB_ESCAPE = 41,
    HID_KEYB_BACKSPACE = 42,
    HID_KEYB_TAB = 43,
    HID_KEYB_SPACEBAR = 44,
    HID_KEYB_UNDERSCORE = 45,
    HID_KEYB_PLUS = 46,
    HID_KEYB_OPEN_BRACKET = 47,
    HID_KEYB_CLOSE_BRACKET = 48,
    HID_KEYB_BACKSLASH = 49,
    HID_KEYB_ASH = 50,
    HID_KEYB_COLON = 51,
    HID_KEYB_QUOTE = 52,
    HID_KEYB_TILDE = 53,
    HID_KEYB_COMMA = 54,
    HID_KEYB_DOT = 55,
    HID_KEYB_SLASH = 56,
    HID_KEYB_CAPS_LOCK = 57,
    HID_KEYB_F1 = 58,
    HID_KEYB_F2 = 59,
    HID_KEYB_F3 = 60,
    HID_KEYB_F4 = 61,
    HID_KEYB_F5 = 62,
    HID_KEYB_F6 = 63,
    HID_KEYB_F7 = 64,
    HID_KEYB_F8 = 65,
    HID_KEYB_F9 = 66,
    HID_KEYB_F10 = 67,
    HID_KEYB_F11 = 68,
    HID_KEYB_F12 = 69,
    HID_KEYB_PRINTSCREEN = 70,
    HID_KEYB_SCROLL_LOCK = 71,
    HID_KEYB_PAUSE = 72,
    HID_KEYB_INSERT = 73,
    HID_KEYB_HOME = 74,
    HID_KEYB_PAGEUP = 75,
    HID_KEYB_DELETE = 76,
    HID_KEYB_END = 77,
    HID_KEYB_PAGEDOWN = 78,
    HID_KEYB_RIGHT = 79,
    HID_KEYB_LEFT = 80,
    HID_KEYB_DOWN = 81,
    HID_KEYB_UP = 82,
    HID_KEYB_KEYPAD_NUM_LOCK = 83,
    HID_KEYB_KEYPAD_DIVIDE = 84,
    HID_KEYB_KEYPAD_AT = 85,
    HID_KEYB_KEYPAD_MULTIPLY = 85,
    HID_KEYB_KEYPAD_MINUS = 86,
    HID_KEYB_KEYPAD_PLUS = 87,
    HID_KEYB_KEYPAD_ENTER = 88,
    HID_KEYB_KEYPAD_1 = 89,
    HID_KEYB_KEYPAD_2 = 90,
    HID_KEYB_KEYPAD_3 = 91,
    HID_KEYB_KEYPAD_4 = 92,
    HID_KEYB_KEYPAD_5 = 93,
    HID_KEYB_KEYPAD_6 = 94,
    HID_KEYB_KEYPAD_7 = 95,
    HID_KEYB_KEYPAD_8 = 96,
    HID_KEYB_KEYPAD_9 = 97,
    HID_KEYB_KEYPAD_0 = 98,
    HID_KEYB_KEYPAD_DOT = 99
} BSP_KEYB_KEYB_USAGE_ID_e;

typedef enum
{
    HID_KEYB_MODIFIER_LEFT_CTRL = 0x01,
    HID_KEYB_MODIFIER_LEFT_SHIFT = 0x02,
    HID_KEYB_MODIFIER_LEFT_ALT = 0x04,
    HID_KEYB_MODIFIER_LEFT_UI = 0x08,
    HID_KEYB_MODIFIER_RIGHT_CTRL = 0x10,
    HID_KEYB_MODIFIER_RIGHT_SHIFT = 0x20,
    HID_KEYB_MODIFIER_RIGHT_ALT = 0x40,
    HID_KEYB_MODIFIER_RIGHT_UI = 0x80
} BSP_KEYB_KEYB_MODIFIER_e;

typedef enum
{
    HID_KEYB_LED_NUM_LOCK = 0x01,
    HID_KEYB_LED_CAPS_LOCK = 0x02,
    HID_KEYB_LED_SCROLL_LOCK = 0x04,
    HID_KEYB_LED_COMPOSE = 0x08,
    HID_KEYB_LED_KANA = 0x10
} BSP_KEYB_KEYB_LED_e;

typedef enum
{
    HID_KEYB_VENDOR_FN,
} BSP_KEYB_VENDOR_DEFINE_e;

typedef enum
{
    HID_MEDIA_PLAY,
    HID_MEDIA_TERMINATE,
    HID_MEDIA_BACKWARD,
    HID_MEDIA_FORWARD,
    HID_MEDIA_VOLUME_INC,
    HID_MEDIA_VOLUME_DEC,
    HID_MEDIA_MUTE,
    HID_MEDIA_HOME,
    HID_MEDIA_CALC,
    HID_MEDIA_EMAIL,
    HID_MEDIA_COMPUTER,
    HID_MEDIA_COLLECT,
    HID_MEDIA_BRIGHTNESS_INC,
    HID_MEDIA_BRIGHTNESS_DEC,
    HID_MEDIA_PREV,
    HID_MEDIA_NEXT,
    HID_MEDIA_FULLSCREEN,
} BSP_MEDIA_UAGE_ID_e;

typedef enum {
    KEY_TYPE_GENERAL,
    KEY_TYPE_MODIFIER,
    KEY_TYPE_VENDOR_DEFINE,
} BSP_HID_KB_Type_t;

typedef struct {
    KEY_MapId_t map_id;
    BSP_HID_KB_Type_t hid_type;
    uint8_t hid_key;
} KEY_ID_t;


const uint8_t usr_key_map[KEY_ROW_NUM][KB_COL_NUM] =
{
    {KM_ESC    ,  KM_F1,    KM_F2,    KM_F3,   KM_F4,   KM_F5,  KM_F6,   KM_F7,   KM_F8,   KM_F9,    KM_F10,   KM_F11,   KM_F12,     KM_INS,   KM_HOME,   KM_PRINT,  KM_SCL,     KM_PAUSE   },
    {KM_TILDE  ,  KM_1 ,    KM_2 ,    KM_3 ,   KM_4 ,   KM_5 ,  KM_6 ,   KM_7 ,   KM_8 ,   KM_9 ,    KM_0  ,   KM_UDS,   KM_PLUS,    KM_BS,    KM_KP_NUM, KM_KP_DIV, KM_KP_MULT, KM_KP_MINUS},
    {KM_TAB    ,  KM_Q ,    KM_W ,    KM_E ,   KM_R ,   KM_T ,  KM_Y ,   KM_U ,   KM_I ,   KM_O ,    KM_P  ,   KM_OB,    KM_CB,      KM_BSL,   KM_KP_7,   KM_KP_8,   KM_KP_9,    KM_KP_PLUS },
    {KM_CAPS   ,  KM_A ,    KM_S ,    KM_D ,   KM_F ,   KM_G ,  KM_H ,   KM_J ,   KM_K ,   KM_L ,    KM_COLON, KM_QUOTE, KM_NONE,    KM_ENTER, KM_KP_4,   KM_KP_5,   KM_KP_6,    KM_NONE    },
    {KM_SHIFT_L,  KM_Z ,    KM_X ,    KM_C ,   KM_V ,   KM_B ,  KM_N ,   KM_M ,   KM_COMM, KM_DOT,   KM_SLASH, KM_DEL,   KM_SHIFT_R, KM_UP,    KM_KP_1,   KM_KP_2,   KM_KP_3,    KM_KP_ENTER},
    {KM_CTRL_L ,  KM_WIN_L, KM_ALT_L, KM_NONE, KM_NONE, KM_SP,  KM_NONE, KM_NONE, KM_NONE, KM_ALT_R, KM_FN,    KM_CTRL_R,KM_LEFT,    KM_DOWN,  KM_RIGHT,  KM_KP_0,   KM_KP_DEL,  KM_NONE    },
};

const KEY_ID_t KeyMapId_to_HidId[] =
{
    // map_id, hid_type, hid_key
    // general keys.
    { KM_A              ,    KEY_TYPE_GENERAL,    HID_KEYB_A               },
    { KM_B              ,    KEY_TYPE_GENERAL,    HID_KEYB_B               },
    { KM_C              ,    KEY_TYPE_GENERAL,    HID_KEYB_C               },
    { KM_D              ,    KEY_TYPE_GENERAL,    HID_KEYB_D               },
    { KM_E              ,    KEY_TYPE_GENERAL,    HID_KEYB_E               },
    { KM_F              ,    KEY_TYPE_GENERAL,    HID_KEYB_F               },
    { KM_G              ,    KEY_TYPE_GENERAL,    HID_KEYB_G               },
    { KM_H              ,    KEY_TYPE_GENERAL,    HID_KEYB_H               },
    { KM_I              ,    KEY_TYPE_GENERAL,    HID_KEYB_I               },
    { KM_J              ,    KEY_TYPE_GENERAL,    HID_KEYB_J               },
    { KM_K              ,    KEY_TYPE_GENERAL,    HID_KEYB_K               },
    { KM_L              ,    KEY_TYPE_GENERAL,    HID_KEYB_L               },
    { KM_M              ,    KEY_TYPE_GENERAL,    HID_KEYB_M               },
    { KM_N              ,    KEY_TYPE_GENERAL,    HID_KEYB_N               },
    { KM_O              ,    KEY_TYPE_GENERAL,    HID_KEYB_O               },
    { KM_P              ,    KEY_TYPE_GENERAL,    HID_KEYB_P               },
    { KM_Q              ,    KEY_TYPE_GENERAL,    HID_KEYB_Q               },
    { KM_R              ,    KEY_TYPE_GENERAL,    HID_KEYB_R               },
    { KM_S              ,    KEY_TYPE_GENERAL,    HID_KEYB_S               },
    { KM_T              ,    KEY_TYPE_GENERAL,    HID_KEYB_T               },
    { KM_U              ,    KEY_TYPE_GENERAL,    HID_KEYB_U               },
    { KM_V              ,    KEY_TYPE_GENERAL,    HID_KEYB_V               },
    { KM_W              ,    KEY_TYPE_GENERAL,    HID_KEYB_W               },
    { KM_X              ,    KEY_TYPE_GENERAL,    HID_KEYB_X               },
    { KM_Y              ,    KEY_TYPE_GENERAL,    HID_KEYB_Y               },
    { KM_Z              ,    KEY_TYPE_GENERAL,    HID_KEYB_Z               },
    { KM_1              ,    KEY_TYPE_GENERAL,    HID_KEYB_1               },
    { KM_2              ,    KEY_TYPE_GENERAL,    HID_KEYB_2               },
    { KM_3              ,    KEY_TYPE_GENERAL,    HID_KEYB_3               },
    { KM_4              ,    KEY_TYPE_GENERAL,    HID_KEYB_4               },
    { KM_5              ,    KEY_TYPE_GENERAL,    HID_KEYB_5               },
    { KM_6              ,    KEY_TYPE_GENERAL,    HID_KEYB_6               },
    { KM_7              ,    KEY_TYPE_GENERAL,    HID_KEYB_7               },
    { KM_8              ,    KEY_TYPE_GENERAL,    HID_KEYB_8               },
    { KM_9              ,    KEY_TYPE_GENERAL,    HID_KEYB_9               },
    { KM_0              ,    KEY_TYPE_GENERAL,    HID_KEYB_0               },
    { KM_ENTER          ,    KEY_TYPE_GENERAL,    HID_KEYB_ENTER           },
    { KM_ESC            ,    KEY_TYPE_GENERAL,    HID_KEYB_ESCAPE          },
    { KM_BS             ,    KEY_TYPE_GENERAL,    HID_KEYB_BACKSPACE       },
    { KM_TAB            ,    KEY_TYPE_GENERAL,    HID_KEYB_TAB             },
    { KM_SP             ,    KEY_TYPE_GENERAL,    HID_KEYB_SPACEBAR        },
    { KM_UDS            ,    KEY_TYPE_GENERAL,    HID_KEYB_UNDERSCORE      },
    { KM_PLUS           ,    KEY_TYPE_GENERAL,    HID_KEYB_PLUS            },
    { KM_OB             ,    KEY_TYPE_GENERAL,    HID_KEYB_OPEN_BRACKET    },
    { KM_CB             ,    KEY_TYPE_GENERAL,    HID_KEYB_CLOSE_BRACKET   },
    { KM_BSL            ,    KEY_TYPE_GENERAL,    HID_KEYB_BACKSLASH       },
    { KM_ASH            ,    KEY_TYPE_GENERAL,    HID_KEYB_ASH             },
    { KM_COLON          ,    KEY_TYPE_GENERAL,    HID_KEYB_COLON           },
    { KM_QUOTE          ,    KEY_TYPE_GENERAL,    HID_KEYB_QUOTE           },
    { KM_TILDE          ,    KEY_TYPE_GENERAL,    HID_KEYB_TILDE           },
    { KM_COMM           ,    KEY_TYPE_GENERAL,    HID_KEYB_COMMA           },
    { KM_DOT            ,    KEY_TYPE_GENERAL,    HID_KEYB_DOT             },
    { KM_SLASH          ,    KEY_TYPE_GENERAL,    HID_KEYB_SLASH           },
    { KM_CAPS           ,    KEY_TYPE_GENERAL,    HID_KEYB_CAPS_LOCK       },
    { KM_F1             ,    KEY_TYPE_GENERAL,    HID_KEYB_F1              },
    { KM_F2             ,    KEY_TYPE_GENERAL,    HID_KEYB_F2              },
    { KM_F3             ,    KEY_TYPE_GENERAL,    HID_KEYB_F3              },
    { KM_F4             ,    KEY_TYPE_GENERAL,    HID_KEYB_F4              },
    { KM_F5             ,    KEY_TYPE_GENERAL,    HID_KEYB_F5              },
    { KM_F6             ,    KEY_TYPE_GENERAL,    HID_KEYB_F6              },
    { KM_F7             ,    KEY_TYPE_GENERAL,    HID_KEYB_F7              },
    { KM_F8             ,    KEY_TYPE_GENERAL,    HID_KEYB_F8              },
    { KM_F9             ,    KEY_TYPE_GENERAL,    HID_KEYB_F9              },
    { KM_F10            ,    KEY_TYPE_GENERAL,    HID_KEYB_F10             },
    { KM_F11            ,    KEY_TYPE_GENERAL,    HID_KEYB_F11             },
    { KM_F12            ,    KEY_TYPE_GENERAL,    HID_KEYB_F12             },
    { KM_PRINT          ,    KEY_TYPE_GENERAL,    HID_KEYB_PRINTSCREEN     },
    { KM_SCL            ,    KEY_TYPE_GENERAL,    HID_KEYB_SCROLL_LOCK     },
    { KM_PAUSE          ,    KEY_TYPE_GENERAL,    HID_KEYB_PAUSE           },
    { KM_INS            ,    KEY_TYPE_GENERAL,    HID_KEYB_INSERT          },
    { KM_HOME           ,    KEY_TYPE_GENERAL,    HID_KEYB_HOME            },
    { KM_PAGEUP         ,    KEY_TYPE_GENERAL,    HID_KEYB_PAGEUP          },
    { KM_DEL            ,    KEY_TYPE_GENERAL,    HID_KEYB_DELETE          },
    { KM_END            ,    KEY_TYPE_GENERAL,    HID_KEYB_END             },
    { KM_PAGEDOWN       ,    KEY_TYPE_GENERAL,    HID_KEYB_PAGEDOWN        },
    { KM_RIGHT          ,    KEY_TYPE_GENERAL,    HID_KEYB_RIGHT           },
    { KM_LEFT           ,    KEY_TYPE_GENERAL,    HID_KEYB_LEFT            },
    { KM_DOWN           ,    KEY_TYPE_GENERAL,    HID_KEYB_DOWN            },
    { KM_UP             ,    KEY_TYPE_GENERAL,    HID_KEYB_UP              },
    { KM_KP_NUM         ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_NUM_LOCK },
    { KM_KP_DIV         ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_DIVIDE   },
    { KM_KP_AT          ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_AT       },
    { KM_KP_MULT        ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_MULTIPLY },
    { KM_KP_MINUS       ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_MINUS    },
    { KM_KP_PLUS        ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_PLUS     },
    { KM_KP_ENTER       ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_ENTER    },
    { KM_KP_1           ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_1        },
    { KM_KP_2           ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_2        },
    { KM_KP_3           ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_3        },
    { KM_KP_4           ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_4        },
    { KM_KP_5           ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_5        },
    { KM_KP_6           ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_6        },
    { KM_KP_7           ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_7        },
    { KM_KP_8           ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_8        },
    { KM_KP_9           ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_9        },
    { KM_KP_0           ,    KEY_TYPE_GENERAL,    HID_KEYB_KEYPAD_0        },
    { KM_KP_DEL         ,    KEY_TYPE_GENERAL,    99   },//HID_KEYB_KEYPAD_DELETE

    // modifier keys.
    { KM_CTRL_L         ,    KEY_TYPE_MODIFIER,    HID_KEYB_MODIFIER_LEFT_CTRL   },
    { KM_SHIFT_L        ,    KEY_TYPE_MODIFIER,    HID_KEYB_MODIFIER_LEFT_SHIFT  },
    { KM_ALT_L          ,    KEY_TYPE_MODIFIER,    HID_KEYB_MODIFIER_LEFT_ALT    },
    { KM_WIN_L          ,    KEY_TYPE_MODIFIER,    HID_KEYB_MODIFIER_LEFT_UI     },
    { KM_CTRL_R         ,    KEY_TYPE_MODIFIER,    HID_KEYB_MODIFIER_RIGHT_CTRL  },
    { KM_SHIFT_R        ,    KEY_TYPE_MODIFIER,    HID_KEYB_MODIFIER_RIGHT_SHIFT },
    { KM_ALT_R          ,    KEY_TYPE_MODIFIER,    HID_KEYB_MODIFIER_RIGHT_ALT   },
    { KM_WIN_R          ,    KEY_TYPE_MODIFIER,    HID_KEYB_MODIFIER_RIGHT_UI    },

    // vendor define keys.
    { KM_FN             ,    KEY_TYPE_VENDOR_DEFINE,    HID_KEYB_VENDOR_FN },
};

uint8_t KEY_GetMapIdByRowAndCol(uint8_t row, uint8_t col);
uint8_t KEY_HidType_And_HidKey_Get_ByMapId(uint8_t map_id, BSP_HID_KB_Type_t* hid_type, uint8_t* hid_key);
uint8_t KEY_GetHidTypeAndHidKey(uint8_t row, uint8_t col, BSP_HID_KB_Type_t* hid_type, uint8_t* hid_key);



struct KeyBtnView
{
    char tag[16];
    KEY_MapId_t id;
    int x;
    int y;
    int w;
    int h;
    // HSKeyFlagBits flags;

    bool hover;
    bool active;

    GLuint VBO, VAO;
    std::vector<float> vertices;

    GLuint LightVBO, LightVAO;
    std::vector<float> LightVertices;
    glm::vec4 color;

    int brightness;

    float face_vertex[12];

    KeyBtnView() {};
    KeyBtnView(const char* tag, KEY_MapId_t id, int offsetX, int offsetY, int width = SL, int height = SL);
};


typedef void (*kbv_hover_cb)(KeyBtnView& view);

struct KeyboardGLContext
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;

    std::unique_ptr<Shader> shader;
    std::unique_ptr<Shader> shader_board;

    Camera camera;
    glm::vec2 mouse_pos;

    glm::vec4 clear_color;

    /* background board */
    std::vector<float> BoardVertices;
    GLuint BoardVAO;
    GLuint BoardVBO;

    GLuint FBO;
    GLuint UBO;
    GLuint texColorBuffer;
    GLuint rbo;

    glm::mat4 projection_matrix;
    glm::mat4 view_matrix;
    glm::mat4 model_matrix;

    bool mouse_down;
    KEY_MapId_t active_index;

    kbv_hover_cb onhover;
};

typedef KeyboardGLContext KeyboardInstance;

void ShowKeyboardWindow(bool* p_open);

void KeyboardGLReCreate(int width, int height);
void KeyboardGLInit(int width, int height);
void KeyboardGLDraw();
void KeyboardGLDestroy();

KeyboardInstance* KeyboardGetInstance();
KEY_MapId_t KeyboardGetActiveMapID();