#include "spriteinstance.h"

#include "sprite.h"
#include "Display.h"

KD_SpriteInstance::KD_SpriteInstance(KD_Sprite *spr)
{
	this->spr = spr;

	currentAnim=0;
	currentFrame=0;
	framePerSec=0;
	loop=true;
	x=0;
	y=0;
	ticks=SDL_GetTicks();
}

KD_SpriteInstance::~KD_SpriteInstance()
{

}

void KD_SpriteInstance::setLoop(bool loop)
{
	this->loop = loop;
}

void KD_SpriteInstance::setFramesPerSeconds(float framePerSec)
{
	this->framePerSec = framePerSec;
}

void KD_SpriteInstance::setAnim(int anim)
{
	currentAnim = anim;
}

bool KD_SpriteInstance::Display()
{
	int animSize = spr->anims[currentAnim]->images.size();
	bool ret = false;

	// If we display at the maximum possible framerate
	if (framePerSec == 0)
	{
		currentFrame++;

		if (currentFrame == animSize && loop == true)
		{
			currentFrame = 0;
			onFinishAnim();
			ret = true;
		}
		else if (currentFrame == animSize-1 && loop == false)
		{
			onFinishAnim();
			ret = true;
		}
		else if (currentFrame == animSize && loop == false)
		{
			currentFrame--;
			onFinishAnim();
			ret = true;
		}

		spr->Display(x,y, currentAnim, (int)currentFrame);
	}
	else
	{
		currentFrame += ((float)(SDL_GetTicks() - ticks))*framePerSec/1000.0f;

		ticks = SDL_GetTicks();

		if (currentFrame >= animSize && loop == true)
		{
			// Not exact because we round up the number of frames, but never mind....
			currentFrame = (int)(currentFrame) % animSize;
			onFinishAnim();
			ret = true;
		}
		else if (currentFrame >= animSize && loop == false)
		{
			currentFrame = animSize-1;
			onFinishAnim();
			ret = true;
		}

		spr->Display(x,y, currentAnim, (int)currentFrame);
	}

	return ret;
}
