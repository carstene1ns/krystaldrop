#include "Resource.h"

#include "../Video/Display.h"

KD_Resource::KD_Resource() : refCount(0)
{
}

KD_Resource::~KD_Resource()
{

}

int KD_Resource::GetRefCount()
{
	return refCount;
}
