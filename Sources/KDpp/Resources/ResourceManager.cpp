#include "ResourceManager.h"

#include "Resource.h"
#include "../Video/Display.h"
#include "../Video/OglImage.h"
#include "../Video/SdlImage.h"
#include "../Video/Sprite.h"
#include "../Sound/Sound.h"
#include "../Tools/Logfile.h"

#include <assert.h>

KD_ResourceManager *KD_ResourceManager::singleton = NULL;

#define KD_NB_IMAGE_EXT 10
char* IMAGE_EXTENSIONS[KD_NB_IMAGE_EXT]= {
   "png", "jpg", "gif", "bmp", "pnm", "xpm", "lbm", "pcx", "jpeg", "tga" };

#define KD_NB_SOUND_EXT 6
char* SOUND_EXTENSIONS[KD_NB_SOUND_EXT]= {
   "wav",  "aif",  "aiff",  "riff",  "rif", "voc" };


KD_ResourceManager::KD_ResourceManager() : undefinedCount(0)
{

}

KD_ResourceManager::~KD_ResourceManager()
{
	// If no resources are remaining in the ResourceManager, it's ok, break.
	if (resources.size() == 0) return;

	// Else, Display a warning message and free each resource that has not been desallocated.
	map<KD_FilePath, KD_Resource *>::iterator cur = resources.begin();
	
	for (unsigned int i=0; i<resources.size(); i++)
	{
		KD_FilePath name = (*cur).first;
#ifdef DEBUG
		KD_LogFile::printf2("Warning, file %s not freed. Deleting it now.\n",name.GetFullPath().c_str());
#endif	
		delete resources[(*cur).first];
		cur++;
	}
}

KD_ResourceManager *KD_ResourceManager::InitResourceManager()
{
	if (singleton==NULL)
		singleton = new KD_ResourceManager();
	
	return singleton;
}

void KD_ResourceManager::CloseResourceManager()
{
	if (singleton==NULL)
		return;

	delete singleton;
	singleton = NULL;
}

KD_ResourceManager *KD_ResourceManager::GetResourceManager()
{
	return singleton;
}

void KD_ResourceManager::IncreaseUndefinedCount()
{
	undefinedCount++;
}

int KD_ResourceManager::GetUndefinedCount()
{
	return undefinedCount;
}

KD_Resource *KD_ResourceManager::GetResource(const KD_FilePath &name)
{
	map<KD_FilePath, KD_Resource *> :: iterator iter;

	iter = resources.find(name);

	KD_Resource *resource;

	if (iter == resources.end())
		resource = LoadResource(name);
	else
		resource = iter->second;

// ## Quitter si chargement a échoué ?  
  assert (resource!= NULL);
  
	resource->refCount++;
	return resource;
}

void KD_ResourceManager::ReleaseResource(const KD_FilePath &name)
{
	resources[name]->refCount--;
	if (resources[name]->refCount <= 0)
	{
		delete resources[name];
		resources.erase(name);
	}
}

void KD_ResourceManager::ReleaseResource(KD_Resource *img)
{
	map<KD_FilePath, KD_Resource *>::iterator cur = resources.begin();
	
	for (unsigned int i=0; i<resources.size(); i++)
	{
		if (img == /*resources[(*cur).first]*/(*cur).second)
		{
			ReleaseResource((*cur).first);
			return;
		}
		cur++;
	}

	assert(0);
}

bool KD_ResourceManager::IsLoaded(const KD_FilePath &file) const
{
	if (resources.find(file) == resources.end())
		return false;
	else
		return true;
}

KD_Resource *KD_ResourceManager::LoadResource(const KD_FilePath &file)
{
	string extension = file.GetFileExtension();
	int i;
	/* Note that fileName can be an archived file */

	// test if this is an image from the extension:
	for (i=0; i<KD_NB_IMAGE_EXT; i++)
		if (extension == IMAGE_EXTENSIONS[i])
			break;

	if (i!=KD_NB_IMAGE_EXT)
		return LoadImage2(file, true);

	// test if this is a sound from the extension:
	for (i=0; i<KD_NB_SOUND_EXT; i++)
		if (extension == SOUND_EXTENSIONS[i])
			break;

#ifndef NO_SOUND
	if (i!=KD_NB_SOUND_EXT)
		return LoadSound(file);
#endif

	if (extension == "spr")
		return LoadSprite(file);

	KD_LogFile::printf2("Error: unknown resource extension: %s",extension.c_str());
	return NULL;
}

KD_Image *KD_ResourceManager::LoadImage2(const KD_FilePath &fileName, bool loadOpenGL)
{
	KD_Image *img;

	
#ifndef NO_OPENGL    
	if ((Display::isOpenGL && loadOpenGL))
		img = new KD_OGLImage();
      else
#endif      
		img = new KD_SDLImage();

	img->Load(fileName);

	resources[fileName]=img;

	return img;
}

#ifndef NO_SOUND
KD_Sound *KD_ResourceManager::LoadSound(const KD_FilePath &fileName)
{
	KD_Sound *snd = new KD_Sound();
	/* Note that fileName can be an archived file */

	bool res = snd->LoadSound(fileName);
	if (res == false)
	{
		KD_LogFile::printf2("Warning, unable to load sound file %s\n",fileName.GetFullPath().c_str());
		delete snd;
		return NULL;
	}

	resources[fileName]= snd;

	return snd;
}
#endif

KD_Sprite *KD_ResourceManager::LoadSprite(const KD_FilePath &fileName)
{
	KD_Sprite *spr = new KD_Sprite();

	/* Archived xml file is loaded in loadXML. */
	bool res = spr->loadXML(fileName);
	if (res == false)
	{
		KD_LogFile::printf2("Warning, unable to load sprite file %s\n",fileName.GetFullPath().c_str());
		delete spr;
		return NULL;
	}

	resources[fileName]= spr;

	return spr;
}

KD_Image *KD_ResourceManager::NewUnreferencedImage() const
{
#ifndef NO_OPENGL  
	if (Display::isOpenGL) return (new KD_OGLImage()); 
		else
#endif      
	return (new KD_SDLImage());
}

void KD_ResourceManager::DeleteUnreferencedImage(KD_Image *img) const
{
	delete img;
}
