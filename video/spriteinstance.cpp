#include "spriteinstance.h"

#include "sprite.h"
#include "Display.h"
#include "image.h"

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
    currentFrame= 0;
	ticks = SDL_GetTicks();
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
			onFinishAnim(currentAnim);
			currentAnim = anim->next_anim;
			currentFrame = 0;
			ret = true;
		}
		else if (currentFrame == animSize-1 && anim->next_anim == KD_NONEXTANIM)
		{
			onFinishAnim(currentAnim);
			ret = true;
		}
		else if (currentFrame == animSize && anim->next_anim == KD_NONEXTANIM)
		{
			currentFrame--;
			onFinishAnim(currentAnim);
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
			onFinishAnim(currentAnim);
			currentAnim = anim->next_anim;

			animSize = spr->anims[anim->next_anim]->images.size();

			currentFrame = (int)(currentFrame) % animSize;
			ret = true;
		}
		else if (currentFrame >= animSize && anim->next_anim == KD_NONEXTANIM)
		{
			currentFrame = animSize-1;
			onFinishAnim(currentAnim);
			ret = true;
		}

		spr->Display(x,y, currentAnim, (int)currentFrame);
	}

	return ret;
}

bool KD_SpriteInstance::DisplayCentered()
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
			onFinishAnim(currentAnim);
			currentAnim = anim->next_anim;
			currentFrame = 0;
			ret = true;
		}
		else if (currentFrame == animSize-1 && anim->next_anim == KD_NONEXTANIM)
		{
			onFinishAnim(currentAnim);
			ret = true;
		}
		else if (currentFrame == animSize && anim->next_anim == KD_NONEXTANIM)
		{
			currentFrame--;
			onFinishAnim(currentAnim);
			ret = true;
		}

		spr->Display(x-spr->getAnim(currentAnim)->getImage(currentFrame)->getWidth()/2,y-spr->getAnim(currentAnim)->getImage(currentFrame)->getHeight(), currentAnim, (int)currentFrame);
	}
	else
	{
		currentFrame += ((float)(SDL_GetTicks() - ticks))*framePerSec/1000.0f;

		ticks = SDL_GetTicks();

		if (currentFrame >= animSize && anim->next_anim != KD_NONEXTANIM)
		{
			// Not exact because we round up the number of frames, but never mind....
			onFinishAnim(currentAnim);
			currentAnim = anim->next_anim;

			animSize = spr->anims[anim->next_anim]->images.size();

			currentFrame = (int)(currentFrame) % animSize;
			ret = true;
		}
		else if (currentFrame >= animSize && anim->next_anim == KD_NONEXTANIM)
		{
			currentFrame = animSize-1;
			onFinishAnim(currentAnim);
			ret = true;
		}

		spr->Display(x-spr->getAnim(currentAnim)->getImage(currentFrame)->getWidth()/2,y-spr->getAnim(currentAnim)->getImage(currentFrame)->getHeight(), currentAnim, (int)currentFrame);
	}

	return ret;
}

void KD_SpriteInstance::onFinishAnim(int animNo)
{

}
