#ifndef MNK_MONITOR_HPP_
#define MNK_MONITOR_HPP_

#include <SDL3/SDL_events.h>
#include <thread>

#include "libuiohook/uiohook.h"

namespace mnk {

class Monitor {
private:
	std::thread _thread;

	static void buildKeyDown(SDL_UserEvent&       userEvent,
	                         uiohook_event* const hookEvent);
	static void buildKeyUp(SDL_UserEvent&       userEvent,
	                       uiohook_event* const hookEvent);
	static void buildMouseMove(SDL_UserEvent&       userEvent,
	                           uiohook_event* const hookEvent);
	static void buildMouseClick(SDL_UserEvent&       userEvent,
	                            uiohook_event* const hookEvent);
	static void buildMouseRelease(SDL_UserEvent&       userEvent,
	                              uiohook_event* const hookEvent);
	static void buildMouseWheel(SDL_UserEvent&       userEvent,
	                            uiohook_event* const hookEvent);

public:
	Monitor();
	~Monitor();
	static void handleEvent(uiohook_event* const event);
	void        stop();
	void        threadFn();
};

}  // namespace mnk

#endif  // MNK_MONITOR_HPP_
