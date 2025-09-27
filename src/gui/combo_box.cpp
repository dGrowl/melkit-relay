#include "gui/combo_box.hpp"

#include <SDL3/SDL_assert.h>

#include "imgui/imgui.h"

namespace gui {

ComboBox::ComboBox(const char* label, const std::vector<const char*>& options) :
    _label(label),
    _options(options),
    _i(0) {}

bool ComboBox::show() {
	if (_options.empty()) {
		ImGui::Text("N/A");
		return false;
	}
	const char* selectedData = _options[_i];
	bool        updated      = false;
	if (ImGui::BeginCombo(_label, selectedData, 0)) {
		for (size_t i = 0; i < _options.size(); i++) {
			const bool isSelected = (_i == i);
			if (ImGui::Selectable(_options[i], isSelected)) {
				if (_i != i) {
					updated = true;
				}
				_i = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	return updated;
}

size_t ComboBox::getIndex() const {
	return _i;
}

void ComboBox::setIndex(const size_t i) {
	SDL_assert(i < _options.size());

	_i = i;
}

}  // namespace gui
