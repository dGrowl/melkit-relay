#include "imgui/imgui.h"

#include "gui/fonts.hpp"

static constexpr auto DEFAULT_FONT_PATH = "./font/NotoSans-Medium.ttf";
static constexpr auto BOLD_FONT_PATH    = "./font/NotoSans-Bold.ttf";

static constexpr float DEFAULT_SIZE = 20.0f;

static ImFont* DEFAULT_FONT = nullptr;
static ImFont* BOLD_FONT    = nullptr;

namespace gui {

FontScope::FontScope(ImFont* font) {
	ImGui::PushFont(font, 0.0f);
}

FontScope::~FontScope() {
	ImGui::PopFont();
}

void Fonts::init() {
	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig config;
	config.GlyphOffset.y = -1.0f;
	config.OversampleH   = 3;

	DEFAULT_FONT =
	    io.Fonts->AddFontFromFileTTF(DEFAULT_FONT_PATH, DEFAULT_SIZE, &config);
	if (DEFAULT_FONT == nullptr) {
		DEFAULT_FONT = io.FontDefault;
	}

	BOLD_FONT =
	    io.Fonts->AddFontFromFileTTF(BOLD_FONT_PATH, DEFAULT_SIZE, &config);
	if (BOLD_FONT == nullptr) {
		BOLD_FONT = io.FontDefault;
	}
}

FontScope Fonts::scope(const FontType type) {
	switch (type) {
		case FontType::DEFAULT:
			return FontScope(DEFAULT_FONT);
		case FontType::BOLD:
			return FontScope(BOLD_FONT);
	}
	return FontScope(DEFAULT_FONT);
}

}  // namespace gui
