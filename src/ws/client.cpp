#include "ws/client.hpp"

#include <format>
#include <mutex>
#include <thread>
#include <utility>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_stdinc.h>
#include <mongoose.h>

#include "core/settings.hpp"
#include "vts/response.hpp"
#include "ws/controller.hpp"
#include "ws/event.hpp"

#ifdef NDEBUG
#define IS_DEBUG_BUILD false
#else
#define IS_DEBUG_BUILD true
#endif

namespace ws {
void handleMessage(void* event_data) {
	mg_ws_message* wm = (mg_ws_message*)event_data;
	SDL_Event      sdlEvent;
	SDL_zero(sdlEvent);
	sdlEvent.type = Event::MESSAGE;
	vts::buildResponseEvent(sdlEvent.user, wm->data.buf, wm->data.len);
	SDL_PushEvent(&sdlEvent);
}

Client::Client() :
    _alive(false),
    _status(Status::DISCONNECTED),
    _url(SETTINGS.getWsUrl()),
    _thread(),
    _sendMutex(),
    _sendQueue() {
	mg_log_set(MG_LL_DEBUG);
}

void Client::handleEvent(mg_connection* connection,
                         int            event,
                         void*          eventData) {
	Client* client = static_cast<Client*>(connection->fn_data);
	switch (event) {
		case MG_EV_OPEN:
			if (IS_DEBUG_BUILD) {
				connection->is_hexdumping = 1;
			}
			break;
		case MG_EV_ERROR:
			client->handleError((char*)eventData);
			break;
		case MG_EV_WS_OPEN:
			client->handleOpen();
			break;
		case MG_EV_WS_MSG:
			handleMessage(eventData);
			break;
	}

	if (event == MG_EV_ERROR || event == MG_EV_CLOSE) {
		client->stop();
	}
}

void Client::handleError(const char* description) {
	SDL_Log("WebSocket Error: %s\n", description);
}

void Client::handleOpen() {
	setStatus(Status::CONNECTED);

	SDL_Event sdlEvent;
	SDL_zero(sdlEvent);
	sdlEvent.type = Event::OPEN;
	SDL_PushEvent(&sdlEvent);
}

void Client::setStatus(const Status newStatus) {
	_status = newStatus;
}

void Client::threadFn() {
	setStatus(Status::CONNECTING);
	mg_mgr manager;
	mg_mgr_init(&manager);
	auto           extraHeaders = std::format("Host: {}\r\n", _url);
	mg_connection* connection   = mg_ws_connect(&manager,
                                           _url.c_str(),
                                           handleEvent,
                                           this,
                                           extraHeaders.c_str());
	while (_alive) {
		mg_mgr_poll(&manager, 64);
		{
			const std::lock_guard<std::mutex> lock(_sendMutex);
			while (_alive && !_sendQueue.empty() && connection != nullptr) {
				mg_ws_send(connection,
				           _sendQueue.front().c_str(),
				           _sendQueue.front().size(),
				           WEBSOCKET_OP_TEXT);
				_sendQueue.pop();
			}
		}
	}
	mg_mgr_free(&manager);
	setStatus(Status::DISCONNECTED);
}

const char* Client::getUrl() {
	return _url.c_str();
}

Status Client::getStatus() {
	return _status;
}

void Client::sendMessage(std::string&& message) {
	const std::lock_guard<std::mutex> lock(_sendMutex);

	_sendQueue.push(std::move(message));
}

void Client::setUrl(const char* url) {
	_url = url;
	SETTINGS.setWsUrl(url);
}

void Client::start() {
	_alive = false;
	if (_thread.joinable()) {
		_thread.join();
	}
	_alive  = true;
	_thread = std::thread(&Client::threadFn, this);
}

void Client::stop() {
	_alive = false;
	if (_thread.joinable()) {
		if (std::this_thread::get_id() == _thread.get_id()) {
			return;
		}
		_thread.join();
	}
}

}  // namespace ws
