#include "imgui/imgui.h"

#include "gui/theme.hpp"

namespace gui {

void Theme::initColors() {
	ImGui::StyleColorsDark();

	auto& style = ImGui::GetStyle();

	auto& colors = style.Colors;

	colors[ImGuiCol_TextDisabled]        = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
	colors[ImGuiCol_FrameBg]             = ImVec4(0.294f, 0.125f, 0.122f, 1.0f);
	colors[ImGuiCol_FrameBgHovered]      = ImVec4(0.439f, 0.149f, 0.157f, 1.0f);
	colors[ImGuiCol_FrameBgActive]       = ImVec4(0.682f, 0.2f, 0.212f, 1.0f);
	colors[ImGuiCol_TitleBgActive]       = ImVec4(0.478f, 0.169f, 0.161f, 1.0f);
	colors[ImGuiCol_CheckMark]           = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
	colors[ImGuiCol_SliderGrab]          = ImVec4(0.878f, 0.239f, 0.247f, 1.0f);
	colors[ImGuiCol_SliderGrabActive]    = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
	colors[ImGuiCol_Button]              = ImVec4(0.439f, 0.149f, 0.157f, 1.0f);
	colors[ImGuiCol_ButtonHovered]       = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
	colors[ImGuiCol_ButtonActive]        = ImVec4(0.98f, 0.059f, 0.133f, 1.0f);
	colors[ImGuiCol_Header]              = ImVec4(0.357f, 0.133f, 0.141f, 1.0f);
	colors[ImGuiCol_HeaderHovered]       = ImVec4(0.8f, 0.224f, 0.239f, 1.0f);
	colors[ImGuiCol_HeaderActive]        = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
	colors[ImGuiCol_SeparatorHovered]    = ImVec4(0.6f, 0.098f, 0.114f, 1.0f);
	colors[ImGuiCol_SeparatorActive]     = ImVec4(0.749f, 0.102f, 0.122f, 1.0f);
	colors[ImGuiCol_ResizeGrip]          = ImVec4(0.259f, 0.114f, 0.118f, 1.0f);
	colors[ImGuiCol_ResizeGripHovered]   = ImVec4(0.682f, 0.2f, 0.212f, 1.0f);
	colors[ImGuiCol_ResizeGripActive]    = ImVec4(0.933f, 0.251f, 0.271f, 1.0f);
	colors[ImGuiCol_TabHovered]          = ImVec4(0.8f, 0.224f, 0.239f, 1.0f);
	colors[ImGuiCol_Tab]                 = ImVec4(0.51f, 0.165f, 0.165f, 1.0f);
	colors[ImGuiCol_TabSelected]         = ImVec4(0.678f, 0.2f, 0.208f, 1.0f);
	colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
	colors[ImGuiCol_TabDimmed]           = ImVec4(0.149f, 0.067f, 0.067f, 1.0f);
	colors[ImGuiCol_TabDimmedSelected]   = ImVec4(0.424f, 0.137f, 0.141f, 1.0f);
	colors[ImGuiCol_TextLink]            = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
	colors[ImGuiCol_TextSelectedBg]      = ImVec4(0.392f, 0.141f, 0.149f, 1.0f);
	colors[ImGuiCol_NavCursor]           = ImVec4(0.98f, 0.259f, 0.278f, 1.0f);
}

void Theme::initLayout() {
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding.y = 8.0f;
}

void Theme::init() {
	initColors();
	initLayout();
}

}  // namespace gui
