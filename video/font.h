#ifndef FONT_H
#define FONT_H

#include <stdio.h>
#include <stdarg.h>

#define PRINTF_BUF_SIZE 256

class TACCRes;
class KD_Image;

class KD_Font
{
private:
	KD_Image *letters[256];

	/// Numbers of pixels to put for a space character.
	int spaceSize;

	/// Numbers of pixels of a line (height of a line).
	int returnSize;

	/// True if there is a color-key.
	bool isColorKey;

	/// value of the color key.
	unsigned int colorKey;

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

	//{
	/**
		Prints something to the screen. The syntax is the same as xyprintf, except you have to specify the alpha blending, but it will be only available if convertToColorKey has been called before in SDL mode.
	*/
	void xyalphaprintf(int alpha, int x, int y, char *str, ...);
	void xyalpharightprintf(int alpha, int x, int y, char *str, ...);
	void xyalphacenteredprintf(int alpha, int x, int y, char *str, ...);
	//}

	//{
	/**
		Prints something to the screen. The syntax is the same as xyprintf, except you have to specify the alpha blending, but it will be only available if convertToColorKey has been called before in SDL mode.
		You have also to specify the color, the resizing parameters and the angle of rotation but all those will only be available in OpenGL mode.
	*/
	void xycoloralpharotozoomprintf(int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle, int x, int y, char *str, ...);
	void xycoloralpharotozoomrightprintf(int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle, int x, int y, char *str, ...);
	void xycoloralpharotozoomcenteredprintf(int r, int g, int b, int alpha, float resizeX, float resizeY, int rotX, int rotY, float angle, int x, int y, char *str, ...);

	//}

	/**
		Compute the length of the given string.
		If the string is composed of several lines, computeLength will return the value of the longest line.
	*/
	int computeLength(char *buf);

	/**
		Same effect as xyprintf but the coordinates must be specified from the right bottom corner.
	*/
	void xyrightprintf(int x, int y, char *str, ...);

	/**
		Same effect as xyprintf but the coordinate x must be specified from the center.
	*/
	void xycenteredprintf(int x, int y, char *str, ...);

	/**
		Generate a new Font class shrunk or expanded from the given ratio.
	*/
	KD_Font *resize(float ratio);

	/**
		Converts the alpha surfaces to surfaces without alpha transparency, but with a color-key.
		If the alpha channel is less than "alphaTrigger", the pixel will be converted to colorKey,
		else, the pixels will kept.
	*/
	void convertToColorKey(unsigned int key, int alphaTrigger);
};

#endif
