#include "DisplayableResource.h"

#include "DisplayableInstance.h"

KD_DisplayableResource::KD_DisplayableResource() : KD_Resource()
{

}

KD_DisplayableResource::~KD_DisplayableResource()
{

}

void KD_DisplayableResource::deleteInstance(KD_DisplayableInstance *instance)
{
	delete instance;
}
