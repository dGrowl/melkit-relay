#ifndef VTS_REQUEST_HPP_
#define VTS_REQUEST_HPP_

#include <string>

#include "ws/client.hpp"

namespace vts {

void authenticate(ws::Client& client);
void requestToken(ws::Client& client);
void saveToken(const std::string& token);

};  // namespace vts

#endif  // VTS_REQUEST_HPP_
