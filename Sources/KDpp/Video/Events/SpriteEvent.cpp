//#include "../global.h"

#include "SpriteEvent.h"
#include "../DisplayableInstance.h"
#include "../DisplayableResource.h"

#include <assert.h>

KD_SpriteEvent::KD_SpriteEvent() : KD_MovableEvent()
{
	spr= NULL;
//	DisplayFrom= KD_SPRITE_CENTERED;
}

KD_SpriteEvent::~KD_SpriteEvent()
{ //DELETE (spr);
	if (spr)
		delete spr;
}

void KD_SpriteEvent::UpdateMovable(float timeElapsed)
{
}

void KD_SpriteEvent::DisplayMovable()
{
	assert(spr);
	
	spr->DisplayColorZoomRotate(x,y,red,green,blue,alpha,resizeX,resizeY,x,y,angle);
}

/*void KD_SpriteEvent::DisplayFromTopLeft()
{
	DisplayFrom = KD_SPRITE_TOP_LEFT;
}

void KD_SpriteEvent::DisplayFromCenter()
{
	DisplayFrom = KD_SPRITE_CENTERED;
}

void KD_SpriteEvent::DisplayFromCenterBottom()
{
	DisplayFrom = KD_SPRITE_CENTERED_HORIZ;
}*/

void KD_SpriteEvent::setSprite(KD_DisplayableResource *model)
{
	//DELETE (spr);
	if (spr)
	{
		spr = model->createInstance();
	}
	//spr = new KD_SpriteInstance(model);
}
