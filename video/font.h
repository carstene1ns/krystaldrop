#ifndef FONT_H
#define FONT_H

#include "SDL/SDL.h"

#include <stdio.h>
#include <stdarg.h>

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
	bool Load (char *fileName);

	/**
		Prints something to the screen. The syntax is the same as printf, except you have to specify x and y before.
	*/
	void xyprintf(int x, int y, char *str, ...);
};

#endif