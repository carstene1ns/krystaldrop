#include "InputBox.h"

#include <assert.h>

#include "../../KDpp/Video/Font.h"
#include "../../KDpp/Controller/UserInterface/Keyboard.h"

#define KD_PRINT_FROM_LEFT 0
#define KD_PRINT_FROM_CENTER 1
#define KD_PRINT_FROM_RIGHT 2

KD_InputBox::KD_InputBox() : KD_Event()
{
	font=0;
	x=0;
	y=0;
	PrintFrom = KD_PRINT_FROM_LEFT;
	maxLength=3;
	str="";
}

KD_InputBox::~KD_InputBox()
{

}

void KD_InputBox::Update(float timeElapsed)
{
	unsigned short unicode = KD_Keyboard::GetKeyboard()->GetAsciiCharacter();

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
	switch (PrintFrom)
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

void KD_InputBox::SetTextCoordinates(int x, int y)
{
	this->x = x;
	this->y = y;
}

void KD_InputBox::SetText(char *str, ...)
{
	this->str = str;
}

string &KD_InputBox::GetText()
{
	return str;
}

void KD_InputBox::SetTextFont(KD_Font *font)
{
	this->font = font;
}

void KD_InputBox::PrintFromLeft()
{
	PrintFrom = KD_PRINT_FROM_LEFT;
}

void KD_InputBox::PrintFromCenter()
{
	PrintFrom = KD_PRINT_FROM_CENTER;
}

void KD_InputBox::PrintFromRight()
{
	PrintFrom = KD_PRINT_FROM_RIGHT;
}

void KD_InputBox::SetMaxLength(unsigned short maxLength)
{
	this->maxLength = maxLength;
}

int KD_InputBox::GetLength()
{
	return str.size();
}
