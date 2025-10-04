#include "ws/event.hpp"

#include <cstddef>

#include <SDL3/SDL_events.h>

namespace ws {
static const size_t N_EVENTS = 2;

void allocateEvents() {
	SDL_RegisterEvents(N_EVENTS);
}
}  // namespace ws
