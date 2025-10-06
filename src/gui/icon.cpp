#include "gui/icon.hpp"

#include <unordered_map>

#include "imgui/imgui.h"
#include "libuiohook/uiohook.h"

#include "gui/fonts.hpp"
#include "impulse/code.hpp"

namespace gui {

using IconMap = std::unordered_map<imp::TargetTag, const char*>;

static IconMap keyStrings{
    {VC_TAB,           "\u242B"},
    {VC_LEFT,          "\u23F4"},
    {VC_RIGHT,         "\u23F5"},
    {VC_UP,            "\u23F6"},
    {VC_DOWN,          "\u23F7"},
    {VC_PAGE_UP,       "\u2436"},
    {VC_PAGE_DOWN,     "\u2439"},
    {VC_HOME,          "\u2435"},
    {VC_END,           "\u2438"},
    {VC_INSERT,        "\u2434"},
    {VC_DELETE,        "\u2437"},
    {VC_BACKSPACE,     "\u242D"},
    {VC_SPACE,         "\u243A"},
    {VC_ENTER,         "\u242E"},
    {VC_ESCAPE,        "\u242F"},

    {VC_0,             "\uFF10"},
    {VC_1,             "\uFF11"},
    {VC_2,             "\uFF12"},
    {VC_3,             "\uFF13"},
    {VC_4,             "\uFF14"},
    {VC_5,             "\uFF15"},
    {VC_6,             "\uFF16"},
    {VC_7,             "\uFF17"},
    {VC_8,             "\uFF18"},
    {VC_9,             "\uFF19"},
    {VC_A,             "\uFF21"},
    {VC_B,             "\uFF22"},
    {VC_C,             "\uFF23"},
    {VC_D,             "\uFF24"},
    {VC_E,             "\uFF25"},
    {VC_F,             "\uFF26"},
    {VC_G,             "\uFF27"},
    {VC_H,             "\uFF28"},
    {VC_I,             "\uFF29"},
    {VC_J,             "\uFF2A"},
    {VC_K,             "\uFF2B"},
    {VC_L,             "\uFF2C"},
    {VC_M,             "\uFF2D"},
    {VC_N,             "\uFF2E"},
    {VC_O,             "\uFF2F"},
    {VC_P,             "\uFF30"},
    {VC_Q,             "\uFF31"},
    {VC_R,             "\uFF32"},
    {VC_S,             "\uFF33"},
    {VC_T,             "\uFF34"},
    {VC_U,             "\uFF35"},
    {VC_V,             "\uFF36"},
    {VC_W,             "\uFF37"},
    {VC_X,             "\uFF38"},
    {VC_Y,             "\uFF39"},
    {VC_Z,             "\uFF3A"},

    {VC_F1,            "\u2460"},
    {VC_F2,            "\u2461"},
    {VC_F3,            "\u2462"},
    {VC_F4,            "\u2463"},
    {VC_F5,            "\u2464"},
    {VC_F6,            "\u2465"},
    {VC_F7,            "\u2466"},
    {VC_F8,            "\u2467"},
    {VC_F9,            "\u2468"},
    {VC_F10,           "\u2469"},
    {VC_F11,           "\u246A"},
    {VC_F12,           "\u246B"},
    {VC_F13,           "\u2426"},
    {VC_F14,           "\u2426"},
    {VC_F15,           "\u2426"},
    {VC_F16,           "\u2426"},
    {VC_F17,           "\u2426"},
    {VC_F18,           "\u2426"},
    {VC_F19,           "\u2426"},
    {VC_F20,           "\u2426"},
    {VC_F21,           "\u2426"},
    {VC_F22,           "\u2426"},
    {VC_F23,           "\u2426"},
    {VC_F24,           "\u2426"},

    {VC_QUOTE,         "\uFF02"},
    {VC_COMMA,         "\uFF0C"},
    {VC_MINUS,         "\uFF0D"},
    {VC_PERIOD,        "\uFF0E"},
    {VC_SLASH,         "\uFF0F"},
    {VC_SEMICOLON,     "\uFF1B"},
    {VC_EQUALS,        "\uFF1D"},
    {VC_OPEN_BRACKET,  "\uFF3B"},
    {VC_BACK_SLASH,    "\uFF3C"},
    {VC_CLOSE_BRACKET, "\uFF3D"},
    {VC_BACKQUOTE,     "\uFF5E"},
    {VC_CAPS_LOCK,     "\u242C"},
    {VC_SCROLL_LOCK,   "\u2431"},
    {VC_NUM_LOCK,      "\u2433"},
    {VC_PRINTSCREEN,   "\u2430"},
    {VC_PAUSE,         "\u2432"},

    {VC_KP_0,          "\uFF10"},
    {VC_KP_1,          "\uFF11"},
    {VC_KP_2,          "\uFF12"},
    {VC_KP_3,          "\uFF13"},
    {VC_KP_4,          "\uFF14"},
    {VC_KP_5,          "\uFF15"},
    {VC_KP_6,          "\uFF16"},
    {VC_KP_7,          "\uFF17"},
    {VC_KP_8,          "\uFF18"},
    {VC_KP_9,          "\uFF19"},

    {VC_KP_SEPARATOR,  "\uFF0D"},
    {VC_KP_DIVIDE,     "\uFF0F"},
    {VC_KP_MULTIPLY,   "\uFF0A"},
    {VC_KP_SUBTRACT,   "\uFF0D"},
    {VC_KP_ADD,        "\uFF0B"},
    {VC_KP_ENTER,      "\u242E"},
    {VC_KP_EQUALS,     "\uFF1D"},

    {VC_ALT_L,         "\u244B"},
    {VC_ALT_R,         "\u244C"},
    {VC_CONTROL_L,     "\u244D"},
    {VC_CONTROL_R,     "\u244E"},
    {VC_SHIFT_L,       "\u244F"},
    {VC_SHIFT_R,       "\u2450"},
};

static IconMap mouseButtonStrings{
    {imp::MouseButton::LEFT,   "\u278A"},
    {imp::MouseButton::RIGHT,  "\u278B"},
    {imp::MouseButton::MIDDLE, "\u278C"},
};

static IconMap mouseWheelStrings{
    {imp::MouseWheel::UP,   "\u27F0"},
    {imp::MouseWheel::DOWN, "\u27F1"},
};

static IconMap mouseMoveStrings{
    {imp::Axis::X, "\u27FA"},
    {imp::Axis::Y, "\u27FB"},
};

static IconMap gamepadButtonStrings{
    {imp::GamepadButton::NORTH,          "\u21A5"},
    {imp::GamepadButton::SOUTH,          "\u21A7"},
    {imp::GamepadButton::WEST,           "\u21A4"},
    {imp::GamepadButton::EAST,           "\u21A6"},
    {imp::GamepadButton::LEFT_SHOULDER,  "\u2198"},
    {imp::GamepadButton::RIGHT_SHOULDER, "\u2199"},
    {imp::GamepadButton::DPAD_UP,        "\u219F"},
    {imp::GamepadButton::DPAD_DOWN,      "\u21A1"},
    {imp::GamepadButton::DPAD_LEFT,      "\u219E"},
    {imp::GamepadButton::DPAD_RIGHT,     "\u21A0"},
    {imp::GamepadButton::LEFT_STICK,     "\u21BA"},
    {imp::GamepadButton::RIGHT_STICK,    "\u21BB"},
};

static IconMap gamepadTriggerStrings{
    {imp::Side::LEFT,  "\u2196"},
    {imp::Side::RIGHT, "\u2197"},
};

static IconMap gamepadStickLeftStrings{
    {imp::Axis::X, "\u21C4"},
    {imp::Axis::Y, "\u21C5"},
};

static IconMap gamepadStickRightStrings{
    {imp::Axis::X, "\u21C6"},
    {imp::Axis::Y, "\u21F5"},
};

static const std::unordered_map<const char*, float> Y_OFFSETS{
    {"\uE0E2", -3.0F},
    {"\uE0F2", 3.0F },
};

void drawIconOrDefault(const imp::TargetTag target,
                       const float          alpha,
                       const IconMap&       lookup,
                       const char*          defaultString) {
	const char* iconString = defaultString;
	auto        stringIter = lookup.find(target);
	if (stringIter != lookup.end()) {
		iconString = stringIter->second;
	}
	ImGui::SameLine();
	const auto dyIter = Y_OFFSETS.find(iconString);
	if (dyIter != Y_OFFSETS.end()) {
		const ImVec2 cursorPos = ImGui::GetCursorPos();
		ImGui::SetCursorPos(ImVec2(cursorPos.x, cursorPos.y + dyIter->second));
	}
	ImGui::TextColored(ImVec4(1.0F, 1.0F, 1.0F, alpha), "%s", iconString);
}

void drawIcon(const imp::Code code, const float alpha) {
	const imp::EventTag::T event  = code & 0xFFFF;
	const imp::TargetTag   target = code & 0xFFFF0000;
	FONT_SCOPE(FontType::IMPULSE);
	switch (event) {
		case imp::EventTag::KEY:
			drawIconOrDefault(target >> 16, alpha, keyStrings, "\u248F");
			break;
		case imp::EventTag::MOUSE_BUTTON:
			drawIconOrDefault(target, alpha, mouseButtonStrings, "\u2791");
			break;
		case imp::EventTag::MOUSE_WHEEL:
			drawIconOrDefault(target, alpha, mouseWheelStrings, "\u27F2");
			break;
		case imp::EventTag::MOUSE_MOVE_ABS:
		case imp::EventTag::MOUSE_MOVE_REL:
			drawIconOrDefault(target, alpha, mouseMoveStrings, "\u27FC");
			break;
		case imp::EventTag::GAMEPAD_BUTTON:
			drawIconOrDefault(target, alpha, gamepadButtonStrings, "\u21A8");
			break;
		case imp::EventTag::GAMEPAD_TRIGGER:
			drawIconOrDefault(target, alpha, gamepadTriggerStrings, "\u21EA");
			break;
		case imp::EventTag::GAMEPAD_STICK_LEFT:
			drawIconOrDefault(target, alpha, gamepadStickLeftStrings, "\u21CB");
			break;
		case imp::EventTag::GAMEPAD_STICK_RIGHT:
			drawIconOrDefault(target, alpha, gamepadStickRightStrings, "\u21CC");
			break;
	}
}

}  // namespace gui
