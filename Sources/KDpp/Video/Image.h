#ifndef Image_H
#define Image_H

#include "../Tools/defines.h"

//#include "../Resources/Resource.h"
#include "DisplayableResource.h"
#include "../Tools/FilePath.h"

#include <SDL/SDL.h>

class KD_ImageManager;

/**
	Class containing an image (SDL_Surfaces).
	These images are used by ImageManager
  */
class DllExport KD_Image : public KD_DisplayableResource
{
	friend class KD_ResourceManager;
protected:
	/**
		The constructor is made private, it can only be called by ImageManager which is a friend class of Image.
	*/
	KD_Image();

	/**
		The destructor is made private, it can only be called by ImageManager which is a friend class of Image.
	*/
	virtual ~KD_Image();

public:

	/**
		Make an image instance from this instance.
	*/
	virtual KD_DisplayableInstance *createInstance();

	/**
		Makes a KD_Image from a SDL_Surface
		The SDL_Surface must NOT be used afterwards and must NOT be deleted.
		KD_Image will delete the image for the user.
	*/
	virtual void makeImageFromSDLSurface(SDL_Surface *surf);


	/**
		Loads an image from the file fileName.
	*/
	virtual void Load(KD_FilePath fileName)=0;

	/**
		Displays the image in (x,y).
	*/
	virtual void Display(float x, float y)=0;

	/**
		Displays the image in (x,y) with some alpha blending
	*/
	virtual void DisplayAlpha(float x, float y, int alpha);

	/**
		Displays the image turned around (1,0,0) at (x,y)
	*/
//    virtual void DisplayRotateX(int x, int y, float angle)= 0;
    
	/**
		Displays the image turned around (0,1,0) at (x,y)
	*/  
//    virtual void DisplayRotateY(int x, int y, float angle)= 0;

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
	virtual void DisplayColorZoomRotate(float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY, float rotX, float rotY, float angle);

	/**
		Displays a part of the image in (x,y) with some alpha blending
		The part Displayed is delimited by (xPart,yPart,xPart+widthPart,yPart+heightPart)
	*/
	virtual void DisplayAlphaPart(float x, float y, int alpha, int xPart, int yPart, int widthPart, int heightPart);

	/**
		Displays the sprite with full poarameter passing: the coordinates color and alpha of each corner!
	*/
	virtual void DisplayFullParameters(float x1, float y1, int r1, int g1, int b1, int alpha1, 
                                     float x2, float y2, int r2, int g2, int b2, int alpha2, 
                                     float x3, float y3, int r3, int g3, int b3, int alpha3, 
                                     float x4, float y4, int r4, int g4, int b4, int alpha4);

	/**
		Sets the colorkey of the image.
	*/
	virtual void SetColorKey(Uint32 key)=0;
	virtual void SetColorKey(Uint8 r, Uint8 g, Uint8 b)=0;

	/**
		Returns the SDL_Surface to do dirty things on it.
	*/
	virtual SDL_Surface *getSDL_Surface();

	/**
		Returns the height of the surface.
	*/
	virtual int GetHeight()=0;

	/**
		Returns the height of the surface.
	*/
	virtual int GetWidth()=0;
	
	/**
		Disable the alpha transparency blitting on the surface.
	*/
	virtual void DisableAlpha()=0;

	/**
		Enable the alpha transparency blitting on the surface.
	*/
	virtual void EnableAlpha()=0;

	/**
		Converts the alpha surface to a surface without alpha transparency, but with a color-key.
		If the alpha channel is less than "alphaTrigger", the pixel will be converted to colorKey,
		else, the pixels will kept.
	*/
	virtual void ConvertToColorKey(Uint8 r, Uint8 g, Uint8 b, int alphaTrigger)=0;
	virtual void ConvertToColorKey(unsigned int key, int alphaTrigger)=0;

	/**
		Resizes the current image with the specified ratio.
		Returns true on success.
		Will fail only if the ratio makes an Image with a length less than 1 or a height less than 1.
	*/
	virtual bool resize(float ratio)=0;

	/**
		Copy the KD_Image to another similar image.
		This new copy won't be referenced in the KD_ImageManager!
	*/
	virtual KD_Image *copy()=0;

	/**
		Copies only one part of the image in a new KD_Image.
		This new copy won't be referenced in the KD_ImageManager!
	*/
	virtual KD_Image *copy(int x, int y, int width, int height)=0;
};

#endif
