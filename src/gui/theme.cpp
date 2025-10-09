#include "gui/theme.hpp"

#include <algorithm>
#include <charconv>
#include <cmath>
#include <string_view>
#include <unordered_map>

#include "imgui/imgui.h"

#include "core/settings.hpp"

static constexpr ImVec4 hexToRgba(std::string_view hex) {
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

static constexpr ImVec4 rgbaToHsla(const ImVec4& rgba) {
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

static constexpr ImVec4 hslaToRgba(const ImVec4& hsla) {
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

static constexpr ImVec4 hueShiftRgba(const ImVec4& rgba, const float shift) {
	ImVec4 hsla = rgbaToHsla(rgba);
	hsla.x += shift;
	return hslaToRgba(hsla);
}

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
}

}  // namespace gui
