#include "sprite.h"

#include "SDL/SDL_image.h"

#include "Display.h"

#include "../util/direct.h"

KD_Anim::KD_Anim()
{

}

KD_Anim::~KD_Anim()
{
	for (int i=0; i<images.size(); i++)
		SDL_FreeSurface(images[i]);
}

void KD_Anim::addFileImage(char *name)
{
	// Load the surface
	SDL_Surface *surfLoaded = IMG_Load(name);

	// Converts the surface to the display format
	SDL_Surface *surfDisp = SDL_DisplayFormat(surfLoaded);

	// Free the old surface
	SDL_FreeSurface(surfLoaded);
	
	// Add the converted surface to the anim
	addSurface(surfDisp);
}

void KD_Anim::addFileImageFromACC(TACCRes *accFile, char *name)
{
	int idAcc = accFile->EntryId(name);
	void *ptr = (void *)accFile->EntryPtr(idAcc);

	SDL_RWops *sdlPtr = SDL_RWFromMem(ptr, accFile->EntryLength(idAcc));

	SDL_Surface *surfLoaded = IMG_Load_RW(sdlPtr, 0);

	SDL_FreeRW(sdlPtr);
	
	// Converts the surface to the display format
	SDL_Surface *surfDisp = SDL_DisplayFormat(surfLoaded);

	// Free the old surface
	SDL_FreeSurface(surfLoaded);
	
	// Add the converted surface to the anim
	addSurface(surfDisp);
}

void KD_Anim::addSurface(SDL_Surface *surf)
{
	images.push_back(surf);
}

void KD_Anim::setColorKey(Uint32 key)
{
	for (int i=0; i<images.size(); i++)
		SDL_SetColorKey(images[i], SDL_SRCCOLORKEY , key);
}

void KD_Anim::setColorKey(Uint8 r, Uint8 g, Uint8 b)
{
	setColorKey(SDL_MapRGB(Display::screen->format, r, g, b));
}

void KD_Anim::Display(int x, int y, int frame)
{
	SDL_Rect rect ;
	rect.x = x;
	rect.y = y;

	SDL_BlitSurface(images[frame], 0, Display::screen, &rect);
}

/************************************************************************************/

KD_Sprite::KD_Sprite()
{

}

KD_Sprite::~KD_Sprite()
{
	for (int i=0; i<anims.size(); i++)
		delete anims[i];
}

void KD_Sprite::Display(int x, int y, int anim, int frame)
{
	anims[anim]->Display(x, y, frame);
}

void KD_Sprite::setColorKey(Uint32 key)
{
	for (int i=0; i<anims.size(); i++)
		anims[i]->setColorKey(key);	
}

void KD_Sprite::setColorKey(Uint8 r, Uint8 g, Uint8 b)
{
	for (int i=0; i<anims.size(); i++)
		anims[i]->setColorKey(r,g,b);	
}

KD_Anim *KD_Sprite::newAnim()
{
	KD_Anim *anim = new KD_Anim();
	anims.push_back(anim);
	return anim;
}
