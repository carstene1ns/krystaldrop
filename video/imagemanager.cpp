#include <assert.h>

#include "Display.h"
#include "image.h"
#include "imagemanager.h"
#ifndef NO_OPENGL
#include "oglimage.h"
#endif
#include "sdlimage.h"
#include "../util/logfile.h"

KD_ImageManager *KD_ImageManager::singleton= 0;

KD_ImageManager::KD_ImageManager()
{
}

KD_ImageManager::~KD_ImageManager()
{
	// If no images are remaining in the ImageManager, it's ok, break.
	if (images.size() == 0) return;

	map<string, KD_Image *>::iterator cur = images.begin();
	
	for (unsigned int i=0; i<images.size(); i++)
	{
#ifdef DEBUG      
		KD_LogFile::printf("Warning, file %s not freed. Deleting it now.\n",(*cur).first.c_str());
#endif      
		printf("Warning, file %s not freed. Deleting it now.\n",(*cur).first.c_str());
		delete images[(*cur).first];
		cur++;
	}
}

KD_ImageManager *KD_ImageManager::getImageManager()
{
	if (singleton==0) singleton = new KD_ImageManager();
	return singleton;
}

KD_Image *KD_ImageManager::getImage(const string &name)
{
	images[name]->countRef++;
	return images[name];
}

void KD_ImageManager::releaseImage(const string &name)
{
	images[name]->countRef--;
	if (images[name]->countRef <= 0)
	{
		delete images[name];
		images.erase(name);
	}
}

void KD_ImageManager::releaseImage(KD_Image *img)
{
	map<string, KD_Image *>::iterator cur = images.begin();
	
	for (unsigned int i=0; i<images.size(); i++)
	{
		if (img == images[(*cur).first])
		{
			releaseImage((*cur).first);
			return;
		}
		cur++;
	}

	assert(0);
}

/*bool KD_ImageManager::Load(char *fileName, bool loadOpenGL=true)
{
	// Test if an image with that name already exists.
	if ( images.find(fileName) != images.end() )
		return false;

	KD_Image *img;
	if (Display::isOpenGL && loadOpenGL)
		img = new KD_OGLImage();
	else
		img = new KD_SDLImage();

	img->Load(fileName);
	images[fileName]=img;
	return true;
}*/

bool KD_ImageManager::Load(TACCRes *accFile, char *fileName, bool loadOpenGL)
{
	// Test if an image with that name already exists.
	if ( images.find(fileName) !=  images.end() )
		return false;

	KD_Image *img;
#ifndef NO_OPENGL    
	if ((Display::isOpenGL && loadOpenGL)) img = new KD_OGLImage();
      else
#endif      
		img = new KD_SDLImage();

	if (accFile)
		img->Load(accFile, fileName);
	else
		img->Load(fileName);
	images[fileName]=img;
	return true;
}

KD_Image *KD_ImageManager::newUnreferencedImage()
{
#ifndef NO_OPENGL  
  if (Display::isOpenGL) return (new KD_OGLImage()); 
    else
#endif      
      return (new KD_SDLImage());
}

void KD_ImageManager::deleteUnreferencedImage(KD_Image *img)
{
	delete img;
}