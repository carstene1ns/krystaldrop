#ifndef DisplayableInstance_H
#define DisplayableInstance_H

#include "../Tools/defines.h"

#include "Color.h"

#include <SDL/SDL.h>

class KD_DisplayableResource;

/**
	Class containing the instance of a disaplayble object (an image instance or a sprtie instance).
  */
class DllExport KD_DisplayableInstance
{
	friend class KD_DisplayableResource;

protected:
	/**
		The resource to be Displayed.
	*/
	KD_DisplayableResource *resource;

	/**
		The constructor is made private, it can only be called by inherited members
	*/
	KD_DisplayableInstance(KD_DisplayableResource *resource);

public:
	/**
		The destructor is made pure virtual, it can only be called by inherited members
	*/
	virtual ~KD_DisplayableInstance();

	/**
		Displays the image in (x,y).
	*/
	virtual void Display(float x, float y);

	/**
		Displays the image in (x,y) with some alpha blending
	*/
	virtual void DisplayAlpha(float x, float y, int alpha);

	/**
		Displays the image turned around (1,0,0) at (x,y)
	*/
    //virtual void DisplayRotateX(int x, int y, float angle);
    
	/**
		Displays the image turned around (0,1,0) at (x,y)
	*/  
    //virtual void DisplayRotateY(int x, int y, float angle);

	/**
		Displays the image in (x,y) with some alpha blending, zooming and coloring.
		This is only supported by the OpenGL renderer.
		The SDL renderer won't perform zoom nor coloring
	*/
	virtual void DisplayColorZoom(float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY);

	/**
		Displays the image with some alpha blending, zooming and coloring and rotation.
		The rotation center must be given and the angle too. The sprite will be Displayed usually in (x,y), resized and then rotated.
		This is only supported by the OpenGL renderer.
		The SDL renderer won't perform zoom nor coloring
	*/
	virtual void DisplayColorZoomRotate(float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle);

	/**
		Displays a part of the image in (x,y) with some alpha blending
		The part Displayed is delimited by (xPart,yPart,xPart+widthPart,yPart+heightPart)
	*/
	virtual void DisplayAlphaPart(float x, float y, int alpha, int xPart, int yPart, int widthPart, int heightPart)=0;

	/**
		Displays the sprite with full poarameter passing: the coordinates color and alpha of each corner!
	*/
	virtual void DisplayFullParameters(float x1, float y1, int r1, int g1, int b1, int alpha1, float x2, float y2, int r2, int g2, int b2, int alpha2, float x3, float y3, int r3, int g3, int b3, int alpha3, float x4, float y4, int r4, int g4, int b4, int alpha4)=0;

	/**
		Displays the sprite with full poarameter passing: the coordinates color and alpha of each corner!
	*/
	virtual void DisplayFullParameters(float x1, float y1, const KD_Color &c1, int alpha1, float x2, float y2, const KD_Color &c2, int alpha2, float x3, float y3, const KD_Color &c3, int alpha3, float x4, float y4, const KD_Color &c4, int alpha4);

	/**
		Returns the height of the surface.
	*/
	virtual int GetHeight()=0;

	/**
		Returns the height of the surface.
	*/
	virtual int GetWidth()=0;
};

#endif
