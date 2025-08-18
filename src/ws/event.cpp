#include "ws/event.hpp"

namespace ws {
static const size_t N_EVENTS = 2;

void allocateEvents() {
	SDL_RegisterEvents(N_EVENTS);
}
}  // namespace ws
