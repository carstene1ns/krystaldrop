#ifndef ResourceManager_H
#define ResourceManager_H

#include <string>
#include <map>

using namespace std;


#include "../Tools/defines.h"
#include "../Tools/FilePath.h"

class KD_Resource;
class KD_Image;
class KD_Sprite;
class KD_Sound;

/**
	The ResourceManager references every file loaded.
	If a Resource is to be loaded twice, it will instead return the previous value of the resource.
  */
class DllExport KD_ResourceManager
{
	friend class KD_ResourceSet;
	/**
		Singleton ResourceManager
	*/
	static KD_ResourceManager *singleton;

	/**
		Hashtable containing all the Resources instanciated
	*/
	map<KD_FilePath, KD_Resource *> resources;

	KD_ResourceManager();

	virtual ~KD_ResourceManager();

	/// Count for generating the name of the resources that are not loaded by the resourceManager but given by a third party function.
	int undefinedCount;

public:

	static KD_ResourceManager *InitResourceManager();
	static void CloseResourceManager();
	static KD_ResourceManager *GetResourceManager();

	/**
		Increases by one the undefinedCount variable.
	*/
	void IncreaseUndefinedCount();

	/**
		return the undefinedCount variable.
	*/
	int GetUndefinedCount();

	/**
		Returns the resource pointed by file "file"
		Increase the reference counter for that resource by 1.
	*/
	KD_Resource *GetResource(const KD_FilePath &file);

	//{
	/**
		Releases the resource
		Decrease the reference counter for that resource by 1.
		If the refCount is 0, deletes the resource.
	*/
	void ReleaseResource(const KD_FilePath &name);
	void ReleaseResource(KD_Resource *resource);
	//}

	/**
		Returns true if the resource is available, no if it has to be loaded (if the resource is not in the map).
	*/
	bool IsLoaded(const KD_FilePath &file) const;

private:
	/**
		Loads the file "file".
	*/
	KD_Resource *LoadResource(const KD_FilePath &file);

public:
	/**
		Loads an image from a file if the image has not already been loaded.
		The image will be loaded as an OpenGL texture if possible and unless specified not too (with loadOpenGL=false)
		If the image has already be loaded, it will be returned and its refCount will be incremented.
	*/
	KD_Image *LoadImage2(const KD_FilePath &fileName, bool loadOpenGL = true);

	/**
		Loads a sound from a file if the sound has not already been loaded.
		If the sound has already be loaded, it will be returned and its refCount will be incremented.
	*/
#ifndef NO_SOUND
	KD_Sound *LoadSound(const KD_FilePath &fileName);
#endif

	/**
		Loads a sprite from a xml file if the sprite has not already been loaded.
		If the sprite has already be loaded, it will be returned and its refCount will be incremented.
	*/
	KD_Sprite *LoadSprite(const KD_FilePath &fileName);


	/**
		Returns a new image that won't be referenced inside the hashtable of KD_ImageManager
	*/
	KD_Image *NewUnreferencedImage() const;

	/**
		Delete an unreferenced image
	*/
	void DeleteUnreferencedImage(KD_Image *img) const;
};

#endif
