#ifndef VTS_RESPONSE_HPP_
#define VTS_RESPONSE_HPP_

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_stdinc.h>

namespace vts {

enum ResponseCode : Sint32 {
	UNKNOWN,
	API_ERROR,
	AUTHENTICATION_TOKEN,
	AUTHENTICATION_SUCCESS,
	AUTHENTICATION_FAILURE,
	INPUT_PARAMETER_LIST,
	PARAMETER_CREATION,
	PARAMETER_DELETION,
};

void buildResponseEvent(SDL_UserEvent& user, char* jsonChars, int nChars);

}  // namespace vts

#endif  // VTS_RESPONSE_HPP_
