#include <assert.h>

#include "SdlImage.h"
//#include "../util/direct.h"
//#include "../util/logfile.h"
#include "Display.h"
#include "SDL/SDL_image.h"
#include "SDL_rotozoom.h"



KD_SDLImage::KD_SDLImage() : KD_Image() , image(0)
{
}

KD_SDLImage::~KD_SDLImage()
{
	if (image!=0)
		SDL_FreeSurface(image);
}

void KD_SDLImage::makeImageFromSDLSurface(SDL_Surface *surf)
{
	image = SDL_DisplayFormatAlpha(surf);
	SDL_FreeSurface(surf);
}

void KD_SDLImage::Load(KD_FilePath fileName)
{
	if (!fileName.IsArchived())
	{
		// Load the surface
		SDL_Surface *surfLoaded = IMG_Load(fileName.GetPath().c_str());

		assert(surfLoaded);

		// Converts the surface to the Display format
		image = SDL_DisplayFormatAlpha(surfLoaded);

		// Free the old surface
		SDL_FreeSurface(surfLoaded);
	}
	else
	{
		// Put here the loading from archive files.
	}
}

/*void KD_SDLImage::Load(TACCRes *accFile, char *fileName)
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
			printf("File %s not found: ACC File not properly Initialized.\n",fileName);
			KD_LogFile::printf("File %s not found: ACC File not properly Initialized.\n",fileName);
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

	// Converts the surface to the Display format
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

	SDL_LockSurface(surfHw);
	
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

	SDL_UnlockSurface(surfHw);

	//printf("%d\n",(((((unsigned int *)surfHw->pixels)[0] & 0xff000000)>>24)));
	//if (isAlpha == false) printf("Pas de alpha détécté dans le fichier %s\n",fileName);

	if (isAlpha == true)
		SDL_SetAlpha(image, SDL_SRCALPHA | SDL_RLEACCEL, 0);
	else
		SDL_SetAlpha(image, SDL_RLEACCEL, 0);
}*/

void KD_SDLImage::Display(float x, float y)
{
	SDL_Rect rect ;
	rect.x = (int) x;
	rect.y = (int) y;
  assert (image);
  
	SDL_BlitSurface(image, 0, Display::screen, &rect);
}

void KD_SDLImage::DisplayAlpha(float x, float y, int alpha)
{
	if (alpha == 255)
	{
		Display( (int) x, (int) y);
		return;
	}

	SDL_Rect rect;
	rect.x = (int) x;
	rect.y = (int) y;
	assert (image);
  
	SDL_SetAlpha(image, SDL_RLEACCEL | SDL_SRCALPHA , alpha);
	SDL_BlitSurface(image, 0, Display::screen, &rect);
	SDL_SetAlpha(image, SDL_RLEACCEL , 0);
}

/*void KD_SDLImage::DisplayRotateX (int x, int y, float alpha)
{ Display (x, y); }

void KD_SDLImage::DisplayRotateY (int x, int y, float alpha)
{ Display (x, y); }*/

void KD_SDLImage::DisplayColorZoom(float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY)
{
	DisplayAlpha((int)x, (int)y,alpha);
}

void KD_SDLImage::DisplayColorZoomRotate(float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY, float rotX, float rotY, float angle)
{
	DisplayAlpha((int)x, (int)y,alpha);
}

void KD_SDLImage::DisplayAlphaPart(float x, float y, int alpha, int xPart, int yPart, int widthPart, int heightPart)
{
	SDL_Rect srcRect;
	srcRect.x = xPart;
	srcRect.y = yPart;
	srcRect.w = widthPart;
	srcRect.h = heightPart;

	SDL_Rect destRect ;
	destRect.x = (int) x;
	destRect.y = (int) y;
	assert (image);
  
	SDL_SetAlpha(image, SDL_RLEACCEL | SDL_SRCALPHA , alpha);
	SDL_BlitSurface(image, &srcRect, Display::screen, &destRect);
	SDL_SetAlpha(image, SDL_RLEACCEL , 0);
}

void KD_SDLImage::DisplayFullParameters(float x1, float y1, int r1, int g1, int b1, int alpha1, float x2, float y2, int r2, int g2, int b2, int alpha2, float x3, float y3, int r3, int g3, int b3, int alpha3, float x4, float y4, int r4, int g4, int b4, int alpha4)
{
	DisplayAlpha( (int) x1, (int) y1,alpha1);
}



void KD_SDLImage::SetColorKey(Uint32 key)
{
	SDL_SetColorKey(image, SDL_SRCCOLORKEY | SDL_RLEACCEL, key);
}

void KD_SDLImage::SetColorKey(Uint8 r, Uint8 g, Uint8 b)
{
	SetColorKey(SDL_MapRGB(Display::screen->format, r, g, b));
}

SDL_Surface *KD_SDLImage::getSDL_Surface()
{
	return image;
}

int KD_SDLImage::getHeight()
{
	return image->h;
}

int KD_SDLImage::getWidth()
{
	return image->w;
}

void KD_SDLImage::DisableAlpha()
{
	SDL_SetAlpha(image, SDL_RLEACCEL, 0);
}

void KD_SDLImage::EnableAlpha()
{
	SDL_SetAlpha(image, SDL_SRCALPHA | SDL_RLEACCEL, 0);
}


void KD_SDLImage::ConvertToColorKey(Uint8 r, Uint8 g, Uint8 b, int alphaTrigger)
{
	unsigned int key = SDL_MapRGB(Display::screen->format, r, g, b);
	ConvertToColorKey(key, alphaTrigger);
}

void KD_SDLImage::ConvertToColorKey(unsigned int key, int alphaTrigger)
{
	if (image->format->Amask == 0) return;

	SDL_Surface *surf = SDL_CreateRGBSurface(SDL_HWSURFACE, image->w, image->h, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, 0);

	SDL_LockSurface(image);
	SDL_LockSurface(surf);

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

	SDL_UnlockSurface(image);
	SDL_UnlockSurface(surf);

	SDL_FreeSurface(image);
	image=surf;

	DisableAlpha();
	SetColorKey(key);
}

bool KD_SDLImage::resize(float ratio)
{
	SDL_Surface *newImage;
  
	// If resized image is not wide enough to be seen, do not resize it.
	if (image->w*ratio < 1 || image->h*ratio < 1)
		return false;
	
	newImage=zoomSurface(image, ratio, ratio, SMOOTHING_ON);

	SDL_FreeSurface(image);
	image=newImage;
	return true;
}

KD_Image *KD_SDLImage::copy()
{
	KD_SDLImage *copy = new KD_SDLImage();

	copy->image = SDL_DisplayFormatAlpha(image);

	return copy;
}

KD_Image *KD_SDLImage::copy(int x, int y, int width, int height)
{
	// First, let's correct any impossible entry data asked.
	// width and height must be at least 1.
	if (width<=0) width=1;
	if (height<=0) height=1;

	int x2=x+width;
	int y2=y+height;

	if (x<0) x=0;
	if (x>=getWidth()) x=getWidth()-1;
	if (y<0) y=0;
	if (y>=getHeight()) y=getHeight()-1;
	if (x2<=0) x2=1;
	if (x2>getWidth()) x2=getWidth();
	if (y2<=0) y2=1;
	if (y2>getHeight()) y2=getHeight();

	width = x2-x;
	height = y2-y;

	KD_SDLImage *copy = new KD_SDLImage();

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		int amask = 0x000000ff;
	#else
		int amask = 0xff000000;
	#endif

	SDL_Surface *imageCopy = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, width, height, image->format->BitsPerPixel, image->format->Rmask, image->format->Gmask, image->format->Bmask, amask);

	SDL_LockSurface(image);
	SDL_LockSurface(imageCopy);

	// WE MUST BLIT THE ALPHA CHANNEL TOO!
	for (int j=0; j<height ; j++)
		for (int i=0; i<width ; i++)
		{
			((unsigned int *)imageCopy->pixels)[i+j*(imageCopy->pitch>>2)] = ((unsigned int *)image->pixels)[i+x+(j+y)*(image->pitch>>2)];
		}

	SDL_UnlockSurface(image);
	SDL_UnlockSurface(imageCopy);

	copy->image = imageCopy;
	return copy;
}
