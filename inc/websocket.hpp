#ifndef WEBSOCKET_HPP_
#define WEBSOCKET_HPP_

#include <string>

#include <SDL3/SDL_events.h>

enum : Uint32 {
	WS_EVENT_OPEN = SDL_EVENT_USER + 5,
	WS_EVENT_CLOSE = WS_EVENT_OPEN + 1,
	WS_EVENT_ERROR = WS_EVENT_OPEN + 2,
	WS_EVENT_MESSAGE = WS_EVENT_OPEN + 3,
};

void allocateWsEvents();

int wsThreadFn(void*);
void stopWsThread();
void wsSend(const std::string& message);

void handleOpen();
void handleMessage(void* event_data);

#endif  // WEBSOCKET_HPP_
