#include "sprite.h"

#include "SDL/SDL_image.h"

#include "Display.h"
#include "imagemanager.h"
#include "image.h"

#include "../util/direct.h"

KD_Anim::KD_Anim()
{

}

KD_Anim::~KD_Anim()
{
	KD_ImageManager *imgMgr = KD_ImageManager::getImageManager();


	for (unsigned int i=0; i<images.size(); i++)
		imgMgr->releaseImage(images[i]);
		//SDL_FreeSurface(images[i]);
}

void KD_Anim::addFileImage(char *name)
{
	KD_ImageManager *imgMgr = KD_ImageManager::getImageManager();

	imgMgr->Load(name);
	addSurface(imgMgr->getImage(name));

/*	// Load the surface
	SDL_Surface *surfLoaded = IMG_Load(name);

	// Converts the surface to the display format
	SDL_Surface *surfDisp = SDL_DisplayFormatAlpha(surfLoaded);

	// Free the old surface
	SDL_FreeSurface(surfLoaded);
	
	// Add the converted surface to the anim
	addSurface(surfDisp);*/
}

void KD_Anim::addFileImageFromACC(TACCRes *accFile, char *name)
{
	KD_ImageManager *imgMgr = KD_ImageManager::getImageManager();

	imgMgr->Load(accFile, name);
	addSurface(imgMgr->getImage(name));

/*	int idAcc = accFile->EntryId(name);
	void *ptr = (void *)accFile->EntryPtr(idAcc);

	SDL_RWops *sdlPtr = SDL_RWFromMem(ptr, accFile->EntryLength(idAcc));

	SDL_Surface *surfLoaded = IMG_Load_RW(sdlPtr, 0);

	SDL_FreeRW(sdlPtr);
	
	// Converts the surface to the display format
	SDL_Surface *surfDisp = SDL_DisplayFormatAlpha(surfLoaded);

	// Free the old surface
	SDL_FreeSurface(surfLoaded);
	
	// Add the converted surface to the anim
	addSurface(surfDisp);*/
}

void KD_Anim::addSurface(KD_Image *surf)
{
	images.push_back(surf);
}

void KD_Anim::setColorKey(Uint32 key)
{
	for (unsigned int i=0; i<images.size(); i++)
		images[i]->setColorKey(key);
}

void KD_Anim::setColorKey(Uint8 r, Uint8 g, Uint8 b)
{
	for (unsigned int i=0; i<images.size(); i++)
		images[i]->setColorKey(r,g,b);
}

void KD_Anim::Display(int x, int y, int frame)
{
	images[frame]->Display(x,y);
}

/************************************************************************************/

KD_Sprite::KD_Sprite()
{

}

KD_Sprite::~KD_Sprite()
{
	for (unsigned int i=0; i<anims.size(); i++)
		delete anims[i];
}

void KD_Sprite::Display(int x, int y, int anim, int frame)
{
	anims[anim]->Display(x, y, frame);
}

void KD_Sprite::setColorKey(Uint32 key)
{
	for (unsigned int i=0; i<anims.size(); i++)
		anims[i]->setColorKey(key);	
}

void KD_Sprite::setColorKey(Uint8 r, Uint8 g, Uint8 b)
{
	for (unsigned int i=0; i<anims.size(); i++)
		anims[i]->setColorKey(r,g,b);	
}

KD_Anim *KD_Sprite::newAnim()
{
	KD_Anim *anim = new KD_Anim();
	anims.push_back(anim);
	return anim;
}
