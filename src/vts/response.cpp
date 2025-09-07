#include <vector>

#include <SDL3/SDL_log.h>
#include <rapidjson/document.h>

#include "vts/meta.hpp"
#include "vts/parameter.hpp"
#include "vts/response.hpp"

namespace rj = rapidjson;

namespace vts {

static const auto AUTHENTICATION_TOKEN_TYPESTRING =
    rj::Value("AuthenticationTokenResponse");

static const auto AUTHENTICATION_TYPESTRING =
    rj::Value("AuthenticationResponse");

static const auto INPUT_PARAMETER_LIST_TYPESTRING =
    rj::Value("InputParameterListResponse");

static const auto PARAMETER_CREATION_TYPESTRING =
    rj::Value("ParameterCreationResponse");

static const auto PARAMETER_DELETION_TYPESTRING =
    rj::Value("ParameterDeletionResponse");

static const auto PLUGIN_NAME_VALUE = rj::Value(rj::StringRef(PLUGIN_NAME));

void buildAuthenticationTokenEvent(SDL_UserEvent&      event,
                                   const rj::Document& json) {
	const auto& data         = json["data"];
	const auto& token        = data["authenticationToken"];
	event.code               = ResponseCode::AUTHENTICATION_TOKEN;
	event.data1              = new AuthenticationTokenData();
	auto* authTokenResponse  = static_cast<AuthenticationTokenData*>(event.data1);
	authTokenResponse->token = token.GetString();
}

void buildAuthenticationEvent(SDL_UserEvent& event, const rj::Document& json) {
	const auto& data    = json["data"];
	const auto& success = data["authenticated"];
	event.code          = success.GetBool() ? ResponseCode::AUTHENTICATION_SUCCESS
	                                        : ResponseCode::AUTHENTICATION_FAILURE;
}

std::vector<ParameterData> extractParameters(const rapidjson::Value& array) {
	std::vector<ParameterData> params;

	if (!array.IsArray()) {
		return params;
	}

	for (const auto& item : array.GetArray()) {
		if (!item.IsObject()) {
			continue;
		}

		auto addedByItr = item.FindMember("addedBy");

		if ((addedByItr == item.MemberEnd())
		    || (addedByItr->value != PLUGIN_NAME_VALUE)) {
			continue;
		}
		ParameterData param;
		if (item.HasMember("name") && item["name"].IsString()) {
			param.name = item["name"].GetString();
		}
		if (item.HasMember("defaultValue") && item["defaultValue"].IsFloat()) {
			param.defaultValue = item["defaultValue"].GetFloat();
		}
		if (item.HasMember("max") && item["max"].IsFloat()) {
			param.max = item["max"].GetFloat();
		}
		if (item.HasMember("min") && item["min"].IsFloat()) {
			param.min = item["min"].GetFloat();
		}

		params.emplace_back(std::move(param));
	}

	return params;
}

void buildInputParameterListEvent(SDL_UserEvent&      event,
                                  const rj::Document& json) {
	const auto& data             = json["data"];
	const auto& customParameters = data["customParameters"];
	event.code                   = ResponseCode::INPUT_PARAMETER_LIST;
	auto* responseData           = new InputParameterListData();
	responseData->parameters     = extractParameters(customParameters);
	event.data1                  = static_cast<void*>(responseData);
}

void buildParameterCreationEvent(SDL_UserEvent&      event,
                                 const rj::Document& json) {
	const auto& data = json["data"];
	if (!data.HasMember("parameterName") || !data["parameterName"].IsString()) {
		return;
	}
	event.code = ResponseCode::PARAMETER_CREATION;
}

void buildParameterDeletionEvent(SDL_UserEvent&      event,
                                 const rj::Document& json) {
	const auto& data = json["data"];
	if (!data.HasMember("parameterName") || !data["parameterName"].IsString()) {
		return;
	}
	event.code = ResponseCode::PARAMETER_DELETION;
}

void buildResponseEvent(SDL_UserEvent& event,
                        char*          jsonString,
                        const int      nChars) {
	rj::Document json;
	json.Parse(jsonString, nChars);

	event.code = ResponseCode::UNKNOWN;

	const auto& type = json["messageType"];
	if (type == AUTHENTICATION_TOKEN_TYPESTRING) {
		buildAuthenticationTokenEvent(event, json);
	}
	else if (type == AUTHENTICATION_TYPESTRING) {
		buildAuthenticationEvent(event, json);
	}
	else if (type == INPUT_PARAMETER_LIST_TYPESTRING) {
		buildInputParameterListEvent(event, json);
	}
	else if (type == PARAMETER_CREATION_TYPESTRING) {
		buildParameterCreationEvent(event, json);
	}
	else if (type == PARAMETER_DELETION_TYPESTRING) {
		buildParameterDeletionEvent(event, json);
	}
}

}  // namespace vts
