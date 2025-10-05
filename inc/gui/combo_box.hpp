#ifndef GUI_COMBO_BOX_HPP_
#define GUI_COMBO_BOX_HPP_

#include <cstddef>
#include <vector>

namespace gui {

class ComboBox {
private:
	const char*                     _label;
	const std::vector<const char*>& _options;
	size_t                          _i;

public:
	ComboBox(const char* label, const std::vector<const char*>& options);

	[[nodiscard]] size_t getIndex() const;

	bool show();
	void setIndex(const size_t i);
};

}  // namespace gui

#endif  // GUI_COMBO_BOX_HPP_
