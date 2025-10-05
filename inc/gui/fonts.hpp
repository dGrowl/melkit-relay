#ifndef GUI_FONTS_HPP_
#define GUI_FONTS_HPP_

#include <cstdint>

#include "imgui/imgui.h"

namespace gui {

enum class FontType : std::uint8_t {
	DEFAULT,
	BOLD,
	IMPULSE,
};

class FontScope {
private:
	bool _pushed;

public:
	FontScope(ImFont* font, float size);
	~FontScope();
};

class Fonts {
public:
	static FontScope scope(FontType type);
	static void      init();
	static void      push(FontType type);
};

}  // namespace gui

#define FONT_SCOPE(type) \
	[[maybe_unused]] auto _font_scope_##__LINE__ = gui::Fonts::scope(type)

#endif  // GUI_FONTS_HPP_
