#include "gui/theme.hpp"

#include <charconv>
#include <string_view>

#include "imgui/imgui.h"

ImVec4 hexToImVec4(std::string_view hex) {
	if (hex.starts_with('#')) {
		hex.remove_prefix(1);
	}

	unsigned hexValue;
	std::from_chars(hex.data(), hex.data() + hex.size(), hexValue, 16);

	const float r = ((hexValue >> 16) & 0xFF) / 255.0F;
	const float g = ((hexValue >> 8) & 0xFF) / 255.0F;
	const float b = (hexValue & 0xFF) / 255.0F;

	return ImVec4(r, g, b, 1.0F);
}

namespace gui {

void Theme::initColors() {
	ImGui::StyleColorsDark();

	auto& style = ImGui::GetStyle();

	auto& colors = style.Colors;

	colors[ImGuiCol_FrameBg]             = hexToImVec4("#4B201F");
	colors[ImGuiCol_FrameBgHovered]      = hexToImVec4("#702628");
	colors[ImGuiCol_FrameBgActive]       = hexToImVec4("#AE3336");
	colors[ImGuiCol_TitleBgActive]       = hexToImVec4("#7A2B29");
	colors[ImGuiCol_CheckMark]           = hexToImVec4("#FA4247");
	colors[ImGuiCol_SliderGrab]          = hexToImVec4("#E03D3F");
	colors[ImGuiCol_SliderGrabActive]    = hexToImVec4("#FA4247");
	colors[ImGuiCol_Button]              = hexToImVec4("#702628");
	colors[ImGuiCol_ButtonHovered]       = hexToImVec4("#FA4247");
	colors[ImGuiCol_ButtonActive]        = hexToImVec4("#FA0F22");
	colors[ImGuiCol_Header]              = hexToImVec4("#5B2224");
	colors[ImGuiCol_HeaderHovered]       = hexToImVec4("#CC393D");
	colors[ImGuiCol_HeaderActive]        = hexToImVec4("#FA4247");
	colors[ImGuiCol_SeparatorHovered]    = hexToImVec4("#99191D");
	colors[ImGuiCol_SeparatorActive]     = hexToImVec4("#BF1A1F");
	colors[ImGuiCol_ResizeGrip]          = hexToImVec4("#421D1E");
	colors[ImGuiCol_ResizeGripHovered]   = hexToImVec4("#AE3336");
	colors[ImGuiCol_ResizeGripActive]    = hexToImVec4("#EE4045");
	colors[ImGuiCol_TabHovered]          = hexToImVec4("#CC393D");
	colors[ImGuiCol_Tab]                 = hexToImVec4("#822A2A");
	colors[ImGuiCol_TabSelected]         = hexToImVec4("#AD3335");
	colors[ImGuiCol_TabSelectedOverline] = hexToImVec4("#FA4247");
	colors[ImGuiCol_TabDimmed]           = hexToImVec4("#261111");
	colors[ImGuiCol_TabDimmedSelected]   = hexToImVec4("#6C2324");
	colors[ImGuiCol_TextLink]            = hexToImVec4("#FA4247");
	colors[ImGuiCol_TextSelectedBg]      = hexToImVec4("#642426");
	colors[ImGuiCol_NavCursor]           = hexToImVec4("#FA4247");
}

void Theme::initLayout() {
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding.y = 8.0F;
}

void Theme::init() {
	initColors();
	initLayout();
}

}  // namespace gui
