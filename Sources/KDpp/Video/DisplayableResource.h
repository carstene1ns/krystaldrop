#ifndef DisplayableResource_H
#define DisplayableResource_H

#include "../Tools/defines.h"

#include "../Resources/Resource.h"

#include <SDL/SDL.h>

class KD_DisplayableInstance;

/**
	Class containing a resource that can be Displayed when instanciated (an image, a sprite... but not a sprite instance).
	Note: images don't need to be isntanciated to be Displayed because they don't have any state (no animation).
  */
class DllExport KD_DisplayableResource : public KD_Resource
{
	
protected:
	/**
		The constructor is made private, it can only be called by inherited members
	*/
	KD_DisplayableResource();

	/**
		The destructor is made private, it can only be called by inherited members
	*/
	virtual ~KD_DisplayableResource();

public:
	virtual KD_DisplayableInstance *createInstance()=0;
	virtual void deleteInstance(KD_DisplayableInstance *instance);
	
};

#endif
