#ifndef TextFile_H
#define TextFile_H

#include <stdio.h>
#include <stdarg.h>

#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <string>

using namespace std;

class TACCRes;

/**
	class used to read text files, whether they are or not in a .ACC file.
  */
class KD_TextFile
{
	/// The text contained in the file
	char *text;
	
	/// The position of the pointer in the file
	int pos;

	/// Size of the file
	int size;

public:
	/**
		Default constructor.
	*/
	KD_TextFile();

	/**
		Opens the textfile named fileName and copies it into memory.
	*/
	KD_TextFile(char *fileName);

	/**
		Make the TextFile points to the acc file. A copy is made of it, so that the ACC file can be released safely.
	*/
	KD_TextFile(TACCRes *accFile, char *fileName);

	/**
		Opens the textfile named fileName and copies it into memory.
	*/
	bool Load(char *fileName);

	/**
		Make the TextFile points to the acc file. A copy is made of it, so that the ACC file can be released safely.
		If accFile is 0, then the method attempt to load the real file fileName.
	*/
	bool Load(TACCRes *accFile, char *fileName);

	/**
		Destructor, it frees the space allocated for the file.
	*/
	~KD_TextFile();

	/**
		Returns a pointer to the current position in the file.
	*/
	char *getPosition();

	/**
		Goes through the file until we find the character c
		Returns a pointer to that position in the file.
	*/
	char *find(char c);

	/**
		Goes through the file until we find a return character
		Returns a pointer to the position just after in the file.
	*/
	char *jumpLine();

	/**
		Returns true if the end of File has been reached.
	*/
	bool isEOF();

	/**
		Returns the next string in the file between "..."
	*/
	string getString();
};

#endif
