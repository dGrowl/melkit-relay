#ifndef GUI_PARAMETER_TEMPLATE_MODAL_
#define GUI_PARAMETER_TEMPLATE_MODAL_

#include "gui/combo_box.hpp"
#include "ws/controller.hpp"

namespace gui {

class ITemplate {
protected:
	ITemplate() = default;

public:
	virtual ~ITemplate()                   = default;
	ITemplate(const ITemplate&)            = delete;
	ITemplate& operator=(const ITemplate&) = delete;
	ITemplate(ITemplate&&)                 = delete;
	ITemplate& operator=(ITemplate&&)      = delete;

	[[nodiscard]] virtual bool isValid() const = 0;

	virtual void execute(ws::IController& wsController) = 0;
	virtual void show()                                 = 0;
};

class BrushTemplate : public ITemplate {
private:
	bool _hasPosition;
	bool _hasStroke;

	static void createPositionParameters(ws::IController& wsController);
	static void createStrokeParameters(ws::IController& wsController);

public:
	BrushTemplate();

	[[nodiscard]] bool isValid() const override;

	void execute(ws::IController& wsController) override;
	void show() override;
};

class ControllerTemplate : public ITemplate {
private:
	bool _hasPress;
	bool _hasShoulders;
	bool _hasSticks;
	bool _hasTriggers;

	bool _useController;
	bool _useMouseKeyboard;

	void createPressParameters(ws::IController& wsController) const;
	void createShoulderParameters(ws::IController& wsController) const;
	void createStickParameters(ws::IController& wsController) const;
	void createTriggerParameters(ws::IController& wsController) const;

public:
	ControllerTemplate();

	[[nodiscard]] bool isValid() const override;

	void execute(ws::IController& wsController) override;
	void show() override;
};

class ParameterTemplateModal {
private:
	ws::IController& _wsController;

	ComboBox _templateSelector;

	BrushTemplate      _brushTemplate;
	ControllerTemplate _controllerTemplate;

	[[nodiscard]] bool isValid() const;

	void execute();
	void showCloseButtons();

public:
	explicit ParameterTemplateModal(ws::IController& wsController);

	void show();

	constexpr static const char* NAME = "Add Template";
};

}  // namespace gui

#endif  // GUI_PARAMETER_TEMPLATE_MODAL_
