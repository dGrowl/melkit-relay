#ifndef VTS_RESPONSE_HPP_
#define VTS_RESPONSE_HPP_

#include <SDL3/SDL_events.h>

#include <string>

namespace vts {
enum ResponseCode : Sint32 {
	UNKNOWN,
	AUTHENTICATION_TOKEN,
};

struct AuthenticationTokenResponse {
	std::string token;
};

void buildResponseEvent(SDL_UserEvent& user,
                        char* jsonString,
                        const int nChars);
}  // namespace vts

#endif  // VTS_RESPONSE_HPP_
