#include "imagemanager.h"

#include <assert.h>

#include "image.h"
#include "../util/logfile.h"

KD_ImageManager *KD_ImageManager::singleton=0;

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
		KD_LogFile::printf("Warning, the file %s has not been cleanly cleared. ImageManager is deleting it itself.\n",(*cur).first.c_str());
		printf("Warning, the file %s has not been cleanly cleared. ImageManager is deleting it itself.\n",(*cur).first.c_str());
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

bool KD_ImageManager::Load(char *fileName)
{
	printf ("loading %s",fileName);
	fflush(stdout);
	// Test if an image with that name already exists.
	if ( images.find(fileName) != images.end() )
		return false;

	KD_Image *img = new KD_Image();
	img->Load(fileName);
	images[fileName]=img;
	return true;
}

bool KD_ImageManager::Load(TACCRes *accFile, char *fileName)
{
	// Test if an image with that name already exists.
	if ( images.find(fileName) !=  images.end() )
		return false;

	KD_Image *img = new KD_Image();
	img->Load(accFile, fileName);
	images[fileName]=img;
	return true;
}
