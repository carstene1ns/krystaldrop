#include "GlobalResourceSet.h"

KD_GlobalResourceSet *KD_GlobalResourceSet::singleton = NULL;

KD_GlobalResourceSet::KD_GlobalResourceSet() : KD_ResourceSet()
{

}

KD_GlobalResourceSet::~KD_GlobalResourceSet()
{

}

KD_GlobalResourceSet *KD_GlobalResourceSet::InitGlobalResourceSet()
{
	if (singleton==NULL)
		singleton = new KD_GlobalResourceSet();
	
	return singleton;
}

void KD_GlobalResourceSet::CloseGlobalResourceSet()
{
	if (singleton==NULL)
		return;

	delete singleton;
	singleton = NULL;
}

KD_GlobalResourceSet *KD_GlobalResourceSet::GetGlobalResource()
{
	return singleton;
}
