#ifndef GlobalResourceSet_H
#define GlobalResourceSet_H

#include "../Tools/defines.h"
#include "ResourceSet.h"

/**
	The GlobalResourceSet references the ressources that must be easily accessible by the whole application and not stored in only a controller for instance.
	Typically, this would be resources like fonts.
	It is in fact a singleton on top of a classic ResourceSet
  */
class DllExport KD_GlobalResourceSet : public KD_ResourceSet
{
	KD_GlobalResourceSet();
	virtual ~KD_GlobalResourceSet();
	
	static KD_GlobalResourceSet *singleton;

public:
	static KD_GlobalResourceSet *InitGlobalResourceSet();
	static void CloseGlobalResourceSet();
	static KD_GlobalResourceSet *GetGlobalResource();

};

#endif
