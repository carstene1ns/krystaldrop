#include <assert.h>

#include "OglImage.h"
//#include "../global.h"
//#include "../util/direct.h"
//#include "../util/logfile.h"
#include "Display.h"
#include "SDL/SDL_image.h"
#include "SDL_rotozoom.h"


KD_OGLImage::KD_OGLImage() : KD_Image() , texture(0)
{
	hasAlphaChannel=false;
}

KD_OGLImage::~KD_OGLImage()
{
	if (texture!=0)
		glDeleteTextures(1,&texture); //Free here
}

void KD_OGLImage::makeImageFromSDLSurface(SDL_Surface *surf)
{
	// Pour un channel alpha, utiliser SDL_DisplayFormatAlpha
	//SDL_Surface *surfTemp = SDL_DisplayFormat(surf);
	SDL_Surface *surfTemp = SDL_DisplayFormatAlpha(surf);

	SDL_FreeSurface(surf);

	SDL_PixelFormat *fmt = surfTemp->format;


	hasAlphaChannel=false;

	int i,j;
	// Let's detect if there is an alpha channel other than 255!
	for (j=0; j<surfTemp->h ; j++)
		for (i=0; i<surfTemp->w ; i++)
		{
			int alpha;
		#if SDL_BYTEORDER == SDL_BIG_ENDIAN
			alpha = (((unsigned int *)surfTemp->pixels)[i+j*(surfTemp->pitch>>2)] & 0x000000ff);
		#else
			alpha = ((((unsigned int *)surfTemp->pixels)[i+j*(surfTemp->pitch>>2)] & 0xff000000)>>24);
		#endif
			if (alpha!=255)
			{
				hasAlphaChannel = true;
				goto endTestAlpha;
			}
		}
endTestAlpha:

	int tw, th;
	tw = 1 << (int) ceil(log(surfTemp->w) / log(2.0));
	th = 1 << (int) ceil(log(surfTemp->h) / log(2.0));

	SDL_Surface *surf2 = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, tw, th, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);

	// we must blit the alpha channel too!
	//SDL_BlitSurface(surfTemp, 0, surf2, 0);
	for (j=0; j<surfTemp->h ; j++)
		for (i=0; i<surfTemp->w ; i++)
		{
			((unsigned int *)surf2->pixels)[i+j*(surf2->pitch>>2)] = ((unsigned int *)surfTemp->pixels)[i+j*(surfTemp->pitch>>2)];
		}

	

	SDL_LockSurface(surfTemp);
	SDL_LockSurface(surf2);

	/*if (!hasAlphaChannel)
	{
		for (int i=0; i<tw*th; i++)
		{
			((unsigned int *)surf2->pixels)[i] = ((unsigned int *)surf2->pixels)[i] | (255<<24);
		}
	}*/


	glGenTextures(1, &texture);					// Create The Texture
	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texture);
	// Generate The Texture
	glTexImage2D(GL_TEXTURE_2D, 0, 4, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, surf2->pixels);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering

	width = surfTemp->w;
	height = surfTemp->h;
	tx = (float)width / (float)tw;
	ty = (float)height / (float)th;

	SDL_UnlockSurface(surfTemp);
	SDL_UnlockSurface(surf2);
	SDL_FreeSurface(surfTemp);
	SDL_FreeSurface(surf2);

}

void KD_OGLImage::Load(KD_FilePath fileName)
{
	SDL_Surface* surfLoaded= NULL;
	const char* p_data;
	unsigned long size;

	if (!fileName.IsArchived())
	{
		surfLoaded = IMG_Load(fileName.GetPath().c_str());
	}
	else
	{
		KD_ArchiveManager::FetchResource (fileName.GetArchiveName(), fileName.GetPath(), &p_data, &size);
		surfLoaded= IMG_Load_RW(SDL_RWFromMem((void*)p_data, size), 0);
	}

	assert(surfLoaded);
	// c'est un peu violent non ?

	// Converts the surface to the Display format
	makeImageFromSDLSurface(surfLoaded);

	if (fileName.IsArchived())
	{
		KD_ArchiveManager::FreeResource (fileName.GetArchiveName(), fileName.GetPath());
	}
}

void KD_OGLImage::Display(float x, float y)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,1.0f);
		glTexCoord2f(0, 0); glVertex3f(x, (float)Display::height-y, -1.0);
		glTexCoord2f(0, ty); glVertex3f(x, (float)Display::height-y-height, -1.0);
		glTexCoord2f(tx, ty); glVertex3f(x+width, (float)Display::height-y-height, -1.0);
		glTexCoord2f(tx, 0); glVertex3f(x+width, (float)Display::height-y, -1.0);
	glEnd();		
}

void KD_OGLImage::DisplayAlpha(float x, float y, int alpha)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
	glColor4f(1.0f,1.0f,1.0f,(float)alpha/255.0f);
		glTexCoord2f(0, 0); glVertex3f(x, (float)Display::height-y, -1.0);
		glTexCoord2f(0, ty); glVertex3f(x, (float)Display::height-y-height, -1.0);
		glTexCoord2f(tx, ty); glVertex3f(x+width, (float)Display::height-y-height, -1.0);
		glTexCoord2f(tx, 0); glVertex3f(x+width, (float)Display::height-y, -1.0);
	glEnd();
}

void KD_OGLImage::DisplayColorZoom(float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
	glColor4f((float)r/255.0f,(float)g/255.0f,(float)b/255.0f,(float)alpha/255.0f);
		glTexCoord2f(0, 0); glVertex3f(x, (float)Display::height-y, -1.0);
		glTexCoord2f(0, ty); glVertex3f(x, (float)Display::height-y-height*resizeY, -1.0);
		glTexCoord2f(tx, ty); glVertex3f(x+width*resizeX, (float)Display::height-y-height*resizeY, -1.0);
		glTexCoord2f(tx, 0); glVertex3f(x+width*resizeX, (float)Display::height-y, -1.0);
	glEnd();
}

void KD_OGLImage::DisplayColorZoomRotate(float x, float y, int r, int g, int b, int alpha, float resizeX, float resizeY, float rotX, float rotY, float angle)
{
	float costh = (float)cos(angle);
	float sinth = (float)sin(angle);

	// Calculating the upper left:
	float x1 = (x-rotX)*costh - (y-rotY)*sinth + rotX;
	float y1 = (y-rotY)*costh + (x-rotX)*sinth + rotY;
	
	// Calculating the translation to get to the upper right corner
	float rx = resizeX*width*costh;
	float ry = resizeX*width*sinth;
	// Calculating the translation to get to the lower left corner
	float lx = -resizeY*height*sinth;
	float ly = resizeY*height*costh;

	glBindTexture(GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
	glColor4f((float)r/255.0f,(float)g/255.0f,(float)b/255.0f,(float)alpha/255.0f);
		glTexCoord2f(0, 0); glVertex3f(x1, Display::height-y1, -1.0);
		glTexCoord2f(0, ty); glVertex3f(x1+lx, Display::height-y1-ly, -1.0);
		glTexCoord2f(tx, ty); glVertex3f(x1+lx+rx, Display::height-y1-ly-ry, -1.0);
		glTexCoord2f(tx, 0); glVertex3f(x1+rx, Display::height-y1-ry, -1.0);
	glEnd();
}

void KD_OGLImage::DisplayAlphaPart(float x, float y, int alpha, int xPart, int yPart, int widthPart, int heightPart)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	
	float x1f = xPart*tx/width;
	float y1f = yPart*ty/height;
	float x2f = (xPart+widthPart)*tx/width;
	float y2f = (yPart+heightPart)*ty/height;

	glBegin(GL_QUADS);
	glColor4f(1.0f,1.0f,1.0f,(float)alpha/255.0f);
		glTexCoord2f(x1f, y1f); glVertex3f(x, (float)Display::height-y, -1.0);
		glTexCoord2f(x1f, y2f); glVertex3f(x, (float)Display::height-y-heightPart, -1.0);
		glTexCoord2f(x2f, y2f); glVertex3f(x+widthPart, (float)Display::height-y-heightPart, -1.0);
		glTexCoord2f(x2f, y1f); glVertex3f(x+widthPart, (float)Display::height-y, -1.0);
	glEnd();
}

/*void KD_OGLImage::DisplayRotateX (int x, int y, float angle)
{ glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef ((GLfloat)0, (GLfloat)Display::height- y- height/ 2, (GLfloat)-5000); 
  // -5000 to be sure that portions that verify z<0 will not be clipped 
  glRotatef (angle, 1.0, 0.0, 0.0);
  glTranslatef ((GLfloat)0, (GLfloat)y+ height/ 2- Display::height, (GLfloat)+5000);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
    glTexCoord2f(0, 0); glVertex3f((GLfloat)x, (GLfloat)Display::height-y, -5000.0);
    glTexCoord2f(0, ty); glVertex3f((GLfloat)x, (GLfloat)Display::height-y- height, -5000.0);
    glTexCoord2f(tx, ty); glVertex3f((GLfloat)x+ width, (GLfloat)Display::height-y- height, -5000.0);
    glTexCoord2f(tx, 0); glVertex3f((GLfloat)x+ width, (GLfloat)Display::height-y, -5000.0);
	glEnd();
  glMatrixMode (GL_MODELVIEW); 
  glLoadIdentity();  
}

void KD_OGLImage::DisplayRotateY (int x, int y, float angle)
{ glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef ( (GLfloat)x+ width/ 2, 0, -5000.0); 
  // -5000 to be sure that portions that verify z<0 will not be clipped 
  glRotatef (angle, 0.0, 1.0, 0.0);
  glTranslatef ((GLfloat)-x- width/ 2, 0, +5000.0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
    glTexCoord2f(0, 0); glVertex3f((GLfloat)x, (GLfloat)Display::height-y, -5000.0);
    glTexCoord2f(0, ty); glVertex3f((GLfloat)x, (GLfloat)Display::height-y- height, -5000.0);
    glTexCoord2f(tx, ty); glVertex3f((GLfloat)x+ width, (GLfloat)Display::height-y- height, -5000.0);
    glTexCoord2f(tx, 0); glVertex3f((GLfloat)x+ width, (GLfloat)Display::height-y, -5000.0);
	glEnd(); 
  glMatrixMode (GL_MODELVIEW); 
  glLoadIdentity();  
}*/

void KD_OGLImage::DisplayFullParameters(float x1, float y1, int r1, int g1, int b1, int alpha1, 
                                        float x2, float y2, int r2, int g2, int b2, int alpha2, 
                                        float x3, float y3, int r3, int g3, int b3, int alpha3, 
                                        float x4, float y4, int r4, int g4, int b4, int alpha4)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
		glColor4f((float)r1/255.0f,(float)g1/255.0f,(float)b1/255.0f,(float)alpha1/255.0f);
		glTexCoord2f(0, 0); glVertex3f(x1, (float)Display::height-y1, -1.0);
		glColor4f((float)r4/255.0f,(float)g4/255.0f,(float)b4/255.0f,(float)alpha4/255.0f);
		glTexCoord2f(0, ty); glVertex3f(x4, (float)Display::height-y4, -1.0);
		glColor4f((float)r3/255.0f,(float)g3/255.0f,(float)b3/255.0f,(float)alpha3/255.0f);
		glTexCoord2f(tx, ty); glVertex3f(x3, (float)Display::height-y3, -1.0);
		glColor4f((float)r2/255.0f,(float)g2/255.0f,(float)b2/255.0f,(float)alpha2/255.0f);
		glTexCoord2f(tx, 0); glVertex3f(x2, (float)Display::height-y2, -1.0);
	glEnd();
}


void KD_OGLImage::SetColorKey(Uint32 key)
{ }

void KD_OGLImage::SetColorKey(Uint8 r, Uint8 g, Uint8 b)
{
	
}

SDL_Surface *KD_OGLImage::getSDL_Surface()
{
	return NULL;
}

int KD_OGLImage::GetHeight()
{
	return height;
}

int KD_OGLImage::GetWidth()
{
	return width;
}

void KD_OGLImage::DisableAlpha()
{
	//SDL_SetAlpha(image, SDL_RLEACCEL, 0);
}

void KD_OGLImage::EnableAlpha()
{
	//SDL_SetAlpha(image, SDL_SRCALPHA | SDL_RLEACCEL, 0);
}

/*void KD_OGLImage::EnableAlpha(int alpha)
{
	//SDL_SetAlpha(image, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
}*/

void KD_OGLImage::ConvertToColorKey(Uint8 r, Uint8 g, Uint8 b, int alphaTrigger)
{
}

void KD_OGLImage::ConvertToColorKey(unsigned int key, int alphaTrigger)
{
}

bool KD_OGLImage::resize(float ratio)
{
/*	SDL_Surface *newImage;
  
	// If resized image is not wide enough to be seen, do not resize it.
	if (image->w*ratio < 1 || image->h*ratio < 1)
		return false;
	
	newImage=zoomSurface(image, ratio, ratio, SMOOTHING_ON);

	SDL_FreeSurface(image);
	image=newImage;*/

	width = (int)((float)width * ratio);
	height = (int)((float)height * ratio);

	return true;
}

/**
	Copy the KD_Image to another similar image.
	This new copy won't be referenced in the KD_ImageManager!
	*/
KD_Image *KD_OGLImage::copy()
{
	KD_OGLImage *copy = new KD_OGLImage();

	copy->hasAlphaChannel = hasAlphaChannel;

	copy->width = width;
	copy->height = height;
	copy->tx = tx;
	copy->ty = ty;

	int texWidth, texHeight;
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);

	int *pixels = new int[texWidth*texHeight];

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  
	glGenTextures(1, &(copy->texture));					// Create The Texture
	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, copy->texture);
	// Generate The Texture
	glTexImage2D(GL_TEXTURE_2D, 0, 4, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
	
	delete[] pixels;

	return copy;
}

KD_Image *KD_OGLImage::copy(int x, int y, int widthAsked, int heightAsked)
{
	// First, let's correct any impossible entry data asked.
	// width and height must be at least 1.
	if (widthAsked<=0) widthAsked=1;
	if (heightAsked<=0) heightAsked=1;

	int x2=x+widthAsked;
	int y2=y+heightAsked;

	if (x<0) x=0;
	if (x>=GetWidth()) x=GetWidth()-1;
	if (y<0) y=0;
	if (y>=GetHeight()) y=GetHeight()-1;
	if (x2<=0) x2=1;
	if (x2>GetWidth()) x2=GetWidth();
	if (y2<=0) y2=1;
	if (y2>GetHeight()) y2=GetHeight();

	widthAsked = x2-x;
	heightAsked = y2-y;


	KD_OGLImage *copy = new KD_OGLImage();

	copy->hasAlphaChannel = hasAlphaChannel;

	int tw, th;
	tw = 1 << (int) ceil(log(widthAsked) / log(2.0));
	th = 1 << (int) ceil(log(heightAsked) / log(2.0));
	
	copy->width = widthAsked;
	copy->height = heightAsked;
	copy->tx = (float)copy->width / (float)tw;
	copy->ty = (float)copy->height / (float)th;

	int texWidth, texHeight;
	
	glBindTexture(GL_TEXTURE_2D, texture);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &texWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &texHeight);

	int *pixels = new int[texWidth*texHeight];

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  
	int *pixelsCopy = new int[tw*th];

	for (int j=0; j<heightAsked; j++)
		for (int i=0; i<widthAsked; i++)
		{
			pixelsCopy[i+j*tw] = pixels[x+i+(y+j)*texWidth];
		}

	delete[] pixels;

	glGenTextures(1, &(copy->texture));					// Create The Texture
	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, copy->texture);
	// Generate The Texture
	glTexImage2D(GL_TEXTURE_2D, 0, 4, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelsCopy);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// Linear Filtering
	
	delete[] pixelsCopy;

	return copy;
}
