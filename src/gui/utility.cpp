#include <unordered_map>

#include "gui/utility.hpp"

static const std::unordered_map<ImGuiKey, vts::InputId> imguiToUioKeyMap = {
    {ImGuiKey_Tab, VC_TAB},
    {ImGuiKey_LeftArrow, VC_LEFT},
    {ImGuiKey_RightArrow, VC_RIGHT},
    {ImGuiKey_UpArrow, VC_UP},
    {ImGuiKey_DownArrow, VC_DOWN},
    {ImGuiKey_PageUp, VC_PAGE_UP},
    {ImGuiKey_PageDown, VC_PAGE_DOWN},
    {ImGuiKey_Home, VC_HOME},
    {ImGuiKey_End, VC_END},
    {ImGuiKey_Insert, VC_INSERT},
    {ImGuiKey_Delete, VC_DELETE},
    {ImGuiKey_Backspace, VC_BACKSPACE},
    {ImGuiKey_Space, VC_SPACE},
    {ImGuiKey_Enter, VC_ENTER},
    {ImGuiKey_Escape, VC_ESCAPE},

    {ImGuiKey_0, VC_0},
    {ImGuiKey_1, VC_1},
    {ImGuiKey_2, VC_2},
    {ImGuiKey_3, VC_3},
    {ImGuiKey_4, VC_4},
    {ImGuiKey_5, VC_5},
    {ImGuiKey_6, VC_6},
    {ImGuiKey_7, VC_7},
    {ImGuiKey_8, VC_8},
    {ImGuiKey_9, VC_9},

    {ImGuiKey_A, VC_A},
    {ImGuiKey_B, VC_B},
    {ImGuiKey_C, VC_C},
    {ImGuiKey_D, VC_D},
    {ImGuiKey_E, VC_E},
    {ImGuiKey_F, VC_F},
    {ImGuiKey_G, VC_G},
    {ImGuiKey_H, VC_H},
    {ImGuiKey_I, VC_I},
    {ImGuiKey_J, VC_J},
    {ImGuiKey_K, VC_K},
    {ImGuiKey_L, VC_L},
    {ImGuiKey_M, VC_M},
    {ImGuiKey_N, VC_N},
    {ImGuiKey_O, VC_O},
    {ImGuiKey_P, VC_P},
    {ImGuiKey_Q, VC_Q},
    {ImGuiKey_R, VC_R},
    {ImGuiKey_S, VC_S},
    {ImGuiKey_T, VC_T},
    {ImGuiKey_U, VC_U},
    {ImGuiKey_V, VC_V},
    {ImGuiKey_W, VC_W},
    {ImGuiKey_X, VC_X},
    {ImGuiKey_Y, VC_Y},
    {ImGuiKey_Z, VC_Z},

    {ImGuiKey_F1, VC_F1},
    {ImGuiKey_F2, VC_F2},
    {ImGuiKey_F3, VC_F3},
    {ImGuiKey_F4, VC_F4},
    {ImGuiKey_F5, VC_F5},
    {ImGuiKey_F6, VC_F6},
    {ImGuiKey_F7, VC_F7},
    {ImGuiKey_F8, VC_F8},
    {ImGuiKey_F9, VC_F9},
    {ImGuiKey_F10, VC_F10},
    {ImGuiKey_F11, VC_F11},
    {ImGuiKey_F12, VC_F12},
    {ImGuiKey_F13, VC_F13},
    {ImGuiKey_F14, VC_F14},
    {ImGuiKey_F15, VC_F15},
    {ImGuiKey_F16, VC_F16},
    {ImGuiKey_F17, VC_F17},
    {ImGuiKey_F18, VC_F18},
    {ImGuiKey_F19, VC_F19},
    {ImGuiKey_F20, VC_F20},
    {ImGuiKey_F21, VC_F21},
    {ImGuiKey_F22, VC_F22},
    {ImGuiKey_F23, VC_F23},
    {ImGuiKey_F24, VC_F24},

    {ImGuiKey_Apostrophe, VC_QUOTE},
    {ImGuiKey_Comma, VC_COMMA},
    {ImGuiKey_Minus, VC_MINUS},
    {ImGuiKey_Period, VC_PERIOD},
    {ImGuiKey_Slash, VC_SLASH},
    {ImGuiKey_Semicolon, VC_SEMICOLON},
    {ImGuiKey_Equal, VC_EQUALS},
    {ImGuiKey_LeftBracket, VC_OPEN_BRACKET},
    {ImGuiKey_Backslash, VC_BACK_SLASH},
    {ImGuiKey_RightBracket, VC_CLOSE_BRACKET},
    {ImGuiKey_GraveAccent, VC_BACKQUOTE},

    {ImGuiKey_CapsLock, VC_CAPS_LOCK},
    {ImGuiKey_ScrollLock, VC_SCROLL_LOCK},
    {ImGuiKey_NumLock, VC_NUM_LOCK},
    {ImGuiKey_PrintScreen, VC_PRINTSCREEN},
    {ImGuiKey_Pause, VC_PAUSE},

    {ImGuiKey_Keypad0, VC_KP_0},
    {ImGuiKey_Keypad1, VC_KP_1},
    {ImGuiKey_Keypad2, VC_KP_2},
    {ImGuiKey_Keypad3, VC_KP_3},
    {ImGuiKey_Keypad4, VC_KP_4},
    {ImGuiKey_Keypad5, VC_KP_5},
    {ImGuiKey_Keypad6, VC_KP_6},
    {ImGuiKey_Keypad7, VC_KP_7},
    {ImGuiKey_Keypad8, VC_KP_8},
    {ImGuiKey_Keypad9, VC_KP_9},
    {ImGuiKey_KeypadDecimal, VC_KP_SEPARATOR},
    {ImGuiKey_KeypadDivide, VC_KP_DIVIDE},
    {ImGuiKey_KeypadMultiply, VC_KP_MULTIPLY},
    {ImGuiKey_KeypadSubtract, VC_KP_SUBTRACT},
    {ImGuiKey_KeypadAdd, VC_KP_ADD},
    {ImGuiKey_KeypadEnter, VC_KP_ENTER},
    {ImGuiKey_KeypadEqual, VC_KP_EQUALS},
};

std::unordered_map<vts::InputId, const char*> uioToStringMap = []() {
	std::unordered_map<vts::InputId, const char*> map;
	for (const auto& pair : imguiToUioKeyMap) {
		map[pair.second] = ImGui::GetKeyName(pair.first);
	}
	return map;
}();

namespace gui {

KeycodeView validImGuiKeys() {
	return imguiToUioKeyMap | std::views::keys;
}

vts::InputId convertImGuiToUioKey(const ImGuiKey keycode) {
	return imguiToUioKeyMap.at(keycode);
}

const char* getUioKeyName(const Uint32 keycode) {
	return uioToStringMap.at(keycode);
}

void initColors() {
	auto& style         = ImGui::GetStyle();
	style.DisabledAlpha = 1.0f;

	auto& colors                         = style.Colors;
	colors[ImGuiCol_TextDisabled]        = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	colors[ImGuiCol_FrameBg]             = ImVec4(0.294f, 0.125f, 0.122f, 1.0f);
	colors[ImGuiCol_FrameBgHovered]      = ImVec4(0.439f, 0.149f, 0.157f, 1.0f);
	colors[ImGuiCol_FrameBgActive]       = ImVec4(0.682f, 0.2f, 0.212f, 1.0f);
	colors[ImGuiCol_TitleBgActive]       = ImVec4(0.478f, 0.169f, 0.161f, 1.0f);
	colors[ImGuiCol_CheckMark]           = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
	colors[ImGuiCol_SliderGrab]          = ImVec4(0.878f, 0.239f, 0.247f, 1.0f);
	colors[ImGuiCol_SliderGrabActive]    = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
	colors[ImGuiCol_Button]              = ImVec4(0.439f, 0.149f, 0.157f, 1.0f);
	colors[ImGuiCol_ButtonHovered]       = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
	colors[ImGuiCol_ButtonActive]        = ImVec4(0.98f, 0.059f, 0.133f, 1.0f);
	colors[ImGuiCol_Header]              = ImVec4(0.357f, 0.133f, 0.141f, 1.0f);
	colors[ImGuiCol_HeaderHovered]       = ImVec4(0.8f, 0.224f, 0.239f, 1.0f);
	colors[ImGuiCol_HeaderActive]        = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
	colors[ImGuiCol_SeparatorHovered]    = ImVec4(0.6f, 0.098f, 0.114f, 1.0f);
	colors[ImGuiCol_SeparatorActive]     = ImVec4(0.749f, 0.102f, 0.122f, 1.0f);
	colors[ImGuiCol_ResizeGrip]          = ImVec4(0.259f, 0.114f, 0.118f, 1.0f);
	colors[ImGuiCol_ResizeGripHovered]   = ImVec4(0.682f, 0.2f, 0.212f, 1.0f);
	colors[ImGuiCol_ResizeGripActive]    = ImVec4(0.933f, 0.251f, 0.271f, 1.0f);
	colors[ImGuiCol_TabHovered]          = ImVec4(0.8f, 0.224f, 0.239f, 1.0f);
	colors[ImGuiCol_Tab]                 = ImVec4(0.51f, 0.165f, 0.165f, 1.0f);
	colors[ImGuiCol_TabSelected]         = ImVec4(0.678f, 0.2f, 0.208f, 1.0f);
	colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
	colors[ImGuiCol_TabDimmed]           = ImVec4(0.149f, 0.067f, 0.067f, 1.0f);
	colors[ImGuiCol_TabDimmedSelected]   = ImVec4(0.424f, 0.137f, 0.141f, 1.0f);
	colors[ImGuiCol_TextLink]            = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
	colors[ImGuiCol_TextSelectedBg]      = ImVec4(0.392f, 0.141f, 0.149f, 1.0f);
	colors[ImGuiCol_NavCursor]           = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
}

}  // namespace gui
