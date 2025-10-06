#ifndef WS_CLIENT_HPP_
#define WS_CLIENT_HPP_

#include <atomic>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

#include <mongoose.h>

#include "ws/controller.hpp"

namespace ws {

class Client : public IController {
private:
	std::atomic<bool>   _alive;
	std::atomic<Status> _status;
	std::string         _url;
	std::thread         _thread;

	std::mutex              _sendMutex;
	std::queue<std::string> _sendQueue;

public:
	Client();
	static void handleEvent(mg_connection* connection, int event, void* eventData);
	static void handleMessage(mg_ws_message* message);
	void        handleError(const char* description);
	void        handleOpen();
	void        setStatus(Status newStatus);
	void        threadFn();

	// IController
	const char* getUrl() override;
	Status      getStatus() override;
	void        sendMessage(std::string&& message) override;
	void        setUrl(const char* url) override;
	void        start() override;
	void        stop() override;
};

}  // namespace ws

#endif  // WS_CLIENT_HPP_
