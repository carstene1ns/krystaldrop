#ifndef BackgroundController_H
#define BackgroundController_H

#include "../../KDpp/Controller/Controller.h"
#include "../../KDpp/Resources/ResourceSet.h"

class KD_Background;

class KD_BackgroundController : public KD_Controller, KD_ResourceSet
{ 
protected:
	KD_Background*     back;
	float flashTime;
  
public: 
    KD_BackgroundController();
    ~KD_BackgroundController();

	virtual bool Init();
	virtual bool ProcessEvent (int value);
	virtual bool Process();
	virtual bool Display();
	virtual bool Quit();
	virtual bool OnEnable();
	virtual bool OnDisable();

	void Flash();
};

#endif
