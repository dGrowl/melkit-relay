#include "imgui/imgui.h"

#include "gui/parameter_template_modal.hpp"
#include "gui/utility.hpp"

static constexpr size_t TEMPLATE_CONTROLLER = 0;

static std::vector<const char*> TEMPLATES{"Controller"};

namespace gui {

void ParameterTemplateModal::showCloseButtons() {
	bool isValid = (_hasPress || _hasShoulders || _hasSticks || _hasTriggers)
	               && (_useController || _useKeyboard || _useMouse);
	ImGui::BeginDisabled(!isValid);
	if (ImGui::Button("Create", ImVec2(128.0f, 0.0f))) {
		ImGui::CloseCurrentPopup();
	}
	ImGui::EndDisabled();
	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();
	if (ImGui::Button("Cancel", ImVec2(128.0f, 0.0f))) {
		ImGui::CloseCurrentPopup();
	}
}

ParameterTemplateModal::ParameterTemplateModal() :
    _hasPress(true),
    _hasShoulders(true),
    _hasSticks(true),
    _hasTriggers(true),
    _useController(true),
    _useKeyboard(true),
    _useMouse(true),
    _templateSelector("##template-selector", TEMPLATES) {}

void ParameterTemplateModal::show() {
	centerNextWindow();
	if (ImGui::BeginPopupModal(NAME,
	                           nullptr,
	                           ImGuiWindowFlags_AlwaysAutoResize
	                               | ImGuiWindowFlags_NoMove
	                               | ImGuiWindowFlags_NoNav
	                               | ImGuiWindowFlags_NoSavedSettings)) {
		ImGui::SeparatorText("Template");

		ImGui::Text("Select the type of device your VTuber model will control.");
		_templateSelector.show();

		ImGui::SeparatorText("Actions");

		ImGui::Text(
		    "Select which parts of the game controller your model will be able to "
		    "interact with.");

		ImGui::Checkbox("##action-press", &_hasPress);
		ImGui::SameLine();
		ImGui::Text("Press (pushing on the sides)");

		ImGui::Checkbox("##action-shoulders", &_hasShoulders);
		ImGui::SameLine();
		ImGui::Text("Shoulders (LB, RB)");

		ImGui::Checkbox("##action-sticks", &_hasSticks);
		ImGui::SameLine();
		ImGui::Text("Sticks (LS, RS)");

		ImGui::Checkbox("##action-triggers", &_hasTriggers);
		ImGui::SameLine();
		ImGui::Text("Triggers (LT, RT)");

		ImGui::SeparatorText("Inputs");

		ImGui::Text(
		    "Select which of your physical devices will send input to the virtual "
		    "controller.");

		ImGui::Checkbox("##device-controller", &_useController);
		ImGui::SameLine();
		ImGui::Text("Controller");

		ImGui::Checkbox("##device-mouse", &_useMouse);
		ImGui::SameLine();
		ImGui::Text("Mouse");

		ImGui::Checkbox("##device-keyboard", &_useKeyboard);
		ImGui::SameLine();
		ImGui::Text("Keyboard");

		showCloseButtons();

		ImGui::EndPopup();
	}
}

}  // namespace gui
