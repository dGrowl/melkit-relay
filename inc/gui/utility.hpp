#ifndef GUI_UTILITY_HPP_
#define GUI_UTILITY_HPP_

#include <ranges>
#include <unordered_map>
#include <utility>

#include <SDL3/SDL_stdinc.h>

#include "imgui/imgui.h"

#include "impulse/code.hpp"

namespace gui {

using UioKey = Uint32;

using KeycodeView =
    decltype(std::declval<const std::unordered_map<ImGuiKey, imp::TargetTag>&>()
             | std::views::keys);

KeycodeView validImGuiKeys();

imp::TargetTag convertImGuiToUioKey(ImGuiKey keycode);

const char* getUioKeyName(UioKey keycode);

void centerNextWindow();

ImVec4 hexToRgba(std::string_view hex);
ImVec4 rgbaToHsla(const ImVec4& rgba);
ImVec4 hslaToRgba(const ImVec4& hsla);
ImVec4 hueShiftRgba(const ImVec4& rgba, const float shift);

}  // namespace gui

#endif  // GUI_UTILITY_HPP_
