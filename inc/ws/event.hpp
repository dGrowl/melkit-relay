#ifndef WS_EVENT_HPP_
#define WS_EVENT_HPP_

#include <SDL3/SDL_events.h>

namespace ws {

enum Event : Uint32 {
	OPEN    = SDL_EVENT_USER + 2,
	MESSAGE = OPEN + 1,
};

void allocateEvents();

}  // namespace ws

#endif  // WS_EVENT_HPP_
