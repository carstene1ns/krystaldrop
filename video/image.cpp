#include "image.h"

#include <assert.h>

#include "../util/direct.h"
#include "Display.h"

#include "SDL/SDL_image.h"


KD_Image::KD_Image() : image(0), countRef(0)
{
}

KD_Image::~KD_Image()
{
	if (image!=0)
		SDL_FreeSurface(image);
}

void KD_Image::Load(char *fileName)
{
	// Load the surface
	SDL_Surface *surfLoaded = IMG_Load(fileName);

	assert(surfLoaded);

	// Converts the surface to the display format
	image = SDL_DisplayFormatAlpha(surfLoaded);

	// Free the old surface
	SDL_FreeSurface(surfLoaded);
}

void KD_Image::Load(TACCRes *accFile, char *fileName)
{
	int idAcc = accFile->EntryId(fileName);
	void *ptr = (void *)accFile->EntryPtr(idAcc);

	SDL_RWops *sdlPtr = SDL_RWFromMem(ptr, accFile->EntryLength(idAcc));

	SDL_Surface *surfLoaded = IMG_Load_RW(sdlPtr, 0);

	assert(surfLoaded);

	SDL_FreeRW(sdlPtr);
	
	// Converts the surface to the display format
	image = SDL_DisplayFormatAlpha(surfLoaded);

	// Free the old surface
	SDL_FreeSurface(surfLoaded);
}

void KD_Image::Display(int x, int y)
{
	SDL_Rect rect ;
	rect.x = x;
	rect.y = y;

	SDL_BlitSurface(image, 0, Display::screen, &rect);
}

void KD_Image::setColorKey(Uint32 key)
{
	SDL_SetColorKey(image, SDL_SRCCOLORKEY , key);
}

void KD_Image::setColorKey(Uint8 r, Uint8 g, Uint8 b)
{
	setColorKey(SDL_MapRGB(Display::screen->format, r, g, b));
}

SDL_Surface *KD_Image::getSDL_Surface()
{
	return image;
}