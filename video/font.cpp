#include "SDL/SDL_image.h"

#include "Display.h"
#include "font.h"
#include "imagemanager.h"
#include "image.h"
#include "../util/textfile.h"
#include "../util/logfile.h"

#include "SDL_rotozoom.h"

#include <assert.h>


KD_Font::KD_Font ()
{

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
		SDL_FreeSurface(letters[i]);
    }
}

bool KD_Font::Load (char *fileName)
{
	return LoadFromAcc(0,fileName);
}

bool KD_Font::LoadFromAcc (TACCRes *accFile, char *fileName)
{
	KD_TextFile file;
		
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
		KD_ImageManager::getImageManager()->Load(accFile, buf);
	else
		KD_ImageManager::getImageManager()->Load(buf);


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

		// WE MUST BLIT THE ALPHA CHANNEL TOO!
		for (int j=0; j<y2-y1 ; j++)
			for (int i=0; i<x2-x1 ; i++)
			{
				((unsigned int *)surfLetter->pixels)[i+j*(surfLetter->pitch>>2)] = ((unsigned int *)surf->pixels)[i+x1+(j+y1)*(surf->pitch>>2)];
			}

		// If default value for non existing characters
		if (nb==0)
		{
			for (int i=0; i<256; i++)
				if (letters[i]==0) letters[i]=surfLetter;
		}
		else
			letters[nb]=surfLetter;
	}

	KD_ImageManager::getImageManager()->releaseImage(buf);

	return true;
}

void KD_Font::xyprintf(int x, int y, char *str, ...)
{
	char buf[1000];

	va_list argptr;
	va_start (argptr, str);
	vsprintf (buf, str, argptr);
	va_end (argptr);

	SDL_Rect dest;
	dest.x = x;
	dest.y = y;

	int i=0;

	while (buf[i])
	{
		switch ((unsigned char) buf[i])
		{
		case ' ':
			dest.x += spaceSize;
			break;

		case '\n':
			dest.x = x;
			dest.y += returnSize;
			break;

		default:
			dest.y -= letters[(unsigned char)buf[i]]->h;
			SDL_BlitSurface(letters[(unsigned char)buf[i]], 0, Display::screen, &dest);
			dest.y += letters[(unsigned char)buf[i]]->h;
			dest.x += letters[(unsigned char)buf[i]]->w;
			break;
		}
		i++;
	}

}

KD_Font *KD_Font::resize(float ratio)
{
	KD_Font *newFont = new KD_Font();

	newFont->spaceSize = (int)(spaceSize*ratio);
	newFont->returnSize = (int)(returnSize*ratio);

	int i;

	for (i=0; i<256; i++)
	  newFont->letters[i]=0;

	for (i=0; i<256; i++)
	{
		if (letters[i]==0)
			continue;
	        
		newFont->letters[i]=zoomSurface(letters[i], ratio, ratio, SMOOTHING_ON);

		for (int j=i+1; j<256; j++)
		    if (letters[i]==letters[j])
		      newFont->letters[j]=newFont->letters[i];
	}

	return newFont;
}
