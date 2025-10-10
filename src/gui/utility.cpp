#include "gui/utility.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <ranges>
#include <unordered_map>

#include <SDL3/SDL_stdinc.h>

#include "imgui/imgui.h"
#include "libuiohook/uiohook.h"

#include "impulse/code.hpp"

static const std::unordered_map<ImGuiKey, imp::TargetTag> imguiToUioKeyMap = {
    {ImGuiKey_Tab,            VC_TAB          },
    {ImGuiKey_LeftArrow,      VC_LEFT         },
    {ImGuiKey_RightArrow,     VC_RIGHT        },
    {ImGuiKey_UpArrow,        VC_UP           },
    {ImGuiKey_DownArrow,      VC_DOWN         },
    {ImGuiKey_PageUp,         VC_PAGE_UP      },
    {ImGuiKey_PageDown,       VC_PAGE_DOWN    },
    {ImGuiKey_Home,           VC_HOME         },
    {ImGuiKey_End,            VC_END          },
    {ImGuiKey_Insert,         VC_INSERT       },
    {ImGuiKey_Delete,         VC_DELETE       },
    {ImGuiKey_Backspace,      VC_BACKSPACE    },
    {ImGuiKey_Space,          VC_SPACE        },
    {ImGuiKey_Enter,          VC_ENTER        },
    {ImGuiKey_Escape,         VC_ESCAPE       },

    {ImGuiKey_0,              VC_0            },
    {ImGuiKey_1,              VC_1            },
    {ImGuiKey_2,              VC_2            },
    {ImGuiKey_3,              VC_3            },
    {ImGuiKey_4,              VC_4            },
    {ImGuiKey_5,              VC_5            },
    {ImGuiKey_6,              VC_6            },
    {ImGuiKey_7,              VC_7            },
    {ImGuiKey_8,              VC_8            },
    {ImGuiKey_9,              VC_9            },

    {ImGuiKey_A,              VC_A            },
    {ImGuiKey_B,              VC_B            },
    {ImGuiKey_C,              VC_C            },
    {ImGuiKey_D,              VC_D            },
    {ImGuiKey_E,              VC_E            },
    {ImGuiKey_F,              VC_F            },
    {ImGuiKey_G,              VC_G            },
    {ImGuiKey_H,              VC_H            },
    {ImGuiKey_I,              VC_I            },
    {ImGuiKey_J,              VC_J            },
    {ImGuiKey_K,              VC_K            },
    {ImGuiKey_L,              VC_L            },
    {ImGuiKey_M,              VC_M            },
    {ImGuiKey_N,              VC_N            },
    {ImGuiKey_O,              VC_O            },
    {ImGuiKey_P,              VC_P            },
    {ImGuiKey_Q,              VC_Q            },
    {ImGuiKey_R,              VC_R            },
    {ImGuiKey_S,              VC_S            },
    {ImGuiKey_T,              VC_T            },
    {ImGuiKey_U,              VC_U            },
    {ImGuiKey_V,              VC_V            },
    {ImGuiKey_W,              VC_W            },
    {ImGuiKey_X,              VC_X            },
    {ImGuiKey_Y,              VC_Y            },
    {ImGuiKey_Z,              VC_Z            },

    {ImGuiKey_F1,             VC_F1           },
    {ImGuiKey_F2,             VC_F2           },
    {ImGuiKey_F3,             VC_F3           },
    {ImGuiKey_F4,             VC_F4           },
    {ImGuiKey_F5,             VC_F5           },
    {ImGuiKey_F6,             VC_F6           },
    {ImGuiKey_F7,             VC_F7           },
    {ImGuiKey_F8,             VC_F8           },
    {ImGuiKey_F9,             VC_F9           },
    {ImGuiKey_F10,            VC_F10          },
    {ImGuiKey_F11,            VC_F11          },
    {ImGuiKey_F12,            VC_F12          },
    {ImGuiKey_F13,            VC_F13          },
    {ImGuiKey_F14,            VC_F14          },
    {ImGuiKey_F15,            VC_F15          },
    {ImGuiKey_F16,            VC_F16          },
    {ImGuiKey_F17,            VC_F17          },
    {ImGuiKey_F18,            VC_F18          },
    {ImGuiKey_F19,            VC_F19          },
    {ImGuiKey_F20,            VC_F20          },
    {ImGuiKey_F21,            VC_F21          },
    {ImGuiKey_F22,            VC_F22          },
    {ImGuiKey_F23,            VC_F23          },
    {ImGuiKey_F24,            VC_F24          },

    {ImGuiKey_Apostrophe,     VC_QUOTE        },
    {ImGuiKey_Comma,          VC_COMMA        },
    {ImGuiKey_Minus,          VC_MINUS        },
    {ImGuiKey_Period,         VC_PERIOD       },
    {ImGuiKey_Slash,          VC_SLASH        },
    {ImGuiKey_Semicolon,      VC_SEMICOLON    },
    {ImGuiKey_Equal,          VC_EQUALS       },
    {ImGuiKey_LeftBracket,    VC_OPEN_BRACKET },
    {ImGuiKey_Backslash,      VC_BACK_SLASH   },
    {ImGuiKey_RightBracket,   VC_CLOSE_BRACKET},
    {ImGuiKey_GraveAccent,    VC_BACKQUOTE    },

    {ImGuiKey_CapsLock,       VC_CAPS_LOCK    },
    {ImGuiKey_ScrollLock,     VC_SCROLL_LOCK  },
    {ImGuiKey_NumLock,        VC_NUM_LOCK     },
    {ImGuiKey_PrintScreen,    VC_PRINTSCREEN  },
    {ImGuiKey_Pause,          VC_PAUSE        },

    {ImGuiKey_Keypad0,        VC_KP_0         },
    {ImGuiKey_Keypad1,        VC_KP_1         },
    {ImGuiKey_Keypad2,        VC_KP_2         },
    {ImGuiKey_Keypad3,        VC_KP_3         },
    {ImGuiKey_Keypad4,        VC_KP_4         },
    {ImGuiKey_Keypad5,        VC_KP_5         },
    {ImGuiKey_Keypad6,        VC_KP_6         },
    {ImGuiKey_Keypad7,        VC_KP_7         },
    {ImGuiKey_Keypad8,        VC_KP_8         },
    {ImGuiKey_Keypad9,        VC_KP_9         },
    {ImGuiKey_KeypadDecimal,  VC_KP_SEPARATOR },
    {ImGuiKey_KeypadDivide,   VC_KP_DIVIDE    },
    {ImGuiKey_KeypadMultiply, VC_KP_MULTIPLY  },
    {ImGuiKey_KeypadSubtract, VC_KP_SUBTRACT  },
    {ImGuiKey_KeypadAdd,      VC_KP_ADD       },
    {ImGuiKey_KeypadEnter,    VC_KP_ENTER     },
    {ImGuiKey_KeypadEqual,    VC_KP_EQUALS    },

    {ImGuiKey_LeftAlt,        VC_ALT_L        },
    {ImGuiKey_LeftCtrl,       VC_CONTROL_L    },
    {ImGuiKey_LeftShift,      VC_SHIFT_L      },
    {ImGuiKey_RightAlt,       VC_ALT_R        },
    {ImGuiKey_RightCtrl,      VC_CONTROL_R    },
    {ImGuiKey_RightShift,     VC_SHIFT_R      },
};

std::unordered_map<imp::TargetTag, const char*> uioToStringMap = []() {
	std::unordered_map<imp::TargetTag, const char*> map;
	for (const auto& pair : imguiToUioKeyMap) {
		map[pair.second] = ImGui::GetKeyName(pair.first);
	}
	return map;
}();

namespace gui {

KeycodeView validImGuiKeys() {
	return imguiToUioKeyMap | std::views::keys;
}

imp::TargetTag convertImGuiToUioKey(const ImGuiKey keycode) {
	return imguiToUioKeyMap.at(keycode);
}

const char* getUioKeyName(const UioKey keycode) {
	return uioToStringMap.at(keycode);
}

const ImVec2 PIVOT_CENTER(0.5F, 0.5F);

void centerNextWindow() {
	const ImVec2 viewportSize = ImGui::GetMainViewport()->Size;
	const ImVec2 viewportCenter =
	    ImVec2(viewportSize.x * 0.5F, viewportSize.y * 0.5F);
	ImGui::SetNextWindowPos(viewportCenter, ImGuiCond_Always, PIVOT_CENTER);
}

ImVec4 hexToRgba(std::string_view hex) {
	if (hex.starts_with('#')) {
		hex.remove_prefix(1);
	}

	unsigned hexValue = 0;
	std::from_chars(hex.data(), hex.data() + hex.size(), hexValue, 16);

	const auto r = static_cast<float>((hexValue >> 16) & 0xFF) / 255.0F;
	const auto g = static_cast<float>((hexValue >> 8) & 0xFF) / 255.0F;
	const auto b = static_cast<float>(hexValue & 0xFF) / 255.0F;

	return {r, g, b, 1.0F};
}

ImVec4 rgbaToHsla(const ImVec4& rgba) {
	const float r = rgba.x;
	const float g = rgba.y;
	const float b = rgba.z;

	const float maxValue = std::max({r, g, b});
	const float minValue = std::min({r, g, b});
	const float delta    = maxValue - minValue;

	float       h = 0.0F;
	float       s = 0.0F;
	const float l = (maxValue + minValue) / 2.0F;

	if (delta != 0.0F) {
		s = (l > 0.5F) ? delta / (2.0F - maxValue - minValue)
		               : delta / (maxValue + minValue);

		if (maxValue == r) {
			h = (g - b) / delta + (g < b ? 6.0F : 0.0F);
		}
		else if (maxValue == g) {
			h = (b - r) / delta + 2.0F;
		}
		else {  // maxValue == b
			h = (r - g) / delta + 4.0F;
		}
		h /= 6.0F;
	}

	h = std::fmod(h, 1.0F);
	if (h < 0.0F) {
		h += 1.0F;
	}

	return {h, s, l, 1.0F};
}

static constexpr float hueToRgbComponent(float p, float q, float t) {
	if (t < 0.0F) {
		t += 1.0F;
	}
	if (t > 1.0F) {
		t -= 1.0F;
	}
	if (t < 1.0F / 6.0F) {
		return p + ((q - p) * 6.0F * t);
	}
	if (t < 1.0F / 2.0F) {
		return q;
	}
	if (t < 2.0F / 3.0F) {
		return p + ((q - p) * (2.0F / 3.0F - t) * 6.0F);
	}
	return p;
}

ImVec4 hslaToRgba(const ImVec4& hsla) {
	float h = hsla.x;
	float s = hsla.y;
	float l = hsla.z;

	h = std::fmod(h, 1.0F);
	if (h < 0.0F) {
		h += 1.0F;
	}
	s = std::clamp(s, 0.0F, 1.0F);
	l = std::clamp(l, 0.0F, 1.0F);

	float r = 0.0F;
	float g = 0.0F;
	float b = 0.0F;

	if (s == 0.0F) {
		r = l;
		g = l;
		b = l;
	}
	else {
		const float q = (l < 0.5F) ? l * (1.0F + s) : l + s - (l * s);
		const float p = (2.0F * l) - q;

		r = hueToRgbComponent(p, q, h + (1.0F / 3.0F));
		g = hueToRgbComponent(p, q, h);
		b = hueToRgbComponent(p, q, h - (1.0F / 3.0F));
	}

	r = std::clamp(r, 0.0F, 1.0F);
	g = std::clamp(g, 0.0F, 1.0F);
	b = std::clamp(b, 0.0F, 1.0F);

	return {r, g, b, 1.0F};
}

ImVec4 hueShiftRgba(const ImVec4& rgba, const float shift) {
	ImVec4 hsla = rgbaToHsla(rgba);
	hsla.x += shift;
	return hslaToRgba(hsla);
}

}  // namespace gui
