#ifndef Sprite_H
#define Sprite_H

#include "../Tools/defines.h"

#include "deque"
using namespace std;

#include "SDL/SDL.h"

#include "DisplayableResource.h"
#include "../Tools/FilePath.h"

class KD_SpriteInstance;
class TACCRes;
class KD_Image;

#define KD_NONEXTANIM -1

/**
	Class containing a serie of frames (SDL_Surfaces).
	This is basically an animation.
  */
class DllExport KD_Anim
{
	friend class KD_SpriteInstance;
private:
  /* temp*/
public:
	class CenteredImage
	{
	public:
		KD_Image *image;
		int x;
		int y;

		CenteredImage(KD_Image *_image,int _x=0,int _y=0) : image(_image), x(_x), y(_y) {}
	};

	/**
		The vector containing all the images.
	*/
	deque<CenteredImage> images;

	/**
		Anim of the sprite that should be set when this one finishes.
		If the value is -1, then the anim should stop on last frame.
	*/
	int next_anim;

public:
	 KD_Anim();
	virtual ~KD_Anim();

	/**
		Loads a single image into the next frame.
	*/
	void addFileImage(const KD_FilePath &name, int x=0, int y=0);
	//void addFileImage(char *name);

	//void addFileImageFromACC(TACCRes *accFile, char *name);
	
	/**
		Adds the specified surface to the anim.
	*/
	//void addSurface(SDL_Surface *surf);
	void addSurface(KD_Image *surf, int x=0, int y=0);

	/**
		Set's the color-key for every frame of the anim
	*/
	void SetColorKey(Uint32 key);
	void SetColorKey(Uint8 r, Uint8 g, Uint8 b);

	/**
		Displays frame "frame" in (x,y)
	*/
	void Display(int frame, float x, float y);

	/**
		Displays frame "frame" in (x,y) with a bunch of parameters for special effects!
		Except alpha blending (if colorkey is enabled) all the effects will occur only in OpenGL mode.
	*/
//	void Display(int x, int y, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle, int frame);

	/**
		Displays the image "frame" of the anim "anim" in (x,y) with some alpha blending
	*/
	virtual void DisplayAlpha(int frame, float x, float y, int alpha);

	/**
		Displays the image "frame" of the anim "anim" in (x,y) with some alpha blending, zooming and coloring.
		This is only supported by the OpenGL renderer.
		The SDL renderer won't perform zoom nor coloring
	*/
	virtual void DisplayColorZoom(int frame, float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY);

	/**
		Displays the image "frame" of the anim "anim" with some alpha blending, zooming and coloring and rotation.
		The rotation center must be given and the angle too. The sprite will be Displayed usually in (x,y), resized and then rotated.
		This is only supported by the OpenGL renderer.
		The SDL renderer won't perform zoom nor coloring
	*/
	virtual void DisplayColorZoomRotate(int frame, float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle);

	/**
		Displays a part of the image "frame" of the anim "anim" in (x,y) with some alpha blending
		The part Displayed is delimited by (xPart,yPart,xPart+widthPart,yPart+heightPart)
	*/
	virtual void DisplayAlphaPart(int frame, float x, float y, int alpha, int xPart, int yPart, int widthPart, int heightPart);

	/**
		Displays the image "frame" of the anim "anim" with full parameter passing: the coordinates color and alpha of each corner!
	*/
	virtual void DisplayFullParameters(int frame, float x1, float y1, int r1, int g1, int b1, int alpha1, float x2, float y2, int r2, int g2, int b2, int alpha2, float x3, float y3, int r3, int g3, int b3, int alpha3, float x4, float y4, int r4, int g4, int b4, int alpha4);


	/**
		Returns the height of the image "frame" of the anim "anim".
	*/
	virtual int GetHeight(int frame);

	/**
		Returns the height of the image "frame" of the anim "anim".
	*/
	virtual int GetWidth(int frame);


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
class DllExport KD_Sprite : public KD_DisplayableResource
{
	friend class KD_SpriteInstance;
	friend class KD_XMLSpriteParser;
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
		Set's the color-key for every anim of the sprite
	*/
	void SetColorKey(Uint32 key);
	void SetColorKey(Uint8 r, Uint8 g, Uint8 b);

	/**
		Adds a new anim to the sprite and returns a pointer to it in order to fill it.
	*/
	KD_Anim *newAnim();
    
	/**
		public getter for the ith anim.
	*/
	KD_Anim *getAnim(int i);

	/**
		Setter of framePerSec
	*/
	void setFramePerSec(float framePerSeconds);

	/**
		resizes the sprite with a ratio "ratio".
		Will always return true.
	*/
	bool resize(float ratio);

	/**
		Loads a Sprite from its file descriptor fileName
	*/
	bool load(const KD_FilePath &fileName);

	/**
		Loads a Sprite from its XML file descriptor fileName
	*/
#ifndef NO_XML
	bool loadXML(const KD_FilePath &fileName);
#endif

	//	bool Load(char *fileName);

	/**
		Loads a Sprite from its file descriptor fileName in tha ACC file accFile,
		or from a real file if accFile is 0.
	*/
//	bool Load(TACCRes *accFile, char *fileName);

	virtual KD_DisplayableInstance *createInstance();

	/**
		Displays the image "frame" of the anim "anim" in (x,y)
	*/
	void Display(int anim, int frame, float x, float y);

	/**
		Displays the image "frame" of the anim "anim" in (x,y) with a bunch of parameters for special effects!
		Except alpha blending (if colorkey is enabled) all the effects will occur only in OpenGL mode.
	*/
//	void Display(int x, int y, int anim, int frame, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle);

	/**
		Displays the image "frame" of the anim "anim" in (x,y) with some alpha blending
	*/
	virtual void DisplayAlpha(int anim, int frame, float x, float y, int alpha);

	/**
		Displays the image "frame" of the anim "anim" in (x,y) with some alpha blending, zooming and coloring.
		This is only supported by the OpenGL renderer.
		The SDL renderer won't perform zoom nor coloring
	*/
	virtual void DisplayColorZoom(int anim, int frame, float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY);

	/**
		Displays the image "frame" of the anim "anim" with some alpha blending, zooming and coloring and rotation.
		The rotation center must be given and the angle too. The sprite will be Displayed usually in (x,y), resized and then rotated.
		This is only supported by the OpenGL renderer.
		The SDL renderer won't perform zoom nor coloring
	*/
	virtual void DisplayColorZoomRotate(int anim, int frame, float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle);

	/**
		Displays a part of the image "frame" of the anim "anim" in (x,y) with some alpha blending
		The part Displayed is delimited by (xPart,yPart,xPart+widthPart,yPart+heightPart)
	*/
	virtual void DisplayAlphaPart(int anim, int frame, float x, float y, int alpha, int xPart, int yPart, int widthPart, int heightPart);

	/**
		Displays the image "frame" of the anim "anim" with full parameter passing: the coordinates color and alpha of each corner!
	*/
	virtual void DisplayFullParameters(int anim, int frame, float x1, float y1, int r1, int g1, int b1, int alpha1, float x2, float y2, int r2, int g2, int b2, int alpha2, float x3, float y3, int r3, int g3, int b3, int alpha3, float x4, float y4, int r4, int g4, int b4, int alpha4);


	/**
		Returns the height of the image "frame" of the anim "anim".
	*/
	virtual int GetHeight(int anim, int frame);

	/**
		Returns the height of the image "frame" of the anim "anim".
	*/
	virtual int GetWidth(int anim, int frame);
};

#endif
