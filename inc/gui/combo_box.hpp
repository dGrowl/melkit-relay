#ifndef GUI_COMBO_BOX_HPP_
#define GUI_COMBO_BOX_HPP_

#include <vector>

namespace gui {

class ComboBox {
private:
	const char* _label;
	const std::vector<const char*>& _options;
	unsigned _i;

public:
	ComboBox(const char* label, const std::vector<const char*>& options);

	unsigned getIndex() const;
	bool show();
};

}  // namespace gui

#endif  // GUI_COMBO_BOX_HPP_
