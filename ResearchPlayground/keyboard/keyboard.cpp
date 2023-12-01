#include "keyboard.h"

#include <iostream>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"

#include "util/utils.h"

#include "function/function.h"

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

KeyBtnView keyBtnViewList[] = {
	KeyBtnView("ESC"		, KM_ESC, 0, 0, SL, SL),
	KeyBtnView("F1"			, KM_F1,  ( 1 * MG +  1 * SL), 0, SL, SL),
	KeyBtnView("F2"			, KM_F2,  ( 2 * MG +  2 * SL), 0, SL, SL),
	KeyBtnView("F3"			, KM_F3,  ( 3 * MG +  3 * SL), 0, SL, SL),
	KeyBtnView("F4"			, KM_F4,  ( 4 * MG +  4 * SL), 0, SL, SL),
	KeyBtnView("F5"			, KM_F5,  ( 5 * MG +  5 * SL), 0, SL, SL),
	KeyBtnView("F6"			, KM_F6,  ( 6 * MG +  6 * SL), 0, SL, SL),
	KeyBtnView("F7"			, KM_F7,  ( 7 * MG +  7 * SL), 0, SL, SL),
	KeyBtnView("F8"			, KM_F8,  ( 8 * MG +  8 * SL), 0, SL, SL),
	KeyBtnView("F9"			, KM_F9,  ( 9 * MG +  9 * SL), 0, SL, SL),
	KeyBtnView("F10"		, KM_F10, (10 * MG + 10 * SL), 0, SL, SL),
	KeyBtnView("F11"		, KM_F11, (11 * MG + 11 * SL), 0, SL, SL),
	KeyBtnView("F12"		, KM_F12, (12 * MG + 12 * SL), 0, SL, SL),
	KeyBtnView("INS"		, KM_INS, (13 * MG + 13 * SL), 0, SL, SL),
	KeyBtnView("DEL"		, KM_DEL, (14 * MG + 14 * SL), 0, SL, SL),

	KeyBtnView("~"			, KM_TILDE,                       0, (FL + SL), SL, SL),
	KeyBtnView("1"			, KM_1    , ( 1 * MG +  1 * SL + 0), (FL + SL), SL, SL),
	KeyBtnView("2"			, KM_2    , ( 2 * MG +  2 * SL + 0), (FL + SL), SL, SL),
	KeyBtnView("3"			, KM_3    , ( 3 * MG +  3 * SL + 0), (FL + SL), SL, SL),
	KeyBtnView("4"			, KM_4    , ( 4 * MG +  4 * SL + 0), (FL + SL), SL, SL),
	KeyBtnView("5"			, KM_5    , ( 5 * MG +  5 * SL + 0), (FL + SL), SL, SL),
	KeyBtnView("6"			, KM_6    , ( 6 * MG +  6 * SL + 0), (FL + SL), SL, SL),
	KeyBtnView("7"			, KM_7    , ( 7 * MG +  7 * SL + 0), (FL + SL), SL, SL),
	KeyBtnView("8"			, KM_8    , ( 8 * MG +  8 * SL + 0), (FL + SL), SL, SL),
	KeyBtnView("9"			, KM_9    , ( 9 * MG +  9 * SL + 0), (FL + SL), SL, SL),
	KeyBtnView("0"			, KM_0    , (10 * MG + 10 * SL + 0), (FL + SL), SL, SL),
	KeyBtnView("-"			, KM_UDS  , (11 * MG + 11 * SL + 0), (FL + SL), SL, SL),
	KeyBtnView("="			, KM_PLUS , (12 * MG + 12 * SL + 0), (FL + SL), SL, SL),
	KeyBtnView("BACKSPACE"	, KM_BS   , (13 * MG + 13 * SL + 0), (FL + SL), BACKSPACEW, SL),

	KeyBtnView("TAB"		, KM_TAB, (        0 * MG +  0 * SL), (FL + 2 * SL + MG), TABW, SL),
	KeyBtnView("Q"			, KM_Q  , (TABW +  1 * MG +  0 * SL), (FL + 2 * SL + MG), SL, SL),
	KeyBtnView("W"			, KM_W  , (TABW +  2 * MG +  1 * SL), (FL + 2 * SL + MG), SL, SL),
	KeyBtnView("E"			, KM_E  , (TABW +  3 * MG +  2 * SL), (FL + 2 * SL + MG), SL, SL),
	KeyBtnView("R"			, KM_R  , (TABW +  4 * MG +  3 * SL), (FL + 2 * SL + MG), SL, SL),
	KeyBtnView("T"			, KM_T  , (TABW +  5 * MG +  4 * SL), (FL + 2 * SL + MG), SL, SL),
	KeyBtnView("Y"			, KM_Y  , (TABW +  6 * MG +  5 * SL), (FL + 2 * SL + MG), SL, SL),
	KeyBtnView("U"			, KM_U  , (TABW +  7 * MG +  6 * SL), (FL + 2 * SL + MG), SL, SL),
	KeyBtnView("I"			, KM_I  , (TABW +  8 * MG +  7 * SL), (FL + 2 * SL + MG), SL, SL),
	KeyBtnView("O"			, KM_O  , (TABW +  9 * MG +  8 * SL), (FL + 2 * SL + MG), SL, SL),
	KeyBtnView("P"			, KM_P  , (TABW + 10 * MG +  9 * SL), (FL + 2 * SL + MG), SL, SL),
	KeyBtnView("["			, KM_OB , (TABW + 11 * MG + 10 * SL), (FL + 2 * SL + MG), SL, SL),
	KeyBtnView("]"			, KM_CB , (TABW + 12 * MG + 11 * SL), (FL + 2 * SL + MG), SL, SL),
	KeyBtnView("\\"			, KM_BSL, (TABW + 13 * MG + 12 * SL), (FL + 2 * SL + MG), SLASHW, SL),

	KeyBtnView("CAPS LOCK"	, KM_CAPS , (             0 * MG +  0 * SL), (FL + 3 * SL + 2 * MG), CAPSLOCKW, SL),
	KeyBtnView("A"			, KM_A    , (CAPSLOCKW +  1 * MG +  0 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
	KeyBtnView("S"			, KM_S    , (CAPSLOCKW +  2 * MG +  1 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
	KeyBtnView("D"			, KM_D    , (CAPSLOCKW +  3 * MG +  2 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
	KeyBtnView("F"			, KM_F    , (CAPSLOCKW +  4 * MG +  3 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
	KeyBtnView("G"			, KM_G    , (CAPSLOCKW +  5 * MG +  4 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
	KeyBtnView("H"			, KM_H    , (CAPSLOCKW +  6 * MG +  5 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
	KeyBtnView("J"			, KM_J    , (CAPSLOCKW +  7 * MG +  6 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
	KeyBtnView("K"			, KM_K    , (CAPSLOCKW +  8 * MG +  7 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
	KeyBtnView("L"			, KM_L    , (CAPSLOCKW +  9 * MG +  8 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
	KeyBtnView(":"			, KM_COLON, (CAPSLOCKW + 10 * MG +  9 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
	KeyBtnView("'"			, KM_QUOTE, (CAPSLOCKW + 11 * MG + 10 * SL), (FL + 3 * SL + 2 * MG), SL, SL),
	KeyBtnView("ENTER"		, KM_ENTER, (CAPSLOCKW + 12 * MG + 11 * SL), (FL + 3 * SL + 2 * MG), ENTERW, SL),

	KeyBtnView("SHIFT"      , KM_SHIFT_L, (           0 * MG +  0 * SL), (FL + 4 * SL + 3 * MG), LSHIFTW, SL),
	KeyBtnView("Z"			, KM_Z      , (LSHIFTW +  1 * MG +  0 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
	KeyBtnView("X"			, KM_X      , (LSHIFTW +  2 * MG +  1 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
	KeyBtnView("C"			, KM_C      , (LSHIFTW +  3 * MG +  2 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
	KeyBtnView("V"			, KM_V      , (LSHIFTW +  4 * MG +  3 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
	KeyBtnView("B"			, KM_B      , (LSHIFTW +  5 * MG +  4 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
	KeyBtnView("N"			, KM_N      , (LSHIFTW +  6 * MG +  5 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
	KeyBtnView("M"			, KM_M      , (LSHIFTW +  7 * MG +  6 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
	KeyBtnView(","			, KM_COMM   , (LSHIFTW +  8 * MG +  7 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
	KeyBtnView("."			, KM_DOT    , (LSHIFTW +  9 * MG +  8 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
	KeyBtnView("/"			, KM_SLASH  , (LSHIFTW + 10 * MG +  9 * SL), (FL + 4 * SL + 3 * MG), SL, SL),
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
	KeyBtnView("NUM"		, KM_KP_NUM   , (LW + KP + 0 * MG + 0 * SL), (FL + 1 * SL + 0 * MG), SL, SL),
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
	return keyBtnViewList[kbv_draw_ctx.active_index].id;
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

	p[12] = p[8] + glm::vec3(thickness, thickness, -thickness);
	p[13] = p[9] + glm::vec3(-thickness, thickness, -thickness);
	p[14] = p[10] + glm::vec3(-thickness, -thickness, -thickness);
	p[15] = p[11] + glm::vec3(thickness, -thickness, -thickness);

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

void RenderModelInit()
{
	glm::vec3 global_pos(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < keyBtnViewSize; ++i)
	{
		glm::vec3 pos1(global_pos.x + keyBtnViewList[i].x, global_pos.y + keyBtnViewList[i].y, 0.0f);
		glm::vec3 shape(keyBtnViewList[i].w, keyBtnViewList[i].h, KEY_DEPTH);
		pos1 /= SECTOR;
		shape /= SECTOR;

		KeyCapInit(pos1, shape, 0.1f, keyBtnViewList[i].face_vertex);
	}
}

bool FunctionIsModified(KLFunctionLayerType layerType, KEY_MapId_t kmID)
{
	auto configManager = KLFunctionConfigManager::GetInstance();

	KLFunctionID fid = FindFunctionIDByMapID(kmID);

	auto function = configManager->m_CurrentConfig->layers[configManager->m_CurrentLayerType][kmID];

	if (function.id == fid)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void RenderModelUpdate()
{
	kbv_draw_ctx.vertices.resize(0);

	glm::vec3 global_pos(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < keyBtnViewSize; ++i)
	{
		glm::vec3 pos1(global_pos.x + keyBtnViewList[i].x, global_pos.y + keyBtnViewList[i].y, 0.0f);
		glm::vec3 shape(keyBtnViewList[i].w, keyBtnViewList[i].h, KEY_DEPTH);
		pos1 /= SECTOR;
		shape /= SECTOR;

		bool modified = FunctionIsModified(KLFunctionConfigManager::GetInstance()->m_CurrentLayerType, keyBtnViewList[i].id);
		glm::vec4 color;
		if (i == kbv_draw_ctx.active_index && modified)
		{
			color = MODIFIED_ACTIVE_COLOR;
		}
		else if (i == kbv_draw_ctx.active_index)
		{
			color = ACTIVE_COLOR;
		}
		else if (keyBtnViewList[i].hover) 
		{
			color = HOVER_COLOR;
		}
		else if (modified)
		{
			color = MODIFIED_COLOR;
		}
		else
		{
			color = DEFAULT_COLOR;
		}
		DrawKeycap(pos1, shape, PADDING, THINKNESS, color, kbv_draw_ctx.vertices);
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


void assignment_layout_kbv_hover_cb(KeyBtnView& view)
{
	ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
	ImGui::OpenPopup("show_kbv_bind_function");
	if (ImGui::BeginPopup("show_kbv_bind_function"))
	{
		KLFunctionID fid = FindFunctionIDByMapID(view.id);
		KLFunction defaultFunction = FindFunctionByFunctionID(fid);

		auto configManager = KLFunctionConfigManager::GetInstance();

		KLFunction currentFunction = configManager->m_CurrentConfig->layers[configManager->m_CurrentLayerType][view.id];

		char temp_buf[64];
		sprintf(temp_buf, " [default:%s|current:%s] ", defaultFunction.name, currentFunction.name);
		ImGui::Text(temp_buf);
		ImGui::EndPopup();
	}
}

void KeyboardGLInit()
{
	auto& style = ImGui::GetStyle();
	ImVec4 bgcolor = style.Colors[ImGuiCol_WindowBg];

	kbv_draw_ctx.onhover = assignment_layout_kbv_hover_cb;
	kbv_draw_ctx.camera.Position = glm::vec3(10.4f, -3.5f, 14.4f);
	kbv_draw_ctx.projection_matrix = glm::perspective(glm::radians(kbv_draw_ctx.camera.Zoom), (float)kbv_draw_ctx.w / (float)kbv_draw_ctx.h, 0.1f, 100.0f);
	kbv_draw_ctx.view_matrix = kbv_draw_ctx.camera.GetViewMatrix();
	kbv_draw_ctx.model_matrix = glm::mat4(1.0f);
	kbv_draw_ctx.clear_color = glm::vec4(bgcolor.x, bgcolor.y, bgcolor.z, bgcolor.w);
	kbv_draw_ctx.shader = std::make_unique<Shader>("3.2.blending.vs", "3.2.blending.fs");

	RenderModelInit();
	RenderModelUpdate();

	// first, configure the cube's VAO (and VBO)
	glGenVertexArrays(1, &kbv_draw_ctx.VAO);
	glGenBuffers(1, &kbv_draw_ctx.VBO);

	glBindBuffer(GL_ARRAY_BUFFER, kbv_draw_ctx.VBO);
	glBufferData(GL_ARRAY_BUFFER, kbv_draw_ctx.vertices.size() * sizeof(float), kbv_draw_ctx.vertices.data(), GL_STREAM_DRAW);

	glBindVertexArray(kbv_draw_ctx.VAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// color attribute
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


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
	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, kbv_draw_ctx.w, kbv_draw_ctx.w);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void KeyboardGLDraw()
{
	glBindFramebuffer(GL_FRAMEBUFFER, kbv_draw_ctx.FBO);

	glViewport(0.0f, 0.0f, kbv_draw_ctx.w, kbv_draw_ctx.h);

	glClearColor(
		kbv_draw_ctx.clear_color.r, kbv_draw_ctx.clear_color.g, 
		kbv_draw_ctx.clear_color.b, kbv_draw_ctx.clear_color.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// draw objects
	kbv_draw_ctx.shader->use();
	kbv_draw_ctx.shader->setMat4("model", kbv_draw_ctx.model_matrix);
	kbv_draw_ctx.shader->setMat4("projection", kbv_draw_ctx.projection_matrix);
	kbv_draw_ctx.shader->setMat4("view", kbv_draw_ctx.view_matrix);

	// render the cube
	glBindBuffer(GL_ARRAY_BUFFER, kbv_draw_ctx.VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, kbv_draw_ctx.vertices.size() * sizeof(float), kbv_draw_ctx.vertices.data());

	glBindVertexArray(kbv_draw_ctx.VAO);
	glDrawArrays(GL_TRIANGLES, 0, kbv_draw_ctx.vertices.size() / 10);

}
void KeyboardGLDestroy()
{
	//glDeleteVertexArrays(VAO);
	// TODO:
}



void ShowKeyboardWindow(bool* p_open)
{
	ImGuiWindowFlags window_flags = 0;
	if (!ImGui::Begin(WINNAME_KEYBOARD, p_open, window_flags)) {
		ImGui::End();
		return;
	}

	auto win = ImGui::GetCurrentWindow();
	auto& DC = win->DC;
	auto& io = ImGui::GetIO();

	/* Update the position of texture view */
	kbv_draw_ctx.mouse_pos.x = io.MousePos.x - DC.CursorPos.x;
	kbv_draw_ctx.mouse_pos.y = io.MousePos.y - DC.CursorPos.y;
	kbv_draw_ctx.mouse_down = io.MouseDown[0];

	/* Convert mouse screen coordinates to world coordinates */
	glm::vec4 viewport(0.0f, 0.0f, kbv_draw_ctx.w, kbv_draw_ctx.h);
	glm::mat4 modelViewMatrix = kbv_draw_ctx.view_matrix * kbv_draw_ctx.model_matrix;
	glm::vec3 mousePos_NDC(kbv_draw_ctx.mouse_pos.x, kbv_draw_ctx.h - kbv_draw_ctx.mouse_pos.y, 0.1f);
	glm::vec3 mousePos_3DC = glm::unProject(mousePos_NDC, modelViewMatrix, kbv_draw_ctx.projection_matrix, viewport);

	for (int i = 0; i < keyBtnViewSize; ++i)
	{
		if (wmouse_in_quad(mousePos_3DC, kbv_draw_ctx.camera.Position, keyBtnViewList[i].face_vertex))
		{
			keyBtnViewList[i].hover = true;

			if (kbv_draw_ctx.onhover)
				kbv_draw_ctx.onhover(keyBtnViewList[i]);

			if (kbv_draw_ctx.mouse_down)
				kbv_draw_ctx.active_index = i;
		}
		else
		{
			keyBtnViewList[i].hover = false;
		}
	}

	RenderModelUpdate();


	// Draw texture
	ImGui::Image(
		(ImTextureID)kbv_draw_ctx.texColorBuffer,	// TextureID
		ImVec2(kbv_draw_ctx.w, kbv_draw_ctx.h),		// Width and Height
		ImVec2(0, 1), ImVec2(1, 0)					// UV mapping
	);

	ImGui::End();
}