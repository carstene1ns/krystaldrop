#ifndef ImageManager_H
#define ImageManager_H

#ifdef WIN32
#pragma warning(disable:4786)
#endif

#include <string>
#include <map>

using namespace std;


class TACCRes;
class KD_Image;

/**
	Class containing an image (SDL_Surfaces).
	These images are used by ImageManager
  */
class KD_ImageManager
{
private:
	/// Singleton of the class KD_ImageManager
	static KD_ImageManager *singleton;

	/**
		The constructor is private. The only way to call it is to call the getImagaManager static method.
	*/
	KD_ImageManager();

public:
	
	~KD_ImageManager();

	/**
		Hashtable containing all the Images instanciated
	*/
	map<string, KD_Image *> images;

	/**
		Static Function returning the singleton KD_ImageManager
		If the singleton does not exist, the function does an instance of it.
	*/
	static KD_ImageManager *getImageManager();

	/**
		Returns the image named "name"
		Increase the reference counter for that image of 1.
	*/
	KD_Image *getImage(const string &name);

	/**
		Releases the image
		Decrease the reference counter for that image of 1.
		If the refCount is 0, deletes the Image.
	*/
	void releaseImage(const string &name);
	void releaseImage(KD_Image *img);

	/**
		Loads an image from a file.
		The name associated with the image will be the name of the file.
		The image will be loaded as an OpenGL texture if possible and unless specified not too (with loadOpenGL=false)
	*/
	//bool Load(char *fileName, bool loadOpenGL);

	/**
		Loads an image from an ACC file or a normal file.
		The name associated with the image will be the name of the file, not the one of the ACC file.
		If accFile is 0, the name will be the name of the real file.
		The image will be loaded as an OpenGL texture if possible and unless specified not too (with loadOpenGL=false)
	*/
	bool Load(TACCRes *accFile, char *fileName, bool loadOpenGL=true);

	/**
		Returns a new image that won't be referenced inside the hashtable of KD_ImageManager
	*/
	KD_Image *newUnreferencedImage();

	/**
		Delete an unreferenced image
	*/
	void deleteUnreferencedImage(KD_Image *img);

};

#endif
