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
	*/
	bool Load(char *fileName);

	/**
		Loads an image from an ACC file.
		The name associated with the image will be the name of the file, not the one of the ACC file.
	*/
	bool Load(TACCRes *accFile, char *fileName);
};

#endif
