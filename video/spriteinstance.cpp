#include "spriteinstance.h"

#include "sprite.h"
#include "Display.h"

KD_SpriteInstance::KD_SpriteInstance(KD_Sprite *spr)
{
	this->spr = spr;

	currentAnim=0;
	currentFrame=0;
	framePerSec=spr->framePerSeconds;
	x=0;
	y=0;
	ticks=SDL_GetTicks();
}

KD_SpriteInstance::~KD_SpriteInstance()
{

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
	KD_Anim *anim = spr->anims[currentAnim];
	int animSize = anim->images.size();
	bool ret = false;

	// If we display at the maximum possible framerate
	if (framePerSec == 0)
	{
		currentFrame++;

		if (currentFrame == animSize && anim->next_anim != KD_NONEXTANIM)
		{
			currentAnim = anim->next_anim;
			currentFrame = 0;
			onFinishAnim();
			ret = true;
		}
		else if (currentFrame == animSize-1 && anim->next_anim == KD_NONEXTANIM)
		{
			onFinishAnim();
			ret = true;
		}
		else if (currentFrame == animSize && anim->next_anim == KD_NONEXTANIM)
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

		if (currentFrame >= animSize && anim->next_anim != KD_NONEXTANIM)
		{
			// Not exact because we round up the number of frames, but never mind....
			onFinishAnim();
			currentAnim = anim->next_anim;
			currentFrame = (int)(currentFrame) % animSize;
			ret = true;
		}
		else if (currentFrame >= animSize && anim->next_anim == KD_NONEXTANIM)
		{
			currentFrame = animSize-1;
			onFinishAnim();
			ret = true;
		}

		spr->Display(x,y, currentAnim, (int)currentFrame);
	}

	return ret;
}
