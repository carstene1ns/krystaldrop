#ifndef Sprite_H
#define Sprite_H

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include "deque"
using namespace std;

#include "SDL/SDL.h"

class KD_SpriteInstance;
class TACCRes;

/**
	Class containing a serie of frames (SDL_Surfaces).
	This is basically an animation.
  */
class KD_Anim
{
	friend class KD_SpriteInstance;
private:
	/**
		The vector containing all the images.
	*/
	deque<SDL_Surface *> images;

public:
	 KD_Anim();
	~KD_Anim();

	/**
		Loads a single image into the next frame.
	*/
	void addFileImage(char *name);

	void addFileImageFromACC(TACCRes *accFile, char *name);
	//void addFileImageFromACC(char *name);

	/**
		Adds the specified surface to the anim.
	*/
	void addSurface(SDL_Surface *surf);

	/**
		Set's the color-key for every frame of the anim
	*/
	void setColorKey(Uint32 key);
	void setColorKey(Uint8 r, Uint8 g, Uint8 b);

	/**
		Displays frame "frame" in (x,y)
	*/
	void Display(int x, int y, int frame);
};

/**
	Class containing a serie of anims.
  */
class KD_Sprite
{
	friend class KD_SpriteInstance;
private:
	/**
		Set of anims that the sprite contains.
	*/
	deque<KD_Anim *> anims;
	
public:
	         KD_Sprite();
	virtual ~KD_Sprite();

	/**
		Displays the image "frame" of the anim "anim" in (x,y)
	*/
	void Display(int x, int y, int anim, int frame);

	/**
		Set's the color-key for every anim of the sprite
	*/
	void setColorKey(Uint32 key);
	void setColorKey(Uint8 r, Uint8 g, Uint8 b);

	/**
		Adds a new anim to the sprite and returns a pointer to it in order to fill it.
	*/
	KD_Anim *newAnim();
};

#endif