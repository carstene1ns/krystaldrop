#include "Mouse.h"

KD_Mouse *KD_Mouse::singleton=0;

KD_Mouse::KD_Mouse(): 
    hasMovedSinceLastClick(true),
    doubleClickSensitivity(500),
    doubleClickDisabled(false)
{
	for (int i=0; i<256; i++)
		buttons[i]=false;
}

KD_Mouse::~KD_Mouse()
{
	
}


KD_Mouse *KD_Mouse::GetMouse()
{
	return singleton;
}

KD_Mouse *KD_Mouse::InitMouse()
{
	if (singleton==0)
		singleton = new KD_Mouse();
	return singleton;
}

bool KD_Mouse::CloseMouse()
{
	if (!singleton) return false;
	delete singleton;
	singleton = 0;
	return true;
}

void KD_Mouse::UpdateMousePosition(const SDL_Event &event)
{
	if (x != (unsigned short int)event.motion.x || y != (unsigned short int)event.motion.y)
		hasMovedSinceLastClick = true;

	x = (unsigned short int)event.motion.x;
	y = (unsigned short int)event.motion.y;
}

int KD_Mouse::onButtonDown(const SDL_Event &event)
{
	x = (unsigned short int)event.button.x;
	y = (unsigned short int)event.button.y;

	xLastClick = x;
	yLastClick = y;

	buttons[event.button.button] = true;

	// if double click
	if (hasMovedSinceLastClick == false && doubleClickDisabled == false && (lastClickTime + doubleClickSensitivity > SDL_GetTicks()))
	{
		lastClickTime = 0;
		return KD_DOUBLECLICK;
	}
	
	lastClickTime = SDL_GetTicks();
	
	hasMovedSinceLastClick = false;

	return event.button.button;
}

int KD_Mouse::onButtonUp(const SDL_Event &event)
{
	x = (unsigned short int)event.button.x;
	y = (unsigned short int)event.button.y;
	
	buttons[event.button.button] = false;

	return event.button.button;
}

void KD_Mouse::DisableDoubleClick()
{
	doubleClickDisabled = true;
}
	
void KD_Mouse::EnableDoubleClick()
{
	doubleClickDisabled = false;
}

void KD_Mouse::GetLastClickCoordinates(int &xLastClick, int &yLastClick)
{
	xLastClick = this->xLastClick;
	yLastClick = this->yLastClick;
}

void KD_Mouse::GetCoordinates(int &x, int &y)
{
	x = this->x;
	y = this->y;
}

bool KD_Mouse::GetButtonState(int nb) const
{
	return buttons[nb];
}
