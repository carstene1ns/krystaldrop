#ifndef SpriteInstance_H
#define SpriteInstance_H

#include "../Tools/defines.h"

#include "DisplayableInstance.h"

class KD_Sprite;

#define KD_SPRITE_TOP_LEFT 0
#define KD_SPRITE_CENTERED_HORIZ 1
#define KD_SPRITE_CENTERED 2

/**
	This class represent an instance of a sprite, that is, 
    a sprite, its position on the screen and its current frame
    as part of a animation.
  */
class DllExport KD_SpriteInstance : public KD_DisplayableInstance
{ private:
	/**
		The sprite to be Displayed
	*/
//	KD_Sprite *spr;
  
    /** OnFinishCalled is set to 1 when an animation has finished,
        OnFinishAnim has been called, but no other animation is following 
        OnFinishCalled is reset to 0 by setAnim(). */
    short OnFinishCalled; 

	/**
		Current anim Displayed
	*/
	int currentAnim;

	/**
		Current frame of the anim Displayed
	*/
	//float currentFrame;
	int currentFrame;

	/**
		Number of frames par seconds wanted
		0 means 1 frame per blit.
	*/
	//float framePerSec;


	/**
		Number of milliseconds between the frame
		0 means 1 frame per blit.
	*/
	int msecBetweenFrames;

	/**
		Computes which frame should be Displayed.
		Eventually call the OnFinishAnim method.
	*/
	void computeNextFrame();

	/**
		Time in millisecond when the previous frame as been Displayed.
	*/
	//int ticks;

public:
	/**
		Position of the sprite.
	*/
//	int x;
//	int y;

	/**
		Constructor.
		You should give the sprite to be Displayed in argument.
	*/
    KD_SpriteInstance(KD_Sprite *spr);

	virtual ~KD_SpriteInstance();

	/**
		Set if the sprite is going to loop its anim or not.
        Warning: must not be used when the sprite's animation was
                 not a loop (anim->next_anim== KD_NONEXTANIM)
                 because in that case, OnFinishCalled is 1, therefore
                 the sprite would be animated.
                 You have to call setAnim to reset OnFinishCalled first.
	*/
	void setFramesPerSeconds(float framePerSec);

	/**
		Set the current animation of the sprite.
	*/
	void setAnim(int anim);

	/**
		Returns the current animation number of the sprite.
	*/
	int getAnim();

	/**
		Displays the sprite
		The (x,y) coordinates represent the upper left corner of the sprite
        if center= KD_SPRITE_TOP_LEFT, or the center of the sprite if center = KD_SPRITE_CENTERED
		or the bottom middle of the sprite if center = KD_SPRITE_CENTERED_HORIZ
		Return true if the animed is finished or looping back to the beginning
		false otherwise
	*/
	//bool Display (short center= KD_SPRITE_TOP_LEFT);

	/**
		Displays the sprite
		The (x,y) coordinates represent the upper left corner of the sprite
        if center= 0, or the center of the sprite else.
		Return true if the animed is finished or looping back to the beginning
		false otherwise

		There are a bunch of parameters for special effects!
		Except alpha blending (if colorkey is enabled) all the effects will occur only in OpenGL mode.
	*/
	//bool Display(int x, int y, short center, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle);

	/*bool DisplayCentered();*/

	virtual void OnFinishAnim (int animNo);

	
	/**
		Displays the image in (x,y).
		Note: only call a Display function once a frame!
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
	virtual void DisplayAlphaPart(float x, float y, int alpha, int xPart, int yPart, int widthPart, int heightPart);

	/**
		Displays the sprite with full poarameter passing: the coordinates color and alpha of each corner!
	*/
	virtual void DisplayFullParameters(float x1, float y1, int r1, int g1, int b1, int alpha1, float x2, float y2, int r2, int g2, int b2, int alpha2, float x3, float y3, int r3, int g3, int b3, int alpha3, float x4, float y4, int r4, int g4, int b4, int alpha4);

	/**
		Displays the sprite with full poarameter passing: the coordinates color and alpha of each corner!
	*/
	virtual void DisplayFullParameters(float x1, float y1, const KD_Color &c1, int alpha1, float x2, float y2, const KD_Color &c2, int alpha2, float x3, float y3, const KD_Color &c3, int alpha3, float x4, float y4, const KD_Color &c4, int alpha4);

	/**
		Returns the height of the surface.
	*/
	virtual int GetHeight();

	/**
		Returns the height of the surface.
	*/
	virtual int GetWidth();
};

#endif
