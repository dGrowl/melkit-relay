#ifndef GUI_IMAGE_HPP_
#define GUI_IMAGE_HPP_

#include <SDL3/SDL_surface.h>

namespace gui {

class Surface {
private:
	SDL_Surface* _surface;

public:
	explicit Surface(const char* path);
	explicit Surface(SDL_Surface* surface);
	~Surface();

	static Surface APP_ICON();

	operator SDL_Surface*() const;  // NOLINT(google-explicit-constructor)

	void shiftHue(float amount);
};

}  // namespace gui

#endif  // GUI_IMAGE_HPP_
