#include "SDL/SDL_image.h"

#include "Display.h"
#include "font.h"

KD_Font::KD_Font ()
{

}

KD_Font::KD_Font (char *fileName)
{
	Load(fileName);
}

bool KD_Font::Load (char *fileName)
{
	FILE *fpt = fopen(fileName, "r");

	char buf[256];

	int res = fscanf(fpt, "%s", buf);
	
	if (!res)
	{
		printf("Warning, no font-file loaded!");
		return false;
	}

	// Load the surface
	SDL_Surface *surfLoaded = IMG_Load(buf);

	// Converts the surface to the display format
	SDL_Surface *surf = SDL_DisplayFormatAlpha(surfLoaded);

	
	///// TEST IF THERE IS ANY CHANCE TO GET AN ALPHA CHANNEL
	//for (int i=0; i<surf->w*surf->h; i++)
	//	/*if (((unsigned int*)surfLoaded->pixels)[i] & 0xff000000 != 0)*/ printf("%u\n",((unsigned int*)surf->pixels)[i]>>24);

	/// Inverting the alpha channel.
	/*for (int i=0; i<surf->w*surf->h; i++)
	{
		unsigned int alpha = ((unsigned int*)surf->pixels)[i]>>24;
		alpha = 255-alpha;

		((unsigned int*)surf->pixels)[i] &= 0x00ffffff;
		((unsigned int*)surf->pixels)[i] += alpha<<24;
		//((unsigned int*)surf->pixels)[i] = 0xccffffff;

		printf("%u\n",((unsigned int*)surf->pixels)[i]>>24);

	}*/

	// Free the old surface
	SDL_FreeSurface(surfLoaded);

	int nb, x1, x2, y1, y2;
	SDL_PixelFormat *fmt = Display::screen->format;

	// Number of pixels for the space and the return character.
	fscanf(fpt, "%d %d", &spaceSize, &returnSize);

	// Sets the array of letters to the NULL pointer
	for (int i=0; i<256; i++)
		letters[i]=0;

	while (/*fscanf(fpt, "%d %d %d %d %d", &nb, &x1, &x2, &y1, &y2) != EOF*/1)
	{
		res = fscanf(fpt, "%d %d %d %d %d", &nb, &x1, &x2, &y1, &y2);
		if (res == EOF) break;
		else if (res != 5)
		{
			char trash[10000];
			fscanf(fpt, "%s", trash);
			continue;
		}

/*#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif*/

		SDL_Surface *surfLetter = SDL_CreateRGBSurface(SDL_HWSURFACE | SDL_SRCALPHA, x2-x1, y2-y1, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, /*fmt->Amask*/0xff000000);

		// WE MUST BLIT THE ALPHA CHANNEL TOO!
		//SDL_BlitSurface(surf, &srcrect, surfLetter, 0);		
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

	SDL_FreeSurface(surf);

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
