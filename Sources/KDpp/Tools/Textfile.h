#ifndef TextFile_H
#define TextFile_H

#include <stdio.h>
#include <stdarg.h>
#include <string>

#include "defines.h"
#include "FilePath.h"

using namespace std;


/**
	class used to read text files, whether they are or not in a .ACC file.
  */
class DllExport KD_TextFile
{
	/// The text contained in the file
	char *text;
	
	/// The position of the pointer in the file
public:
	int pos;
private:
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
	KD_TextFile(const KD_FilePath &fileName);

	/**
		Opens the textfile named fileName and copies it into memory.
	*/
	bool Load(const KD_FilePath &fileName);

	/**
		Destructor, it frees the space allocated for the file.
	*/
	~KD_TextFile();

	/**
		Returns a pointer to the current position in the file.
	*/
	char *GetPosition();

	/**
		Goes through the file until we find the character c
		Returns a pointer to that position in the file.
	*/
	char *ReadNewLine();

	/**
		Goes through the file until we find a return character
		Returns a pointer to the position just after in the file.
	*/
	char *JumpLine();

	/**
		Returns true if the end of File has been reached.
	*/
	bool IsEOF();

	/**
		Returns the next string in the file between "..."
	*/
	string GetString();
};

#endif
