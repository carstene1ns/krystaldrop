#include "DisplayableInstance.h"

#include "DisplayableResource.h"

#include <math.h>

KD_DisplayableInstance::KD_DisplayableInstance(KD_DisplayableResource *resource)
{
	this->resource = resource;
}

KD_DisplayableInstance::~KD_DisplayableInstance()
{
}

void KD_DisplayableInstance::Display(float x, float y)
{
	DisplayAlpha(x,y,255);
}

void KD_DisplayableInstance::DisplayAlpha(float x, float y, int alpha)
{
	DisplayFullParameters(x,y,255,255,255,alpha,x+GetWidth(),y,255,255,255,alpha,x+GetWidth(),y+GetHeight(),255,255,255,alpha,x,y+GetHeight(),255,255,255,alpha);
}

/*void KD_DisplayableInstance::DisplayRotateX(int x, int y, float angle)
{

}

void KD_DisplayableInstance::DisplayRotateY(int x, int y, float angle)
{

}*/

void KD_DisplayableInstance::DisplayColorZoom(float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY)
{
	DisplayColorZoomRotate(x,y,r,g,b,alpha,resizeX,resizeY,0,0,0);
}

void KD_DisplayableInstance::DisplayColorZoomRotate(float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle)
{
	float costh = (float)cos(angle);
	float sinth = (float)sin(angle);

	// Calculating the upper left:
	float x1 = (x-rotX)*costh - (y-rotY)*sinth + rotX;
	float y1 = (y-rotY)*costh + (x-rotX)*sinth + rotY;
	
	// Calculating the translation to get to the upper right corner
	float rx = resizeX*GetWidth()*costh;
	float ry = resizeX*GetWidth()*sinth;
	// Calculating the translation to get to the lower left corner
	float lx = -resizeY*GetHeight()*sinth;
	float ly = resizeY*GetHeight()*costh;

	DisplayFullParameters((int)x1,(int)y1,r,g,b,alpha, (int)(x1+rx),(int)(y1+ry),r,g,b,alpha, (int)(x1+lx+rx),(int)(y1+ly+ry),r,g,b,alpha, (int)(x1+lx),(int)(y1+ly),r,g,b,alpha);
}

void KD_DisplayableInstance::DisplayFullParameters(float x1, float y1, const KD_Color &c1, int alpha1, float x2, float y2, const KD_Color &c2, int alpha2, float x3, float y3, const KD_Color &c3, int alpha3, float x4, float y4, const KD_Color &c4, int alpha4)
{
	DisplayFullParameters(x1,y1,(int)(c1.r*255),(int)(c1.g*255),(int)(c1.b*255),alpha1,x2,y2,(int)(c2.r*255),(int)(c2.g*255),(int)(c2.b*255),alpha2,x3,y3,(int)(c3.r*255),(int)(c3.g*255),(int)(c3.b*255),alpha3,x4,y4,(int)(c4.r*255),(int)(c4.g*255),(int)(c4.b*255),alpha4);
}
