#include "spriteevent.h"

#include "spriteinstance.h"

#include <assert.h>

KD_SpriteEvent::KD_SpriteEvent() : KD_MovableEvent()
{
	spr=0;
	displayFrom = KD_SPRITE_CENTERED;
}

KD_SpriteEvent::~KD_SpriteEvent()
{
	if (spr)
		delete spr;
}

void KD_SpriteEvent::UpdateMovable(float timeElapsed)
{
}

void KD_SpriteEvent::DisplayMovable()
{
	assert(spr);

	spr->Display(x,y,displayFrom,red,green,blue,alpha,resizeX,resizeY,x,y,angle);
}

void KD_SpriteEvent::displayFromTopLeft()
{
	displayFrom = KD_SPRITE_TOP_LEFT;
}

void KD_SpriteEvent::displayFromCenter()
{
	displayFrom = KD_SPRITE_CENTERED;
}

void KD_SpriteEvent::displayFromCenterBottom()
{
	displayFrom = KD_SPRITE_CENTERED_HORIZ;
}

void KD_SpriteEvent::setSprite(KD_Sprite *model)
{
	if (spr)
		delete spr;
	spr = new KD_SpriteInstance(model);
}
