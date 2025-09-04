#ifndef VTS_REQUEST_HPP_
#define VTS_REQUEST_HPP_

#include "vts/parameter.hpp"
#include "ws/controller.hpp"

namespace vts {

void authenticate(ws::IController& wsController);

void requestToken(ws::IController& wsController);

void createParameter(ws::IController& wsController,
                     const ParameterData& parameter);

void createParameter(ws::IController& wsController, const Parameter& parameter);

void deleteParameter(ws::IController& wsController, const Parameter& parameter);

void getParameters(ws::IController& wsController);

void setParameter(ws::IController& wsController, const Parameter& parameter);

};  // namespace vts

#endif  // VTS_REQUEST_HPP_
