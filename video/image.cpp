#include "image.h"

#include <assert.h>

#include "../util/direct.h"
#include "../util/logfile.h"
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
	if (accFile == 0)
	{
		Load(fileName);
		return;
	}

	int idAcc = accFile->EntryId(fileName);

	if (idAcc<0)
	{
		switch (idAcc)
		{
		case ACC_ENTRYNOTFOUND:
			printf("File %s not found in ACC file %s\n", fileName, accFile->CurrentFile);
			KD_LogFile::printf("File %s not found in ACC file %s\n", fileName, accFile->CurrentFile);
			assert(0);
			return;
		case ACC_NOTINITIALIZED:
			printf("File %s not found: ACC File not properly initialized.\n",fileName);
			KD_LogFile::printf("File %s not found: ACC File not properly initialized.\n",fileName);
			assert(0);
			return;
		default:
			printf("Unknown error in ACC File. Aborting.\n");
			KD_LogFile::printf("Unknown error in ACC File. Aborting.\n");
			assert(0);
			return;
		}
	}

	void *ptr = (void *)accFile->EntryPtr(idAcc);

	SDL_RWops *sdlPtr = SDL_RWFromMem(ptr, accFile->EntryLength(idAcc));

	SDL_Surface *surfLoaded = IMG_Load_RW(sdlPtr, 0);

	assert(surfLoaded);

	SDL_FreeRW(sdlPtr);
	
	// Sets the RLE Acceleration of the sprite.
	SDL_SetAlpha(surfLoaded, SDL_SRCALPHA | SDL_RLEACCEL, 0);

	// Converts the surface to the display format
	SDL_Surface *convertedImage = SDL_DisplayFormatAlpha(surfLoaded);

	//// TRY TO COPY ALL THIS IN HARDWARE MEMORY:
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	    int amask = 0x000000ff;
	#else
	    int amask = 0xff000000;
	#endif
	SDL_Surface *surfHw = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, convertedImage->w, convertedImage->h, convertedImage->format->BitsPerPixel, convertedImage->format->Rmask, convertedImage->format->Gmask, convertedImage->format->Bmask, amask);

	////////////// A CONTINUER!!!!!!!!!!!!! /////////////////
	SDL_BlitSurface(convertedImage,0,surfHw,0);

	image = surfHw;

	// Free the old surface
	SDL_FreeSurface(surfLoaded);

	SDL_FreeSurface(convertedImage);
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

int KD_Image::getHeight()
{
	return image->h;
}

int KD_Image::getWidth()
{
	return image->w;
}
