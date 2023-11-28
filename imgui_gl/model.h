#pragma once

#include <vector>
#include <iostream>

#include "glm/glm.hpp"

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

} KEY_MapId_t;

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

    float face_vertex[12];

    KeyBtnView(const char* tag, KEY_MapId_t id, int offsetX, int offsetY, int width = SL, int height = SL);
};

KeyBtnView::KeyBtnView(const char* tag, KEY_MapId_t id, int offsetX, int offsetY, int width, int height)
{
    strcpy(this->tag, tag);
    this->id = id;
    this->x = offsetX;
    this->y = offsetY;
    this->w = width;
    this->h = height;

    this->hover = false;
    this->active = false;
}



KeyBtnView keyBtnViewList[] = {
    KeyBtnView("ESC"		, KM_ESC, 0, 0, SL, SL),
    KeyBtnView("F1"			, KM_F1,  (1 * MG + 1 * SL), 0, SL, SL),
    KeyBtnView("F2"			, KM_F2,  (2 * MG + 2 * SL), 0, SL, SL),
    KeyBtnView("F3"			, KM_F3,  (3 * MG + 3 * SL), 0, SL, SL),
    KeyBtnView("F4"			, KM_F4,  (4 * MG + 4 * SL), 0, SL, SL),
    KeyBtnView("F5"			, KM_F5,  (5 * MG + 5 * SL), 0, SL, SL),
    KeyBtnView("F6"			, KM_F6,  (6 * MG + 6 * SL), 0, SL, SL),
    KeyBtnView("F7"			, KM_F7,  (7 * MG + 7 * SL), 0, SL, SL),
    KeyBtnView("F8"			, KM_F8,  (8 * MG + 8 * SL), 0, SL, SL),
    KeyBtnView("F9"			, KM_F9,  (9 * MG + 9 * SL), 0, SL, SL),
    KeyBtnView("F10"		, KM_F10, (10 * MG + 10 * SL), 0, SL, SL),
    KeyBtnView("F11"		, KM_F11, (11 * MG + 11 * SL), 0, SL, SL),
    KeyBtnView("F12"		, KM_F12, (12 * MG + 12 * SL), 0, SL, SL),
    KeyBtnView("INS"		, KM_INS, (13 * MG + 13 * SL), 0, SL, SL),
    KeyBtnView("DEL"		, KM_DEL, (14 * MG + 14 * SL), 0, SL, SL),

    KeyBtnView("~"			, KM_TILDE,                       0, (FL + SL), SL, SL),
    KeyBtnView("1"			, KM_1    , (1 * MG + 1 * SL + 0), (FL + SL), SL, SL),
    KeyBtnView("2"			, KM_2    , (2 * MG + 2 * SL + 0), (FL + SL), SL, SL),
    KeyBtnView("3"			, KM_3    , (3 * MG + 3 * SL + 0), (FL + SL), SL, SL),
    KeyBtnView("4"			, KM_4    , (4 * MG + 4 * SL + 0), (FL + SL), SL, SL),
    KeyBtnView("5"			, KM_5    , (5 * MG + 5 * SL + 0), (FL + SL), SL, SL),
    KeyBtnView("6"			, KM_6    , (6 * MG + 6 * SL + 0), (FL + SL), SL, SL),
    KeyBtnView("7"			, KM_7    , (7 * MG + 7 * SL + 0), (FL + SL), SL, SL),
    KeyBtnView("8"			, KM_8    , (8 * MG + 8 * SL + 0), (FL + SL), SL, SL),
    KeyBtnView("9"			, KM_9    , (9 * MG + 9 * SL + 0), (FL + SL), SL, SL),
    KeyBtnView("0"			, KM_0    , (10 * MG + 10 * SL + 0), (FL + SL), SL, SL),
    KeyBtnView("-"			, KM_UDS  , (11 * MG + 11 * SL + 0), (FL + SL), SL, SL),
    KeyBtnView("="			, KM_PLUS , (12 * MG + 12 * SL + 0), (FL + SL), SL, SL),
    KeyBtnView("BACKSPACE"	, KM_BS   , (13 * MG + 13 * SL + 0), (FL + SL), BACKSPACEW, SL),

    KeyBtnView("TAB"		, KM_TAB, (0 * MG + 0 * SL), (FL + 2 * SL + MG), TABW, SL),
    KeyBtnView("Q"			, KM_Q  , (TABW + 1 * MG + 0 * SL), (FL + 2 * SL + MG), SL, SL),
    KeyBtnView("W"			, KM_W  , (TABW + 2 * MG + 1 * SL), (FL + 2 * SL + MG), SL, SL),
    KeyBtnView("E"			, KM_E  , (TABW + 3 * MG + 2 * SL), (FL + 2 * SL + MG), SL, SL),
    KeyBtnView("R"			, KM_R  , (TABW + 4 * MG + 3 * SL), (FL + 2 * SL + MG), SL, SL),
    KeyBtnView("T"			, KM_T  , (TABW + 5 * MG + 4 * SL), (FL + 2 * SL + MG), SL, SL),
    KeyBtnView("Y"			, KM_Y  , (TABW + 6 * MG + 5 * SL), (FL + 2 * SL + MG), SL, SL),
    KeyBtnView("U"			, KM_U  , (TABW + 7 * MG + 6 * SL), (FL + 2 * SL + MG), SL, SL),
    KeyBtnView("I"			, KM_I  , (TABW + 8 * MG + 7 * SL), (FL + 2 * SL + MG), SL, SL),
    KeyBtnView("O"			, KM_O  , (TABW + 9 * MG + 8 * SL), (FL + 2 * SL + MG), SL, SL),
    KeyBtnView("P"			, KM_P  , (TABW + 10 * MG + 9 * SL), (FL + 2 * SL + MG), SL, SL),
    KeyBtnView("["			, KM_OB , (TABW + 11 * MG + 10 * SL), (FL + 2 * SL + MG), SL, SL),
    KeyBtnView("]"			, KM_CB , (TABW + 12 * MG + 11 * SL), (FL + 2 * SL + MG), SL, SL),
    KeyBtnView("\\"			, KM_BSL, (TABW + 13 * MG + 12 * SL), (FL + 2 * SL + MG), SLASHW, SL),

    KeyBtnView("CAPS LOCK"	, KM_CAPS , (0 * MG + 0 * SL), (FL + 3 * SL + 2 * MG), CAPSLOCKW, SL),
    KeyBtnView("A"			, KM_A    , (CAPSLOCKW + 1 * MG + 0 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView("S"			, KM_S    , (CAPSLOCKW + 2 * MG + 1 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView("D"			, KM_D    , (CAPSLOCKW + 3 * MG + 2 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView("F"			, KM_F    , (CAPSLOCKW + 4 * MG + 3 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView("G"			, KM_G    , (CAPSLOCKW + 5 * MG + 4 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView("H"			, KM_H    , (CAPSLOCKW + 6 * MG + 5 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView("J"			, KM_J    , (CAPSLOCKW + 7 * MG + 6 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView("K"			, KM_K    , (CAPSLOCKW + 8 * MG + 7 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView("L"			, KM_L    , (CAPSLOCKW + 9 * MG + 8 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView(":"			, KM_COLON, (CAPSLOCKW + 10 * MG + 9 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView("'"			, KM_QUOTE, (CAPSLOCKW + 11 * MG + 10 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView("ENTER"		, KM_ENTER, (CAPSLOCKW + 12 * MG + 11 * SL), (FL + 3 * SL + 2 * MG), ENTERW, SL),

    KeyBtnView("SHIFT"      , KM_SHIFT_L, (0 * MG + 0 * SL), (FL + 4 * SL + 3 * MG), LSHIFTW, SL),
    KeyBtnView("B"			, KM_B      , (LSHIFTW + 1 * MG + 0 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
    KeyBtnView("C"			, KM_C      , (LSHIFTW + 2 * MG + 1 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
    KeyBtnView("M"			, KM_M      , (LSHIFTW + 3 * MG + 2 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
    KeyBtnView("N"			, KM_N      , (LSHIFTW + 4 * MG + 3 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
    KeyBtnView("V"			, KM_V      , (LSHIFTW + 5 * MG + 4 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
    KeyBtnView("X"			, KM_X      , (LSHIFTW + 6 * MG + 5 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
    KeyBtnView("Z"			, KM_Z      , (LSHIFTW + 7 * MG + 6 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
    KeyBtnView(","			, KM_COMM   , (LSHIFTW + 8 * MG + 7 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
    KeyBtnView("."			, KM_DOT    , (LSHIFTW + 9 * MG + 8 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
    KeyBtnView("/"			, KM_SLASH  , (LSHIFTW + 10 * MG + 9 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
    KeyBtnView("SHIFT"		, KM_SHIFT_R, (LSHIFTW + 11 * MG + 10 * SL), (FL + 4 * SL + 3 * MG), RSHIFTW, SL),

    KeyBtnView("CTRL"		, KM_CTRL_L, (0)                                                , (FL + 5 * SL + 4 * MG), LCTRLW, SL),
    KeyBtnView("WIN"        , KM_WIN_L , (LCTRLW + 1 * MG)                                  , (FL + 5 * SL + 4 * MG), LWINW , SL),
    KeyBtnView("ALT"		, KM_ALT_L , (LCTRLW + LWINW + 2 * MG)                          , (FL + 5 * SL + 4 * MG), LALTW , SL),
    KeyBtnView("SPACE"      , KM_SP    , (LCTRLW + LWINW + LALTW + 3 * MG)                  , (FL + 5 * SL + 4 * MG), SPACEW, SL),
    KeyBtnView("ALT"        , KM_ALT_R , (LCTRLW + LWINW + LALTW + SPACEW + 4 * MG)         , (FL + 5 * SL + 4 * MG), SL    , SL),
    KeyBtnView("FN"         , KM_FN    , (LCTRLW + LWINW + LALTW + SPACEW + 5 * MG + 1 * SL), (FL + 5 * SL + 4 * MG), SL    , SL),
    KeyBtnView("CTRL"		, KM_CTRL_R, (LCTRLW + LWINW + LALTW + SPACEW + 6 * MG + 2 * SL), (FL + 5 * SL + 4 * MG), SL    , SL),

    KeyBtnView("RIGHT"      , KM_RIGHT, ((LCTRLW + LWINW + LALTW + SPACEW + 6 * MG + 3 * SL + OM) + (0 * SL + 0 * MG)), ((FL + 4 * SL + 2 * MG + OM) + (1 * SL + 1 * MG)), SL, SL),
    KeyBtnView("LEFT"       , KM_LEFT , ((LCTRLW + LWINW + LALTW + SPACEW + 6 * MG + 3 * SL + OM) + (2 * SL + 2 * MG)), ((FL + 4 * SL + 2 * MG + OM) + (1 * SL + 1 * MG)), SL, SL),
    KeyBtnView("DOWN"       , KM_DOWN , ((LCTRLW + LWINW + LALTW + SPACEW + 6 * MG + 3 * SL + OM) + (1 * SL + 1 * MG)), ((FL + 4 * SL + 2 * MG + OM) + (1 * SL + 1 * MG)), SL, SL),
    KeyBtnView("UP"         , KM_UP   , ((LCTRLW + LWINW + LALTW + SPACEW + 6 * MG + 3 * SL + OM) + (1 * SL + 1 * MG)), ((FL + 4 * SL + 2 * MG + OM) + (0 * SL + 0 * MG)), SL, SL),

    KeyBtnView("HOME"		, KM_HOME     , (LW + KP + 0 * MG + 0 * SL), (0), SL, SL),
    KeyBtnView("PRTSC"		, KM_PRINT    , (LW + KP + 1 * MG + 1 * SL), (0), SL, SL),
    KeyBtnView("SCRLK"		, KM_SCL      , (LW + KP + 2 * MG + 2 * SL), (0), SL, SL),
    KeyBtnView("PAUSE"		, KM_PAUSE    , (LW + KP + 3 * MG + 3 * SL), (0), SL, SL),
    KeyBtnView("KP_NUM"		, KM_KP_NUM   , (LW + KP + 0 * MG + 0 * SL), (FL + 1 * SL + 0 * MG), SL, SL),
    KeyBtnView("/"			, KM_KP_DIV   , (LW + KP + 1 * MG + 1 * SL), (FL + 1 * SL + 0 * MG), SL, SL),
    KeyBtnView("*"			, KM_KP_MULT  , (LW + KP + 2 * MG + 2 * SL), (FL + 1 * SL + 0 * MG), SL, SL),
    KeyBtnView("-"			, KM_KP_MINUS , (LW + KP + 3 * MG + 3 * SL), (FL + 1 * SL + 0 * MG), SL, SL),
    KeyBtnView("7"			, KM_KP_7     , (LW + KP + 0 * MG + 0 * SL), (FL + 2 * SL + 1 * MG), SL, SL),
    KeyBtnView("8"			, KM_KP_8     , (LW + KP + 1 * MG + 1 * SL), (FL + 2 * SL + 1 * MG), SL, SL),
    KeyBtnView("9"			, KM_KP_9     , (LW + KP + 2 * MG + 2 * SL), (FL + 2 * SL + 1 * MG), SL, SL),
    KeyBtnView("+"			, KM_KP_PLUS  , (LW + KP + 3 * MG + 3 * SL), (FL + 2 * SL + 1 * MG), SL, NUMPLUSH),
    KeyBtnView("4"			, KM_KP_4     , (LW + KP + 0 * MG + 0 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView("5"			, KM_KP_5     , (LW + KP + 1 * MG + 1 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView("6"			, KM_KP_6     , (LW + KP + 2 * MG + 2 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
    KeyBtnView("1"          , KM_KP_1     , (LW + KP + 0 * MG + 0 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
    KeyBtnView("2"          , KM_KP_2     , (LW + KP + 1 * MG + 1 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
    KeyBtnView("3"          , KM_KP_3     , (LW + KP + 2 * MG + 2 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
    KeyBtnView("ENTER"		, KM_KP_ENTER , (LW + KP + 3 * MG + 3 * SL), (FL + 4 * SL + 3 * MG), SL, NUMENTERH),
    KeyBtnView("0"			, KM_KP_0     , (LW + KP + 1 * MG + 1 * SL), (FL + 5 * SL + 4 * MG), SL, SL),
    KeyBtnView("."			, KM_KP_DEL   , (LW + KP + 2 * MG + 2 * SL), (FL + 5 * SL + 4 * MG), SL, SL),


    //KeyBtnView("#"			, KM_ASH     , 0, 0, SL, SL),
    //
    //KeyBtnView("PAGEUP"		, KM_PAGEUP  , 0, 0, SL, SL),
    //KeyBtnView("END"		, KM_END     , 0, 0, SL, SL),
    //KeyBtnView("PAGEDOWN"	, KM_PAGEDOWN, 0, 0, SL, SL),
    //
    //KeyBtnView("@"			, KM_KP_AT   , 0, 0, SL, SL),
    //KeyBtnView("WIN"		, KM_WIN_R   , 0, 0, SL, SL),
};

int keyBtnViewSize = sizeof(keyBtnViewList) / sizeof(keyBtnViewList[0]);

glm::vec3 cube_point[] = {
    {-0.5f, -0.5f, -0.5f},
    { 0.5f, -0.5f, -0.5f},
    { 0.5f,  0.5f, -0.5f},
    {-0.5f,  0.5f, -0.5f},
    {-0.5f, -0.5f,  0.5f},
    { 0.5f, -0.5f,  0.5f},
    { 0.5f,  0.5f,  0.5f},
    {-0.5f,  0.5f,  0.5f},
};

float vertices_cube[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

void DrawSurfaceTri(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec4 rgba, std::vector<float>& output)
{
    glm::vec3 d1 = p1 - p2;
    glm::vec3 d2 = p3 - p2;

    glm::vec3 n = glm::cross(d1, d2);
    n = glm::normalize(n);
    
    output.push_back(p1.x);
    output.push_back(p1.y);
    output.push_back(p1.z);
    output.push_back(n.x);
    output.push_back(n.y);
    output.push_back(n.z);
    output.push_back(rgba.r);
    output.push_back(rgba.g);
    output.push_back(rgba.b);
    output.push_back(rgba.a);
    output.push_back(p2.x);
    output.push_back(p2.y);
    output.push_back(p2.z);
    output.push_back(n.x);
    output.push_back(n.y);
    output.push_back(n.z);
    output.push_back(rgba.r);
    output.push_back(rgba.g);
    output.push_back(rgba.b);
    output.push_back(rgba.a);
    output.push_back(p3.x);
    output.push_back(p3.y);
    output.push_back(p3.z);
    output.push_back(n.x);
    output.push_back(n.y);
    output.push_back(n.z);
    output.push_back(rgba.r);
    output.push_back(rgba.g);
    output.push_back(rgba.b);
    output.push_back(rgba.a);
}

void DrawSurfaceRect(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec4 rgba, std::vector<float>& output)
{
    DrawSurfaceTri(p1, p2, p3, rgba, output);
    DrawSurfaceTri(p3, p4, p1, rgba, output);
}

void DrawKeycap(glm::vec3 pos, glm::vec3 shap, float padding, float thickness, glm::vec4 rgba, std::vector<float>& output)
{
    glm::vec3 p[16];
    p[0] = pos + glm::vec3(  0.0f,   0.0f, 0.0f);
    p[1] = pos + glm::vec3(shap.x,   0.0f, 0.0f);
    p[2] = pos + glm::vec3(shap.x, shap.y, 0.0f);
    p[3] = pos + glm::vec3(  0.0f, shap.y, 0.0f);

    p[4] = p[0] + glm::vec3( thickness,  thickness, 0.0f);
    p[5] = p[1] + glm::vec3(-thickness,  thickness, 0.0f);
    p[6] = p[2] + glm::vec3(-thickness, -thickness, 0.0f);
    p[7] = p[3] + glm::vec3( thickness, -thickness, 0.0f);

    p[8]  = p[0] + glm::vec3( padding,  padding, shap.z);
    p[9]  = p[1] + glm::vec3(-padding,  padding, shap.z);
    p[10] = p[2] + glm::vec3(-padding, -padding, shap.z);
    p[11] = p[3] + glm::vec3( padding, -padding, shap.z);

    p[12] = p[8]  + glm::vec3( thickness,  thickness, -thickness);
    p[13] = p[9]  + glm::vec3(-thickness,  thickness, -thickness);
    p[14] = p[10] + glm::vec3(-thickness, -thickness, -thickness);
    p[15] = p[11] + glm::vec3( thickness, -thickness, -thickness);

    for (int i = 0; i < 16; ++i)
        p[i] *= glm::vec3(1.0f, -1.0f, 1.0f);

    DrawSurfaceRect(p[0], p[4], p[7], p[3], rgba, output);
    DrawSurfaceRect(p[1], p[5], p[4], p[0], rgba, output);
    DrawSurfaceRect(p[2], p[6], p[5], p[1], rgba, output);
    DrawSurfaceRect(p[3], p[7], p[6], p[2], rgba, output);

    DrawSurfaceRect(p[8], p[0], p[3], p[11], rgba, output);
    DrawSurfaceRect(p[9], p[1], p[0], p[8], rgba, output);
    DrawSurfaceRect(p[10], p[2], p[1], p[9], rgba, output);
    DrawSurfaceRect(p[11], p[3], p[2], p[10], rgba, output);

    DrawSurfaceRect(p[15], p[7], p[4], p[12], rgba, output);
    DrawSurfaceRect(p[12], p[4], p[5], p[13], rgba, output);
    DrawSurfaceRect(p[13], p[5], p[6], p[14], rgba, output);
    DrawSurfaceRect(p[14], p[6], p[7], p[15], rgba, output);

    DrawSurfaceRect(p[11], p[10], p[9], p[8], rgba, output);
    DrawSurfaceRect(p[12], p[13], p[14], p[15], rgba, output);
}

void KeyCapInit(glm::vec3 pos, glm::vec3 shap, float padding, float* face_vertex) 
{
    glm::vec3 p[4];
    p[0] = pos + glm::vec3(  0.0f,   0.0f, 0.0f) + glm::vec3( padding,  padding, shap.z);
    p[1] = pos + glm::vec3(shap.x,   0.0f, 0.0f) + glm::vec3(-padding,  padding, shap.z);
    p[2] = pos + glm::vec3(shap.x, shap.y, 0.0f) + glm::vec3(-padding, -padding, shap.z);
    p[3] = pos + glm::vec3(  0.0f, shap.y, 0.0f) + glm::vec3( padding, -padding, shap.z);

    for (int i = 0; i < 4; ++i)
        p[i] *= glm::vec3(1.0f, -1.0f, 1.0f);

    face_vertex[0] = p[0].x;
    face_vertex[1] = p[0].y;
    face_vertex[2] = p[0].z;
    face_vertex[3] = p[1].x;
    face_vertex[4] = p[1].y;
    face_vertex[5] = p[1].z;
    face_vertex[6] = p[2].x;
    face_vertex[7] = p[2].y;
    face_vertex[8] = p[2].z;
    face_vertex[9] = p[3].x;
    face_vertex[10] = p[3].y;
    face_vertex[11] = p[3].z;
}

#define SECTOR 180.0f

void RenderModelInit()
{
    glm::vec3 global_pos(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < keyBtnViewSize; ++i)
    {
        glm::vec3 pos1(global_pos.x + keyBtnViewList[i].x, global_pos.y + keyBtnViewList[i].y, 0.0f);
        glm::vec3 shape(keyBtnViewList[i].w, keyBtnViewList[i].h, 90.0f);
        pos1 /= SECTOR;
        shape /= SECTOR;

        KeyCapInit(pos1, shape, 0.1f, keyBtnViewList[i].face_vertex);
    }
}

std::vector<float> RenderModelUpdate()
{
    std::vector<float> collection(0);

    glm::vec3 global_pos(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < keyBtnViewSize; ++i)
    {
        glm::vec3 pos1(global_pos.x + keyBtnViewList[i].x, global_pos.y + keyBtnViewList[i].y, 0.0f);
        glm::vec3 shape(keyBtnViewList[i].w, keyBtnViewList[i].h, 90.0f);
        pos1 /= SECTOR;
        shape /= SECTOR;

        if (keyBtnViewList[i].hover) 
        {
            DrawKeycap(pos1, shape, 0.1f, 0.1f, glm::vec4(1.0f, 0.0f, 0.0f, 0.1f), collection);
        }
        else
        {
            DrawKeycap(pos1, shape, 0.1f, 0.1f, glm::vec4(1.0f, 1.0f, 1.0f, 0.1f), collection);
        }
    }

    return collection;
}