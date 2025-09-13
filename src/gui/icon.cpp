#include <unordered_map>

#include "libuiohook/uiohook.h"

#include "gui/fonts.hpp"
#include "gui/icon.hpp"

namespace gui {

using IconMap = std::unordered_map<vts::InputId, const char*>;

static IconMap keyStrings{
    {VC_TAB,           "\uE0CB"},
    {VC_LEFT,          "\uE01F"},
    {VC_RIGHT,         "\uE021"},
    {VC_UP,            "\uE023"},
    {VC_DOWN,          "\uE01D"},
    {VC_PAGE_UP,       "\uE0A7"},
    {VC_PAGE_DOWN,     "\uE0A5"},
    {VC_HOME,          "\uE086"},
    {VC_END,           "\uE05C"},
    {VC_INSERT,        "\uE08A"},
    {VC_DELETE,        "\uE058"},
    {VC_BACKSPACE,     "\uE038"},
    {VC_SPACE,         "\uE0C5"},
    {VC_ENTER,         "\uE0B7"},
    {VC_ESCAPE,        "\uE062"},

    {VC_0,             "\uE001"},
    {VC_1,             "\uE003"},
    {VC_2,             "\uE005"},
    {VC_3,             "\uE007"},
    {VC_4,             "\uE009"},
    {VC_5,             "\uE00B"},
    {VC_6,             "\uE00D"},
    {VC_7,             "\uE00F"},
    {VC_8,             "\uE011"},
    {VC_9,             "\uE013"},
    {VC_A,             "\uE015"},
    {VC_B,             "\uE036"},
    {VC_C,             "\uE046"},
    {VC_D,             "\uE056"},
    {VC_E,             "\uE05A"},
    {VC_F,             "\uE066"},
    {VC_G,             "\uE082"},
    {VC_H,             "\uE084"},
    {VC_I,             "\uE088"},
    {VC_J,             "\uE08C"},
    {VC_K,             "\uE08E"},
    {VC_L,             "\uE090"},
    {VC_M,             "\uE092"},
    {VC_N,             "\uE096"},
    {VC_O,             "\uE09E"},
    {VC_P,             "\uE0A3"},
    {VC_Q,             "\uE0AF"},
    {VC_R,             "\uE0B5"},
    {VC_S,             "\uE0B9"},
    {VC_T,             "\uE0C9"},
    {VC_U,             "\uE0D3"},
    {VC_V,             "\uE0D5"},
    {VC_W,             "\uE0D7"},
    {VC_X,             "\uE0DB"},
    {VC_Y,             "\uE0DD"},
    {VC_Z,             "\uE0DF"},

    {VC_F1,            "\uE067"},
    {VC_F2,            "\uE06F"},
    {VC_F3,            "\uE071"},
    {VC_F4,            "\uE073"},
    {VC_F5,            "\uE075"},
    {VC_F6,            "\uE077"},
    {VC_F7,            "\uE079"},
    {VC_F8,            "\uE07B"},
    {VC_F9,            "\uE07D"},
    {VC_F10,           "\uE068"},
    {VC_F11,           "\uE06A"},
    {VC_F12,           "\uE06C"},
    {VC_F13,           "\uE080"},
    {VC_F14,           "\uE080"},
    {VC_F15,           "\uE080"},
    {VC_F16,           "\uE080"},
    {VC_F17,           "\uE080"},
    {VC_F18,           "\uE080"},
    {VC_F19,           "\uE080"},
    {VC_F20,           "\uE080"},
    {VC_F21,           "\uE080"},
    {VC_F22,           "\uE080"},
    {VC_F23,           "\uE080"},
    {VC_F24,           "\uE080"},

    {VC_QUOTE,         "\uE0B3"},
    {VC_COMMA,         "\uE050"},
    {VC_MINUS,         "\uE094"},
    {VC_PERIOD,        "\uE0A9"},
    {VC_SLASH,         "\uE0C3"},
    {VC_SEMICOLON,     "\uE0BB"},
    {VC_EQUALS,        "\uE060"},
    {VC_OPEN_BRACKET,  "\uE044"},
    {VC_BACK_SLASH,    "\uE0C1"},
    {VC_CLOSE_BRACKET, "\uE03E"},
    {VC_BACKQUOTE,     "\uE0D1"},
    {VC_CAPS_LOCK,     "\uE048"},
    {VC_SCROLL_LOCK,   "\uE025"},
    {VC_NUM_LOCK,      "\uE098"},
    {VC_PRINTSCREEN,   "\uE0AD"},
    {VC_PAUSE,         "\uE080"},

    {VC_KP_0,          "\uE001"},
    {VC_KP_1,          "\uE003"},
    {VC_KP_2,          "\uE005"},
    {VC_KP_3,          "\uE007"},
    {VC_KP_4,          "\uE009"},
    {VC_KP_5,          "\uE00B"},
    {VC_KP_6,          "\uE00D"},
    {VC_KP_7,          "\uE00F"},
    {VC_KP_8,          "\uE011"},
    {VC_KP_9,          "\uE013"},

    {VC_KP_SEPARATOR,  "\uE0A9"},
    {VC_KP_DIVIDE,     "\uE0C3"},
    {VC_KP_MULTIPLY,   "\uE034"},
    {VC_KP_SUBTRACT,   "\uE094"},
    {VC_KP_ADD,        "\uE09C"},
    {VC_KP_ENTER,      "\uE09A"},
    {VC_KP_EQUALS,     "\uE060"},
};

static IconMap mouseButtonStrings{
    {vts::MouseButton::LEFT,   "\uE0E4"},
    {vts::MouseButton::RIGHT,  "\uE0E8"},
    {vts::MouseButton::MIDDLE, "\uE0EC"},
};

static IconMap mouseMoveStrings{
    {vts::Axis::X, "\uE0E2"},
    {vts::Axis::Y, "\uE0F2"},
};

static IconMap gamepadButtonStrings{
    {vts::GamepadButton::NORTH,          "\uE032"},
    {vts::GamepadButton::SOUTH,          "\uE029"},
    {vts::GamepadButton::WEST,           "\uE02D"},
    {vts::GamepadButton::EAST,           "\uE030"},
    {vts::GamepadButton::LEFT_SHOULDER,  "\uE018"},
    {vts::GamepadButton::RIGHT_SHOULDER, "\uE01B"},
    {vts::GamepadButton::DPAD_UP,        "\uE043"},
    {vts::GamepadButton::DPAD_DOWN,      "\uE03A"},
    {vts::GamepadButton::DPAD_LEFT,      "\uE03E"},
    {vts::GamepadButton::DPAD_RIGHT,     "\uE041"},
    {vts::GamepadButton::LEFT_STICK,     "\uE074"},
    {vts::GamepadButton::RIGHT_STICK,    "\uE075"},
};

static IconMap gamepadTriggerStrings{
    {vts::Side::LEFT,  "\uE023"},
    {vts::Side::RIGHT, "\uE025"},
};

static IconMap gamepadStickLeftStrings{
    {vts::Axis::X, "\uE066"},
    {vts::Axis::Y, "\uE06B"},
};

static IconMap gamepadStickRightStrings{
    {vts::Axis::X, "\uE06E"},
    {vts::Axis::Y, "\uE073"},
};

void drawIconOrDefault(const vts::InputId target,
                       const float        alpha,
                       const IconMap&     lookup,
                       const char*        defaultString,
                       const float        yOffset = 0.0f) {
	const char* iconString = defaultString;
	auto        it         = lookup.find(target);
	if (it != lookup.end()) {
		iconString = it->second;
	}
	ImGui::SameLine();
	if (yOffset != 0.0f) {
		ImVec2 cursorPos = ImGui::GetCursorPos();
		ImGui::SetCursorPos(ImVec2(cursorPos.x, cursorPos.y + yOffset));
	}
	ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, alpha), iconString);
}

void drawIcon(const vts::InputId id, const float alpha) {
	const vts::InputId event  = id & 0xFFFF;
	const vts::InputId target = id & 0xFFFF0000;
	switch (event) {
		case vts::InputEvent::KEY:
			Fonts::push(FontType::MOUSE_KEYBOARD);
			drawIconOrDefault(target >> 16, alpha, keyStrings, "\uE000");
			break;
		case vts::InputEvent::MOUSE_BUTTON:
			Fonts::push(FontType::MOUSE_KEYBOARD);
			drawIconOrDefault(target, alpha, mouseButtonStrings, "\uE0E6");
			break;
		case vts::InputEvent::MOUSE_MOVE_ABS:
		case vts::InputEvent::MOUSE_MOVE_REL:
			Fonts::push(FontType::MOUSE_KEYBOARD);
			drawIconOrDefault(target, alpha, mouseMoveStrings, "\uE0E6", target == vts::Axis::X ? -3.0f : 3.0f);
			break;
		case vts::InputEvent::GAMEPAD_BUTTON:
			Fonts::push(FontType::GAMEPAD);
			drawIconOrDefault(target, alpha, gamepadButtonStrings, "\uE003");
			break;
		case vts::InputEvent::GAMEPAD_TRIGGER:
			Fonts::push(FontType::GAMEPAD);
			drawIconOrDefault(target, alpha, gamepadTriggerStrings, "\uE023");
			break;
		case vts::InputEvent::GAMEPAD_STICK_LEFT:
			Fonts::push(FontType::GAMEPAD);
			drawIconOrDefault(target, alpha, gamepadStickLeftStrings, "\uE064");
			break;
		case vts::InputEvent::GAMEPAD_STICK_RIGHT:
			Fonts::push(FontType::GAMEPAD);
			drawIconOrDefault(target, alpha, gamepadStickRightStrings, "\uE06C");
			break;
	}
	ImGui::PopFont();
}

}  // namespace gui
