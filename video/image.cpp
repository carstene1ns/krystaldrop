#include "image.h"

#include <assert.h>

#include "../util/direct.h"
#include "../util/logfile.h"
#include "Display.h"

#include "SDL/SDL_image.h"
#include "SDL_rotozoom.h"



KD_Image::KD_Image() : countRef(0)
{
}

KD_Image::~KD_Image()
{
}

void KD_Image::makeImageFromSDLSurface(SDL_Surface *surf)
{
}

void KD_Image::Load(char *fileName)
{
}

void KD_Image::Load(TACCRes *accFile, char *fileName)
{
}

void KD_Image::Display(int x, int y)
{
}

void KD_Image::DisplayAlpha(int x, int y, int alpha)
{
}

/*void KD_Image::DisplayRotateX(int x, int y, float angle) { }
void KD_Image::DisplayRotateY(int x, int y, float angle) { }*/

void KD_Image::DisplayColorZoom(int x, int y, int r, int g, int b, int alpha, float resizeX, float resizeY)
{
}

void KD_Image::DisplayColorZoomRotate(int x, int y, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle)
{
}

void KD_Image::setColorKey(Uint32 key)
{
}

void KD_Image::setColorKey(Uint8 r, Uint8 g, Uint8 b)
{
}

SDL_Surface *KD_Image::getSDL_Surface()
{
	return 0;
}

int KD_Image::getHeight()
{
	return 0;
}

int KD_Image::getWidth()
{
	return 0;
}

void KD_Image::disableAlpha()
{
}

void KD_Image::enableAlpha()
{
}

void KD_Image::convertToColorKey(Uint8 r, Uint8 g, Uint8 b, int alphaTrigger)
{
}

void KD_Image::convertToColorKey(unsigned int key, int alphaTrigger)
{

}

bool KD_Image::resize(float ratio)
{
	return false;
}

KD_Image *KD_Image::copy()
{
	return 0;
}

KD_Image *KD_Image::copy(int x, int y, int width, int height)
{
	return NULL;
}