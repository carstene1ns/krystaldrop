#ifndef ResourceSet_H
#define ResourceSet_H

#include "../Tools/defines.h"
#include "../Tools/FilePath.h"

#include <string>
#include <map>

using namespace std;

class KD_Resource;
class KD_Image;

/**
	A ResourceSet references a resource and links it to a name.
	It calls the ResourceManager to do this.
  */
class DllExport KD_ResourceSet
{
	/**
		Hashtable linking the resources names to the file paths. 
	*/
	map<string, KD_FilePath> files;

public:

	KD_ResourceSet();

	virtual ~KD_ResourceSet();

	/**
		Returns the resource pointed by file "file"
		Increase the reference counter for that resource by 1.
	*/
	KD_Resource *GetResource(const string &name);

	/**
		Releases the resource
		Decrease the reference counter for that resource by 1.
		If the refCount is 0, deletes the resource.
	*/
	void ReleaseResource(const string &name);
	void ReleaseResource(KD_Resource *resource);

	bool IsDefined(const string &name) const;

	/**
		Link a name to a new file.
		The file is not loaded as long as GetResource is not called.
	*/
	bool RegisterResource(string resourceName, KD_FilePath fileName);
	
	/**
		Link a name to a new resource.
		This method should be called as few as possible (in fact it should be called for resources that does not restrict to a file, like ttf textures that need the font size).
	*/
	bool RegisterResource(string resourceName, KD_Resource *resource);

	/**
		Removes the link between a name and a resource.
		Warning, this method should be used sparsely, since the resources are not deleted and might still be unsuccesfully called by there name.
	*/
	bool UnRegisterResource(string resourceName);

	/**
		Removes the link between all names and all resources.
		Warning, this method should be used sparsely, since the resources are not deleted and might still be unsuccesfully called by there name.
	*/
	void UnRegisterAllResources();


	/**
		Returns the filename associated with the resource "resourceName"
	*/
	KD_FilePath GetFileName(string resourceName);

	/**
		Load a resource file (a file containing a mapping between names and files).
	*/
	bool LoadResourceFile(const KD_FilePath &fileName);
};

#endif
