#ifndef VTS_RESPONSE_HPP_
#define VTS_RESPONSE_HPP_

#include <SDL3/SDL_events.h>

#include <string>
#include <vector>

#include "vts/parameter.hpp"

namespace vts {

enum ResponseCode : Sint32 {
	UNKNOWN,
	AUTHENTICATION_TOKEN,
	AUTHENTICATION_SUCCESS,
	AUTHENTICATION_FAILURE,
	INPUT_PARAMETER_LIST
};

struct AuthenticationTokenData {
	std::string token;
};

struct InputParameterListData {
	std::vector<ParameterData> parameters;
};

void buildResponseEvent(SDL_UserEvent& user,
                        char* jsonString,
                        const int nChars);

}  // namespace vts

#endif  // VTS_RESPONSE_HPP_
