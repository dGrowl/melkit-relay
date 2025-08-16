#include <rapidjson/document.h>

#include "vts/response.hpp"

namespace rj = rapidjson;

namespace vts {
static const auto AUTHENTICATION_TOKEN_TYPESTRING =
    rj::Value("AuthenticationTokenResponse");

void buildResponseEvent(SDL_UserEvent& user,
                        char* jsonString,
                        const int nChars) {
	rj::Document json;
	json.Parse(jsonString, nChars);
	user.code = ResponseCode::UNKNOWN;

	const auto& type = json["messageType"];
	if (type == AUTHENTICATION_TOKEN_TYPESTRING) {
		const auto& data = json["data"];
		const auto& token = data["authenticationToken"];
		user.code = ResponseCode::AUTHENTICATION_TOKEN;
		user.data1 = new AuthenticationTokenResponse();
		auto* authTokenResponse =
		    static_cast<AuthenticationTokenResponse*>(user.data1);
		authTokenResponse->token = token.GetString();
	}
}
}  // namespace vts
