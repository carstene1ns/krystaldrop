#include <assert.h>

#include "Display.h"
#include "Image.h"
#include "Sprite.h"
#include "SpriteInstance.h"

KD_SpriteInstance::KD_SpriteInstance(KD_Sprite *spr) : KD_DisplayableInstance(spr)
{
//	this->spr= spr;

	currentAnim= 0;
	currentFrame= 0;
	OnFinishCalled= 0;
	msecBetweenFrames= (int)(1000.0f/((KD_Sprite *)resource)->framePerSeconds);
}

KD_SpriteInstance::~KD_SpriteInstance()
{
}

void KD_SpriteInstance::setFramesPerSeconds(float framePerSec)
{
	this->msecBetweenFrames = (int)(1000.0f/((KD_Sprite *)resource)->framePerSeconds);
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
	KD_Anim *anim = ((KD_Sprite *)resource)->anims[currentAnim];
	int animSize = anim->images.size();
	bool ret = false;

	printf("start current frame: %f, anim size: %d nextanim: %d framePerSec:%f\n", currentFrame,animSize,anim->next_anim,framePerSec);
	fflush(stdout);

	// If we Display at the maximum possible framerate
	if (framePerSec == 0)
	{
		currentFrame++;

		if (currentFrame >= animSize && anim->next_anim != KD_NONEXTANIM)
		{
			OnFinishAnim(currentAnim);
			currentAnim = anim->next_anim;
			currentFrame = 0;
			ret = true;
		}
		else if (currentFrame >= animSize-1 && anim->next_anim == KD_NONEXTANIM)
		{
			OnFinishAnim(currentAnim);
			ret = true;
		}
		else if (currentFrame >= animSize && anim->next_anim == KD_NONEXTANIM)
		{
			currentFrame--;
			OnFinishAnim(currentAnim);
			ret = true;
		}
assert ( ((int)currentFrame)>=0 && ((int)currentFrame<animSize));
		((KD_Sprite *)resource)->Display(x,y, currentAnim, (int)currentFrame);
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
 			OnFinishAnim(currentAnim);
			currentAnim = anim->next_anim;

			animSize = ((KD_Sprite *)resource)->anims[anim->next_anim]->images.size();

			currentFrame = (int)(currentFrame) % animSize;
          assert ( ((int)currentFrame)>=0 && ((int)currentFrame<animSize));
			ret = true;
		}
		else if (currentFrame >= animSize && anim->next_anim == KD_NONEXTANIM)
		{
			currentFrame = animSize-1;
			OnFinishAnim(currentAnim);
			ret = true;
		}
		//if (currentAnim==1) {printf("%f\n",currentFrame);((KD_Sprite *)resource)->Display(x,y, currentAnim, 1);}
		//else

		//printf("after: %f\n", currentFrame);

		printf("current frame: %f, anim size: %d last current: %f nextanim: %d\n", currentFrame,animSize,lastCur,anim->next_anim);
		fflush(stdout);
		assert(((int)currentFrame)<animSize);
		((KD_Sprite *)resource)->Display(x,y, currentAnim, (int)currentFrame);
	}

	return ret;
}
*/

/*bool KD_SpriteInstance::Display (short center)
{	KD_Anim *anim = ((KD_Sprite *)resource)->anims[currentAnim];
	int animSize = (int)anim->images.size();
	bool ret= false;
 
  // increase currentFrame if needed 
    if (OnFinishCalled== 0) // if the end of the animation has not been reached    
    { if (msecBetweenFrames== 0)
		currentFrame++; // Display at the maximum framerate
      else              // Display at a given framerate
		currentFrame+= Display::getTimeSlice (msecBetweenFrames);     
    }
    		
	if (currentFrame>= animSize) // the end of the anim has been reached
    { if (anim->next_anim!= KD_NONEXTANIM) // if there is a new anim afterwards
	  { OnFinishAnim(currentAnim);
        // do not set OnFinishCalled to 1 
        // since a new anim starts 
        
		currentAnim= anim->next_anim;
        animSize= (int)((KD_Sprite *)resource)->anims[anim->next_anim]->images.size();
		currentFrame= currentFrame% animSize;
		assert(((int)currentFrame)< animSize); // sanity check						
	  }
	  else // (anim->next_anim== KD_NONEXTANIM)  // end of the animation
	  { currentFrame= animSize- 1;
		OnFinishAnim(currentAnim);
        OnFinishCalled= 1;
        
		ret= true;
	  }
    }

	assert(currentFrame< animSize && currentFrame>= 0);
    
	switch (center)
	{
	case KD_SPRITE_TOP_LEFT:
		((KD_Sprite *)resource)->Display(x,y, currentAnim, currentFrame);
		break;
	case KD_SPRITE_CENTERED_HORIZ:
		((KD_Sprite *)resource)->Display(x-((KD_Sprite *)resource)->getAnim(currentAnim)->getImage(currentFrame)->GetWidth()/2,
                 y-((KD_Sprite *)resource)->getAnim(currentAnim)->getImage(currentFrame)->GetHeight(),
                 currentAnim, currentFrame);
		break;
	case KD_SPRITE_CENTERED:
		((KD_Sprite *)resource)->Display(x-((KD_Sprite *)resource)->getAnim(currentAnim)->getImage(currentFrame)->GetWidth()/2,
                 y-((KD_Sprite *)resource)->getAnim(currentAnim)->getImage(currentFrame)->GetHeight()/2,
                 currentAnim, currentFrame);
		break;
	default:
		printf("Unknown parameter center specified in KD_SpriteInstance::Display!\n");
		break;
	}

	return ret;
}

bool KD_SpriteInstance::Display(int x, int y, short center, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle)
{
	KD_Anim *anim = ((KD_Sprite *)resource)->anims[currentAnim];
	int animSize = (int)anim->images.size();
	bool ret= false;
 
  // increase currentFrame if needed 
    if (OnFinishCalled== 0) // if the end of the animation has not been reached    
    { if (msecBetweenFrames== 0)
		currentFrame++; // Display at the maximum framerate
      else              // Display at a given framerate
		currentFrame+= Display::getTimeSlice (msecBetweenFrames);     
    }
    		
	if (currentFrame>= animSize) // the end of the anim has been reached
    { if (anim->next_anim!= KD_NONEXTANIM) // if there is a new anim afterwards
	  { OnFinishAnim(currentAnim);
        // do not set OnFinishCalled to 1 
        //   since a new anim starts 
        
		currentAnim= anim->next_anim;
        animSize= (int)((KD_Sprite *)resource)->anims[anim->next_anim]->images.size();
		currentFrame= currentFrame% animSize;
		assert(((int)currentFrame)< animSize); // sanity check						
	  }
	  else // (anim->next_anim== KD_NONEXTANIM)  // end of the animation
	  { currentFrame= animSize- 1;
		OnFinishAnim(currentAnim);
        OnFinishCalled= 1;
        
		ret= true;
	  }
    }

	assert(currentFrame< animSize && currentFrame>= 0);
    
	switch (center)
	{
	case KD_SPRITE_TOP_LEFT:
		((KD_Sprite *)resource)->Display(x,y, currentAnim, currentFrame,r,g,b,alpha,resizeX,resizeY,rotX,rotY,angle);
		break;
	case KD_SPRITE_CENTERED_HORIZ:
		((KD_Sprite *)resource)->Display(x-((KD_Sprite *)resource)->getAnim(currentAnim)->getImage(currentFrame)->GetWidth()/2,
                 y-((KD_Sprite *)resource)->getAnim(currentAnim)->getImage(currentFrame)->GetHeight(),
                 currentAnim, currentFrame,r,g,b,alpha,resizeX,resizeY,rotX,rotY,angle);
		break;
	case KD_SPRITE_CENTERED:
		((KD_Sprite *)resource)->Display(x-((KD_Sprite *)resource)->getAnim(currentAnim)->getImage(currentFrame)->GetWidth()/2,
                 y-((KD_Sprite *)resource)->getAnim(currentAnim)->getImage(currentFrame)->GetHeight()/2,
                 currentAnim, currentFrame,r,g,b,alpha,resizeX,resizeY,rotX,rotY,angle);
		break;
	default:
		printf("Unknown parameter center specified in KD_SpriteInstance::Display!\n");
		break;
	}

	return ret;
}*/

void KD_SpriteInstance::OnFinishAnim (int animNo)
{

}

void KD_SpriteInstance::computeNextFrame()
{
	KD_Anim *anim = ((KD_Sprite *)resource)->anims[currentAnim];
	int animSize = (int)anim->images.size();
	bool ret= false;
 
	// increase currentFrame if needed 
    if (OnFinishCalled== 0) // if the end of the animation has not been reached    
    { if (msecBetweenFrames== 0)
		currentFrame++; // Display at the maximum framerate
      else              // Display at a given framerate
		currentFrame+= Display::GetTimeSlice (msecBetweenFrames);     
    }
    		
	if (currentFrame>= animSize) // the end of the anim has been reached
    { if (anim->next_anim!= KD_NONEXTANIM) // if there is a new anim afterwards
	  { OnFinishAnim(currentAnim);
        // do not set OnFinishCalled to 1 
        //   since a new anim starts 
        
		currentAnim= anim->next_anim;
        animSize= (int)((KD_Sprite *)resource)->anims[anim->next_anim]->images.size();
		currentFrame= currentFrame% animSize;
		assert(((int)currentFrame)< animSize); // sanity check						
	  }
	  else // (anim->next_anim== KD_NONEXTANIM)  // end of the animation
	  { currentFrame= animSize- 1;
		OnFinishAnim(currentAnim);
        OnFinishCalled= 1;
        
		ret= true;
	  }
    }

	assert(currentFrame< animSize && currentFrame>= 0);
}

void KD_SpriteInstance::Display(float x, float y)
{
	computeNextFrame();
	((KD_Sprite *)resource)->Display(currentAnim, currentFrame, x,y);
}

void KD_SpriteInstance::DisplayAlpha(float x, float y, int alpha)
{
	computeNextFrame();
	((KD_Sprite *)resource)->DisplayAlpha(currentAnim, currentFrame, x,y,alpha);
}

void KD_SpriteInstance::DisplayColorZoom(float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY)
{
	computeNextFrame();
	((KD_Sprite *)resource)->DisplayColorZoom(currentAnim, currentFrame, x,y,r,g,b,alpha,resizeX,resizeY);
}

void KD_SpriteInstance::DisplayColorZoomRotate(float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle)
{
	computeNextFrame();
	((KD_Sprite *)resource)->DisplayColorZoomRotate(currentAnim, currentFrame, x,y,r,g,b,alpha,resizeX,resizeY,rotX,rotY,angle);
}

void KD_SpriteInstance::DisplayAlphaPart(float x, float y, int alpha, int xPart, int yPart, int widthPart, int heightPart)
{
	computeNextFrame();
	((KD_Sprite *)resource)->DisplayAlphaPart(currentAnim, currentFrame, x,y,alpha, xPart, yPart, widthPart, heightPart);
}

void KD_SpriteInstance::DisplayFullParameters(float x1, float y1, int r1, int g1, int b1, int alpha1, float x2, float y2, int r2, int g2, int b2, int alpha2, float x3, float y3, int r3, int g3, int b3, int alpha3, float x4, float y4, int r4, int g4, int b4, int alpha4)
{
	computeNextFrame();
	((KD_Sprite *)resource)->DisplayFullParameters(currentAnim, currentFrame, x1,y1,r1,g1,b1,alpha1, x2,y2,r2,g2,b2,alpha2, x3,y3,r3,g3,b3,alpha3, x4,y4,r4,g4,b4,alpha4);
}

void KD_SpriteInstance::DisplayFullParameters(float x1, float y1, const KD_Color &c1, int alpha1, float x2, float y2, const KD_Color &c2, int alpha2, float x3, float y3, const KD_Color &c3, int alpha3, float x4, float y4, const KD_Color &c4, int alpha4)
{
	computeNextFrame();
	((KD_Sprite*)resource)->DisplayFullParameters(currentAnim, currentFrame, x1,y1,(int)(c1.r*255),(int)(c1.g*255),(int)(c1.b*255),alpha1,x2,y2,(int)(c2.r*255),(int)(c2.g*255),(int)(c2.b*255),alpha2,x3,y3,(int)(c3.r*255),(int)(c3.g*255),(int)(c3.b*255),alpha3,x4,y4,(int)(c4.r*255),(int)(c4.g*255),(int)(c4.b*255),alpha4);
}

int KD_SpriteInstance::GetHeight()
{
	computeNextFrame();
	return ((KD_Sprite *)resource)->GetHeight(currentAnim, currentFrame);
}

int KD_SpriteInstance::GetWidth()
{
	return ((KD_Sprite *)resource)->GetWidth(currentAnim, currentFrame);
}
