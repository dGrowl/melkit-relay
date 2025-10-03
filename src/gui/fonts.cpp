#include "gui/fonts.hpp"

#include "imgui/imgui.h"

static constexpr auto DEFAULT_FONT_PATH = "./font/NotoSans-Medium.ttf";
static constexpr auto BOLD_FONT_PATH    = "./font/NotoSans-Bold.ttf";
static constexpr auto IMPULSE_FONT_PATH = "./font/promptfont.ttf";

static constexpr float DEFAULT_SIZE = 20.0f;
static constexpr float ICON_SIZE    = 32.0f;

static ImFont* DEFAULT_FONT = nullptr;
static ImFont* BOLD_FONT    = nullptr;
static ImFont* IMPULSE_FONT = nullptr;

namespace gui {

ImFont* loadTtf(const char* path, const ImFontConfig* config) {
	ImGuiIO& io   = ImGui::GetIO();
	ImFont*  font = io.Fonts->AddFontFromFileTTF(path, DEFAULT_SIZE, config);
	return font == nullptr ? io.FontDefault : font;
}

FontScope::FontScope(ImFont* font, const float size) {
	ImGui::PushFont(font, size);
}

FontScope::~FontScope() {
	ImGui::PopFont();
}

FontScope Fonts::scope(const FontType type) {
	switch (type) {
		case FontType::DEFAULT:
			return FontScope(DEFAULT_FONT, DEFAULT_SIZE);
		case FontType::BOLD:
			return FontScope(BOLD_FONT, DEFAULT_SIZE);
		case FontType::IMPULSE:
			return FontScope(IMPULSE_FONT, ICON_SIZE);
	}
	return FontScope(DEFAULT_FONT, DEFAULT_SIZE);
}

void Fonts::init() {
	ImFontConfig config;
	config.GlyphOffset.y = -1.0f;
	config.OversampleH   = 3;

	DEFAULT_FONT = loadTtf(DEFAULT_FONT_PATH, &config);
	BOLD_FONT    = loadTtf(BOLD_FONT_PATH, &config);

	ImFontConfig iconConfig;
	iconConfig.GlyphOffset.y = -4.0f;

	IMPULSE_FONT = loadTtf(IMPULSE_FONT_PATH, &iconConfig);
}

void Fonts::push(const FontType type) {
	switch (type) {
		case FontType::DEFAULT:
			ImGui::PushFont(DEFAULT_FONT, DEFAULT_SIZE);
			return;
		case FontType::BOLD:
			ImGui::PushFont(BOLD_FONT, DEFAULT_SIZE);
			return;
			return;
		case FontType::IMPULSE:
			ImGui::PushFont(IMPULSE_FONT, ICON_SIZE);
			return;
	}
	ImGuiIO& io = ImGui::GetIO();
	ImGui::PushFont(io.FontDefault, 0.0f);
}

}  // namespace gui
