#ifndef FONT_H
#define FONT_H

#include <stdio.h>
#include <stdarg.h>

#include "../Tools/defines.h"
#include "../Tools/FilePath.h"
#include "../Resources/Resource.h"

#define PRINTF_BUF_SIZE 256

class KD_Image;

#define KD_FONT_NORMAL		0x00
#define KD_FONT_BOLD		0x01
#define KD_FONT_ITALIC		0x02
#define KD_FONT_UNDERLINE	0x04

/**
	Class containing a font.
	There are 2 different ways to load a font:
	From a ttf file or from a txt (Krystal Drop format) file.
*/
class DllExport KD_Font : public KD_Resource
{
private:
	KD_Image *letters[256];

	/// Horizontal spacing between letters. This can be different from font width.
	int spacing[256];

	/// Numbers of pixels to put for a space character.
	int spaceSize;

	/// Numbers of pixels of a line (height of a line).
	int returnSize;

	/// Height of the font.
	int fontHeight;

	/// True if there is a color-key.
	bool isColorKey;

	/// value of the color key.
	unsigned int colorKey;

public:
	KD_Font ();
	KD_Font (const KD_FilePath &fileName, const string font_dir= "");
	KD_Font (const KD_FilePath &fileName, int ptsize, int style= KD_FONT_NORMAL, int r=255, int g=255, int b=255, const string font_dir= "");
	~KD_Font ();

	/**
		Free the font from memory
	*/
	void free();

	/**
		Loads a font from the file fileName (Krystal Drop font format)
	*/
	virtual bool Load (const KD_FilePath &fileName, const string font_dir= "");

	/**
		Loads a font from the ttf file "fileName", with point size "ptsize" and style "style".
	*/
	virtual bool LoadTTF (const KD_FilePath &fileName, int ptsize, int style= KD_FONT_NORMAL, int r= 255, int g= 255, int b= 255, const string font_dir= "");

	/**
		Prints something to the screen. The syntax is the same as printf, except you have to specify x and y before.
	*/
	void xyprintf(float x, float y, char *str, ...);

	//{
	/**
		Prints something to the screen. The syntax is the same as xyprintf, except you have to specify the alpha blending, but it will be only available if ConvertToColorKey has been called before in SDL mode.
	*/
	void xyalphaprintf(int alpha, float x, float y, char *str, ...);
	void xyalpharightprintf(int alpha, float x, float y, char *str, ...);
	void xyalphacenteredprintf(int alpha, float x, float y, char *str, ...);
	//}

	//{
	/**
		Prints something to the screen. The syntax is the same as xyprintf, except you have to specify the alpha blending, but it will be only available if ConvertToColorKey has been called before in SDL mode.
		You have also to specify the color, the resizing parameters and the angle of rotation but all those will only be available in OpenGL mode.
	*/
	void xycoloralpharotozoomprintf(int r, int g, int b, int alpha, float resizeX, float resizeY, float rotX, float rotY, float angle, float x, float y, char *str, ...);
	void xycoloralpharotozoomrightprintf(int r, int g, int b, int alpha, float resizeX, float resizeY, float rotX, float rotY, float angle, float x, float y, char *str, ...);
	void xycoloralpharotozoomcenteredprintf(int r, int g, int b, int alpha, float resizeX, float resizeY, float rotX, float rotY, float angle, float x, float y, char *str, ...);

	//}

	/**
		Compute the length of the given string.
		If the string is composed of several lines, ComputeLength will return the value of the longest line.
	*/
	int ComputeLength(char *buf);

	/**
		Same effect as xyprintf but the coordinates must be specified from the right bottom corner.
	*/
	void xyrightprintf(float x, float y, char *str, ...);

	/**
		Same effect as xyprintf but the coordinate x must be specified from the center.
	*/
	void xycenteredprintf(float x, float y, char *str, ...);

	/**
		Generate a new Font class shrunk or expanded from the given ratio.
	*/
	KD_Font *resize(float ratio);

	/**
		Converts the alpha surfaces to surfaces without alpha transparency, but with a color-key.
		If the alpha channel is less than "alphaTrigger", the pixel will be converted to colorKey,
		else, the pixels will be kept.
	*/
	void ConvertToColorKey(unsigned int key, int alphaTrigger);
	
	/**
		Returns the size in pixels between 2 lines
	*/
	int GetReturnSize() const;

	/**
		Returns the height in pixels of the font
	*/
	int GetFontHeight() const;
};

#endif
