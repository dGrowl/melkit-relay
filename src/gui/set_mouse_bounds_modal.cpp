#include "gui/set_mouse_bounds_modal.hpp"

#include <algorithm>

#include "imgui/imgui.h"

#include "gui/fonts.hpp"
#include "gui/utility.hpp"
#include "impulse/code.hpp"
#include "impulse/processor.hpp"
#include "vts/parameter.hpp"

constexpr imp::Code MOUSE_POSITION_X_IMPULSE =
    imp::Axis::X | imp::EventTag::MOUSE_MOVE_ABS;

constexpr imp::Code MOUSE_POSITION_Y_IMPULSE =
    imp::Axis::Y | imp::EventTag::MOUSE_MOVE_ABS;

constexpr float COORDINATE_FIELD_WIDTH = 100.0f;

namespace gui {

void SetMouseBoundsModal::updateBounds() {
	_newBounds.top    = std::min(_cornerOne.y, _cornerTwo.y);
	_newBounds.left   = std::min(_cornerOne.x, _cornerTwo.x);
	_newBounds.bottom = std::max(_cornerOne.y, _cornerTwo.y);
	_newBounds.right  = std::max(_cornerOne.x, _cornerTwo.x);
	_impulseProcessor.setMouseBounds(_newBounds);
}

SetMouseBoundsModal::SetMouseBoundsModal(imp::Processor& impulseProcessor,
                                         vts::Parameter& editingParameter) :
    _impulseProcessor(impulseProcessor),
    _editingParameter(editingParameter),
    _currentBounds(),
    _newBounds(),
    _cornerOne(),
    _cornerTwo() {}

void SetMouseBoundsModal::refresh() {
	_currentBounds    = _impulseProcessor.getMouseBounds();
	_newBounds        = _currentBounds;
	_cornerOne.x      = _currentBounds.left;
	_cornerOne.y      = _currentBounds.top;
	_cornerTwo.x      = _currentBounds.right;
	_cornerTwo.y      = _currentBounds.bottom;
	_editingParameter = vts::Parameter();
	_editingParameter.addImpulse(MOUSE_POSITION_X_IMPULSE);
	_editingParameter.addImpulse(MOUSE_POSITION_Y_IMPULSE);
}

void SetMouseBoundsModal::show() {
	centerNextWindow();
	if (ImGui::BeginPopupModal(NAME,
	                           nullptr,
	                           ImGuiWindowFlags_AlwaysAutoResize
	                               | ImGuiWindowFlags_NoMove
	                               | ImGuiWindowFlags_NoNav
	                               | ImGuiWindowFlags_NoSavedSettings)) {
		{
			FONT_SCOPE(FontType::BOLD);
			ImGui::SeparatorText("Instructions");
		}

		if (ImGui::IsKeyPressed(ImGuiKey_A, false)) {
			_cornerOne.x = _impulseProcessor.getMouseState().x;
			_cornerOne.y = _impulseProcessor.getMouseState().y;
			updateBounds();
		}
		if (ImGui::IsKeyPressed(ImGuiKey_Z, false)) {
			_cornerTwo.x = _impulseProcessor.getMouseState().x;
			_cornerTwo.y = _impulseProcessor.getMouseState().y;
			updateBounds();
		}

		ImGui::Text(
		    "Set up your mouse tracking area:\n\n"
		    "1. Move your mouse to one corner and press 'A'.\n"
		    "2. Move your mouse to the opposite corner and press 'Z'.\n\n"
		    "You can fine tune as needed by typing below.");

		{
			FONT_SCOPE(FontType::BOLD);
			ImGui::SeparatorText("Corners");
		}

		ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(12.0f, 2.0f));

		if (ImGui::BeginTable("InputTable",
		                      3,
		                      ImGuiTableFlags_PadOuterX
		                          | ImGuiTableFlags_RowBg
		                          | ImGuiTableFlags_SizingFixedFit)) {
			ImGui::TableSetupColumn("Corner", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableHeadersRow();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("First (A)");

			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(COORDINATE_FIELD_WIDTH);
			ImGui::InputInt("##corner-one-x",
			                &_cornerOne.x,
			                0,
			                0,
			                ImGuiInputTextFlags_None);

			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(COORDINATE_FIELD_WIDTH);
			ImGui::InputInt("##corner-one-y",
			                &_cornerOne.y,
			                0,
			                0,
			                ImGuiInputTextFlags_None);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Second (Z)");

			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(COORDINATE_FIELD_WIDTH);
			ImGui::InputInt("##corner-two-x",
			                &_cornerTwo.x,
			                0,
			                0,
			                ImGuiInputTextFlags_None);

			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(COORDINATE_FIELD_WIDTH);
			ImGui::InputInt("##corner-two-y",
			                &_cornerTwo.y,
			                0,
			                0,
			                ImGuiInputTextFlags_None);

			ImGui::EndTable();
		}

		const auto& mouseX = _editingParameter.getReceiver(MOUSE_POSITION_X_IMPULSE);
		const auto& mouseY = _editingParameter.getReceiver(MOUSE_POSITION_Y_IMPULSE);
		{
			FONT_SCOPE(FontType::BOLD);
			ImGui::SeparatorText("Live Values");
		}

		if (ImGui::BeginTable("InputTable",
		                      4,
		                      ImGuiTableFlags_PadOuterX
		                          | ImGuiTableFlags_RowBg
		                          | ImGuiTableFlags_SizingFixedFit)) {
			ImGui::TableSetupColumn("Device", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Event", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Target", ImGuiTableColumnFlags_WidthFixed);
			ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
			ImGui::TableHeadersRow();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Mouse");

			ImGui::TableNextColumn();
			ImGui::Text("Position");

			ImGui::TableNextColumn();
			ImGui::Text("X");

			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(128.0f);
			ImGui::BeginDisabled();
			float xValue = mouseX.getValue();
			ImGui::SliderFloat("##x-value",
			                   &xValue,
			                   mouseX.getMin(),
			                   mouseX.getMax(),
			                   "%.3f",
			                   ImGuiSliderFlags_NoInput);
			ImGui::EndDisabled();

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Mouse");

			ImGui::TableNextColumn();
			ImGui::Text("Position");

			ImGui::TableNextColumn();
			ImGui::Text("Y");

			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(128.0f);
			ImGui::BeginDisabled();
			float yValue = mouseY.getValue();
			ImGui::SliderFloat("##y-value",
			                   &yValue,
			                   mouseY.getMin(),
			                   mouseY.getMax(),
			                   "%.3f",
			                   ImGuiSliderFlags_NoInput);
			ImGui::EndDisabled();

			ImGui::EndTable();
		}

		ImGui::PopStyleVar();

		if (ImGui::Button("Save", ImVec2(128.0f, 0.0f))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Discard", ImVec2(128.0f, 0.0f))) {
			_impulseProcessor.setMouseBounds(_currentBounds);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

}  // namespace gui
