#ifndef OGLImage_H
#define OGLImage_H

#include <SDL/SDL.h>
#include "../Tools/defines.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

#include "Image.h"

/**
	Class containing an image (OpenGL Texture).
	These images are used by ImageManager
  */
class DllExport KD_OGLImage : public KD_Image
{
	friend class KD_ResourceManager;
protected:
	/**
		The constructor is made private, it can only be called by ImageManager which is a friend class of Image.
	*/
	KD_OGLImage();

	/**
		The destructor is made private, it can only be called by ImageManager which is a friend class of Image.
	*/
	virtual ~KD_OGLImage();

	/**
		The OpenGL Texture.
	*/
	GLuint texture;

	/**
		Does the texture possesses an alpha channel?
	*/
	bool hasAlphaChannel;

	int width;
	int height;
	/// Texture coordinates of right side
	float tx;
	/// Texture coordinates of bottom side
	float ty;

public:
	/**
		Makes a KD_Image from a SDL_Surface
		The SDL_Surface must NOT be used afterwards and must NOT be deleted.
		KD_Image will delete the image for the user.
	*/
	virtual void makeImageFromSDLSurface(SDL_Surface *surf);

	/**
		Loads an image from the file fileName.
	*/
	virtual void Load(KD_FilePath fileName);
	
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
//    virtual void DisplayRotateX(int x, int y, float angle);
    
	/**
		Displays the image turned around (0,1,0) at (x,y)
	*/  
//    virtual void DisplayRotateY(int x, int y, float angle);

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
	virtual void SetColorKey(Uint32 key);
	virtual void SetColorKey(Uint8 r, Uint8 g, Uint8 b);

	/**
		Returns the SDL_Surface to do dirty things on it.
	*/
	virtual SDL_Surface *getSDL_Surface();

	/**
		Returns the height of the surface.
	*/
	virtual int GetHeight();

	/**
		Returns the height of the surface.
	*/
	virtual int GetWidth();
	
	/**
		Disable the alpha transparency blitting on the surface.
	*/
	virtual void DisableAlpha();

	/**
		Enable the alpha transparency blitting on the surface.
		But alpha is always enabled in OpenGL....
	*/
	virtual void EnableAlpha();
	
	//{
	/**
		Converts the alpha surface to a surface without alpha transparency, but with a color-key.
		This function has no meaning here, since OpenGL does handle only alpha surfaces, and it does it well.
		So we will put a stub instead.
	*/
	virtual void ConvertToColorKey(Uint8 r, Uint8 g, Uint8 b, int alphaTrigger);
	virtual void ConvertToColorKey(unsigned int key, int alphaTrigger);
	//}

	/**
		Resizes the current image with the specified ratio.
		Returns true on success.
		Will fail only if the ratio makes a SDL_Surface with a length less than 1 or a height less than 1.
	*/
	virtual bool resize(float ratio);

	/**
		Copy the KD_Image to another similar image.
		This new copy won't be referenced in the KD_ImageManager!
	*/
	virtual KD_Image *copy();

	/**
		Copies only one part of the image in a new KD_Image.
		This new copy won't be referenced in the KD_ImageManager!
	*/
	virtual KD_Image *copy(int x, int y, int width, int height);
};

#endif
