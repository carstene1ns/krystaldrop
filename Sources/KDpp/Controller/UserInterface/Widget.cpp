#include "Widget.h"

#include "../../Video/Display.h"

KD_Widget::KD_Widget() : xWidget(0), yWidget(0), alphaWidget(255), ownerController(NULL)
{
	widthWidget = Display::width;
	heightWidget = Display::height;
	
}

/*KD_Widget::KD_Widget(KD_WidgetController *_ownerController) : xWidget(0), yWidget(0), ownerController(_ownerController)
{
	widthWidget = Display::width;
	heightWidget = Display::height;
}*/

KD_Widget::~KD_Widget()
{
}

bool KD_Widget::onWidgetKeyDown(SDLKey key)
{
	return false;
}
	
bool KD_Widget::onWidgetKeyUp(SDLKey key)
{
	return false;
}

bool KD_Widget::onWidGetMouseMotion(int x, int y, int xPrevious, int yPrevious)
{
	return false;
}

bool KD_Widget::onMouseEnter()
{
	return false;
}

bool KD_Widget::onMouseLeave()
{
	return false;
}

bool KD_Widget::onWidgetClick(int button)
{
	return false;
}

bool KD_Widget::onWidGetMouseButtonDown(int button, int x, int y)
{
	return false;
}

bool KD_Widget::onWidGetMouseButtonUp(int button, int x, int y, int xLastClick, int yLastClick)
{
	return false;
}

void KD_Widget::OnEnableWidget()
{

}

void KD_Widget::OnDisableWidget()
{

}

void KD_Widget::GetWidgetPosition(int &x, int &y, int &width, int &height)
{
	x = xWidget;
	y = yWidget;
	width = widthWidget;
	height = heightWidget;
}

bool KD_Widget::isInside(int x, int y) const
{
	if (x >= xWidget && y >= yWidget && x < xWidget + widthWidget && y < yWidget + heightWidget)
		return true;
	else
		return false;
}

void KD_Widget::PerformWidget()
{

}

void KD_Widget::DisplayWidget(int alpha)
{

}

void KD_Widget::SetAlpha(int alpha)
{
	alphaWidget = alpha;
}
