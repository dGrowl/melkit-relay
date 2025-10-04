#include "gui/theme.hpp"

#include "imgui/imgui.h"

namespace gui {

void Theme::initColors() {
	ImGui::StyleColorsDark();

	auto& style = ImGui::GetStyle();

	auto& colors = style.Colors;

	colors[ImGuiCol_FrameBg]             = ImVec4(0.294F, 0.125F, 0.122F, 1.0F);
	colors[ImGuiCol_FrameBgHovered]      = ImVec4(0.439F, 0.149F, 0.157F, 1.0F);
	colors[ImGuiCol_FrameBgActive]       = ImVec4(0.682F, 0.2F, 0.212F, 1.0F);
	colors[ImGuiCol_TitleBgActive]       = ImVec4(0.478F, 0.169F, 0.161F, 1.0F);
	colors[ImGuiCol_CheckMark]           = ImVec4(0.98F, 0.259F, 0.278F, 1.0F);
	colors[ImGuiCol_SliderGrab]          = ImVec4(0.878F, 0.239F, 0.247F, 1.0F);
	colors[ImGuiCol_SliderGrabActive]    = ImVec4(0.98F, 0.259F, 0.278F, 1.0F);
	colors[ImGuiCol_Button]              = ImVec4(0.439F, 0.149F, 0.157F, 1.0F);
	colors[ImGuiCol_ButtonHovered]       = ImVec4(0.98F, 0.259F, 0.278F, 1.0F);
	colors[ImGuiCol_ButtonActive]        = ImVec4(0.98F, 0.059F, 0.133F, 1.0F);
	colors[ImGuiCol_Header]              = ImVec4(0.357F, 0.133F, 0.141F, 1.0F);
	colors[ImGuiCol_HeaderHovered]       = ImVec4(0.8F, 0.224F, 0.239F, 1.0F);
	colors[ImGuiCol_HeaderActive]        = ImVec4(0.98F, 0.259F, 0.278F, 1.0F);
	colors[ImGuiCol_SeparatorHovered]    = ImVec4(0.6F, 0.098F, 0.114F, 1.0F);
	colors[ImGuiCol_SeparatorActive]     = ImVec4(0.749F, 0.102F, 0.122F, 1.0F);
	colors[ImGuiCol_ResizeGrip]          = ImVec4(0.259F, 0.114F, 0.118F, 1.0F);
	colors[ImGuiCol_ResizeGripHovered]   = ImVec4(0.682F, 0.2F, 0.212F, 1.0F);
	colors[ImGuiCol_ResizeGripActive]    = ImVec4(0.933F, 0.251F, 0.271F, 1.0F);
	colors[ImGuiCol_TabHovered]          = ImVec4(0.8F, 0.224F, 0.239F, 1.0F);
	colors[ImGuiCol_Tab]                 = ImVec4(0.51F, 0.165F, 0.165F, 1.0F);
	colors[ImGuiCol_TabSelected]         = ImVec4(0.678F, 0.2F, 0.208F, 1.0F);
	colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.98F, 0.259F, 0.278F, 1.0F);
	colors[ImGuiCol_TabDimmed]           = ImVec4(0.149F, 0.067F, 0.067F, 1.0F);
	colors[ImGuiCol_TabDimmedSelected]   = ImVec4(0.424F, 0.137F, 0.141F, 1.0F);
	colors[ImGuiCol_TextLink]            = ImVec4(0.98F, 0.259F, 0.278F, 1.0F);
	colors[ImGuiCol_TextSelectedBg]      = ImVec4(0.392F, 0.141F, 0.149F, 1.0F);
	colors[ImGuiCol_NavCursor]           = ImVec4(0.98F, 0.259F, 0.278F, 1.0F);
}

void Theme::initLayout() {
	ImGuiStyle& style = ImGui::GetStyle();

	style.WindowPadding.y = 8.0F;
}

void Theme::init() {
	initColors();
	initLayout();
}

}  // namespace gui
