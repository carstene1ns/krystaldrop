#include "FilePath.h"

#include <stdio.h>

#ifndef _WIN32
#include <ctype.h>
#endif

KD_FilePath::KD_FilePath() : fileName("") , filePath(""), archiveName("")
{

}

KD_FilePath::KD_FilePath(char *path)
{
	ComputePath(path);
}

KD_FilePath::KD_FilePath(const string &path)
{
	ComputePath(path);
}

KD_FilePath::KD_FilePath(const KD_FilePath &file)
{
	fileName = file.fileName;
	filePath = file.filePath;
	archiveName = file.archiveName;

	map<string, string>::const_iterator cur = file.parameters.begin();

	for (unsigned i=0; i<file.parameters.size(); i++)
	{
		parameters[(*cur).first] = (*cur).second;
		cur++;
	}
}

KD_FilePath& KD_FilePath::operator = (const KD_FilePath& file)
{
	if (&file != this)
	{
		this->KD_FilePath::~KD_FilePath();
		::new (this) KD_FilePath(file);
	}
	return *this;
}

bool KD_FilePath::operator == (const KD_FilePath &file) const
{
	if (fileName == file.fileName && filePath == file.filePath && archiveName == file.archiveName && parameters.size() == file.parameters.size())
	{
		map<string, string>::const_iterator cur = parameters.begin();
		map<string, string>::const_iterator cur2 = file.parameters.begin();

		for (unsigned i=0; i<parameters.size(); i++)
		{
			if ((*cur).first != (*cur2).first || (*cur).second != (*cur2).second)
				return false;
			cur++;
		}
		return true;
	}
	else
		return false;
}

bool KD_FilePath::operator < (const KD_FilePath &file) const
{
	if (fileName < file.fileName)
		return true;
	else if (fileName == file.fileName && filePath < file.filePath)
		return true;
	else if (fileName == file.fileName && filePath == file.filePath && archiveName < file.archiveName)
		return true;
	else if (fileName == file.fileName && filePath == file.filePath && archiveName == file.archiveName && parameters.size() < file.parameters.size())
		return true;
	else  if (fileName == file.fileName && filePath == file.filePath && archiveName == file.archiveName && parameters.size() == file.parameters.size())
	{
		map<string, string>::const_iterator cur = parameters.begin();
		map<string, string>::const_iterator cur2 = file.parameters.begin();

		for (unsigned i=0; i<parameters.size(); i++)
		{
			if ((*cur).first < (*cur2).first)
				return true;
			else if ((*cur).first > (*cur2).first)
				return false;

			if ((*cur).second < (*cur2).second)
				return true;
			else if ((*cur).second > (*cur2).second)
				return false;

			cur++;
		}
		return false;
	}
	else
		return false;
}

void KD_FilePath::PrefixDirectory(const string &path)
{
	ComputePath( path + GetFullPath() );
}

KD_FilePath::~KD_FilePath()
{

}

void KD_FilePath::AddParameter(string name, string value)
{
	parameters[name] = value;
}

void KD_FilePath::ComputePath(const string &path)
{
	string workingPath = path;

	// Replace all backslashes with forwardslashes
	for (unsigned int i=0; i<workingPath.size(); i++)
		if (workingPath[i]=='\\') workingPath[i]='/';

	// remove trailing '/'
	while (workingPath[workingPath.size()-1]=='/')
	{
		workingPath = workingPath.substr(0,workingPath.size()-1);
	}

	// get the filename.
	size_t pos = workingPath.rfind('/');
	if (pos == workingPath.npos)
	{
		fileName = workingPath;
		filePath = "";
		archiveName = "";
		return;
	}

	fileName = workingPath.substr(pos+1);

	string directory = workingPath.substr(0,pos+1);

#define ARCHIVE_SUFFIX ".acc/"
	pos = directory.find(ARCHIVE_SUFFIX);
	if (pos == directory.npos)
	{
		filePath = directory;
		filePath = NormalizePath(filePath);
		archiveName = "";
		return;
	}

	filePath = directory.substr(pos+5);
	archiveName = directory.substr(0,pos+4);
	archiveName = NormalizePath(archiveName);
	filePath = NormalizePath(filePath);
}

string KD_FilePath::removeDoubleDot(string path)
{
	
	size_t pos = path.find("/../");
	if (pos == path.npos)
		return path;

	string pathCopy = path;

	int pos2 = (int) pathCopy.rfind('/',pos-1);
	if (pos2 == (signed) pathCopy.npos)
		pos2 = -1;

	pathCopy.erase(pos2+1,pos+3-pos2);

	// Recursive Call.
	return removeDoubleDot(pathCopy);
}

/*string KD_FilePath::removeSimpleDot(string path)
{
	size_t pos = path.find("./");
	if (pos == path.npos)
		return path;
	
    string newPath = path;
	newPath.erase(pos,2);
	return removeSimpleDot(newPath);
}*/

string KD_FilePath::NormalizePath(string path)
{
	return /*removeSimpleDot(*/removeDoubleDot(path)/*)*/;
}

string KD_FilePath::GetPath() const
{
	return filePath+fileName;
}

string KD_FilePath::GetFullPath() const
{
	if (archiveName!="")
		return archiveName+'/'+filePath+fileName;
	else
		return filePath+fileName;
}

string KD_FilePath::GetArchiveName() const
{
	return archiveName;
}

bool KD_FilePath::IsArchived() const
{
	return !archiveName.empty();
}

string KD_FilePath::GetDirectory() const
{
	return filePath;
}

string KD_FilePath::GetFullDirectory() const
{
	if (archiveName!="")
		return archiveName+'/'+filePath;
	else
		return filePath;
}

string KD_FilePath::GetFileExtension() const
{
	size_t pos = fileName.rfind('.');
	if (pos == fileName.npos)
		return "";

	string ext = fileName.substr(pos+1);

	for (unsigned int i=0; i<ext.size(); i++)
		ext[i] = tolower(ext[i]);

	return ext;
}

void KD_FilePath::DebugPrint()
{
	printf("%s\n%s\n%s\n",fileName.c_str(),filePath.c_str(),archiveName.c_str());
}
