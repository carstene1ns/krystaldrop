#include "textevent.h"

#include <assert.h>
#include <stdarg.h>

#include "font.h"

#define KD_PRINT_FROM_LEFT 0
#define KD_PRINT_FROM_CENTER 1
#define KD_PRINT_FROM_RIGHT 2

KD_TextEvent::KD_TextEvent() : KD_Event()
{
	font=0;
	x=0;
	y=0;
	printFrom = KD_PRINT_FROM_LEFT;
	isTextTimer=false;
}

KD_TextEvent::~KD_TextEvent()
{

}

void KD_TextEvent::Update(float timeElapsed)
{
	if (!isTextTimer) return;

	int time = (int) timeElapsed;

	if (time%60 < 10)
		sprintf(buf,"%d'0%d",time/60, time%60);
	else
		sprintf(buf,"%d'%d",time/60, time%60);
}

void KD_TextEvent::Display()
{
	assert(font);
	switch (printFrom)
	{
	case KD_PRINT_FROM_LEFT:
		font->xyprintf(x,y,buf);
		break;
	case KD_PRINT_FROM_CENTER:
		font->xycenteredprintf(x,y,buf);
		break;
	case KD_PRINT_FROM_RIGHT:
		font->xyrightprintf(x,y,buf);
		break;
	}
}

void KD_TextEvent::setTextCoordinates(int x, int y)
{
	this->x = x;
	this->y = y;
}


void KD_TextEvent::setText(char *str, ...)
{
	va_list argptr;
	va_start (argptr, str);
	vsprintf (buf, str, argptr);
	va_end (argptr);
}

void KD_TextEvent::setTextFont(KD_Font *font)
{
	this->font = font;
}

void KD_TextEvent::activateTextTimer()
{
	isTextTimer = true;
}

void KD_TextEvent::inactivateTextTimer()
{
	isTextTimer = false;
}

void KD_TextEvent::printFromLeft()
{
	printFrom = KD_PRINT_FROM_LEFT;
}

void KD_TextEvent::printFromCenter()
{
	printFrom = KD_PRINT_FROM_CENTER;
}

void KD_TextEvent::printFromRight()
{
	printFrom = KD_PRINT_FROM_RIGHT;
}
