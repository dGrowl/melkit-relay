#ifndef GUI_IMAGE_HPP_
#define GUI_IMAGE_HPP_

#include <SDL3/SDL_surface.h>

namespace gui {

class Surface {
private:
	SDL_Surface* _surface;

public:
	explicit Surface(const char* path);
	~Surface();

	operator SDL_Surface*() const;  // NOLINT(google-explicit-constructor)
};

constexpr auto ICON_PATH = "./img/icon.png";

}  // namespace gui

#endif  // GUI_IMAGE_HPP_
