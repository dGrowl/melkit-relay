#ifndef GUI_SET_MOUSE_BOUNDS_MODAL_
#define GUI_SET_MOUSE_BOUNDS_MODAL_

#include "impulse/processor.hpp"
#include "vts/parameter.hpp"

namespace gui {

class SetMouseBoundsModal {
private:
	imp::Processor& _impulseProcessor;
	vts::Parameter& _editingParameter;

	math::Rectangle<int> _currentBounds;
	math::Rectangle<int> _newBounds;

	math::Vector2<int> _cornerOne;
	math::Vector2<int> _cornerTwo;

	void updateBounds();

public:
	SetMouseBoundsModal(imp::Processor& impulseProcessor,
	                    vts::Parameter& editingParameter);

	void refresh();
	void show();

	constexpr static const char* NAME = "Set Mouse Bounds";
};

}  // namespace gui

#endif  // GUI_SET_MOUSE_BOUNDS_MODAL_
