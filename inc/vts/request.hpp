#ifndef VTS_REQUEST_HPP_
#define VTS_REQUEST_HPP_

#include <glaze/core/common.hpp>
#include <string_view>

#include "vts/parameter.hpp"
#include "ws/controller.hpp"

namespace vts {

struct ParameterValue {
	std::string id;
	float       value;

	struct glaze {
		using T                     = ParameterValue;
		static constexpr auto value = glz::object("id", &T::id, "value", &T::value);
	};
};

void authenticate(ws::IController& wsController);

void requestToken(ws::IController& wsController);

void createParameter(ws::IController& wsController, const Parameter& parameter);

void deleteParameter(ws::IController&       wsController,
                     const std::string_view name);

void getParameters(ws::IController& wsController);

void setParameters(ws::IController&             wsController,
                   std::vector<ParameterValue>& values);

};  // namespace vts

#endif  // VTS_REQUEST_HPP_
