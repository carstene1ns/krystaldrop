#include "WidgetController.h"

#include "UserInterface/Mouse.h"

KD_WidgetController::KD_WidgetController() : KD_Controller(), KD_WidgetContainer()
{
	ownerController = this;
}

KD_WidgetController::~KD_WidgetController()
{
}

bool KD_WidgetController::ProcessEvent(int value)
{
	return false;
}

bool KD_WidgetController::Process()
{
	PerformWidget();
	return true;
}

bool KD_WidgetController::Display()
{
	DisplayWidget(255);
	return true;
}

bool KD_WidgetController::OnEnable()
{
	OnEnableWidget();
	return true;
}

bool KD_WidgetController::OnDisable()
{
	OnDisableWidget();
	return true;
}

bool KD_WidgetController::ProcessKeyDown(SDLKey key)
{
	bool res = onWidgetKeyDown(key);
	if (res == true)
		return true;

	return ProcessEvent(keysDownBindings[key]);
}

bool KD_WidgetController::ProcessKeyUp(SDLKey key)
{
	bool res = onWidgetKeyUp(key);
	if (res == true)
		return true;

	return ProcessEvent(keysUpBindings[key]);
}

bool KD_WidgetController::ProcessMouseMove(int x, int y, int xRel, int yRel)
{
	onWidGetMouseMotion(x - xWidget,y - yWidget, x-xWidget-xRel, y-yWidget-yRel);

	return false;
}

bool KD_WidgetController::ProcessMouseButtonDown(int button, int x, int y)
{
	return onWidGetMouseButtonDown(button, x - xWidget,y - yWidget);
}

bool KD_WidgetController::ProcessMouseButtonUp(int button, int x, int y)
{
	int xLastClick, yLastClick;
	KD_Mouse::GetMouse()->GetLastClickCoordinates(xLastClick, yLastClick);

	return onWidGetMouseButtonUp(button, x - xWidget,y - yWidget, xLastClick, yLastClick);
}
