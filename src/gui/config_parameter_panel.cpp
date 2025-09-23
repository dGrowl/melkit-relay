#include <algorithm>
#include <string>
#include <unordered_set>

#include "imgui/imgui.h"

#include "gui/config_parameter_panel.hpp"
#include "gui/fonts.hpp"
#include "gui/icon.hpp"
#include "math/formula.hpp"
#include "ws/controller.hpp"

static char charToLower(unsigned char c) {
	return std::tolower(c);
}

static std::string toLower(const std::string& s) {
	std::string copy = s;
	std::transform(copy.begin(), copy.end(), copy.begin(), charToLower);
	return copy;
}

static std::unordered_set<std::string> buildFilterSet(
    vts::ParameterManager& parameterManager,
    const std::string&     filter) {
	std::unordered_set<std::string> result;
	std::string                     lowerFilter = toLower(filter);
	for (const auto& parameter : parameterManager.values()) {
		std::string lowerName = toLower(parameter.getName());
		if (!lowerName.contains(lowerFilter)) {
			result.insert(parameter.getName());
		}
	}

	return result;
}

namespace gui {

bool ConfigParameterPanel::showCreateOption() {
	if (ImGui::Selectable("Create")) {
		_editingParameter = vts::Parameter();
		_editParameterModal.refresh();
		return true;
	}

	return false;
}

void ConfigParameterPanel::showControls() {
	bool shouldOpenEditMenu = false;

	if (ImGui::BeginTable("Parameter Control Row", 2)) {
		ImGui::TableSetupColumn("Filter", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Edit", ImGuiTableColumnFlags_WidthFixed);

		ImGui::TableNextColumn();
		ImGui::SetNextItemWidth(-1.0f);
		ImGui::PushItemFlag(ImGuiItemFlags_NoNav, true);
		if (ImGui::InputTextWithHint("##filter",
		                             "Filter",
		                             _filterBuffer,
		                             IM_ARRAYSIZE(_filterBuffer))) {
			_filteredParameterNames = buildFilterSet(_parameterManager, _filterBuffer);
		}
		ImGui::PopItemFlag();

		ImGui::TableNextColumn();
		if (ImGui::Button("Edit", ImVec2(128.0f, 0.0f))) {
			shouldOpenEditMenu = true;
		}
		ImGui::EndTable();
	}
	if (shouldOpenEditMenu) {
		ImGui::OpenPopup("Edit Menu");
	}
	bool shouldOpenCreate   = false;
	bool shouldOpenDelete   = false;
	bool shouldOpenTemplate = false;
	if (ImGui::BeginPopupContextItem("Edit Menu")) {
		shouldOpenCreate   = showCreateOption();
		shouldOpenTemplate = ImGui::Selectable("Template");
		shouldOpenDelete =
		    ImGui::Selectable("Delete",
		                      false,
		                      ImGuiSelectableFlags_None,
		                      ImVec2(128.0f, 0.0f));  // sets the width of the menu

		ImGui::EndPopup();
	}
	if (shouldOpenCreate) {
		ImGui::OpenPopup(EditParameterModal::NAME);
	}
	if (shouldOpenTemplate) {
		ImGui::OpenPopup(ParameterTemplateModal::NAME);
	}
	if (shouldOpenDelete) {
		ImGui::OpenPopup(DeleteParametersModal::NAME);
	}
}

void ConfigParameterPanel::showData() {
	if (_parameterManager.isEmpty()) {
		ImGui::TextWrapped(
		    "You haven't created any parameters yet. :(\n\n"
		    "To get started, hit the \"Edit\" button in the top right!");
		return;
	}
	bool shouldOpenModal = false;

	ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(12.0f, 2.0f));

	if (ImGui::BeginTable("Parameter Table",
	                      3,
	                      ImGuiTableFlags_PadOuterX
	                          | ImGuiTableFlags_RowBg
	                          | ImGuiTableFlags_ScrollY
	                          | ImGuiTableFlags_SizingFixedFit,
	                      ImVec2(0.0f, 0.0f))) {
		ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("Inputs", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("Output", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();

		for (auto& p : _parameterManager.values()) {
			if (_filteredParameterNames.contains(p.getName())) {
				continue;
			}
			ImGui::PushID(p.getName().c_str());

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			ImVec2 cursorPos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(cursorPos.x, cursorPos.y + 9.0f));
			if (ImGui::Button(p.getName().c_str(), ImVec2(200.0f, 0.0f))) {
				_editingParameter = p;
				_editParameterModal.refresh();
				shouldOpenModal = true;
			}
			unsigned iIcon = 0;
			ImGui::TableNextColumn();

			auto     availableWidth = ImGui::GetContentRegionAvail().x;
			unsigned nIconsPerRow   = std::max(1.0f, availableWidth / 38.0f);
			for (const auto& input : p.getInputs() | std::views::values) {
				const float alpha =
				    math::remapLinear(input.getValue(), -1.0f, 1.0f, 0.1f, 1.0f);
				if (iIcon == nIconsPerRow) {
					iIcon = 0;
					ImGui::NewLine();
				}
				drawIcon(input.getId(), alpha);
				iIcon += 1;
			}

			ImGui::TableNextColumn();
			cursorPos = ImGui::GetCursorPos();
			ImGui::SetCursorPos(ImVec2(cursorPos.x, cursorPos.y + 8.0f));
			ImGui::Text("%.2f", p.getOutput());

			ImGui::PopID();
		}

		ImGui::EndTable();
	}

	ImGui::PopStyleVar();

	if (shouldOpenModal) {
		ImGui::OpenPopup(EditParameterModal::NAME);
	}
}

void ConfigParameterPanel::showModals() {
	_deleteParametersModal.show();
	_editParameterModal.show();
	_parameterTemplateModal.show();
}

void ConfigParameterPanel::showParameters() {
	showControls();
	ImGui::Separator();
	showData();
	showModals();
}

ConfigParameterPanel::ConfigParameterPanel(
    vts::Parameter&        editingParameter,
    vts::ParameterManager& parameterManager,
    ws::IController&       wsController) :
    _editingParameter(editingParameter),
    _parameterManager(parameterManager),
    _wsController(wsController),
    _deleteParametersModal(parameterManager, wsController),
    _editParameterModal(wsController, editingParameter),
    _parameterTemplateModal(wsController),
    _filteredParameterNames(),
    _filterBuffer() {
	SDL_zeroa(_filterBuffer);
}

void ConfigParameterPanel::show() {
	{
		FONT_SCOPE(FontType::BOLD);
		ImGui::SeparatorText("Parameters");
	}
	if (_wsController.getStatus() == ws::Status::CONNECTED) {
		showParameters();
	}
	else {
		ImGui::Text("You'll need to connect to VTS to manage parameters.");
	}
}

}  // namespace gui
