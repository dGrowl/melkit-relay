#include "gui/theme_color_modal.hpp"

#include "core/settings.hpp"
#include "gui/fonts.hpp"
#include "gui/theme.hpp"
#include "gui/utility.hpp"

namespace gui {

ThemeColorModal::ThemeColorModal() :
    _shift(SETTINGS.getThemeHueShift()) {}

static constexpr ImVec2 CLOSE_BUTTON_SIZE{128.0F, 0.0F};

void ThemeColorModal::show() {
	centerNextWindow();
	if (ImGui::BeginPopupModal(NAME,
	                           nullptr,
	                           ImGuiWindowFlags_AlwaysAutoResize
	                               | ImGuiWindowFlags_NoMove
	                               | ImGuiWindowFlags_NoSavedSettings)) {
		{
			FONT_SCOPE(FontType::BOLD);
			ImGui::SeparatorText("Hue");
		}
		ImGui::TextWrapped("Drag below to adjust the color scheme.");
		if (ImGui::SliderFloat("##theme-color-hue-shift", &_shift, -1.0F, 1.0F)) {
			THEME.setHueShift(_shift);
		}
		ImGui::SetItemTooltip(
		    "You can Ctrl + Click on this slider to type in a value.");

		if (ImGui::Button("Save", CLOSE_BUTTON_SIZE)) {
			SETTINGS.setThemeHueShift(_shift);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Discard", CLOSE_BUTTON_SIZE)) {
			_shift = SETTINGS.getThemeHueShift();
			THEME.setHueShift(_shift);
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

}  // namespace gui
