#ifndef Resource_H
#define Resource_H

#include "../Tools/defines.h"

/**
	Class inherited by all resources (images, sounds, sprites...).
	Resources are handled by resources managers.
  */
class DllExport KD_Resource
{
	friend class KD_ResourceManager;
protected:
	/**
		The constructor is made protected, it can only be called by inherited classes and KD_ResourceManager.
	*/
	KD_Resource();

	/**
		The destructor is made private, it can only be called by ImageManager which is a friend class of Image.
	*/
	virtual ~KD_Resource();

	/**
		The reference count for that given Image
	*/
	int refCount;


public:

	/**
		Returns the reference count of the resource.
		Mainly used for debugging purposes.
	*/
	int GetRefCount();
};

#endif
