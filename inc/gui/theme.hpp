#ifndef GUI_THEME_HPP_
#define GUI_THEME_HPP_

namespace gui {

class Theme {
private:
	static void initColors();
	static void initLayout();

public:
	static void init();
};

}  // namespace gui

#endif  // GUI_THEME_HPP_
