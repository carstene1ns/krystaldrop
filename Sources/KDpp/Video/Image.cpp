#include "Image.h"

#include <assert.h>

//#include "../util/direct.h"
//#include "../util/logfile.h"
#include "Display.h"

#include "SDL/SDL_image.h"
#include "SDL_rotozoom.h"

#include "ImageInstance.h"

KD_Image::KD_Image() : KD_DisplayableResource()
{
}

KD_Image::~KD_Image()
{
}

KD_DisplayableInstance *KD_Image::createInstance()
{
	return new KD_ImageInstance(this);
}

void KD_Image::makeImageFromSDLSurface(SDL_Surface *surf)
{
}

void KD_Image::Load(KD_FilePath fileName)
{
}

void KD_Image::Display(float x, float y)
{
}

void KD_Image::DisplayAlpha(float x, float y, int alpha)
{
}

/*void KD_Image::DisplayRotateX(int x, int y, float angle) { }
void KD_Image::DisplayRotateY(int x, int y, float angle) { }*/

void KD_Image::DisplayColorZoom(float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY)
{
}

void KD_Image::DisplayColorZoomRotate(float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY, float rotX, float rotY, float angle)
{
}

void KD_Image::DisplayAlphaPart(float x, float y, int alpha, int xPart, int yPart, int widthPart, int heightPart)
{
}

void KD_Image::DisplayFullParameters(float x1, float y1, int r1, int g1, int b1, int alpha1, 
                                     float x2, float y2, int r2, int g2, int b2, int alpha2, 
				     float x3, float y3, int r3, int g3, int b3, int alpha3, 
				     float x4, float y4, int r4, int g4, int b4, int alpha4)
{
}

void KD_Image::SetColorKey(Uint32 key)
{
}

void KD_Image::SetColorKey(Uint8 r, Uint8 g, Uint8 b)
{
}

SDL_Surface *KD_Image::getSDL_Surface()
{
	return 0;
}

int KD_Image::GetHeight()
{
	return 0;
}

int KD_Image::GetWidth()
{
	return 0;
}

void KD_Image::DisableAlpha()
{
}

void KD_Image::EnableAlpha()
{
}

void KD_Image::ConvertToColorKey(Uint8 r, Uint8 g, Uint8 b, int alphaTrigger)
{
}

void KD_Image::ConvertToColorKey(unsigned int key, int alphaTrigger)
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
