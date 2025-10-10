#ifndef GUI_EVENT_HPP_
#define GUI_EVENT_HPP_

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_stdinc.h>

namespace gui {

enum Event : Uint32 {
	THEME_HUE_CHANGE = SDL_EVENT_USER + 4,
};

void allocateEvents();

}  // namespace gui

#endif  // GUI_EVENT_HPP_
