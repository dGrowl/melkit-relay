#include "mnk/event.hpp"

namespace mnk {

void allocateEvents() {
	SDL_RegisterEvents(N_EVENTS);
}

}  // namespace mnk
