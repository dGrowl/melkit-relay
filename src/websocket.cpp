#include <atomic>

#include <SDL3/SDL_log.h>
#include <mongoose.h>

#include "websocket.hpp"

const char* url = "ws://localhost:8080";

static std::atomic<bool> running(true);

void fn(mg_connection* c, int event, void* event_data) {
	if (event == MG_EV_OPEN) {
		c->is_hexdumping = 1;
	} else if (event == MG_EV_ERROR) {
		SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%p %s", c->fd, (char*)event_data);
	} else if (event == MG_EV_WS_OPEN) {
		mg_ws_send(c, "Greeting", 5, WEBSOCKET_OP_TEXT);
	} else if (event == MG_EV_WS_MSG) {
		mg_ws_message* wm = (mg_ws_message*)event_data;
		SDL_Log("Received message: [%.*s]\n", (int)wm->data.len, wm->data.buf);
	}

	if (event == MG_EV_ERROR || event == MG_EV_CLOSE) {
		running = false;
	}
}

int wsThreadFn(void*) {
	mg_mgr manager;
	mg_mgr_init(&manager);
	mg_log_set(MG_LL_DEBUG);
	mg_connection* connection = mg_ws_connect(&manager, url, fn, nullptr, nullptr);
	while (connection != nullptr && running) {
		mg_mgr_poll(&manager, 1000);
	}
	mg_mgr_free(&manager);
	return 0;
}

void stopWsThread() {
	running = false;
}
