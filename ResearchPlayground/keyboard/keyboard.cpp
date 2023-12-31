#include "keyboard.h"

#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"

#include "util/utils.h"

#include "function/function.h"
#include "layout.h"
#include "resources.h"

#include "keylink.h"

#include "ui_thread.h"
#include "statusbar.h"
#include "protocol/iap.h"
#include "language.h"
#include "light/light.h"
#include "logger.h"

/* kb map */
uint8_t KEY_GetMapIdByRowAndCol(uint8_t row, uint8_t col) {
	return usr_key_map[row][col];
}

uint8_t KEY_HidType_And_HidKey_Get_ByMapId(uint8_t map_id, BSP_HID_KB_Type_t* hid_type, uint8_t* hid_key) {
	for (uint8_t index = 0; index < (sizeof(KeyMapId_to_HidId) / sizeof(KeyMapId_to_HidId[0])); index++) {
		if (map_id == KeyMapId_to_HidId[index].map_id) {
			*hid_type = KeyMapId_to_HidId[index].hid_type;
			*hid_key = KeyMapId_to_HidId[index].hid_key;
			return 0;
		}
	}
	return 1; // Not found.
}

uint8_t KEY_GetHidTypeAndHidKey(uint8_t row, uint8_t col, BSP_HID_KB_Type_t* hid_type, uint8_t* hid_key) {

	/* Get map id. */
	uint8_t map_id = KEY_GetMapIdByRowAndCol(row, col);

	/* Get HID type and hid_id. */
	if (KEY_HidType_And_HidKey_Get_ByMapId(map_id, hid_type, hid_key))
		return 1; // Not found.

	return 0;
}




KeyboardGLContext kbv_draw_ctx;

std::unordered_map<KEY_MapId_t, KeyBtnView> keyBtnViewList = {
	{KM_ESC      , KeyBtnView("ESC"        , KM_ESC      , 0, 0, SL, SL)},
	{KM_F1       , KeyBtnView("F1"         , KM_F1       , (1 * MG + 1 * SL), 0, SL, SL)},
	{KM_F2       , KeyBtnView("F2"         , KM_F2       , (2 * MG + 2 * SL), 0, SL, SL)},
	{KM_F3       , KeyBtnView("F3"         , KM_F3       , (3 * MG + 3 * SL), 0, SL, SL)},
	{KM_F4       , KeyBtnView("F4"         , KM_F4       , (4 * MG + 4 * SL), 0, SL, SL)},
	{KM_F5       , KeyBtnView("F5"         , KM_F5       , (5 * MG + 5 * SL), 0, SL, SL)},
	{KM_F6       , KeyBtnView("F6"         , KM_F6       , (6 * MG + 6 * SL), 0, SL, SL)},
	{KM_F7       , KeyBtnView("F7"         , KM_F7       , (7 * MG + 7 * SL), 0, SL, SL)},
	{KM_F8       , KeyBtnView("F8"         , KM_F8       , (8 * MG + 8 * SL), 0, SL, SL)},
	{KM_F9       , KeyBtnView("F9"         , KM_F9       , (9 * MG + 9 * SL), 0, SL, SL)},
	{KM_F10      , KeyBtnView("F10"        , KM_F10      , (10 * MG + 10 * SL), 0, SL, SL)},
	{KM_F11      , KeyBtnView("F11"        , KM_F11      , (11 * MG + 11 * SL), 0, SL, SL)},
	{KM_F12      , KeyBtnView("F12"        , KM_F12      , (12 * MG + 12 * SL), 0, SL, SL)},
	{KM_INS      , KeyBtnView("INS"        , KM_INS      , (13 * MG + 13 * SL), 0, SL, SL)},
	{KM_DEL      , KeyBtnView("DEL"        , KM_DEL      , (14 * MG + 14 * SL), 0, SL, SL)},

	{KM_TILDE    , KeyBtnView("~"          , KM_TILDE    ,                     0, (FL + SL), SL, SL)},
	{KM_1        , KeyBtnView("1"          , KM_1        , (1 * MG + 1 * SL + 0), (FL + SL), SL, SL)},
	{KM_2        , KeyBtnView("2"          , KM_2        , (2 * MG + 2 * SL + 0), (FL + SL), SL, SL)},
	{KM_3        , KeyBtnView("3"          , KM_3        , (3 * MG + 3 * SL + 0), (FL + SL), SL, SL)},
	{KM_4        , KeyBtnView("4"          , KM_4        , (4 * MG + 4 * SL + 0), (FL + SL), SL, SL)},
	{KM_5        , KeyBtnView("5"          , KM_5        , (5 * MG + 5 * SL + 0), (FL + SL), SL, SL)},
	{KM_6        , KeyBtnView("6"          , KM_6        , (6 * MG + 6 * SL + 0), (FL + SL), SL, SL)},
	{KM_7        , KeyBtnView("7"          , KM_7        , (7 * MG + 7 * SL + 0), (FL + SL), SL, SL)},
	{KM_8        , KeyBtnView("8"          , KM_8        , (8 * MG + 8 * SL + 0), (FL + SL), SL, SL)},
	{KM_9        , KeyBtnView("9"          , KM_9        , (9 * MG + 9 * SL + 0), (FL + SL), SL, SL)},
	{KM_0        , KeyBtnView("0"          , KM_0        , (10 * MG + 10 * SL + 0), (FL + SL), SL, SL)},
	{KM_UDS      , KeyBtnView("-"          , KM_UDS      , (11 * MG + 11 * SL + 0), (FL + SL), SL, SL)},
	{KM_PLUS     , KeyBtnView("="          , KM_PLUS     , (12 * MG + 12 * SL + 0), (FL + SL), SL, SL)},
	{KM_BS       , KeyBtnView("BACKSPACE"  , KM_BS       , (13 * MG + 13 * SL + 0), (FL + SL), BACKSPACEW, SL)},

	{KM_TAB      , KeyBtnView("TAB"        , KM_TAB      , (0 * MG + 0 * SL), (FL + 2 * SL + MG), TABW, SL)},
	{KM_Q        , KeyBtnView("Q"          , KM_Q        , (TABW + 1 * MG + 0 * SL), (FL + 2 * SL + MG), SL, SL)},
	{KM_W        , KeyBtnView("W"          , KM_W        , (TABW + 2 * MG + 1 * SL), (FL + 2 * SL + MG), SL, SL)},
	{KM_E        , KeyBtnView("E"          , KM_E        , (TABW + 3 * MG + 2 * SL), (FL + 2 * SL + MG), SL, SL)},
	{KM_R        , KeyBtnView("R"          , KM_R        , (TABW + 4 * MG + 3 * SL), (FL + 2 * SL + MG), SL, SL)},
	{KM_T        , KeyBtnView("T"          , KM_T        , (TABW + 5 * MG + 4 * SL), (FL + 2 * SL + MG), SL, SL)},
	{KM_Y        , KeyBtnView("Y"          , KM_Y        , (TABW + 6 * MG + 5 * SL), (FL + 2 * SL + MG), SL, SL)},
	{KM_U        , KeyBtnView("U"          , KM_U        , (TABW + 7 * MG + 6 * SL), (FL + 2 * SL + MG), SL, SL)},
	{KM_I        , KeyBtnView("I"          , KM_I        , (TABW + 8 * MG + 7 * SL), (FL + 2 * SL + MG), SL, SL)},
	{KM_O        , KeyBtnView("O"          , KM_O        , (TABW + 9 * MG + 8 * SL), (FL + 2 * SL + MG), SL, SL)},
	{KM_P        , KeyBtnView("P"          , KM_P        , (TABW + 10 * MG + 9 * SL), (FL + 2 * SL + MG), SL, SL)},
	{KM_OB       , KeyBtnView("["          , KM_OB       , (TABW + 11 * MG + 10 * SL), (FL + 2 * SL + MG), SL, SL)},
	{KM_CB       , KeyBtnView("]"          , KM_CB       , (TABW + 12 * MG + 11 * SL), (FL + 2 * SL + MG), SL, SL)},
	{KM_BSL      , KeyBtnView("\\"         , KM_BSL      , (TABW + 13 * MG + 12 * SL), (FL + 2 * SL + MG), SLASHW, SL)},

	{KM_CAPS     , KeyBtnView("CAPS LOCK"  , KM_CAPS     , (0 * MG + 0 * SL), (FL + 3 * SL + 2 * MG), CAPSLOCKW, SL)},
	{KM_A        , KeyBtnView("A"          , KM_A        , (CAPSLOCKW + 1 * MG + 0 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_S        , KeyBtnView("S"          , KM_S        , (CAPSLOCKW + 2 * MG + 1 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_D        , KeyBtnView("D"          , KM_D        , (CAPSLOCKW + 3 * MG + 2 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_F        , KeyBtnView("F"          , KM_F        , (CAPSLOCKW + 4 * MG + 3 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_G        , KeyBtnView("G"          , KM_G        , (CAPSLOCKW + 5 * MG + 4 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_H        , KeyBtnView("H"          , KM_H        , (CAPSLOCKW + 6 * MG + 5 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_J        , KeyBtnView("J"          , KM_J        , (CAPSLOCKW + 7 * MG + 6 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_K        , KeyBtnView("K"          , KM_K        , (CAPSLOCKW + 8 * MG + 7 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_L        , KeyBtnView("L"          , KM_L        , (CAPSLOCKW + 9 * MG + 8 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_COLON    , KeyBtnView(":"          , KM_COLON    , (CAPSLOCKW + 10 * MG + 9 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_QUOTE    , KeyBtnView("'"          , KM_QUOTE    , (CAPSLOCKW + 11 * MG + 10 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_ENTER    , KeyBtnView("ENTER"      , KM_ENTER    , (CAPSLOCKW + 12 * MG + 11 * SL), (FL + 3 * SL + 2 * MG), ENTERW, SL)},

	{KM_SHIFT_L  , KeyBtnView("SHIFT"      , KM_SHIFT_L  , (0 * MG + 0 * SL), (FL + 4 * SL + 3 * MG), LSHIFTW, SL)},
	{KM_Z        , KeyBtnView("Z"          , KM_Z        , (LSHIFTW + 1 * MG + 0 * SL), (FL + 4 * SL + 3 * MG), SL, SL)},
	{KM_X        , KeyBtnView("X"          , KM_X        , (LSHIFTW + 2 * MG + 1 * SL), (FL + 4 * SL + 3 * MG), SL, SL)},
	{KM_C        , KeyBtnView("C"          , KM_C        , (LSHIFTW + 3 * MG + 2 * SL), (FL + 4 * SL + 3 * MG), SL, SL)},
	{KM_V        , KeyBtnView("V"          , KM_V        , (LSHIFTW + 4 * MG + 3 * SL), (FL + 4 * SL + 3 * MG), SL, SL)},
	{KM_B        , KeyBtnView("B"          , KM_B        , (LSHIFTW + 5 * MG + 4 * SL), (FL + 4 * SL + 3 * MG), SL, SL)},
	{KM_N        , KeyBtnView("N"          , KM_N        , (LSHIFTW + 6 * MG + 5 * SL), (FL + 4 * SL + 3 * MG), SL, SL)},
	{KM_M        , KeyBtnView("M"          , KM_M        , (LSHIFTW + 7 * MG + 6 * SL), (FL + 4 * SL + 3 * MG), SL, SL)},
	{KM_COMM     , KeyBtnView(","          , KM_COMM     , (LSHIFTW + 8 * MG + 7 * SL), (FL + 4 * SL + 3 * MG), SL, SL)},
	{KM_DOT      , KeyBtnView("."          , KM_DOT      , (LSHIFTW + 9 * MG + 8 * SL), (FL + 4 * SL + 3 * MG), SL, SL)},
	{KM_SLASH    , KeyBtnView("/"          , KM_SLASH    , (LSHIFTW + 10 * MG + 9 * SL), (FL + 4 * SL + 3 * MG), SL, SL)},
	{KM_SHIFT_R  , KeyBtnView("SHIFT"      , KM_SHIFT_R  , (LSHIFTW + 11 * MG + 10 * SL), (FL + 4 * SL + 3 * MG), RSHIFTW, SL)},

	{KM_CTRL_L   , KeyBtnView("CTRL"       , KM_CTRL_L   , (0)                                                , (FL + 5 * SL + 4 * MG), LCTRLW, SL)},
	{KM_WIN_L    , KeyBtnView("WIN"        , KM_WIN_L    , (LCTRLW + 1 * MG)                                  , (FL + 5 * SL + 4 * MG), LWINW , SL)},
	{KM_ALT_L    , KeyBtnView("ALT"        , KM_ALT_L    , (LCTRLW + LWINW + 2 * MG)                          , (FL + 5 * SL + 4 * MG), LALTW , SL)},
	{KM_SP       , KeyBtnView("SPACE"      , KM_SP       , (LCTRLW + LWINW + LALTW + 3 * MG)                  , (FL + 5 * SL + 4 * MG), SPACEW, SL)},
	{KM_ALT_R    , KeyBtnView("ALT"        , KM_ALT_R    , (LCTRLW + LWINW + LALTW + SPACEW + 4 * MG)         , (FL + 5 * SL + 4 * MG), SL    , SL)},
	{KM_FN       , KeyBtnView("FN"         , KM_FN       , (LCTRLW + LWINW + LALTW + SPACEW + 5 * MG + 1 * SL), (FL + 5 * SL + 4 * MG), SL    , SL)},
	{KM_CTRL_R   , KeyBtnView("CTRL"       , KM_CTRL_R   , (LCTRLW + LWINW + LALTW + SPACEW + 6 * MG + 2 * SL), (FL + 5 * SL + 4 * MG), SL    , SL)},

	{KM_RIGHT    , KeyBtnView("RIGHT"      , KM_RIGHT    , ((LCTRLW + LWINW + LALTW + SPACEW + 6 * MG + 3 * SL + OM) + (0 * SL + 0 * MG)), ((FL + 4 * SL + 2 * MG + OM) + (1 * SL + 1 * MG)), SL, SL)},
	{KM_LEFT     , KeyBtnView("LEFT"       , KM_LEFT     , ((LCTRLW + LWINW + LALTW + SPACEW + 6 * MG + 3 * SL + OM) + (2 * SL + 2 * MG)), ((FL + 4 * SL + 2 * MG + OM) + (1 * SL + 1 * MG)), SL, SL)},
	{KM_DOWN     , KeyBtnView("DOWN"       , KM_DOWN     , ((LCTRLW + LWINW + LALTW + SPACEW + 6 * MG + 3 * SL + OM) + (1 * SL + 1 * MG)), ((FL + 4 * SL + 2 * MG + OM) + (1 * SL + 1 * MG)), SL, SL)},
	{KM_UP       , KeyBtnView("UP"         , KM_UP       , ((LCTRLW + LWINW + LALTW + SPACEW + 6 * MG + 3 * SL + OM) + (1 * SL + 1 * MG)), ((FL + 4 * SL + 2 * MG + OM) + (0 * SL + 0 * MG)), SL, SL)},

	{KM_HOME     , KeyBtnView("HOME"       , KM_HOME     , (LW + KP + 0 * MG + 0 * SL), (0), SL, SL)},
	{KM_PRINT    , KeyBtnView("PRTSC"      , KM_PRINT    , (LW + KP + 1 * MG + 1 * SL), (0), SL, SL)},
	{KM_SCL      , KeyBtnView("SCRLK"      , KM_SCL      , (LW + KP + 2 * MG + 2 * SL), (0), SL, SL)},
	{KM_PAUSE    , KeyBtnView("PAUSE"      , KM_PAUSE    , (LW + KP + 3 * MG + 3 * SL), (0), SL, SL)},
	{KM_KP_NUM   , KeyBtnView("NUM"        , KM_KP_NUM   , (LW + KP + 0 * MG + 0 * SL), (FL + 1 * SL + 0 * MG), SL, SL)},
	{KM_KP_DIV   , KeyBtnView("/"          , KM_KP_DIV   , (LW + KP + 1 * MG + 1 * SL), (FL + 1 * SL + 0 * MG), SL, SL)},
	{KM_KP_MULT  , KeyBtnView("*"          , KM_KP_MULT  , (LW + KP + 2 * MG + 2 * SL), (FL + 1 * SL + 0 * MG), SL, SL)},
	{KM_KP_MINUS , KeyBtnView("-"          , KM_KP_MINUS , (LW + KP + 3 * MG + 3 * SL), (FL + 1 * SL + 0 * MG), SL, SL)},
	{KM_KP_7     , KeyBtnView("7"          , KM_KP_7     , (LW + KP + 0 * MG + 0 * SL), (FL + 2 * SL + 1 * MG), SL, SL)},
	{KM_KP_8     , KeyBtnView("8"          , KM_KP_8     , (LW + KP + 1 * MG + 1 * SL), (FL + 2 * SL + 1 * MG), SL, SL)},
	{KM_KP_9     , KeyBtnView("9"          , KM_KP_9     , (LW + KP + 2 * MG + 2 * SL), (FL + 2 * SL + 1 * MG), SL, SL)},
	{KM_KP_PLUS  , KeyBtnView("+"          , KM_KP_PLUS  , (LW + KP + 3 * MG + 3 * SL), (FL + 2 * SL + 1 * MG), SL, NUMPLUSH)},
	{KM_KP_4     , KeyBtnView("4"          , KM_KP_4     , (LW + KP + 0 * MG + 0 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_KP_5     , KeyBtnView("5"          , KM_KP_5     , (LW + KP + 1 * MG + 1 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_KP_6     , KeyBtnView("6"          , KM_KP_6     , (LW + KP + 2 * MG + 2 * SL), (FL + 3 * SL + 2 * MG), SL, SL)},
	{KM_KP_1     , KeyBtnView("1"          , KM_KP_1     , (LW + KP + 0 * MG + 0 * SL), (FL + 4 * SL + 3 * MG), SL, SL)},
	{KM_KP_2     , KeyBtnView("2"          , KM_KP_2     , (LW + KP + 1 * MG + 1 * SL), (FL + 4 * SL + 3 * MG), SL, SL)},
	{KM_KP_3     , KeyBtnView("3"          , KM_KP_3     , (LW + KP + 2 * MG + 2 * SL), (FL + 4 * SL + 3 * MG), SL, SL)},
	{KM_KP_ENTER , KeyBtnView("ENTER"      , KM_KP_ENTER , (LW + KP + 3 * MG + 3 * SL), (FL + 4 * SL + 3 * MG), SL, NUMENTERH)},
	{KM_KP_0     , KeyBtnView("0"          , KM_KP_0     , (LW + KP + 1 * MG + 1 * SL), (FL + 5 * SL + 4 * MG), SL, SL)},
	{KM_KP_DEL   , KeyBtnView("."          , KM_KP_DEL   , (LW + KP + 2 * MG + 2 * SL), (FL + 5 * SL + 4 * MG), SL, SL)},


	//KeyBtnView("#"			, KM_ASH     , 0, 0, SL, SL), 
	//
	//KeyBtnView("PAGEUP"		, KM_PAGEUP  , 0, 0, SL, SL), 
	//KeyBtnView("END"		, KM_END     , 0, 0, SL, SL), 
	//KeyBtnView("PAGEDOWN"	, KM_PAGEDOWN, 0, 0, SL, SL), 
	//
	//KeyBtnView("@"			, KM_KP_AT   , 0, 0, SL, SL),
	//KeyBtnView("WIN"		, KM_WIN_R   , 0, 0, SL, SL),
};

KeyBtnView::KeyBtnView(const char* tag, KEY_MapId_t id, int offsetX, int offsetY, int width, int height)
{
	strcpy(this->tag, tag);
	memset(this->face_vertex, 0, sizeof(this->face_vertex));
	this->id = id;
	this->x = offsetX;
	this->y = offsetY;
	this->w = width;
	this->h = height;

	this->hover = false;
	this->active = false;
}

KeyboardInstance* KeyboardGetInstance()
{
	return &kbv_draw_ctx;
}
KEY_MapId_t KeyboardGetActiveMapID()
{
	return kbv_draw_ctx.active_index;
}




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
	p[0] = pos + glm::vec3(0.0f, 0.0f, 0.0f);
	p[1] = pos + glm::vec3(shap.x, 0.0f, 0.0f);
	p[2] = pos + glm::vec3(shap.x, shap.y, 0.0f);
	p[3] = pos + glm::vec3(0.0f, shap.y, 0.0f);

	p[4] = p[0] + glm::vec3(thickness, thickness, 0.0f);
	p[5] = p[1] + glm::vec3(-thickness, thickness, 0.0f);
	p[6] = p[2] + glm::vec3(-thickness, -thickness, 0.0f);
	p[7] = p[3] + glm::vec3(thickness, -thickness, 0.0f);

	p[8] = p[0] + glm::vec3(padding, padding, shap.z);
	p[9] = p[1] + glm::vec3(-padding, padding, shap.z);
	p[10] = p[2] + glm::vec3(-padding, -padding, shap.z);
	p[11] = p[3] + glm::vec3(padding, -padding, shap.z);

	if (shap.z > 0.0f)
	{
		p[12] = p[8] + glm::vec3(thickness, thickness, -thickness);
		p[13] = p[9] + glm::vec3(-thickness, thickness, -thickness);
		p[14] = p[10] + glm::vec3(-thickness, -thickness, -thickness);
		p[15] = p[11] + glm::vec3(thickness, -thickness, -thickness);
	}
	else
	{
		p[12] = p[8] + glm::vec3(thickness, thickness, thickness);
		p[13] = p[9] + glm::vec3(-thickness, thickness, thickness);
		p[14] = p[10] + glm::vec3(-thickness, -thickness, thickness);
		p[15] = p[11] + glm::vec3(thickness, -thickness, thickness);
	}

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

void DrawLight(glm::vec3 pos, glm::vec3 shap, float lightW, float lightH, glm::vec4 rgba, std::vector<float>& output)
{
	float halfW = lightW / 2;
	float halfH = lightH / 2;
	float halfWShap = shap.x / 2;
	float halfHShap = shap.y / 2;

	glm::vec3 p[4];
	p[0] = pos + glm::vec3(halfWShap - halfW, halfHShap - halfH, 0.0f);
	p[1] = pos + glm::vec3(halfWShap + halfW, halfHShap - halfH, 0.0f);
	p[2] = pos + glm::vec3(halfWShap + halfW, halfHShap + halfH, 0.0f);
	p[3] = pos + glm::vec3(halfWShap - halfW, halfHShap + halfH, 0.0f);

	for (int i = 0; i < 4; ++i)
		p[i] *= glm::vec3(1.0f, -1.0f, 1.0f);

	DrawSurfaceRect(p[0], p[1], p[2], p[3], rgba, output);
}

void KeyCapInit(glm::vec3 pos, glm::vec3 shap, float padding, float* face_vertex)
{
	glm::vec3 p[4];
	p[0] = pos + glm::vec3(0.0f, 0.0f, 0.0f) + glm::vec3(padding, padding, shap.z);
	p[1] = pos + glm::vec3(shap.x, 0.0f, 0.0f) + glm::vec3(-padding, padding, shap.z);
	p[2] = pos + glm::vec3(shap.x, shap.y, 0.0f) + glm::vec3(-padding, -padding, shap.z);
	p[3] = pos + glm::vec3(0.0f, shap.y, 0.0f) + glm::vec3(padding, -padding, shap.z);

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

void DrawBackgroundBoard(glm::vec3 pos, glm::vec3 shap, float margin, float thickness, glm::vec4 rgba, std::vector<float>& output)
{
	glm::vec3 p = glm::vec3(pos.x - margin - thickness, pos.y - margin - thickness, pos.z);
	glm::vec3 s = glm::vec3(
		shap.x + 2 * (margin + thickness), 
		shap.y + 2 * (margin + thickness), 
		shap.z
	);
	DrawKeycap(p, s, 0.0f, thickness, rgba, output);
}

void DrawSelection(glm::vec4 rgba, std::vector<float>& output)
{
	glm::vec3 p[8] = {
		{0, 0, 0},
		{1, 0, 0},
		{1, 1, 0},
		{0, 1, 0},
		{0, 0, 1},
		{1, 0, 1},
		{1, 1, 1},
		{0, 1, 1},
	};
	for (int i = 0; i < 8; ++i)
		p[i] *= glm::vec3(1.0f, -1.0f, 1.0f);
	
	DrawSurfaceRect(p[0], p[1], p[2], p[3], rgba, output);
	DrawSurfaceRect(p[4], p[5], p[6], p[7], rgba, output);
	DrawSurfaceRect(p[0], p[1], p[5], p[4], rgba, output);
	DrawSurfaceRect(p[1], p[2], p[6], p[5], rgba, output);
	DrawSurfaceRect(p[2], p[3], p[7], p[6], rgba, output);
	DrawSurfaceRect(p[3], p[0], p[4], p[7], rgba, output);
}

void RenderModelInit()
{
	glm::vec3 global_pos(0.0f, 0.0f, 0.0f);
	glm::vec3 bbMax(0.0f, 0.0f, 0.0f);

	float board_concave = 0.2f;
	float board_thickness = 0.5f;

	bbMax.z = -(board_concave + board_thickness);

	for (auto& it : keyBtnViewList)
	{
		auto& keyBtnView = it.second;
		glm::vec3 pos1(global_pos.x + keyBtnView.x, global_pos.y + keyBtnView.y, 0.0f);
		glm::vec3 shape(keyBtnView.w, keyBtnView.h, KEY_DEPTH);
		pos1 /= SECTOR;
		shape /= SECTOR;

		keyBtnView.color = DEFAULT_COLOR;

		KeyCapInit(pos1, shape, 0.1f, keyBtnView.face_vertex);
		DrawKeycap(pos1, shape, PADDING, THINKNESS, DEFAULT_COLOR, keyBtnView.vertices);
		
		
		DrawLight(glm::vec3(pos1.x, pos1.y, pos1.z - board_concave + 0.001f), shape, shape.x / 2, shape.y / 2, DEFAULT_COLOR, keyBtnView.LightVertices);
	
		if (bbMax.x < (pos1.x + shape.x))
			bbMax.x = (pos1.x + shape.x);
		if (bbMax.y < (pos1.y + shape.y))
			bbMax.y = (pos1.y + shape.y);
	}

	DrawBackgroundBoard(global_pos, bbMax, 0.2f, board_thickness, BOARD_DEFAULT_COLOR, kbv_draw_ctx.BoardVertices);

	DrawSelection(DEFAULT_COLOR, kbv_draw_ctx.SelectionVertices);
}

bool FunctionIsModified(KEY_MapId_t kmID)
{
	auto configManager = KLFunctionConfigManager::GetInstance();

	KLFunctionID fid = FindFunctionIDByMapID(kmID);

	auto& function = FindCurrentConfigFunctionByMapID(kmID);

	if (function.id == fid)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// Detect if the mouse has hit a button
static glm::vec3 GetIntersectWithLineAndPlane(glm::vec3 point, glm::vec3 direct, glm::vec3 planeNormal, glm::vec3 planePoint)
{
	float d = glm::dot(planePoint - point, planeNormal) / glm::dot(direct, planeNormal);
	return d * direct + point;
}
bool isPointInsideTriangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P) {
	// Calculate vectors
	glm::vec3 AB = B - A;
	glm::vec3 BC = C - B;
	glm::vec3 CA = A - C;

	glm::vec3 AP = P - A;
	glm::vec3 BP = P - B;
	glm::vec3 CP = P - C;

	// Calculate cross products
	glm::vec3 cross_AB_AP = glm::cross(AB, AP);
	glm::vec3 cross_BC_BP = glm::cross(BC, BP);
	glm::vec3 cross_CA_CP = glm::cross(CA, CP);

	// Check if the vectors have the same direction
	if (glm::dot(cross_AB_AP, cross_BC_BP) >= 0 &&
		glm::dot(cross_BC_BP, cross_CA_CP) >= 0 &&
		glm::dot(cross_CA_CP, cross_AB_AP) >= 0) {
		return true;
	}
	else {
		return false;
	}
}
bool wmouse_in_quad(glm::vec3 mouse_pos, glm::vec3 camera_pos, float* quad)
{
	glm::vec3 dir = glm::normalize(mouse_pos - camera_pos);

	bool intri1 = false;
	bool intri2 = false;
	{
		glm::vec4 p1(quad[0], quad[1], quad[2], 1.0f);
		glm::vec4 p2(quad[3], quad[4], quad[5], 1.0f);
		glm::vec4 p3(quad[6], quad[7], quad[8], 1.0f);

		glm::vec3 d1 = p1 - p2;
		glm::vec3 d2 = p3 - p2;
		glm::vec3 n = glm::cross(d1, d2);
		n = glm::normalize(n);

		glm::vec3 a = GetIntersectWithLineAndPlane(camera_pos, dir, n, p1);

		intri1 = isPointInsideTriangle(p1, p2, p3, a);
	}
	{
		glm::vec4 p1(quad[6], quad[7], quad[8], 1.0f);
		glm::vec4 p2(quad[9], quad[10], quad[11], 1.0f);
		glm::vec4 p3(quad[0], quad[1], quad[2], 1.0f);

		glm::vec3 d1 = p1 - p2;
		glm::vec3 d2 = p3 - p2;
		glm::vec3 n = glm::cross(d1, d2);
		n = glm::normalize(n);

		glm::vec3 a = GetIntersectWithLineAndPlane(camera_pos, dir, n, p1);

		intri2 = isPointInsideTriangle(p1, p2, p3, a);
	}

	return intri1 || intri2;
}
glm::vec3 GetMousePos3DC()
{
	/* Convert mouse screen coordinates to world coordinates */
	glm::vec4 viewport(0.0f, 0.0f, kbv_draw_ctx.w, kbv_draw_ctx.h);
	glm::mat4 modelViewMatrix = kbv_draw_ctx.view_matrix * kbv_draw_ctx.model_matrix;
	glm::vec3 mousePos_NDC(kbv_draw_ctx.mouse_pos.x, kbv_draw_ctx.h - kbv_draw_ctx.mouse_pos.y, 0.1f);
	glm::vec3 mousePos_3DC = glm::unProject(mousePos_NDC, modelViewMatrix, kbv_draw_ctx.projection_matrix, viewport);
	return mousePos_3DC;
}
glm::vec3 CalcInterSection()
{
	glm::vec3 mousePos_3DC = GetMousePos3DC();

	glm::vec3 dir = glm::normalize(mousePos_3DC - kbv_draw_ctx.camera.Position);
	glm::vec3 planeN = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 planeP = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 intersection = GetIntersectWithLineAndPlane(kbv_draw_ctx.camera.Position, dir, planeN, planeP);

	return intersection;
}
bool quad_in_selection(float* quad)
{
	auto& ctx = kbv_draw_ctx;

	float xmin = ctx.selection_w > 0 ? ctx.selection_x : ctx.selection_x + ctx.selection_w;
	float xmax = ctx.selection_w < 0 ? ctx.selection_x : ctx.selection_x + ctx.selection_w;
	float ymin = ctx.selection_h > 0 ? ctx.selection_y : ctx.selection_y + ctx.selection_h;
	float ymax = ctx.selection_h < 0 ? ctx.selection_y : ctx.selection_y + ctx.selection_h;

	float x1 = quad[0], y1 = quad[1];
	float x2 = quad[3], y2 = quad[4];
	float x3 = quad[6], y3 = quad[7];
	float x4 = quad[9], y4 = quad[10];

	float x5 = (x1 + x2) / 2, y5 = (y1 + y2) / 2;
	float x6 = (x2 + x3) / 2, y6 = (y2 + y3) / 2;
	float x7 = (x3 + x4) / 2, y7 = (y3 + y4) / 2;
	float x8 = (x4 + x1) / 2, y8 = (y4 + y1) / 2;
	float x9 = (x5 + x7) / 2, y9 = (y5 + y7) / 2;

	if ((xmin <= x1 && x1 <= xmax) && (ymin <= y1 && y1 <= ymax) ||
		(xmin <= x2 && x2 <= xmax) && (ymin <= y2 && y2 <= ymax) ||
		(xmin <= x3 && x3 <= xmax) && (ymin <= y3 && y3 <= ymax) ||
		(xmin <= x4 && x4 <= xmax) && (ymin <= y4 && y4 <= ymax) ||
		(xmin <= x5 && x5 <= xmax) && (ymin <= y5 && y5 <= ymax) ||
		(xmin <= x6 && x6 <= xmax) && (ymin <= y6 && y6 <= ymax) ||
		(xmin <= x7 && x7 <= xmax) && (ymin <= y7 && y7 <= ymax) ||
		(xmin <= x8 && x8 <= xmax) && (ymin <= y8 && y8 <= ymax) ||
		(xmin <= x9 && x9 <= xmax) && (ymin <= y9 && y9 <= ymax))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void assignment_layout_kbv_hover_cb(KeyBtnView& view)
{
	ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	ImGui::OpenPopup("SHOW_KBV_BIND_FUNCTION");
	if (ImGui::BeginPopup("SHOW_KBV_BIND_FUNCTION"))
	{
		KLFunctionID fid = FindFunctionIDByMapID(view.id);
		KLFunction defaultFunction = FindFunctionByFunctionID(fid);
		KLFunction currentFunction = FindCurrentConfigFunctionByMapID(view.id);
	
		char temp_buf[64];
		sprintf(temp_buf, " [%s:%s|%s:%s] ", 
			KLLABLEA(KLL_KEY_DEFAULT), defaultFunction.name, 
			KLLABLEA(KLL_KEY_CURRENT), currentFunction.name);
		ImGui::Text(temp_buf);
		ImGui::EndPopup();
	}
}

static uint8_t light[KEY_ROW_NUM][KB_COL_NUM] = {
	0
};

void LightUpdate()
{
	static int frame_cnt = 0;
	static int anim_cnt = 0;
	if (frame_cnt++ > 5)
	{
		anim_cnt++;

		int b = 5;
		int a = 0;
		for (uint32_t i = 0; i < KEY_ROW_NUM; ++i)
		{
			for (uint32_t j = 0; j < KB_COL_NUM; ++j)
			{
				light[i][j] = 4 - (j + anim_cnt) % 5;
			}
		}
		frame_cnt = 0;
	}
}

void RenderModelUpdateLight()
{
	LightUpdate();

	for (uint32_t i = 0; i < KEY_ROW_NUM; ++i)
	{
		for (uint32_t j = 0; j < KB_COL_NUM; ++j)
		{
			KEY_MapId_t kmId = (KEY_MapId_t)usr_key_map[i][j];

			auto it = keyBtnViewList.find(kmId);
			if (it != keyBtnViewList.end())
			{
				auto& keyBtnView = it->second;

				keyBtnView.brightness = light[i][j];
			}
		}
	}

	for (auto& it : keyBtnViewList)
	{
		auto& keyBtnView = it.second;

		float a = 0.7f;
		float c = ((float)keyBtnView.brightness / 4) * 0.6f + 0.1f;

		glm::vec4 color(a, a, a, c);

		keyBtnView.color = glm::vec4((a, a, a, c));
	}
}


extern std::unordered_map<KEY_MapId_t, bool> customize_table;
void RenderModelUpdateLightCustomize()
{
	auto win = ImGui::GetCurrentWindow();
	auto& DC = win->DC;
	auto& io = ImGui::GetIO();

	/* Update the position of texture view */
	kbv_draw_ctx.mouse_pos.x = io.MousePos.x - DC.CursorPos.x + DPI(KL_KB_VIEW_TEX_X);
	kbv_draw_ctx.mouse_pos.y = io.MousePos.y - DC.CursorPos.y + DPI(KL_KB_VIEW_TEX_Y);
	kbv_draw_ctx.mouse_clicked = io.MouseClicked[0];

	static bool drag = false;

	ImGuiContext* g = ImGui::GetCurrentContext();
	{
		if (!drag && io.MouseClicked[0]) {
			drag = true;
			glm::vec3 intersection = CalcInterSection();
			kbv_draw_ctx.selection_x = intersection.x;
			kbv_draw_ctx.selection_y = intersection.y;
			kbv_draw_ctx.selection_w = 0.0f;
			kbv_draw_ctx.selection_h = 0.0f;
		}
		if (drag && io.MouseDown[0]) {
			glm::vec3 intersection = CalcInterSection();
			kbv_draw_ctx.selection_w = intersection.x - kbv_draw_ctx.selection_x;
			kbv_draw_ctx.selection_h = intersection.y - kbv_draw_ctx.selection_y;
		}
		if (drag && io.MouseReleased[0]) {
			drag = false;

			if (kbv_draw_ctx.selection_w == 0.0f && kbv_draw_ctx.selection_h == 0.0f)
			{
				glm::vec3 mousePos_3DC = GetMousePos3DC();

				/* click */
				for (auto& kbv : keyBtnViewList) {
					auto& keyBtnView = kbv.second;
					if (wmouse_in_quad(mousePos_3DC, kbv_draw_ctx.camera.Position, keyBtnView.face_vertex)) {
						auto it = customize_table.find(keyBtnView.id);
						if (it == customize_table.end())
							customize_table[keyBtnView.id] = true;
						else
							it->second = !it->second;
					}
				}
			}
			else
			{
				/* selection */
				bool enable = false;
				std::vector<KeyBtnView*> selected;

				for (auto& kbv : keyBtnViewList) {
					auto& keyBtnView = kbv.second;
					if (quad_in_selection(keyBtnView.face_vertex))
						selected.push_back(&keyBtnView);
				}
				
				for (auto& kbv : selected) {
					auto it = customize_table.find(kbv->id);
					if (it == customize_table.end() || !it->second) {
						enable = true;
						break;
					}
				}

				for (auto& kbv : selected) {
					customize_table[kbv->id] = enable;
				}
			}

			kbv_draw_ctx.selection_x = 0.0f;
			kbv_draw_ctx.selection_y = 0.0f;
			kbv_draw_ctx.selection_w = 0.0f;
			kbv_draw_ctx.selection_h = 0.0f;
		}
	}

	for (auto& kbv : keyBtnViewList)
	{
		auto& keyBtnView = kbv.second;

		auto it = customize_table.find(keyBtnView.id);
		bool active = it != customize_table.end() && it->second;
		bool hover = quad_in_selection(keyBtnView.face_vertex);

		if (active && hover)
		{
			keyBtnView.color = MODIFIED_ACTIVE_COLOR;
		}
		else if (active)
		{
			keyBtnView.color = ACTIVE_COLOR;
		}
		else if (hover)
		{
			keyBtnView.color = HOVER_COLOR;
		}
		else
		{
			keyBtnView.color = DEFAULT_COLOR;
		}
	}


}

void RenderModelUpdateAssignment()
{
	auto win = ImGui::GetCurrentWindow();
	auto& DC = win->DC;
	auto& io = ImGui::GetIO();

	/* Update the position of texture view */
	kbv_draw_ctx.mouse_pos.x = io.MousePos.x - DC.CursorPos.x + DPI(KL_KB_VIEW_TEX_X);
	kbv_draw_ctx.mouse_pos.y = io.MousePos.y - DC.CursorPos.y + DPI(KL_KB_VIEW_TEX_Y);
	kbv_draw_ctx.mouse_clicked = io.MouseClicked[0];

	/* Convert mouse screen coordinates to world coordinates */
	glm::vec4 viewport(0.0f, 0.0f, kbv_draw_ctx.w, kbv_draw_ctx.h);
	glm::mat4 modelViewMatrix = kbv_draw_ctx.view_matrix * kbv_draw_ctx.model_matrix;
	glm::vec3 mousePos_NDC(kbv_draw_ctx.mouse_pos.x, kbv_draw_ctx.h - kbv_draw_ctx.mouse_pos.y, 0.1f);
	glm::vec3 mousePos_3DC = glm::unProject(mousePos_NDC, modelViewMatrix, kbv_draw_ctx.projection_matrix, viewport);

	ImGuiContext* g = ImGui::GetCurrentContext();
	if (g->HoveredWindow == g->CurrentWindow && g->ActiveIdWindow == NULL)
	{
		ImGui::IsItemHovered();
		for (auto& it : keyBtnViewList)
		{
			auto& keyBtnView = it.second;
			if (wmouse_in_quad(mousePos_3DC, kbv_draw_ctx.camera.Position, keyBtnView.face_vertex))
			{
				keyBtnView.hover = true;

				if (kbv_draw_ctx.onhover)
					assignment_layout_kbv_hover_cb(keyBtnView);

				if (kbv_draw_ctx.mouse_clicked)
				{
					kbv_draw_ctx.active_index = keyBtnView.id;

					auto& function = FindCurrentConfigFunctionByMapID(kbv_draw_ctx.active_index);

					if (function.type == KL_FUNC_TYPE_MACRO)
					{
						ResetCurrentMacroFunction(function);
					}
				}
			}
			else
			{
				keyBtnView.hover = false;
			}
		}
	}

	glm::vec3 global_pos(0.0f, 0.0f, 0.0f);

	for (auto& it : keyBtnViewList)
	{
		auto& keyBtnView = it.second;

		bool modified = FunctionIsModified(keyBtnView.id);
		if (keyBtnView.id == kbv_draw_ctx.active_index && modified)
		{
			keyBtnView.color = MODIFIED_ACTIVE_COLOR;
		}
		else if (keyBtnView.id == kbv_draw_ctx.active_index)
		{
			keyBtnView.color = ACTIVE_COLOR;
		}
		else if (keyBtnView.hover)
		{
			keyBtnView.color = HOVER_COLOR;
		}
		else if (modified)
		{
			keyBtnView.color = MODIFIED_COLOR;
		}
		else
		{
			keyBtnView.color = DEFAULT_COLOR;
		}
	}
}

void KeyboardGLReCreate(int width, int height)
{
	KeyboardGLDestroy();
	KeyboardGLInit(width, height);
}

void KeyboardGLInit(int width, int height)
{
	kbv_draw_ctx.w = width;
	kbv_draw_ctx.h = height;

	auto& style = ImGui::GetStyle();
	ImVec4 bgcolor = style.Colors[ImGuiCol_WindowBg];


	for (uint32_t i = 0; i < KEY_ROW_NUM; ++i)
	{
		for (uint32_t j = 0; j < KB_COL_NUM; ++j)
		{
			light[i][j] = 4;
		}
	}

	float x = 11.050007f;
	float y = -3.875001f;
	float z = 28.924711f;

	kbv_draw_ctx.onhover = assignment_layout_kbv_hover_cb;
	kbv_draw_ctx.camera.Position = glm::vec3(x, y, z);
	kbv_draw_ctx.projection_matrix = glm::perspective(glm::radians(kbv_draw_ctx.camera.Zoom), (float)kbv_draw_ctx.w / (float)kbv_draw_ctx.h, 0.1f, 100.0f);
	kbv_draw_ctx.view_matrix = kbv_draw_ctx.camera.GetViewMatrix();
	kbv_draw_ctx.model_matrix = glm::mat4(1.0f);
	kbv_draw_ctx.clear_color = glm::vec4(bgcolor.x, bgcolor.y, bgcolor.z, bgcolor.w);

	if (kbv_draw_ctx.shader == nullptr) 
		kbv_draw_ctx.shader = std::make_unique<Shader>(
			utils::getFileAbsolutePath("shader/3.2.blending.vs").c_str(), 
			utils::getFileAbsolutePath("shader/3.2.blending.fs").c_str());
	if (kbv_draw_ctx.shader_board == nullptr)
		kbv_draw_ctx.shader = std::make_unique<Shader>(
			utils::getFileAbsolutePath("shader/board.vs").c_str(),
			utils::getFileAbsolutePath("shader/board.fs").c_str());
	if (kbv_draw_ctx.shader_selection == nullptr)
		kbv_draw_ctx.shader = std::make_unique<Shader>(
			utils::getFileAbsolutePath("shader/selection.vs").c_str(),
			utils::getFileAbsolutePath("shader/selection.fs").c_str());

	RenderModelInit();
	{
		glGenVertexArrays(1, &kbv_draw_ctx.SelectionVAO);
		glGenBuffers(1, &kbv_draw_ctx.SelectionVBO);

		glBindBuffer(GL_ARRAY_BUFFER, kbv_draw_ctx.SelectionVBO);
		glBufferData(GL_ARRAY_BUFFER, kbv_draw_ctx.SelectionVertices.size() * sizeof(float), kbv_draw_ctx.SelectionVertices.data(), GL_STREAM_DRAW);

		glBindVertexArray(kbv_draw_ctx.SelectionVAO);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// color attribute
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	{
		glGenVertexArrays(1, &kbv_draw_ctx.BoardVAO);
		glGenBuffers(1, &kbv_draw_ctx.BoardVBO);

		glBindBuffer(GL_ARRAY_BUFFER, kbv_draw_ctx.BoardVBO);
		glBufferData(GL_ARRAY_BUFFER, kbv_draw_ctx.BoardVertices.size() * sizeof(float), kbv_draw_ctx.BoardVertices.data(), GL_STREAM_DRAW);

		glBindVertexArray(kbv_draw_ctx.BoardVAO);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// color attribute
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}


	for (auto& it : keyBtnViewList)
	{
		KeyBtnView& btn = it.second;
		// first, configure the cube's VAO (and VBO)
		{
			glGenVertexArrays(1, &btn.VAO);
			glGenBuffers(1, &btn.VBO);

			glBindBuffer(GL_ARRAY_BUFFER, btn.VBO);
			glBufferData(GL_ARRAY_BUFFER, btn.vertices.size() * sizeof(float), btn.vertices.data(), GL_STREAM_DRAW);

			glBindVertexArray(btn.VAO);

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// normal attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			// color attribute
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
		}
		{
			glGenVertexArrays(1, &btn.LightVAO);
			glGenBuffers(1, &btn.LightVBO);

			glBindBuffer(GL_ARRAY_BUFFER, btn.LightVBO);
			glBufferData(GL_ARRAY_BUFFER, btn.LightVertices.size() * sizeof(float), btn.LightVertices.data(), GL_STREAM_DRAW);

			glBindVertexArray(btn.LightVAO);

			// position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			// normal attribute
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			// color attribute
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
			glEnableVertexAttribArray(2);
		}
	}
	
	glGenBuffers(1, &kbv_draw_ctx.UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, kbv_draw_ctx.UBO);
	glBufferData(GL_UNIFORM_BUFFER, 4, NULL, GL_STATIC_DRAW); // ����4�ֽڵ��ڴ�
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// create framebuffer
	glGenFramebuffers(1, &kbv_draw_ctx.FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, kbv_draw_ctx.FBO);

	// Texture attachment
	glGenTextures(1, &kbv_draw_ctx.texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, kbv_draw_ctx.texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, kbv_draw_ctx.w, kbv_draw_ctx.h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Attach it to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, kbv_draw_ctx.texColorBuffer, 0);

	// Depth stencil attachment
	glGenRenderbuffers(1, &kbv_draw_ctx.rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, kbv_draw_ctx.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, kbv_draw_ctx.w, kbv_draw_ctx.w);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, kbv_draw_ctx.rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void KeyboardGLDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, kbv_draw_ctx.FBO);

	glViewport(0, 0, kbv_draw_ctx.w, kbv_draw_ctx.h);

	glClearColor(
		kbv_draw_ctx.clear_color.r, kbv_draw_ctx.clear_color.g, 
		kbv_draw_ctx.clear_color.b, kbv_draw_ctx.clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	kbv_draw_ctx.view_matrix = kbv_draw_ctx.camera.GetViewMatrix();
	// draw objects
	kbv_draw_ctx.shader->use();
	kbv_draw_ctx.shader->setMat4("model", kbv_draw_ctx.model_matrix);
	kbv_draw_ctx.shader->setMat4("projection", kbv_draw_ctx.projection_matrix);
	kbv_draw_ctx.shader->setMat4("view", kbv_draw_ctx.view_matrix);


	/* Draw Board */
	kbv_draw_ctx.shader->setVec4("color", BOARD_DEFAULT_COLOR);

	glBindVertexArray(kbv_draw_ctx.BoardVAO);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(kbv_draw_ctx.BoardVertices.size() / 10));

	/* Draw Key */
	for (auto& it : keyBtnViewList)
	{
		KeyBtnView& btn = it.second;	// render the cube

		glm::vec4 lightColor(btn.color.r, btn.color.g, btn.color.b, 1.0f);
		kbv_draw_ctx.shader->setVec4("color", lightColor);

		/* Draw Light */
		glBindVertexArray(btn.LightVAO);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(btn.LightVertices.size() / 10));

		kbv_draw_ctx.shader->setVec4("color", btn.color);

		/* Draw KeyCap */
		glBindVertexArray(btn.VAO);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(btn.vertices.size() / 10));
	}

	/* Draw Selection */
	kbv_draw_ctx.shader->setVec4("color", DEFAULT_COLOR);
	if (kbv_draw_ctx.selection_w != 0 && kbv_draw_ctx.selection_h != 0)
	{
		glm::mat4 m(1.0f);
		m = glm::translate(m, glm::vec3(kbv_draw_ctx.selection_x, kbv_draw_ctx.selection_y, 0.0f));
		m = glm::scale(m, glm::vec3(kbv_draw_ctx.selection_w, -kbv_draw_ctx.selection_h, 1.0f));
		kbv_draw_ctx.shader->setMat4("model", m);

		glBindVertexArray(kbv_draw_ctx.SelectionVAO);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(kbv_draw_ctx.SelectionVertices.size() / 10));
	}
}
void KeyboardGLDestroy()
{
	for (auto& it : keyBtnViewList)
	{
		KeyBtnView& btn = it.second;
		glDeleteVertexArrays(1, &btn.VAO);
		glDeleteBuffers(1, &btn.VBO);
		glDeleteVertexArrays(1, &btn.LightVAO);
		glDeleteBuffers(1, &btn.LightVBO);
	}
	glDeleteVertexArrays(1, &kbv_draw_ctx.BoardVAO);
	glDeleteBuffers(1, &kbv_draw_ctx.BoardVBO);
	glDeleteVertexArrays(1, &kbv_draw_ctx.SelectionVAO);
	glDeleteBuffers(1, &kbv_draw_ctx.SelectionVBO);

	glDeleteBuffers(1, &kbv_draw_ctx.UBO);

	glDeleteFramebuffers(1, &kbv_draw_ctx.FBO);
	glDeleteBuffers(1, &kbv_draw_ctx.texColorBuffer);
	glDeleteRenderbuffers(1, &kbv_draw_ctx.rbo);
}

static bool MyButton(const char* vgname, ImVec2 size)
{
	auto imageManager = KLImageManager::GetInstance();
	auto& io = ImGui::GetIO();
	auto drawList = ImGui::GetWindowDrawList();
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImRect rect(pos, ImVec2(pos.x + size.x, pos.y + size.y));

	bool clicked = false;

	ImGui::ItemSize(size, 0.0f);
	if (rect.Contains(io.MousePos))
	{
		if (io.MouseClicked[0])
			clicked = true;

		ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
		if (io.MouseDown[0])
		{
			imageManager->DrawGraph(drawList, vgname, pos, size, 0xFF00007F);
		}
		else
		{
			imageManager->DrawGraph(drawList, vgname, pos, size, 0xFF0000FF);
		}
	}
	else
	{
		imageManager->DrawGraph(drawList, vgname, pos, size, 0xFFdbdbdb);
	}
	return clicked;
}

void ShowKeyboardWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
	if (!ImGui::Begin(WINNAME_KEYBOARD, p_open, window_flags)) {
		ImGui::End();
		return;
	}
	auto dockNode = ImGui::GetCurrentWindow()->DockNode;
	if (dockNode) dockNode->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;

	auto win = ImGui::GetCurrentWindow();
	auto& DC = win->DC;
	auto& io = ImGui::GetIO();

	auto layoutManager = KLWindowLayoutManager::GetInstance();
	auto imageManager = KLImageManager::GetInstance();
	auto configManager = KLFunctionConfigManager::GetInstance();


	if (KL_LAYOUT_ASSIGNMENT == layoutManager->GetLayoutType())
	{
		static int e;
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("FUNCTION_LAYER_TAB", tab_bar_flags))
		{
			if (ImGui::BeginTabItem(KLLABLEB(KLL_KEY_LAYER_DEFAULT, "LAYER_DEFAULT")))
			{
				e = KL_LAYER_TYPE_DEFAULT;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem(KLLABLEB(KLL_KEY_LAYER_FN1, "LAYER_FN1")))
			{
				e = KL_LAYER_TYPE_FN1;
				ImGui::EndTabItem();
			}
			if (ImGui::BeginTabItem(KLLABLEB(KLL_KEY_LAYER_FN2, "LAYER_FN2")))
			{
				e = KL_LAYER_TYPE_FN2;
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		KLFunctionConfigManager::GetInstance()->m_CurrentLayerType = (KLFunctionLayerType)e;
	}
	else if (KL_LAYOUT_LIGHT == layoutManager->GetLayoutType())
	{
		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("PLACEHOLDER_TAB", tab_bar_flags))
		{
			if (ImGui::BeginTabItem(KLLABLEB(KLL_KEY_PREVIEW, "PLACEHOLDER_TAB_ITEM")))
			{
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
	}
	else
	{
	}

	if (KL_LAYOUT_ASSIGNMENT == layoutManager->GetLayoutType())
	{
		RenderModelUpdateAssignment();
	}
	else if (KL_LAYOUT_LIGHT == layoutManager->GetLayoutType() && IsSelectCustomize())
	{
		RenderModelUpdateLightCustomize();
	}
	else if (KL_LAYOUT_LIGHT == layoutManager->GetLayoutType())
	{
		RenderModelUpdateLight();
	}
	else
	{
	}


	/* Draw texture */
	float texMapV_Min = KL_KB_VIEW_TEX_Y / KL_KB_VIEW_HEIGHT;
	float texMapV_Max = (KL_KB_VIEW_TEX_Y + KL_KB_VIEW_TEX_H) / KL_KB_VIEW_HEIGHT;
	float texMapU_Min = KL_KB_VIEW_TEX_X / KL_KB_VIEW_WIDTH;
	float texMapU_Max = (KL_KB_VIEW_TEX_X + KL_KB_VIEW_TEX_W) / KL_KB_VIEW_WIDTH;
	ImGui::Image(
		VOID_PTR_CAST(kbv_draw_ctx.texColorBuffer),				// TextureID
		ImVec2(DPI(KL_KB_VIEW_TEX_W), DPI(KL_KB_VIEW_TEX_H)),     // Width and Height
		ImVec2(texMapU_Min, (1.0f - texMapV_Min)), ImVec2(texMapU_Max, (1.0f - texMapV_Max))// UV mapping
	);

	//ImGui::Text("%f %f %f", kbv_draw_ctx.camera.Position.x, kbv_draw_ctx.camera.Position.y, kbv_draw_ctx.camera.Position.z);

	if (KL_LAYOUT_ASSIGNMENT == layoutManager->GetLayoutType())
	{
		ImGui::SameLine();

		ImGui::BeginChild("##ASSIGNED_ACTIONS");

		ImVec2 size(DPI(32.0f), DPI(32.0f));
		if (MyButton("btn_save", size)) {
			configManager->SaveConfig();

			auto deviceManager = KLDeviceManager::GetInstance();

			if (deviceManager->HasDevice())
			{
				/* send cmd */
				bool r = drv_set_func_map(deviceManager->GetCurrentDevice());
				
				if (r)
				{
					showAlert_(KLLABLEA(KLL_KEY_SUCCESSFUL));
				}
				else
				{
					showAlert_(KLLABLEA(KLL_KEY_FAILED));
				}
			}
			else
			{
				showAlert_(KLLABLEA(KLL_KEY_DEVICE_NOT_CONNECTED));
			}
		}
		ImGui::SameLine();
		if (MyButton("btn_erase", size)) {
			auto& functions = configManager->GetCurrentConfig().layers[configManager->m_CurrentLayerType];
			KEY_MapId_t mid = KeyboardGetActiveMapID();
			functions[mid] = FindDefaultFunctionByMapID(mid);
		}

		ImGui::EndChild();
	}

	ImGui::End();
}