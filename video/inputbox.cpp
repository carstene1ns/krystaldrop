#include "inputbox.h"

#include <assert.h>

#include "font.h"
#include "../interf/keyboard.h"

#define KD_PRINT_FROM_LEFT 0
#define KD_PRINT_FROM_CENTER 1
#define KD_PRINT_FROM_RIGHT 2

KD_InputBox::KD_InputBox() : KD_Event()
{
	font=0;
	x=0;
	y=0;
	printFrom = KD_PRINT_FROM_LEFT;
	maxLength=3;
	str="";
}

KD_InputBox::~KD_InputBox()
{

}

void KD_InputBox::Update(float timeElapsed)
{
	unsigned short unicode = KD_Keyboard::getKeyboard()->getAsciiCharacter();

	// If the character is out of ASCII range.
	if (unicode == 0 || unicode >= 0x80) return;

	// If backspace
	if (unicode == 8)
		str = str.substr(0, str.size()-1);

	if (str.size() == maxLength)
		return;

	if ( unicode >= 32 && unicode <= 127 )
		str += unicode;
	
}

void KD_InputBox::Display()
{
	assert(font);
	switch (printFrom)
	{
	case KD_PRINT_FROM_LEFT:
		font->xyprintf(x,y,(char *)str.c_str());
		break;
	case KD_PRINT_FROM_CENTER:
		font->xycenteredprintf(x,y,(char *)str.c_str());
		break;
	case KD_PRINT_FROM_RIGHT:
		font->xyrightprintf(x,y,(char *)str.c_str());
		break;
	}
}

void KD_InputBox::setTextCoordinates(int x, int y)
{
	this->x = x;
	this->y = y;
}

void KD_InputBox::setText(char *str, ...)
{
	this->str = str;
}

string &KD_InputBox::getText()
{
	return str;
}

void KD_InputBox::setTextFont(KD_Font *font)
{
	this->font = font;
}

void KD_InputBox::printFromLeft()
{
	printFrom = KD_PRINT_FROM_LEFT;
}

void KD_InputBox::printFromCenter()
{
	printFrom = KD_PRINT_FROM_CENTER;
}

void KD_InputBox::printFromRight()
{
	printFrom = KD_PRINT_FROM_RIGHT;
}

void KD_InputBox::setMaxLength(int maxLength)
{
	this->maxLength = maxLength;
}
