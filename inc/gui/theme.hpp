#ifndef GUI_THEME_HPP_
#define GUI_THEME_HPP_

namespace gui {

class Theme {
private:
	static void setColors(float hueShift = 0.0F);
	static void initLayout();

public:
	static Theme& instance();

	static void init();
	static void setHueShift(float shift);
};

}  // namespace gui

#define THEME (gui::Theme::instance())

#endif  // GUI_THEME_HPP_
