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
private:
	/**
		The constructor is made private, it can only be called by ImageManager which is a friend class of Image.
	*/
	KD_Image();

	/**
		The destructor is made private, it can only be called by ImageManager which is a friend class of Image.
	*/
	~KD_Image();

	/**
		The SDL_image.
	*/
	SDL_Surface *image;

	/**
		The reference count for that given Image
	*/
	int countRef;

public:
	/**
		Loads an image from the file fileName.
	*/
	void Load(char *fileName);

	/**
		Loads an image from the file fileName which stands in the accFile accFile.
	*/
	void Load(TACCRes *accFile, char *fileName);

	/**
		Displays the image in (x,y).
	*/
	void Display(int x, int y);

	/**
		Sets the colorkey of the image.
	*/
	void setColorKey(Uint32 key);
	void setColorKey(Uint8 r, Uint8 g, Uint8 b);
};

#endif
