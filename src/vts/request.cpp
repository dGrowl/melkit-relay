#include "vts/request.hpp"

#include <iostream>

#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <glaze/core/common.hpp>
#include <glaze/core/context.hpp>
#include <glaze/core/reflect.hpp>
#include <glaze/json/write.hpp>

#include "core/meta.hpp"
#include "core/settings.hpp"
#include "vts/parameter.hpp"
#include "ws/controller.hpp"

static void logError(const glz::error_ctx& error, const std::string& buffer) {
	std::cerr
	    << "Error serializing request: "
	    << glz::format_error(error, buffer)
	    << std::endl;
}

template <typename T>
static std::optional<std::string> stringify(const T& value) {
	std::string result;
	auto        error = glz::write_json(value, result);
	if (error) {
		logError(error, result);
		return std::nullopt;
	}
	return result;
}

namespace vts {

struct BaseRequest {
	std::string apiName    = "VTubeStudioPublicAPI";
	std::string apiVersion = "1.0";
	std::string requestId  = "SomeID";
	std::string messageType;

	struct glaze {
		using T                     = BaseRequest;
		static constexpr auto value = glz::object("apiName",
		                                          &T::apiName,
		                                          "apiVersion",
		                                          &T::apiVersion,
		                                          "requestId",
		                                          &T::requestId,
		                                          "messageType",
		                                          &T::messageType);
	};
};

template <typename DataType>
struct Request : BaseRequest {
	DataType data;

	Request(const std::string& messageTypeRef, DataType&& requestData) :
	    BaseRequest{.messageType{messageTypeRef}},
	    data(std::move(requestData)) {}

	struct glaze {
		using T                     = Request<DataType>;
		static constexpr auto value = glz::object("apiName",
		                                          &T::apiName,
		                                          "apiVersion",
		                                          &T::apiVersion,
		                                          "requestId",
		                                          &T::requestId,
		                                          "messageType",
		                                          &T::messageType,
		                                          "data",
		                                          &T::data);
	};
};

struct AuthenticationData {
	std::string pluginName      = core::PLUGIN_NAME;
	std::string pluginDeveloper = core::PLUGIN_DEVELOPER;
	std::string authenticationToken;

	struct glaze {
		using T                     = AuthenticationData;
		static constexpr auto value = glz::object("pluginName",
		                                          &T::pluginName,
		                                          "pluginDeveloper",
		                                          &T::pluginDeveloper,
		                                          "authenticationToken",
		                                          &T::authenticationToken);
	};
};

void authenticate(ws::IController& wsController) {
	std::string token = SETTINGS.getAuthToken();
	if (!token.empty()) {
		const Request<AuthenticationData> request(
		    "AuthenticationRequest",
		    AuthenticationData{.authenticationToken = std::move(token)});
		if (auto message = stringify(request)) {
			wsController.sendMessage(std::move(*message));
		}
	}
	else {
		requestToken(wsController);
	}
}

struct AuthenticationTokenRequestData {
	std::string pluginName      = core::PLUGIN_NAME;
	std::string pluginDeveloper = core::PLUGIN_DEVELOPER;
	std::string pluginIcon      = core::PLUGIN_ICON_BASE64;

	struct glaze {
		using T                     = AuthenticationTokenRequestData;
		static constexpr auto value = glz::object("pluginName",
		                                          &T::pluginName,
		                                          "pluginDeveloper",
		                                          &T::pluginDeveloper,
		                                          "pluginIcon",
		                                          &T::pluginIcon);
	};
};

void requestToken(ws::IController& wsController) {
	const Request<AuthenticationTokenRequestData> request(
	    "AuthenticationTokenRequest",
	    AuthenticationTokenRequestData());
	if (auto message = stringify(request)) {
		wsController.sendMessage(std::move(*message));
	}
}

struct ParameterCreationRequestData {
	std::string parameterName;
	std::string explanation = "This is my new parameter.";
	float       min;
	float       max;
	float       defaultValue = 0.0F;

	struct glaze {
		using T                     = ParameterCreationRequestData;
		static constexpr auto value = glz::object("parameterName",
		                                          &T::parameterName,
		                                          "explanation",
		                                          &T::explanation,
		                                          "min",
		                                          &T::min,
		                                          "max",
		                                          &T::max,
		                                          "defaultValue",
		                                          &T::defaultValue);
	};
};

void createParameter(ws::IController& wsController,
                     const Parameter& parameter) {
	const Request<ParameterCreationRequestData> request(
	    "ParameterCreationRequest",
	    ParameterCreationRequestData{
	        .parameterName = parameter.getName(),
	        .min           = parameter.getMin(),
	        .max           = parameter.getMax(),
	    });
	if (auto message = stringify(request)) {
		wsController.sendMessage(std::move(*message));
	}
}

struct ParameterDeletionData {
	std::string parameterName;

	struct glaze {
		using T                     = ParameterDeletionData;
		static constexpr auto value = glz::object("parameterName", &T::parameterName);
	};
};

void deleteParameter(ws::IController&       wsController,
                     const std::string_view name) {
	const Request<ParameterDeletionData> request(
	    "ParameterDeletionRequest",
	    ParameterDeletionData{.parameterName = std::string{name}});
	if (auto message = stringify(request)) {
		wsController.sendMessage(std::move(*message));
	}
}

void getParameters(ws::IController& wsController) {
	const BaseRequest request{.messageType = "InputParameterListRequest"};
	if (auto message = stringify(request)) {
		wsController.sendMessage(std::move(*message));
	}
}

struct InjectParameterDataRequestData {
	bool                        faceFound = false;
	std::string                 mode      = "set";
	std::vector<ParameterValue> parameterValues;

	struct glaze {
		using T                     = InjectParameterDataRequestData;
		static constexpr auto value = glz::object("faceFound",
		                                          &T::faceFound,
		                                          "mode",
		                                          &T::mode,
		                                          "parameterValues",
		                                          &T::parameterValues);
	};
};

void setParameters(ws::IController&             wsController,
                   std::vector<ParameterValue>& values) {
	const Request<InjectParameterDataRequestData> request(
	    "InjectParameterDataRequest",
	    InjectParameterDataRequestData{.parameterValues = std::move(values)});
	if (auto message = stringify(request)) {
		wsController.sendMessage(std::move(*message));
	}
}

};  // namespace vts
