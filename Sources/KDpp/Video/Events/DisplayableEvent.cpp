//#include "../../global.h"
#include <assert.h>

#include "DisplayableEvent.h"
#include "../DisplayableInstance.h"
#include "../DisplayableResource.h"

KD_DisplayableEvent::KD_DisplayableEvent() : KD_MovableEvent()
{
	spr= NULL;
}

KD_DisplayableEvent::~KD_DisplayableEvent()
{ 
	//DELETE (spr);
	if (spr)
	{
		model->deleteInstance(spr);
	}
}

void KD_DisplayableEvent::UpdateMovable(float timeElapsed)
{
}

void KD_DisplayableEvent::DisplayMovable()
{
	assert(spr);

	spr->DisplayColorZoomRotate(x,y,red,green,blue,alpha,resizeX,resizeY,0,0/*x,y*/,angle);
}

void KD_DisplayableEvent::setDisplayableResource(KD_DisplayableResource *model)
{
	this->model = model;
	if (spr)
	{
		delete spr;
	}
	spr = model->createInstance();
}
