#include <assert.h>

#include "Display.h"
#include "image.h"
#include "sprite.h"
#include "spriteinstance.h"

KD_SpriteInstance::KD_SpriteInstance(KD_Sprite *spr)
{
	this->spr= spr;

	currentAnim= 0;
	currentFrame= 0;
	OnFinishCalled= 0;
	msecBetweenFrames= (int)(1000.0f/spr->framePerSeconds);
	x= 0;
	y= 0;
}

KD_SpriteInstance::~KD_SpriteInstance()
{
}

void KD_SpriteInstance::setFramesPerSeconds(float framePerSec)
{
	this->msecBetweenFrames = (int)(1000.0f/spr->framePerSeconds);
}

void KD_SpriteInstance::setAnim(int anim)
{
	currentAnim = anim;
    currentFrame= 0;
    OnFinishCalled= 0;  
  
	//ticks = SDL_GetTicks();
}

int KD_SpriteInstance::getAnim()
{
	return currentAnim;
}
/*   OLD ONE
bool KD_SpriteInstance::Display()
{
	KD_Anim *anim = spr->anims[currentAnim];
	int animSize = anim->images.size();
	bool ret = false;

	printf("start current frame: %f, anim size: %d nextanim: %d framePerSec:%f\n", currentFrame,animSize,anim->next_anim,framePerSec);
	fflush(stdout);

	// If we display at the maximum possible framerate
	if (framePerSec == 0)
	{
		currentFrame++;

		if (currentFrame >= animSize && anim->next_anim != KD_NONEXTANIM)
		{
			onFinishAnim(currentAnim);
			currentAnim = anim->next_anim;
			currentFrame = 0;
			ret = true;
		}
		else if (currentFrame >= animSize-1 && anim->next_anim == KD_NONEXTANIM)
		{
			onFinishAnim(currentAnim);
			ret = true;
		}
		else if (currentFrame >= animSize && anim->next_anim == KD_NONEXTANIM)
		{
			currentFrame--;
			onFinishAnim(currentAnim);
			ret = true;
		}
assert ( ((int)currentFrame)>=0 && ((int)currentFrame<animSize));
		spr->Display(x,y, currentAnim, (int)currentFrame);
	}
	else
	{
		currentFrame += Display::timeElapsed *framePerSec;
		//currentFrame += ((float)(SDL_GetTicks() - ticks))*framePerSec/1000.0f;
        //ticks = SDL_GetTicks();
	float lastCur=currentFrame;
			
		if (currentFrame >= animSize && anim->next_anim != KD_NONEXTANIM)
		{
			// Not exact because we round up the number of frames, but never mind....
 			onFinishAnim(currentAnim);
			currentAnim = anim->next_anim;

			animSize = spr->anims[anim->next_anim]->images.size();

			currentFrame = (int)(currentFrame) % animSize;
          assert ( ((int)currentFrame)>=0 && ((int)currentFrame<animSize));
			ret = true;
		}
		else if (currentFrame >= animSize && anim->next_anim == KD_NONEXTANIM)
		{
			currentFrame = animSize-1;
			onFinishAnim(currentAnim);
			ret = true;
		}
		//if (currentAnim==1) {printf("%f\n",currentFrame);spr->Display(x,y, currentAnim, 1);}
		//else

		//printf("after: %f\n", currentFrame);

		printf("current frame: %f, anim size: %d last current: %f nextanim: %d\n", currentFrame,animSize,lastCur,anim->next_anim);
		fflush(stdout);
		assert(((int)currentFrame)<animSize);
		spr->Display(x,y, currentAnim, (int)currentFrame);
	}

	return ret;
}
*/

bool KD_SpriteInstance::Display (short center)
{	KD_Anim *anim = spr->anims[currentAnim];
	int animSize = anim->images.size();
	bool ret= false;
 
  /* increase currentFrame if needed */
    if (OnFinishCalled== 0) // if the end of the animation has not been reached    
    { if (msecBetweenFrames== 0)
		currentFrame++; // display at the maximum framerate
      else              // display at a given framerate
		currentFrame+= Display::getTimeSlice (msecBetweenFrames);     
    }
    		
	if (currentFrame>= animSize) // the end of the anim has been reached
    { if (anim->next_anim!= KD_NONEXTANIM) // if there is a new anim afterwards
	  { onFinishAnim(currentAnim);
        /* do not set OnFinishCalled to 1 
           since a new anim starts */
        
		currentAnim= anim->next_anim;
        animSize= spr->anims[anim->next_anim]->images.size();
		currentFrame= currentFrame% animSize;
		assert(((int)currentFrame)< animSize); // sanity check						
	  }
	  else /* (anim->next_anim== KD_NONEXTANIM) */ // end of the animation
	  { currentFrame= animSize- 1;
		onFinishAnim(currentAnim);
        OnFinishCalled= 1;
        
		ret= true;
	  }
    }

	assert(currentFrame< animSize && currentFrame>= 0);
    
    if (center== 0)
    spr->Display(x,y, currentAnim, currentFrame);
    else
	spr->Display(x-spr->getAnim(currentAnim)->getImage(currentFrame)->getWidth()/2,
                 y-spr->getAnim(currentAnim)->getImage(currentFrame)->getHeight(),
                 currentAnim, currentFrame);

	return ret;
}


void KD_SpriteInstance::onFinishAnim (int animNo)
{
}
