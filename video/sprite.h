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
class KD_Image;

#define KD_NONEXTANIM -1

/**
	Class containing a serie of frames (SDL_Surfaces).
	This is basically an animation.
  */
class KD_Anim
{
	friend class KD_SpriteInstance;
private:
  /* temp*/
public:
	/**
		The vector containing all the images.
	*/
	deque<KD_Image *> images;

	/**
		Anim of the sprite that should be set when this one finishes.
		If the value is -1, then the anim should stop on last frame.
	*/
	int next_anim;

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
	//void addSurface(SDL_Surface *surf);
	void addSurface(KD_Image *surf);

	/**
		Set's the color-key for every frame of the anim
	*/
	void setColorKey(Uint32 key);
	void setColorKey(Uint8 r, Uint8 g, Uint8 b);

	/**
		Displays frame "frame" in (x,y)
	*/
	void Display(int x, int y, int frame);

	/**
		Displays frame "frame" in (x,y) with a bunch of parameters for special effects!
		Except alpha blending (if colorkey is enabled) all the effects will occur only in OpenGL mode.
	*/
	void Display(int x, int y, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle, int frame);

	/**
		Set the next_anim to the specified value.
		Set this value to KD_NONEXTANIM to avoid any next anim.
	*/
	void setNextAnim(int next_anim);

	/**
		public getter for the ith image.
	*/
	KD_Image *getImage(int i);

	/**
		returns the number of images in this anim.
	*/
	unsigned getAnimSize();
};

/**
	Class containing a serie of anims.
  */
class KD_Sprite
{
	friend class KD_SpriteInstance;
private:
  
  /*temp*/
public:  
	/**
		Set of anims that the sprite contains.
	*/
	deque<KD_Anim *> anims;

	/**
		Default number of frame per seconds wanted.
		This value can be overwritten by SpriteInstance
	*/
	float framePerSeconds;
	
public:
	         KD_Sprite();
	virtual ~KD_Sprite();

	/**
		Displays the image "frame" of the anim "anim" in (x,y)
	*/
	void Display(int x, int y, int anim, int frame);

	/**
		Displays the image "frame" of the anim "anim" in (x,y) with a bunch of parameters for special effects!
		Except alpha blending (if colorkey is enabled) all the effects will occur only in OpenGL mode.
	*/
	void Display(int x, int y, int anim, int frame, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle);


	/**
		Set's the color-key for every anim of the sprite
	*/
	void setColorKey(Uint32 key);
	void setColorKey(Uint8 r, Uint8 g, Uint8 b);

	/**
		Adds a new anim to the sprite and returns a pointer to it in order to fill it.
	*/
	KD_Anim *newAnim();
    
	/**
		Loads a Sprite from its file descriptor fileName
	*/
	bool Load(char *fileName);

	/**
		Loads a Sprite from its file descriptor fileName in tha ACC file accFile,
		or from a real file if accFile is 0.
	*/
	bool Load(TACCRes *accFile, char *fileName);

	/**
		public getter for the ith anim.
	*/
	KD_Anim *getAnim(int i);

	/**
		resizes the sprite with a ratio "ratio".
		Will always return true.
	*/
	bool resize(float ratio);

};

#endif
