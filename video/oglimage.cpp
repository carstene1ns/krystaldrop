#include "oglimage.h"

#include "../global.h"
#include "../util/direct.h"
#include "../util/logfile.h"
#include "Display.h"

#include "SDL/SDL_image.h"
#include "SDL_rotozoom.h"


KD_OGLImage::KD_OGLImage() : texture(0)
{
	countRef=0;
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

void KD_OGLImage::Load(char *fileName)
{
	// Load the surface
	SDL_Surface *surfLoaded = IMG_Load(fileName);

	assert(surfLoaded);

	// Converts the surface to the display format
	makeImageFromSDLSurface(surfLoaded);
	
}

void KD_OGLImage::Load(TACCRes *accFile, char *fileName)
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

	makeImageFromSDLSurface(surfLoaded);
	/*
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
		*/

}

void KD_OGLImage::Display(int x, int y)
{
/* 	if (x >= Display::width) return;
	if (y >= Display::height) return;
	if (x+width < 0) return;
	if (y+height < 0) return;*/

	glBindTexture(GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
	glColor3f(1.0f,1.0f,1.0f);
		glTexCoord2f(0, 0); glVertex3f((float)x, (float)Display::height-y, -1.0);
		glTexCoord2f(0, ty); glVertex3f((float)x, (float)Display::height-y-height, -1.0);
		glTexCoord2f(tx, ty); glVertex3f((float)x+width, (float)Display::height-y-height, -1.0);
		glTexCoord2f(tx, 0); glVertex3f((float)x+width, (float)Display::height-y, -1.0);
	glEnd();
			
}

void KD_OGLImage::DisplayAlpha(int x, int y, int alpha)
{
/*	if (x >= Display::width) return;
	if (y >= Display::height) return;
	if (x+width < 0) return;
	if (y+height < 0) return; */

	glBindTexture(GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
	glColor4f(1.0f,1.0f,1.0f,(float)alpha/255.0f);
		glTexCoord2f(0, 0); glVertex3f((float)x, (float)Display::height-y, -1.0);
		glTexCoord2f(0, ty); glVertex3f((float)x, (float)Display::height-y-height, -1.0);
		glTexCoord2f(tx, ty); glVertex3f((float)x+width, (float)Display::height-y-height, -1.0);
		glTexCoord2f(tx, 0); glVertex3f((float)x+width, (float)Display::height-y, -1.0);
	glEnd();


}

void KD_OGLImage::DisplayColorZoom(int x, int y, int r, int g, int b, int alpha, float resizeX, float resizeY)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glBegin(GL_QUADS);
	glColor4f((float)r/255.0f,(float)g/255.0f,(float)b/255.0f,(float)alpha/255.0f);
		glTexCoord2f(0, 0); glVertex3f((float)x, (float)Display::height-y, -1.0);
		glTexCoord2f(0, ty); glVertex3f((float)x, (float)Display::height-y-height*resizeY, -1.0);
		glTexCoord2f(tx, ty); glVertex3f((float)x+width*resizeX, (float)Display::height-y-height*resizeY, -1.0);
		glTexCoord2f(tx, 0); glVertex3f((float)x+width*resizeX, (float)Display::height-y, -1.0);
	glEnd();
}

void KD_OGLImage::DisplayColorZoomRotate(int x, int y, int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle)
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

void KD_OGLImage::DisplayRotateX (int x, int y, float angle)
{ glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef (0, Display::height- y- height/ 2, -5000); 
  /* -5000 to be sure that portions that verify z<0 will not be clipped */
  glRotatef (angle, 1.0, 0.0, 0.0);
  glTranslatef (0, y+ height/ 2- Display::height, +5000);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
    glTexCoord2f(0, 0); glVertex3f(x, Display::height-y, -5000.0);
    glTexCoord2f(0, ty); glVertex3f(x, Display::height-y- height, -5000.0);
    glTexCoord2f(tx, ty); glVertex3f(x+ width, Display::height-y- height, -5000.0);
    glTexCoord2f(tx, 0); glVertex3f(x+ width, Display::height-y, -5000.0);
	glEnd();
  glMatrixMode (GL_MODELVIEW); 
  glLoadIdentity();  
}

void KD_OGLImage::DisplayRotateY (int x, int y, float angle)
{ glMatrixMode (GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef ( x+ width/ 2, 0, -5000); 
  /* -5000 to be sure that portions that verify z<0 will not be clipped */
  glRotatef (angle, 0.0, 1.0, 0.0);
  glTranslatef (-x- width/ 2, 0, +5000);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);
	glColor4f (1.0f, 1.0f, 1.0f, 1.0f);
    glTexCoord2f(0, 0); glVertex3f(x, Display::height-y, -5000.0);
    glTexCoord2f(0, ty); glVertex3f(x, Display::height-y- height, -5000.0);
    glTexCoord2f(tx, ty); glVertex3f(x+ width, Display::height-y- height, -5000.0);
    glTexCoord2f(tx, 0); glVertex3f(x+ width, Display::height-y, -5000.0);
	glEnd(); 
  glMatrixMode (GL_MODELVIEW); 
  glLoadIdentity();  
}

void KD_OGLImage::setColorKey(Uint32 key)
{ }

void KD_OGLImage::setColorKey(Uint8 r, Uint8 g, Uint8 b)
{
	
}

SDL_Surface *KD_OGLImage::getSDL_Surface()
{
	return NULL;
}

int KD_OGLImage::getHeight()
{
	return height;
}

int KD_OGLImage::getWidth()
{
	return width;
}

void KD_OGLImage::disableAlpha()
{
	//SDL_SetAlpha(image, SDL_RLEACCEL, 0);
}

void KD_OGLImage::enableAlpha()
{
	//SDL_SetAlpha(image, SDL_SRCALPHA | SDL_RLEACCEL, 0);
}

/*void KD_OGLImage::enableAlpha(int alpha)
{
	//SDL_SetAlpha(image, SDL_SRCALPHA | SDL_RLEACCEL, alpha);
}*/

void KD_OGLImage::convertToColorKey(Uint8 r, Uint8 g, Uint8 b, int alphaTrigger)
{
}

void KD_OGLImage::convertToColorKey(unsigned int key, int alphaTrigger)
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
	if (x>=getWidth()) x=getWidth()-1;
	if (y<0) y=0;
	if (y>=getHeight()) y=getHeight()-1;
	if (x2<=0) x2=1;
	if (x2>getWidth()) x2=getWidth();
	if (y2<=0) y2=1;
	if (y2>getHeight()) y2=getHeight();

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