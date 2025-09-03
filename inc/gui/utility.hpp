#ifndef GUI_UTILITY_HPP_
#define GUI_UTILITY_HPP_

#include <ranges>
#include <unordered_map>

#include "imgui/imgui.h"
#include "libuiohook/uiohook.h"

#include "vts/input.hpp"

namespace gui {

using KeycodeView =
    decltype(std::declval<const std::unordered_map<ImGuiKey, vts::InputId>&>()
             | std::views::keys);

KeycodeView validImGuiKeys();

vts::InputId convertImGuiToUioKey(const ImGuiKey keycode);

const char* getUioKeyName(const Uint32 keycode);

}  // namespace gui

#endif  // GUI_UTILITY_HPP_
