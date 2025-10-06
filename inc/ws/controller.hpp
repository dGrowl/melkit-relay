#ifndef WS_CONTROLLER_HPP_
#define WS_CONTROLLER_HPP_

#include <string>

#include <SDL3/SDL_stdinc.h>

namespace ws {

enum class Status : Uint8 {
	DISCONNECTED,
	CONNECTING,
	UNAUTHENTICATED,
	AUTHENTICATED,
};

class IController {
public:
	virtual ~IController() = default;

	virtual const char* getUrl()                           = 0;
	virtual Status      getStatus()                        = 0;
	virtual void        sendMessage(std::string&& message) = 0;
	virtual void        setUrl(const char* url)            = 0;
	virtual void        start()                            = 0;
	virtual void        stop()                             = 0;
};

};  // namespace ws

#endif  // WS_CONTROLLER_HPP_
