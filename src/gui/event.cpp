#include "gui/event.hpp"

#include <cstddef>

#include <SDL3/SDL_events.h>

namespace gui {

static const size_t N_EVENTS = 1;

void allocateEvents() {
	SDL_RegisterEvents(N_EVENTS);
}

}  // namespace gui
