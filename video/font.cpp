#include "../global.h"

#include <SDL/SDL_image.h>

#include "Display.h"
#include "font.h"
#include "imagemanager.h"
#include "image.h"
#include "sdlimage.h"
#include "SDL_rotozoom.h"
#include "../util/textfile.h"
#include "../util/logfile.h"


KD_Font::KD_Font ()
{
	isColorKey=false;
}

KD_Font::KD_Font (char *fileName)
{
	Load(fileName);
}

KD_Font::KD_Font (TACCRes *accFile, char *fileName)
{
	LoadFromAcc(accFile, fileName);
}

KD_Font::~KD_Font ()
{
	for (int i=0; i<256; i++)
	{
		if (letters[i]==0)
			continue;
		
		// Set the pointers of all the duplicated values to 0 to avoid freeing an already freed memory block.
		for (int j=i+1; j<256; j++)
		{
	
			if (letters[i]==letters[j])
			{
				letters[j]=0;
			}
		}
		KD_ImageManager::getImageManager()->deleteUnreferencedImage(letters[i]);
    }
}

bool KD_Font::Load (char *fileName)
{
	return LoadFromAcc(0,fileName);
}

bool KD_Font::LoadFromAcc (TACCRes *accFile, char *fileName)
{	KD_TextFile file;
		
	if (accFile)
		file.Load(accFile,fileName);
	else
		file.Load(fileName);

	char buf[256];

	int res = sscanf(file.getPosition(), "%s", buf);

	if (!res)
	{
		printf("Warning, no font-file loaded!");
		return false;
	}
	file.jumpLine();

	if (accFile)
		KD_ImageManager::getImageManager()->Load(accFile, buf, false);
	else
		KD_ImageManager::getImageManager()->Load(0,buf, false);


	SDL_Surface *surf = KD_ImageManager::getImageManager()->getImage(buf)->getSDL_Surface();

	int nb, x1, x2, y1, y2;
	SDL_PixelFormat *fmt = Display::screen->format;

	// Number of pixels for the space and the return character.
	sscanf(file.getPosition(), "%d %d", &spaceSize, &returnSize);
	file.jumpLine();

	// Sets the array of letters to the NULL pointer
	for (int i=0; i<256; i++)
		letters[i]=0;

	while (1)
	{
		if (file.isEOF()) 
			break;

		res = sscanf(file.getPosition(), "%d %d %d %d %d", &nb, &x1, &x2, &y1, &y2);
		file.jumpLine();
		if (res != 5)
		{
			continue;
		}

		if (x2-x1<=0 || y2-y1<=0)
		{
			printf("Warning, unvalid font coordinates passed to the program: x1=%d y1=%d x2=%d y2=%d for character %c\n",x1,y1,x2,y2,nb);
			KD_LogFile::printf("Warning, unvalid font coordinates passed to the program: x1=%d y1=%d x2=%d y2=%d for character %c\n",x1,y1,x2,y2,nb);
		}

		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		    int amask = 0x000000ff;
		#else
		    int amask = 0xff000000;
		#endif

		SDL_Surface *surfLetter = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, x2-x1, y2-y1, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, amask);

		SDL_LockSurface(surf);
		SDL_LockSurface(surfLetter);

		// WE MUST BLIT THE ALPHA CHANNEL TOO!
		for (int j=0; j<y2-y1 ; j++)
			for (int i=0; i<x2-x1 ; i++)
			{
				((unsigned int *)surfLetter->pixels)[i+j*(surfLetter->pitch>>2)] = ((unsigned int *)surf->pixels)[i+x1+(j+y1)*(surf->pitch>>2)];
			}

		SDL_UnlockSurface(surf);
		SDL_UnlockSurface(surfLetter);

		KD_Image *letterImg = KD_ImageManager::getImageManager()->newUnreferencedImage();
		letterImg->makeImageFromSDLSurface(surfLetter);

		// If default value for non existing characters
		if (nb==0)
		{
			for (int i=0; i<256; i++)
				if (letters[i]==0) letters[i]=letterImg;
		}
		else
			letters[nb]=letterImg;
	}

	KD_ImageManager::getImageManager()->releaseImage(buf);

	return true;
}

void KD_Font::xyprintf(int x, int y, char *str, ...)
{	char buf[PRINTF_BUF_SIZE];

	va_list argptr;
	va_start (argptr, str);
	vsnprintf (buf, PRINTF_BUF_SIZE, str, argptr);
    buf[PRINTF_BUF_SIZE- 1]= 0;  
	va_end (argptr);

	int xWork= x,yWork= y;
	int i=0;

	while (buf[i])
	{
		switch ((unsigned char) buf[i])
		{
		case ' ':
			xWork += spaceSize;
			break;

		case '\n':
			xWork = x;
			yWork += returnSize;
			break;

		default:
			letters[(unsigned char)buf[i]]->Display(xWork, yWork-letters[(unsigned char)buf[i]]->getHeight());
			
			xWork += letters[(unsigned char)buf[i]]->getWidth();
			break;
		}
		i++;
	}

}

void KD_Font::xyalphaprintf(int alpha, int x, int y, char *str, ...)
{	char buf[PRINTF_BUF_SIZE];

	va_list argptr;
	va_start (argptr, str);
	vsnprintf (buf, PRINTF_BUF_SIZE, str, argptr);
    buf[PRINTF_BUF_SIZE- 1]= 0;  
	va_end (argptr);

	int xWork= x,yWork= y;
	int i=0;

	while (buf[i])
	{
		switch ((unsigned char) buf[i])
		{
		case ' ':
			xWork += spaceSize;
			break;

		case '\n':
			xWork = x;
			yWork += returnSize;
			break;

		default:
			letters[(unsigned char)buf[i]]->DisplayAlpha(xWork, yWork-letters[(unsigned char)buf[i]]->getHeight(),alpha);
			
			xWork += letters[(unsigned char)buf[i]]->getWidth();
			break;
		}
		i++;
	}

}

void KD_Font::xycoloralpharotozoomprintf(int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle, int x, int y, char *str, ...)
{	unsigned char buf[PRINTF_BUF_SIZE];
    float xWorkf= x, yWorkf= y;
	int i= 0;
  
	va_list argptr;
	va_start (argptr, str);
	vsnprintf ((char*)buf, PRINTF_BUF_SIZE, str, argptr);
    buf[PRINTF_BUF_SIZE- 1]= 0;
	va_end (argptr);
  
	while (buf[i])
	{ switch (buf[i])
      { case ' ': xWorkf+= spaceSize* resizeX;
                  break;
        case '\n':xWorkf= x;
                  yWorkf+= returnSize* resizeY;
                  break;
        default: letters[buf[i]]->DisplayColorZoomRotate(
                   (int) rintf(xWorkf),
                   (int) rintf(yWorkf)- letters[buf[i]]->getHeight(),
                   r, g,b,alpha,resizeX, resizeY, rotX, rotY, angle);
                 xWorkf+= letters[buf[i]]->getWidth()* resizeX;
                 break;
      }
      i++;
    }
}

int KD_Font::computeLength(char *buf)
{	int length=0;
	int pastLength=0;
	int i=0;

	while (buf[i])
	{
		switch ((unsigned char) buf[i])
		{
		case ' ':
			length += spaceSize;
			break;

		case '\n':
			if (length>pastLength)
				pastLength = length;
			length = 0;
			break;

		default:
			length += letters[(unsigned char)buf[i]]->getWidth();
			break;
		}
		i++;
	}

	if (pastLength>length) length=pastLength;

	return length;
}

void KD_Font::xyrightprintf(int x, int y, char *str, ...)
{	char buf[PRINTF_BUF_SIZE];

	va_list argptr;
	va_start (argptr, str);
	vsnprintf (buf, PRINTF_BUF_SIZE, str, argptr);
    buf[PRINTF_BUF_SIZE- 1]= 0;  
	va_end (argptr);

	int length = computeLength(buf);

	xyprintf(x-length, y, buf);
}

void KD_Font::xycoloralpharotozoomrightprintf(int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle, int x, int y, char *str, ...)
{	char buf[PRINTF_BUF_SIZE];

	va_list argptr;
	va_start (argptr, str);
	vsnprintf (buf, PRINTF_BUF_SIZE, str, argptr);
    buf[PRINTF_BUF_SIZE- 1]= 0;  
	va_end (argptr);

	int length = computeLength(buf);

	xycoloralpharotozoomprintf(r, g, b, alpha, resizeX, resizeY, rotX, rotY, angle, x-length*resizeX, y, buf);
}

void KD_Font::xyalpharightprintf(int alpha, int x, int y, char *str, ...)
{	char buf[PRINTF_BUF_SIZE];

	va_list argptr;
	va_start (argptr, str);
	vsnprintf (buf, PRINTF_BUF_SIZE, str, argptr);
    buf[PRINTF_BUF_SIZE- 1]= 0;  
	va_end (argptr);

	int length = computeLength(buf);

	xyalphaprintf(alpha, x-length, y, buf);
}

void KD_Font::xycenteredprintf(int x, int y, char *str, ...)
{	char buf[PRINTF_BUF_SIZE];

	va_list argptr;
	va_start (argptr, str);
	vsnprintf (buf, PRINTF_BUF_SIZE, str, argptr);
    buf[PRINTF_BUF_SIZE- 1]= 0;  
	va_end (argptr);

	int length = computeLength(buf);

	xyprintf(x-length/2, y, buf);
}

void KD_Font::xyalphacenteredprintf(int alpha, int x, int y, char *str, ...)
{	char buf[PRINTF_BUF_SIZE];

	va_list argptr;
	va_start (argptr, str);
	vsnprintf (buf, PRINTF_BUF_SIZE, str, argptr);
    buf[PRINTF_BUF_SIZE- 1]= 0;  
	va_end (argptr);

	int length = computeLength(buf);

	xyalphaprintf(alpha, x-length/2, y, buf);
}

void KD_Font::xycoloralpharotozoomcenteredprintf(int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle, int x, int y, char *str, ...)
{	char buf[PRINTF_BUF_SIZE];

	va_list argptr;
	va_start (argptr, str);
	vsnprintf (buf, PRINTF_BUF_SIZE, str, argptr);
    buf[PRINTF_BUF_SIZE- 1]= 0;  
	va_end (argptr);

	int length = computeLength(buf);

	xycoloralpharotozoomprintf(r, g, b, alpha, resizeX, resizeY, rotX, rotY, angle, x-length*resizeX/2, y, buf);
}

KD_Font *KD_Font::resize(float ratio)
{
	KD_Font *newFont = new KD_Font();

	newFont->spaceSize = (int)(spaceSize*ratio);
	newFont->returnSize = (int)(returnSize*ratio);

	newFont->isColorKey = isColorKey;
	newFont->colorKey = colorKey;

	int i;

	for (i=0; i<256; i++)
	  newFont->letters[i]=0;

	for (i=0; i<256; i++)
	{
		if (letters[i]==0)
			continue;

		if (newFont->letters[i]!=0)
			continue;
	    
		if (letters[i]->getWidth()*ratio < 1 || letters[i]->getHeight()*ratio < 1)
			// If resized sprite is not wide enough to be seen, do not resize it.
			newFont->letters[i] = letters[i]->copy();
		else
		{
			// Else resize it.
			newFont->letters[i] = letters[i]->copy();
			newFont->letters[i]->resize(ratio);
		}

		if (isColorKey)
			newFont->letters[i]->setColorKey(colorKey);

		for (int j=i+1; j<256; j++)
		    if (letters[i]==letters[j])
		      newFont->letters[j]=newFont->letters[i];
	}

	return newFont;
}

void KD_Font::convertToColorKey(unsigned int key, int alphaTrigger)
{
	isColorKey=true;
	colorKey = key;

	//int j;
	for (int k=0; k<255; k++)
	{
		if (letters[k]==0) continue;

		letters[k]->convertToColorKey(key, alphaTrigger);

		
	/*	if (letters[k]->format->Amask == 0) continue;

		SDL_Surface *surf = SDL_CreateRGBSurface(SDL_HWSURFACE, letters[k]->w, letters[k]->h, letters[k]->format->BitsPerPixel, letters[k]->format->Rmask, letters[k]->format->Gmask, letters[k]->format->Bmask, 0);

		SDL_LockSurface(letters[k]);
		SDL_LockSurface(surf);

		for (j=0; j<letters[k]->h ; j++)
			for (int i=0; i<letters[k]->w ; i++)
			{
				int alpha;
				int pixel=((unsigned int *)letters[k]->pixels)[i+j*(letters[k]->pitch>>2)];
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

		SDL_UnlockSurface(letters[k]);
		SDL_UnlockSurface(surf);

		SDL_FreeSurface(letters[k]);
		for (j=k+1; j<256; j++)
		    if (letters[k]==letters[j])
			{
		      letters[j]=surf;
			}

		letters[k]=surf;*/
		//SDL_SetAlpha(letters[k], SDL_RLEACCEL | SDL_SRCALPHA , 127);
	//SDL_SetColorKey(((KD_SDLImage *)(letters[k]))->image, SDL_SRCCOLORKEY | SDL_RLEACCEL, key);
	}
}
