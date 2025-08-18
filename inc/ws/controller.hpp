#ifndef WS_CONTROLLER_HPP_
#define WS_CONTROLLER_HPP_

namespace ws {

enum Status : Uint8 {
	DISCONNECTED,
	CONNECTING,
	FAILED,
	CONNECTED,
};

class IController {
public:
	virtual ~IController() = default;

	virtual const char* getUrl() = 0;
	virtual Status getStatus() = 0;
	virtual void setUrl(const char* url) = 0;
	virtual void start() = 0;
	virtual void stop() = 0;
};

};  // namespace ws

#endif  // WS_CONTROLLER_HPP_
