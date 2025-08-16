#ifndef VTS_REQUEST_HPP_
#define VTS_REQUEST_HPP_

#include <string>

namespace vts {

void authenticate();
void requestToken();
void saveToken(const std::string& token);

};  // namespace vts

#endif  // VTS_REQUEST_HPP_
