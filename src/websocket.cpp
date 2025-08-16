#include <atomic>
#include <mutex>
#include <queue>
#include <string>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
#include <mongoose.h>
#include <rapidjson/document.h>

#include "vts/response.hpp"
#include "websocket.hpp"

namespace rj = rapidjson;

const char* host = "localhost:8001";
const char* url = "ws://localhost:8001";

static const int N_WS_EVENTS = 4;

static std::atomic<bool> running(true);
static std::queue<std::string> sendQueue;
static std::mutex sendMutex;

void allocateWsEvents() {
	SDL_RegisterEvents(N_WS_EVENTS);
}

void handleEvent(mg_connection* c, int event, void* event_data) {
	switch (event) {
		case MG_EV_OPEN:
			c->is_hexdumping = 1;
			break;
		case MG_EV_ERROR:
			SDL_Log("%p %s", c->fd, (char*)event_data);
			break;
		case MG_EV_WS_OPEN:
			handleOpen();
			break;
		case MG_EV_WS_MSG:
			handleMessage(event_data);
			break;
	}

	if (event == MG_EV_ERROR || event == MG_EV_CLOSE) {
		running = false;
	}
}

int wsThreadFn(void*) {
	mg_mgr manager;
	mg_mgr_init(&manager);
	mg_log_set(MG_LL_DEBUG);
	const char* extra_headers = "Host: localhost:8001\r\n";
	mg_connection* connection =
	    mg_ws_connect(&manager, url, handleEvent, nullptr, extra_headers);

	if (connection == nullptr) {
		mg_mgr_free(&manager);
		return 1;
	}
	while (running) {
		mg_mgr_poll(&manager, 64);
		{
			std::lock_guard<std::mutex> lock(sendMutex);
			while (!sendQueue.empty() && connection != nullptr) {
				SDL_Log("Sending %s", sendQueue.front().c_str());
				mg_ws_send(connection,
				           sendQueue.front().c_str(),
				           sendQueue.front().size(),
				           WEBSOCKET_OP_TEXT);
				sendQueue.pop();
			}
		}
	}
	mg_mgr_free(&manager);
	return 0;
}

void stopWsThread() {
	running = false;
}

void wsSend(const std::string& message) {
	std::lock_guard<std::mutex> lock(sendMutex);
	sendQueue.push(message);
}

void handleOpen() {
	SDL_Event sdlEvent;
	SDL_zero(sdlEvent);
	sdlEvent.type = WS_EVENT_OPEN;
	SDL_PushEvent(&sdlEvent);
}

void handleMessage(void* event_data) {
	mg_ws_message* wm = (mg_ws_message*)event_data;
	SDL_Log("Received message: [%.*s]\n", (int)wm->data.len, wm->data.buf);
	SDL_Event sdlEvent;
	SDL_zero(sdlEvent);
	sdlEvent.type = WS_EVENT_MESSAGE;
	vts::buildResponseEvent(sdlEvent.user, wm->data.buf, wm->data.len);
	SDL_PushEvent(&sdlEvent);
}
