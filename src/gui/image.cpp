#include "gui/image.hpp"

#include <SDL3/SDL_surface.h>
#include <SDL3_image/SDL_image.h>

namespace gui {

Surface::Surface(const char* path) {
	_surface = IMG_Load(path);
}

Surface::~Surface() {
	SDL_DestroySurface(_surface);
}

Surface::operator SDL_Surface*() const {
	return _surface;
}

}  // namespace gui
