#ifndef Image_H
#define Image_H

#include <SDL/SDL.h>

class TACCRes;
class KD_ImageManager;

/**
	Class containing an image (SDL_Surfaces).
	These images are used by ImageManager
  */
class KD_Image
{
	friend class KD_ImageManager;
protected:
	/**
		The constructor is made private, it can only be called by ImageManager which is a friend class of Image.
	*/
	KD_Image();

	/**
		The destructor is made private, it can only be called by ImageManager which is a friend class of Image.
	*/
	virtual ~KD_Image();

	/**
		The SDL_image.
	*/
	//SDL_Surface *image;

	/**
		The reference count for that given Image
	*/
	int countRef;

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
	virtual void Load(char *fileName)=0;

	/**
		Loads an image from the file fileName which stands in the accFile accFile.
		If accFile == 0, attempts to load the image from the real file fileName.
	*/
	virtual void Load(TACCRes *accFile, char *fileName)=0;

	/**
		Displays the image in (x,y).
	*/
	virtual void Display(int x, int y)=0;

	/**
		Displays the image in (x,y) with some alpha blending
	*/
	virtual void DisplayAlpha(int x, int y, int alpha);

	/**
		Displays the image turned around (1,0,0) at (x,y)
	*/
    virtual void DisplayRotateX(int x, int y, float angle)= 0;
    
	/**
		Displays the image turned around (0,1,0) at (x,y)
	*/  
    virtual void DisplayRotateY(int x, int y, float angle)= 0;

	/**
		Displays the image in (x,y) with some alpha blending, zooming and coloring.
		This is only supported by the OpenGL renderer.
		The SDL renderer won't perform zoom nor coloring
	*/
	virtual void DisplayColorZoom(int x, int y, int r, int g, int b, int alpha, float resizeX, float resizeY);

	/**
		Displays the image with some alpha blending, zooming and coloring and rotation.
		The rotation center must be given and the angle too. The sprite will be displayed usually in (x,y), resized and then rotated.
		This is only supported by the OpenGL renderer.
		The SDL renderer won't perform zoom nor coloring
	*/
	virtual void DisplayColorZoomRotate(int x, int y, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle);

	/**
		Sets the colorkey of the image.
	*/
	virtual void setColorKey(Uint32 key)=0;
	virtual void setColorKey(Uint8 r, Uint8 g, Uint8 b)=0;

	/**
		Returns the SDL_Surface to do dirty things on it.
	*/
	virtual SDL_Surface *getSDL_Surface();

	/**
		Returns the height of the surface.
	*/
	virtual int getHeight()=0;

	/**
		Returns the height of the surface.
	*/
	virtual int getWidth()=0;
	
	/**
		Disable the alpha transparency blitting on the surface.
	*/
	virtual void disableAlpha()=0;

	/**
		Enable the alpha transparency blitting on the surface.
	*/
	virtual void enableAlpha()=0;

	/**
		Converts the alpha surface to a surface without alpha transparency, but with a color-key.
		If the alpha channel is less than "alphaTrigger", the pixel will be converted to colorKey,
		else, the pixels will kept.
	*/
	virtual void convertToColorKey(Uint8 r, Uint8 g, Uint8 b, int alphaTrigger)=0;
	virtual void convertToColorKey(unsigned int key, int alphaTrigger)=0;

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
