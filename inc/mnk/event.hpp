#ifndef MNK_EVENT_HPP_
#define MNK_EVENT_HPP_

#include <SDL3/SDL_events.h>

namespace mnk {

constexpr int N_EVENTS = 2;

enum Event : Uint32 {
	START = SDL_EVENT_USER,
	INPUT = START + 1
};

void allocateEvents();

}  // namespace mnk

#endif  // MNK_EVENT_HPP_
