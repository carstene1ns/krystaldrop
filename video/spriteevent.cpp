#include "../global.h"

#include "spriteevent.h"
#include "spriteinstance.h"

KD_SpriteEvent::KD_SpriteEvent() : KD_MovableEvent()
{
	spr= NULL;
	displayFrom= KD_SPRITE_CENTERED;
}

KD_SpriteEvent::~KD_SpriteEvent()
{ DELETE (spr);
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
	DELETE (spr);
	spr = new KD_SpriteInstance(model);
}
