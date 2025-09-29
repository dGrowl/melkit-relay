#include "vts/response.hpp"

#include <iostream>
#include <glaze/core/context.hpp>
#include <glaze/core/reflect.hpp>
#include <glaze/json/read.hpp>
#include <optional>
#include <vector>

#include "core/meta.hpp"

static void logError(const glz::error_ctx& error, const std::string& buffer) {
	std::cerr
	    << "Error reading response: "
	    << glz::format_error(error, buffer)
	    << std::endl;
}

struct Type {
	static constexpr auto AUTHENTICATION_TOKEN = "AuthenticationTokenResponse";

	static constexpr auto AUTHENTICATION = "AuthenticationResponse";

	static constexpr auto INPUT_PARAMETER_LIST = "InputParameterListResponse";

	static constexpr auto PARAMETER_CREATION = "ParameterCreationResponse";

	static constexpr auto PARAMETER_DELETION = "ParameterDeletionResponse";
};

namespace vts {

struct BaseResponse {
	std::string   apiName;
	std::string   apiVersion;
	std::string   requestId;
	std::string   messageType;
	std::uint64_t timestamp = 0;

	struct glaze {
		using T                     = BaseResponse;
		static constexpr auto value = glz::object("apiName",
		                                          &T::apiName,
		                                          "apiVersion",
		                                          &T::apiVersion,
		                                          "requestID",
		                                          &T::requestId,
		                                          "messageType",
		                                          &T::messageType,
		                                          "timestamp",
		                                          &T::timestamp);
	};
};

template <typename DataType>
struct Response : BaseResponse {
	std::optional<DataType> data;

	struct glaze {
		using T                     = Response<DataType>;
		static constexpr auto value = glz::object("apiName",
		                                          &T::apiName,
		                                          "apiVersion",
		                                          &T::apiVersion,
		                                          "requestID",
		                                          &T::requestId,
		                                          "messageType",
		                                          &T::messageType,
		                                          "timestamp",
		                                          &T::timestamp,
		                                          "data",
		                                          &T::data);
	};
};

template <typename ResponseType>
std::optional<ResponseType> parseResponse(const std::string& jsonString) {
	auto result = glz::read_json<ResponseType>(jsonString);
	if (result) {
		return result.value();
	}
	logError(result.error(), jsonString);
	return std::nullopt;
}

struct AuthenticationResponseData {
	bool        authenticated = false;
	std::string reason;

	struct glaze {
		using T = AuthenticationResponseData;
		static constexpr auto value =
		    glz::object("authenticated", &T::authenticated, "reason", &T::reason);
	};
};

using AuthenticationResponse = Response<AuthenticationResponseData>;

void buildAuthenticationEvent(SDL_UserEvent&                    event,
                              const AuthenticationResponseData& data) {
	event.code = data.authenticated ? ResponseCode::AUTHENTICATION_SUCCESS
	                                : ResponseCode::AUTHENTICATION_FAILURE;
}

struct AuthenticationTokenResponseData {
	std::string authenticationToken;

	struct glaze {
		using T = AuthenticationTokenResponseData;
		static constexpr auto value =
		    glz::object("authenticationToken", &T::authenticationToken);
	};
};

using AuthenticationTokenResponse = Response<AuthenticationTokenResponseData>;

void buildAuthenticationTokenEvent(
    SDL_UserEvent&                         event,
    const AuthenticationTokenResponseData& data) {
	event.code  = ResponseCode::AUTHENTICATION_TOKEN;
	event.data1 = new std::string(data.authenticationToken);
}

struct IncomingParameter {
	std::string name;
	std::string addedBy;
	float       value        = 0.0;
	float       min          = 0.0;
	float       max          = 0.0;
	float       defaultValue = 0.0;

	struct glaze {
		using T                     = IncomingParameter;
		static constexpr auto value = glz::object("name",
		                                          &T::name,
		                                          "addedBy",
		                                          &T::addedBy,
		                                          "value",
		                                          &T::value,
		                                          "min",
		                                          &T::min,
		                                          "max",
		                                          &T::max,
		                                          "defaultValue",
		                                          &T::defaultValue);
	};
};

struct InputParameterListResponseData {
	bool                           modelLoaded;
	std::string                    modelName;
	std::string                    modelId;
	std::vector<IncomingParameter> defaultParameters;
	std::vector<IncomingParameter> customParameters;

	struct glaze {
		using T                     = InputParameterListResponseData;
		static constexpr auto value = glz::object("modelLoaded",
		                                          &T::modelLoaded,
		                                          "modelName",
		                                          &T::modelName,
		                                          "modelID",
		                                          &T::modelId,
		                                          "defaultParameters",
		                                          &T::defaultParameters,
		                                          "customParameters",
		                                          &T::customParameters);
	};
};

using InputParameterListResponse = Response<InputParameterListResponseData>;

void buildInputParameterListEvent(SDL_UserEvent&                        event,
                                  const InputParameterListResponseData& data) {
	auto* parameters = new std::vector<std::string>();
	for (const auto& parameter : data.customParameters) {
		if (parameter.addedBy == core::PLUGIN_NAME) {
			parameters->emplace_back(std::move(parameter.name));
		}
	}
	event.code  = ResponseCode::INPUT_PARAMETER_LIST;
	event.data1 = parameters;
}

struct ParameterCreationResponseData {
	std::string parameterName;

	struct glaze {
		using T                     = ParameterCreationResponseData;
		static constexpr auto value = glz::object("parameterName", &T::parameterName);
	};
};

using ParameterCreationResponse = Response<ParameterCreationResponseData>;

void buildParameterCreationEvent(SDL_UserEvent& event,
                                 const ParameterCreationResponseData&) {
	event.code = ResponseCode::PARAMETER_CREATION;
}

struct ParameterDeletionResponseData {
	std::string parameterName;

	struct glaze {
		using T                     = ParameterDeletionResponseData;
		static constexpr auto value = glz::object("parameterName", &T::parameterName);
	};
};

using ParameterDeletionResponse = Response<ParameterDeletionResponseData>;

void buildParameterDeletionEvent(SDL_UserEvent& event,
                                 const ParameterDeletionResponseData&) {
	event.code = ResponseCode::PARAMETER_DELETION;
}

void buildResponseEvent(SDL_UserEvent& event,
                        char*          jsonChars,
                        const int      nChars) {
	std::string jsonString(jsonChars, nChars);

	BaseResponse base;
	auto         baseError =
	    glz::read<glz::opts{.error_on_unknown_keys = false}>(base, jsonString);
	if (baseError) {
		logError(baseError, jsonString);
		return;
	}

	const std::string& messageType = base.messageType;

	event.code = ResponseCode::UNKNOWN;

	if (messageType == Type::AUTHENTICATION) {
		if (auto response = parseResponse<AuthenticationResponse>(jsonString)) {
			buildAuthenticationEvent(event, *(response->data));
		}
	}
	else if (messageType == Type::AUTHENTICATION_TOKEN) {
		if (auto response = parseResponse<AuthenticationTokenResponse>(jsonString)) {
			buildAuthenticationTokenEvent(event, *(response->data));
		}
	}
	else if (messageType == Type::INPUT_PARAMETER_LIST) {
		if (auto response = parseResponse<InputParameterListResponse>(jsonString)) {
			buildInputParameterListEvent(event, *(response->data));
		}
	}
	else if (messageType == Type::PARAMETER_CREATION) {
		if (auto response = parseResponse<ParameterCreationResponse>(jsonString)) {
			buildParameterCreationEvent(event, *(response->data));
		}
	}
	else if (messageType == Type::PARAMETER_DELETION) {
		if (auto response = parseResponse<ParameterDeletionResponse>(jsonString)) {
			buildParameterDeletionEvent(event, *(response->data));
		}
	}
}

}  // namespace vts
