#ifndef SpriteInstance_H
#define SpriteInstance_H

class KD_Sprite;

/**
	This class represent an instance of a sprite, that is, 
    a sprite, its position on the screen and its current frame
    as part of a animation.
  */
class KD_SpriteInstance
{ private:
	/**
		The sprite to be displayed
	*/
	KD_Sprite *spr;
  
    /** OnFinishCalled is set to 1 when an animation has finished,
        onFinishAnim has been called, but no other animation is following 
        OnFinishCalled is reset to 0 by setAnim(). */
    short OnFinishCalled; 

	/**
		Current anim displayed
	*/
	int currentAnim;

	/**
		Current frame of the anim displayed
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
		Time in millisecond when the previous frame as been displayed.
	*/
	//int ticks;

public:
	/**
		Position of the sprite.
	*/
	int x;
	int y;

	/**
		Constructor.
		You should give the sprite to be displayed in argument.
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
        if center= 0, or the center of the sprite else.
		Return true if the animed is finished or looping back to the beginning
		false otherwise
	*/
	bool Display (short center= 0);

	/*bool DisplayCentered();*/

	virtual void onFinishAnim (int animNo);
};

#endif
