#include "mnk/event.hpp"

#include <SDL3/SDL_events.h>
namespace mnk {

void allocateEvents() {
	SDL_RegisterEvents(N_EVENTS);
}

}  // namespace mnk
