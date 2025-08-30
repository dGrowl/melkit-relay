#include "imgui/imgui.h"

#include "gui/combo_box.hpp"

namespace gui {

ComboBox::ComboBox(const char* label, const std::vector<const char*>& options) :
    _label(label),
    _options(options),
    _i(0) {}

unsigned ComboBox::getIndex() const {
	return _i;
}

void ComboBox::show() {
	const char* selectedData = _options[_i];
	if (ImGui::BeginCombo(_label, selectedData, 0)) {
		for (unsigned i = 0; i < _options.size(); i++) {
			const bool isSelected = (_i == i);
			if (ImGui::Selectable(_options[i], isSelected)) {
				_i = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
}

}  // namespace gui
