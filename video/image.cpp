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

	// WE MUST BLIT THE ALPHA CHANNEL TOO!
	int i,j;
	for (j=0; j<convertedImage->h ; j++)
		for (i=0; i<convertedImage->w ; i++)
		{
			((unsigned int *)surfHw->pixels)[i+j*(surfHw->pitch>>2)] = ((unsigned int *)convertedImage->pixels)[i+j*(convertedImage->pitch>>2)];
		}

	image = surfHw;

	// Free the old surface
	SDL_FreeSurface(surfLoaded);

	SDL_FreeSurface(convertedImage);

	// Now, let's try to detect if there is any alpha channel that would not be 255 here!
	bool isAlpha = false;

	
	for (j=0; j<surfHw->h ; j++)
		for (i=0; i<surfHw->w ; i++)
		{
			int alpha;
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			alpha = (((unsigned int *)surfHw->pixels)[i+j*(surfHw->pitch>>2)] & 0x000000ff);
		#else
			alpha = ((((unsigned int *)surfHw->pixels)[i+j*(surfHw->pitch>>2)] & 0xff000000)>>24);
		#endif
			if (alpha!=255)
			{
				isAlpha = true;
				goto endTestAlpha;
			}
		}

endTestAlpha:

	//printf("%d\n",(((((unsigned int *)surfHw->pixels)[0] & 0xff000000)>>24)));
	//if (isAlpha == false) printf("Pas de alpha détécté dans le fichier %s\n",fileName);

	if (isAlpha == true)
		SDL_SetAlpha(image, SDL_SRCALPHA | SDL_RLEACCEL, 0);
	else
		SDL_SetAlpha(image, SDL_RLEACCEL, 0);
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

void KD_Image::disableAlpha()
{
	SDL_SetAlpha(image, SDL_RLEACCEL, 0);
}

void KD_Image::enableAlpha()
{
	SDL_SetAlpha(image, SDL_SRCALPHA | SDL_RLEACCEL, 0);
}

void KD_Image::enableAlpha(int alpha)
{
	SDL_SetAlpha(image, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
}

void KD_Image::convertToColorKey(Uint8 r, Uint8 g, Uint8 b, int alphaTrigger)
{
	SDL_Surface *surf = SDL_CreateRGBSurface(SDL_HWSURFACE, image->w, image->h, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, 0);

	unsigned int key = SDL_MapRGB(Display::screen->format, r, g, b);

	for (int j=0; j<image->h ; j++)
		for (int i=0; i<image->w ; i++)
		{
			int alpha;
			int pixel=((unsigned int *)image->pixels)[i+j*(image->pitch>>2)];
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			alpha = pixel & 0x000000ff;
		#else
			alpha = (pixel & 0xff000000)>>24;
		#endif
			if (alpha<alphaTrigger)
				((unsigned int *)surf->pixels)[i+j*(surf->pitch>>2)] = key;
			else
				((unsigned int *)surf->pixels)[i+j*(surf->pitch>>2)] = pixel;

		}

	disableAlpha();
	setColorKey(r,g,b);

	SDL_FreeSurface(image);
	image=surf;
}
