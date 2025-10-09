#ifndef GUI_THEME_COLOR_MODAL_HPP_
#define GUI_THEME_COLOR_MODAL_HPP_

#include "imgui/imgui.h"

namespace gui {

class ThemeColorModal {
private:
	float _shift;

public:
	ThemeColorModal();
	void show();

	constexpr static const char* NAME = "Theme Color";
};

}  // namespace gui

#endif  // GUI_THEME_COLOR_MODAL_HPP_
