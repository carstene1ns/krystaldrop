#include "textevent.h"

#include <assert.h>
#include <stdarg.h>

#include "font.h"

#define KD_PRINT_FROM_LEFT 0
#define KD_PRINT_FROM_CENTER 1
#define KD_PRINT_FROM_RIGHT 2

#define KD_NO_MOVE 0
#define KD_LINEAR_MOVE 1
#define KD_QUADRATIC_MOVE 2

KD_TextEvent::KD_TextEvent() : KD_Event()
{
	font=0;
	x=0;
	y=0;
	alpha=255;
	printFrom = KD_PRINT_FROM_LEFT;
	isTextTimer=false;
	movingType=KD_NO_MOVE;
}

KD_TextEvent::~KD_TextEvent()
{

}

void KD_TextEvent::Update(float timeElapsed)
{
	// If the text is a timer.
	if (isTextTimer)
	{

		int time = (int) timeElapsed;

		if (time%60 < 10)
			sprintf(buf,"%d'0%d",time/60, time%60);
		else
			sprintf(buf,"%d'%d",time/60, time%60);
	}

	// If the sprite is moving:
	switch (movingType)
	{
	case KD_LINEAR_MOVE:
		float x0,y0,x1,y1,alpha0,alpha1,time;
		x0=movingParameters[0];
		y0=movingParameters[1];
		alpha0=movingParameters[2];
		x1=movingParameters[3];
		y1=movingParameters[4];
		alpha1=movingParameters[5];
		time=countDown;
		x=(int)(x0+(x1-x0)*timeElapsed/time);
		y=(int)(y0+(y1-y0)*timeElapsed/time);
		alpha = (int)(alpha0+(alpha1-alpha0)*timeElapsed/time);
		if (alpha>255) alpha=255;
		if (alpha<0) alpha=0;
		break;
	case KD_QUADRATIC_MOVE:
		float a,b,c,t;
		t=timeElapsed;
		a=movingParameters[0];
		b=movingParameters[1];
		c=movingParameters[2];
		x=(int)(a*t*t+b*t+c);
		a=movingParameters[3];
		b=movingParameters[4];
		c=movingParameters[5];
		y=(int)(a*t*t+b*t+c);
		a=movingParameters[6];
		b=movingParameters[7];
		c=movingParameters[8];
		alpha=(int)(a*t*t+b*t+c);
		if (alpha>255) alpha=255;
		if (alpha<0) alpha=0;
		break;
	}
}

void KD_TextEvent::Display()
{
	assert(font);
	switch (printFrom)
	{
	case KD_PRINT_FROM_LEFT:
		font->xyalphaprintf(alpha,x,y,buf);
		break;
	case KD_PRINT_FROM_CENTER:
		font->xyalphacenteredprintf(alpha,x,y,buf);
		break;
	case KD_PRINT_FROM_RIGHT:
		font->xyalpharightprintf(alpha,x,y,buf);
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

void KD_TextEvent::setLinearMove(int x0, int y0, int alpha0, int x1, int y1, int alpha1, float time)
{
	assert(time);
	setCountDownTimer(time);

	movingParameters[0]=(float)x0;
	movingParameters[1]=(float)y0;
	movingParameters[2]=(float)alpha0;
	movingParameters[3]=(float)x1;
	movingParameters[4]=(float)y1;
	movingParameters[5]=(float)alpha1;

	movingType = KD_LINEAR_MOVE;
}

void KD_TextEvent::setQuadraticMove(int x0, int y0, int alpha0, int x1, int y1, int alpha1, int x2, int y2, int alpha2, float time)
{
	assert(time);
	setCountDownTimer(time);

	// x(t) = a t^2 + b t + c
	// with a =
	movingParameters[0] = 2.0f/(time*time)*((float)x2+(float)x0-2.0f*(float)x1);
	// with b =
	movingParameters[1] = ((float)(4*x1-3*x0-x2))/time;
	// with c =
	movingParameters[2] = (float)x0;

	// y(t) = a t^2 + b t + c
	// with a =
	movingParameters[3] = 2.0f/(time*time)*((float)y2+(float)y0-2.0f*(float)y1);
	// with b =
	movingParameters[4] = ((float)(4*y1-3*y0-y2))/time;
	// with c =
	movingParameters[5] = (float)y0;

	// alpha(t) = a t^2 + b t + c
	// with a =
	movingParameters[6] = 2.0f/(time*time)*((float)alpha2+(float)alpha0-2.0f*(float)alpha1);
	// with b =
	movingParameters[7] = ((float)(4*alpha1-3*alpha0-alpha2))/time;
	// with c =
	movingParameters[8] = (float)alpha0;


	movingType = KD_QUADRATIC_MOVE;
}
