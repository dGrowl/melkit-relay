#include "gui/theme.hpp"

#include <string_view>
#include <unordered_map>

#include <SDL3/SDL_events.h>

#include "imgui/imgui.h"

#include "core/settings.hpp"
#include "core/utility.hpp"
#include "gui/event.hpp"
#include "gui/utility.hpp"

namespace gui {

static const std::unordered_map<ImGuiCol, ImVec4> COLORS{
    {ImGuiCol_FrameBg,             hexToRgba("#4B201F")},
    {ImGuiCol_FrameBgHovered,      hexToRgba("#702628")},
    {ImGuiCol_FrameBgActive,       hexToRgba("#AE3336")},
    {ImGuiCol_TitleBgActive,       hexToRgba("#7A2B29")},
    {ImGuiCol_CheckMark,           hexToRgba("#FA4247")},
    {ImGuiCol_SliderGrab,          hexToRgba("#E03D3F")},
    {ImGuiCol_SliderGrabActive,    hexToRgba("#FA4247")},
    {ImGuiCol_Button,              hexToRgba("#702628")},
    {ImGuiCol_ButtonHovered,       hexToRgba("#FA4247")},
    {ImGuiCol_ButtonActive,        hexToRgba("#FA0F22")},
    {ImGuiCol_Header,              hexToRgba("#5B2224")},
    {ImGuiCol_HeaderHovered,       hexToRgba("#CC393D")},
    {ImGuiCol_HeaderActive,        hexToRgba("#FA4247")},
    {ImGuiCol_SeparatorHovered,    hexToRgba("#99191D")},
    {ImGuiCol_SeparatorActive,     hexToRgba("#BF1A1F")},
    {ImGuiCol_ResizeGrip,          hexToRgba("#421D1E")},
    {ImGuiCol_ResizeGripHovered,   hexToRgba("#AE3336")},
    {ImGuiCol_ResizeGripActive,    hexToRgba("#EE4045")},
    {ImGuiCol_TabHovered,          hexToRgba("#CC393D")},
    {ImGuiCol_Tab,                 hexToRgba("#822A2A")},
    {ImGuiCol_TabSelected,         hexToRgba("#AD3335")},
    {ImGuiCol_TabSelectedOverline, hexToRgba("#FA4247")},
    {ImGuiCol_TabDimmed,           hexToRgba("#261111")},
    {ImGuiCol_TabDimmedSelected,   hexToRgba("#6C2324")},
    {ImGuiCol_TextLink,            hexToRgba("#FA4247")},
    {ImGuiCol_TextSelectedBg,      hexToRgba("#642426")},
    {ImGuiCol_NavCursor,           hexToRgba("#FA4247")},
};

void Theme::setColors(float hueShift) {
	ImGui::StyleColorsDark();

	auto& style = ImGui::GetStyle();

	for (const auto& [key, rgba] : COLORS) {
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index)
		style.Colors[key] = hueShiftRgba(rgba, hueShift);
	}
}

void Theme::initLayout() {
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding.y = 8.0F;
}

Theme& Theme::instance() {
	static Theme instance;
	return instance;
}

void Theme::init() {
	setColors(SETTINGS.getThemeHueShift());
	initLayout();
}

void Theme::setHueShift(float shift) {
	setColors(shift);
	SDL_Event event;
	SDL_zero(event);
	event.type       = Event::THEME_HUE_CHANGE;
	event.user.data1 = core::floatToPointer(shift);
	SDL_PushEvent(&event);
}

}  // namespace gui
