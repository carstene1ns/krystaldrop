#ifndef Frame_H
#define Frame_H

#include <SDL/SDL.h>

#include "../../Tools/defines.h"

#include "WidgetContainer.h"

class KD_Image;

/**
	A frame (a rectangular widget container)
  */
class DllExport KD_Frame : public KD_WidgetContainer
{
	KD_Image *background;
	KD_Image *top_border;
	KD_Image *bottom_border;
	KD_Image *left_border;
	KD_Image *right_border;
	KD_Image *topleft_corner;
	KD_Image *topright_corner;
	KD_Image *bottomleft_corner;
	KD_Image *bottomright_corner;
	
public:
	/**
		Constructor setting the owner
	*/
	KD_Frame(int x, int y, int width, int height);

	/**
		Destructor
	*/
	virtual ~KD_Frame();

	/**
		Release from the ResourceSet of the controller the previously needed resources.
	*/
	virtual void deleteFrame();

	/**
		Method called each frame to Display the widget
	*/
	virtual void DisplayWidget(int alpha);

};

#endif
