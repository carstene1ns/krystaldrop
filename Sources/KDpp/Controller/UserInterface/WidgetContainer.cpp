#include "WidgetContainer.h"

#include "../../Tools/Logfile.h"

KD_WidgetContainer::KD_WidgetContainer() : KD_Widget()
{
}

/*KD_WidgetContainer::KD_WidgetContainer(KD_WidgetController *_ownerController) : KD_Widget(_ownerController)
{
}*/

KD_WidgetContainer::~KD_WidgetContainer()
{
	while (!widgets.empty())
	{
		delete widgets.begin()->second;
		widgets.erase(widgets.begin());
	}
}

bool KD_WidgetContainer::onWidgetKeyDown(SDLKey key)
{
	map<string, KD_Widget *> :: iterator iter = widgets.begin();

	// scan all widgets.
	while (iter != widgets.end())
	{
		bool res = iter->second->onWidgetKeyDown(key);
		if (res == true)
			return true;

		iter++;
	}

	return false;
}

bool KD_WidgetContainer::onWidgetKeyUp(SDLKey key)
{
	map<string, KD_Widget *> :: iterator iter = widgets.begin();

	// scan all widgets.
	while (iter != widgets.end())
	{
		bool res = iter->second->onWidgetKeyUp(key);
		if (res == true)
			return true;

		iter++;
	}

	return false;
}

bool KD_WidgetContainer::onWidGetMouseMotion(int x, int y, int xPrevious, int yPrevious)
{
	map<string, KD_Widget *> :: iterator iter = widgets.begin();

	// scan all widgets.
	while (iter != widgets.end())
	{
		KD_Widget *current = iter->second;

		current->onWidGetMouseMotion(x,y, xPrevious, yPrevious);

		// Warning, I'm making the assumption that the coordinates of the widget have not changed since last frame.
		if (current->isInside(x,y) && !current->isInside(xPrevious, yPrevious))
			current->onMouseEnter();
		else if (!current->isInside(x,y) && current->isInside(xPrevious, yPrevious))
			current->onMouseLeave();

		iter++;
	}

	// onMouseMotion always returns false so that the event can be handled by several widgets
	// eg. a onEnter and a onLeave event at the same time.
	return false;
}

bool KD_WidgetContainer::onMouseEnter()
{
	
	return false;
}

bool KD_WidgetContainer::onMouseLeave()
{

	return false;
}

bool KD_WidgetContainer::onWidgetClick(int button)
{
	return false;
}

bool KD_WidgetContainer::onWidGetMouseButtonDown(int button, int x, int y)
{
	map<string, KD_Widget *> :: iterator iter = widgets.begin();

	// scan all widgets.
	while (iter != widgets.end())
	{
		KD_Widget *current = iter->second;

		if (current->isInside(x,y))
		{
			bool res = current->onWidGetMouseButtonDown(button, x, y);
			if (res == true)
				return true;
		}

		iter++;
	}

	return false;
}

bool KD_WidgetContainer::onWidGetMouseButtonUp(int button, int x, int y, int xLastClick, int yLastClick)
{
	map<string, KD_Widget *> :: iterator iter = widgets.begin();

	// scan all widgets.
	while (iter != widgets.end())
	{
		KD_Widget *current = iter->second;

		// First, send the onClick message, warning, once more, I assume the coordinates of the widget have not moved.
		if (current->isInside(x,y) && current->isInside(xLastClick,yLastClick))
		{
			bool res = current->onWidgetClick(button);
			if (res == true)
				return true;
		}
		// Then the mouseButtonUp message if onClick was not received
		if (current->isInside(x,y))
		{
			bool res = current->onWidGetMouseButtonUp(button, x, y, xLastClick, yLastClick);
			if (res == true)
				return true;
		}

		iter++;
	}

	return false;
}

void KD_WidgetContainer::OnEnableWidget()
{
	map<string, KD_Widget *> :: iterator iter = widgets.begin();
	// scan all widgets.
	while (iter != widgets.end())
	{
		iter->second->OnEnableWidget();
		iter++;
	}
}

void KD_WidgetContainer::OnDisableWidget()
{
	map<string, KD_Widget *> :: iterator iter = widgets.begin();
	// scan all widgets.
	while (iter != widgets.end())
	{
		iter->second->OnDisableWidget();
		iter++;
	}
}

void KD_WidgetContainer::PerformWidget()
{
	map<string, KD_Widget *> :: iterator iter = widgets.begin();
	// scan all widgets.
	while (iter != widgets.end())
	{
		iter->second->PerformWidget();
		iter++;
	}
}

void KD_WidgetContainer::DisplayWidget(int alpha)
{
	map<string, KD_Widget *> :: iterator iter = widgets.begin();
	// scan all widgets.
	while (iter != widgets.end())
	{
		iter->second->DisplayWidget((alpha*alphaWidget)/255);
		iter++;
	}
}

bool KD_WidgetContainer::AddWidget(string name, KD_Widget *widget)
{
		// if the name is already stored: cancel
#ifdef DEBUG
	map<string, KD_Widget *> :: iterator iter = widgets.find(name);

	if (iter != widgets.end())
	{
		KD_LogFile::printf2("Warning! Widget %s does already exist, can't add it to WidgetContainer.\n",name.c_str());
		return false;
	}
#endif

	widgets[name] = widget;
	widgets[name]->ownerController = ownerController;

	return true;
}

bool KD_WidgetContainer::RemoveWidget(string name)
{
	// if the name is already stored: cancel
#ifdef DEBUG
	map<string, KD_Widget *> :: iterator iter = widgets.find(name);

	if (iter == widgets.end())
	{
		KD_LogFile::printf2("Warning! Widget %s does not exist, can't delete it in WidgetContainer.\n",name.c_str());
		return false;
	}
#endif

	delete widgets[name];
	widgets.erase(name);
	return true;
}

KD_Widget *KD_WidgetContainer::GetWidget(string name)
{
#ifdef DEBUG
	map<string, KD_Widget *> :: iterator iter = widgets.find(name);

	if (iter == widgets.end())
	{
		KD_LogFile::printf2("Warning! Widget %s does not exist, can't return it.\n",name.c_str());
		return NULL;
	}
#endif

	return widgets[name];
}

bool KD_WidgetContainer::RemoveAll()
{
	map<string, KD_Widget *> :: iterator iter = widgets.begin();

	// deletes all widgets
	while (iter != widgets.end())
	{
		delete iter->second;
		iter++;
	}

	// removes all names
	while (!widgets.empty())
	{
		widgets.erase(widgets.begin());
	}

	return true;
}
