#ifndef FONT_H
#define FONT_H

#include "SDL/SDL.h"

#include <stdio.h>
#include <stdarg.h>

class TACCRes;

class KD_Font
{
private:
	SDL_Surface *letters[256];

	/// Numbers of pixels to put for a space character.
	int spaceSize;

	/// Numbers of pixels of a line (height of a line).
	int returnSize;

public:
	KD_Font ();
	KD_Font (char *fileName);
	KD_Font (TACCRes *accFile, char *fileName);
	~KD_Font ();

	/**
		Loads a font from the file fileName
	*/
	bool Load (char *fileName);

	/**
		Loads a font from the file fileName located in the acc file pointed by accFile.
		If accFile==0, it will behave just as Load(fileName);
	*/
	bool LoadFromAcc (TACCRes *accFile, char *fileName);

	/**
		Prints something to the screen. The syntax is the same as printf, except you have to specify x and y before.
	*/
	void xyprintf(int x, int y, char *str, ...);

	/**
		Generate a new Font class shrunk or expanded from the given ratio.
	*/
	KD_Font *resize(float ratio);
};

#endif
