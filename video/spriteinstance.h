#ifndef SpriteInstance_H
#define SpriteInstance_H


class Sprite;

/**
	Class containing a serie of anims.
  */
class SpriteInstance
{
private:
	/**
		The sprite to be displayed
	*/
	Sprite *spr;

	/**
		Current anim displayed
	*/
	int currentAnim;

	/**
		Current frame of the anim displayed
	*/
	float currentFrame;

	/**
		Number of frames par seconds wanted
		0 means 1 frame per blit.
	*/
	float framePerSec;

	/**
		Must the sprite loop, or be displayed only once?
	*/
	bool loop;

	/**
		Time in millisecond when the previous frame as been displayed.
	*/
	int ticks;

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
	SpriteInstance(Sprite *spr);
	virtual ~SpriteInstance();

	/**
		Set if the sprite is going to loop its anim or not.
	*/
	void setLoop(bool loop);

	/**
		Set if the sprite is going to loop its anim or not.
	*/
	void setFramesPerSeconds(float framePerSec);

	/**
		Displays the sprite
		Return true if the animed is finished or looping back to the beginning
		false otherwise
	*/
	bool Display();

};

#endif