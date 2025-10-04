#include "gui/delete_parameters_modal.hpp"

#include <algorithm>
#include <functional>
#include <ranges>

#include "imgui/imgui.h"

#include "gui/utility.hpp"
#include "vts/parameter_manager.hpp"
#include "vts/request.hpp"
#include "ws/controller.hpp"

namespace gui {

DeleteParametersModal::DeleteParametersModal(
    vts::ParameterManager& parameterManager,
    ws::IController&       wsController) :
    _parameterManager(parameterManager),
    _wsController(wsController) {}

void DeleteParametersModal::show() {
	centerNextWindow();
	if (ImGui::BeginPopupModal(NAME,
	                           nullptr,
	                           ImGuiWindowFlags_AlwaysAutoResize
	                               | ImGuiWindowFlags_NoMove
	                               | ImGuiWindowFlags_NoSavedSettings)) {
		ImGui::Text("Select which parameters to delete.");

		if (ImGui::BeginChild("##delete-selection",
		                      ImVec2(-1.0f, ImGui::GetFontSize() * 12),
		                      ImGuiChildFlags_Borders)) {
			for (auto& p : _parameterManager.values()) {
				if (ImGui::Checkbox(p.getName().c_str(), &_selectedState[p.getName()])) {
					_allSelected = std::ranges::all_of(_selectedState | std::views::values,
					                                   std::identity{});
				}
			}
		}
		ImGui::EndChild();

		if (ImGui::Checkbox("All", &_allSelected)) {
			for (bool& isSelected : _selectedState | std::views::values) {
				isSelected = _allSelected;
			}
		}

		if (ImGui::Button("Delete", ImVec2(128.0f, 0.0f))) {
			for (auto& p : _parameterManager.values()) {
				if (_selectedState[p.getName()]) {
					vts::deleteParameter(_wsController, p.getName());
				}
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();

		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120.0f, 0.0f))) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

}  // namespace gui
