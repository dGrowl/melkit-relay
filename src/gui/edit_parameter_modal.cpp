#include <format>

#include "imgui/imgui.h"

#include "gui/edit_parameter_modal.hpp"

namespace gui {

void EditParameterModal::showAddInput() {
	if (ImGui::Button("Add", ImVec2(128.0f, 0.0f))) {
		ImGui::OpenPopup(AddInputModal::NAME);
	}

	_addInputModal.show();
}

void EditParameterModal::showInputs() {
	ImGui::SeparatorText("Input");

	if (ImGui::BeginTable("Input Table", 4, ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("Device", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Event", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Target", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);
		if (_editingParameter.hasInputs()) {
			ImGui::TableHeadersRow();
		}

		for (auto& [id, data] : _editingParameter.getInputs()) {
			ImGui::PushID(id);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text(data.device);
			ImGui::TableNextColumn();
			ImGui::Text(data.event);
			ImGui::TableNextColumn();
			ImGui::Text(data.target);
			ImGui::TableNextColumn();
			ImGui::SetNextItemWidth(128.0f);
			ImGui::BeginDisabled();
			ImGui::SliderFloat("##input-value",
			                   &data.value,
			                   data.min,
			                   data.max,
			                   "%.3f",
			                   ImGuiSliderFlags_NoInput);
			ImGui::EndDisabled();

			ImGui::PopID();
		}

		ImGui::EndTable();
	}
}

void EditParameterModal::showOutput() {
	ImGui::SeparatorText("Output");

	if (ImGui::BeginTable("Output Table", 2, ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Value", ImGuiTableColumnFlags_WidthStretch);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Default");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::InputScalar("##default",
		                   ImGuiDataType_Float,
		                   &_editingParameter.defaultValue,
		                   nullptr,
		                   nullptr,
		                   nullptr,
		                   ImGuiInputTextFlags_None);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Minimum");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::InputScalar("##minimum",
		                   ImGuiDataType_Float,
		                   &_editingParameter.min,
		                   nullptr,
		                   nullptr,
		                   nullptr,
		                   ImGuiInputTextFlags_None);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Maximum");
		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::InputScalar("##maximum",
		                   ImGuiDataType_Float,
		                   &_editingParameter.max,
		                   nullptr,
		                   nullptr,
		                   nullptr,
		                   ImGuiInputTextFlags_None);

		ImGui::TableNextRow();
		ImGui::TableNextColumn();
		ImGui::Text("Current");
		ImGui::TableNextColumn();
		float outputValue = _editingParameter.getOutput();
		ImGui::SetNextItemWidth(128.0f);
		ImGui::BeginDisabled();
		ImGui::SliderFloat("##output-value",
		                   &outputValue,
		                   _editingParameter.min,
		                   _editingParameter.max,
		                   "%.3f",
		                   ImGuiSliderFlags_NoInput);
		ImGui::EndDisabled();

		ImGui::EndTable();
	}
}

EditParameterModal::EditParameterModal(vts::Parameter& editingParameter) :
    _editingParameter(editingParameter),
    _addInputModal(editingParameter) {}

void EditParameterModal::show() {
	if (ImGui::BeginPopupModal(NAME, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text(_editingParameter.getName().c_str());

		showInputs();
		showAddInput();
		showOutput();

		if (ImGui::Button("Save", ImVec2(128.0f, 0.0f))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Discard", ImVec2(128.0f, 0.0f))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

}  // namespace gui
