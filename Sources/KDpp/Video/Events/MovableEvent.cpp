//#include "../global.h"

#include <assert.h>

#include "MovableEvent.h"

#include "../Display.h"

#define KD_NO_MOVE 0
#define KD_LINEAR_MOVE 1
#define KD_QUADRATIC_MOVE 2
#define KD_GRAVITY_MOVE 3

KD_MovableEvent::KD_MovableEvent() : KD_Event()
{
	x=0;
	y=0;
	alpha=255;
	red=255;
	green=255;
	blue = 255;
	resizeX = 1.0f;
	resizeY = 1.0f;
	angle = 0.0f;
	movingType=KD_NO_MOVE;

	timeVisible=1;
	timeInvisible=0;
}

KD_MovableEvent::~KD_MovableEvent()
{

}

void KD_MovableEvent::setCoordinates(int x, int y)
{
	this->x = x;
	this->y = y;
}

///////////// CHANGER timeELAPSED!!!!!
//// ENTRE EVENT ET DISPLAY, LE NOM DE LA VARIABLE EST ABSOLUMENT PAUMANT!
/// Bon en fait c'est la même chose: c'est le temps écoulé depuis la création...
void KD_MovableEvent::Update(float timeElapsed)
{
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
		time=this->countDown;
		x=(int)(x0+(x1-x0)*timeElapsed/time);
		y=(int)(y0+(y1-y0)*timeElapsed/time);
		alpha = (int)(alpha0+(alpha1-alpha0)*timeElapsed/time);
		if (alpha>255) alpha=255;
		if (alpha<0) alpha=0;
		break;
	case KD_QUADRATIC_MOVE:
		float a,b,c,t;
		t=this->timeElapsed;
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
		a=movingParameters[9];
		b=movingParameters[10];
		c=movingParameters[11];
		red=(int)(a*t*t+b*t+c);
		if (red>255) red=255;
		if (red<0) red=0;
		a=movingParameters[12];
		b=movingParameters[13];
		c=movingParameters[14];
		green=(int)(a*t*t+b*t+c);
		if (green>255) green=255;
		if (green<0) green=0;
		a=movingParameters[15];
		b=movingParameters[16];
		c=movingParameters[17];
		blue=(int)(a*t*t+b*t+c);
		if (blue>255) blue=255;
		if (blue<0) blue=0;
		a=movingParameters[18];
		b=movingParameters[19];
		c=movingParameters[20];
		resizeX=a*t*t+b*t+c;
		a=movingParameters[21];
		b=movingParameters[22];
		c=movingParameters[23];
		resizeY=a*t*t+b*t+c;
		a=movingParameters[24];
		b=movingParameters[25];
		c=movingParameters[26];
		angle=a*t*t+b*t+c;
		break;
	case KD_GRAVITY_MOVE:
		//???????????????????????????
		movingParameters[0] += movingParameters[2]*Display::timeElapsed;
		movingParameters[1] += movingParameters[3]*Display::timeElapsed;
		x = (int)movingParameters[0];
		y = (int)movingParameters[1];
		movingParameters[3]+=movingParameters[4];

		float r0,g0,b0,r1,g1,b1;
		r0=movingParameters[5];
		g0=movingParameters[6];
		b0=movingParameters[7];
		alpha0=movingParameters[8];
		r1=movingParameters[9];
		g1=movingParameters[10];
		b1=movingParameters[11];
		alpha1=movingParameters[12];
		time=countDown;
		red = (int)(r0+(r1-r0)*timeElapsed/time);
		if (red>255) red=255;
		if (red<0) red=0;
		green = (int)(g0+(g1-g0)*timeElapsed/time);
		if (green>255) green=255;
		if (green<0) green=0;
		blue = (int)(b0+(b1-b0)*timeElapsed/time);
		if (blue>255) blue=255;
		if (blue<0) blue=0;
		alpha = (int)(alpha0+(alpha1-alpha0)*timeElapsed/time);
		if (alpha>255) alpha=255;
		if (alpha<0) alpha=0;
		angle += /*time**/movingParameters[13]*Display::timeElapsed;

		break;
	}
	UpdateMovable(timeElapsed);
}

void KD_MovableEvent::UpdateMovable(float timeElapsed)
{
	
}

void KD_MovableEvent::Display()
{
	// blinking
	int ticks = (int)(timeElapsed*1000.0f);
	if (ticks%(timeVisible+timeInvisible)>=timeVisible)
		return;

	DisplayMovable();
}

void KD_MovableEvent::DisplayMovable()
{

}

void KD_MovableEvent::setLinearMove(int x0, int y0, int alpha0, int x1, int y1, int alpha1, float time)
{
	assert(time);
	SetCountDownTimer(time);

	movingParameters[0]=(float)x0;
	movingParameters[1]=(float)y0;
	movingParameters[2]=(float)alpha0;
	movingParameters[3]=(float)x1;
	movingParameters[4]=(float)y1;
	movingParameters[5]=(float)alpha1;

	movingType = KD_LINEAR_MOVE;
}

void KD_MovableEvent::setQuadraticMove(int x0, int y0, int alpha0, int x1, int y1, int alpha1, int x2, int y2, int alpha2, float time)
{
	setQuadraticMove(x0,y0,255,255,255,alpha0,1,1,0,
					x1,y1,255,255,255,alpha1,1,1,0,
					x2,y2,255,255,255,alpha2,1,1,0, time);
}

void KD_MovableEvent::setQuadraticMove(int x0, int y0, int r0, int g0, int b0, int alpha0, float resizeX0, float resizeY0, float angle0,
									int x1, int y1, int r1, int g1, int b1, int alpha1, float resizeX1, float resizeY1, float angle1,
									int x2, int y2, int r2, int g2, int b2, int alpha2, float resizeX2, float resizeY2, float angle2, float time)
{
	assert(time);
	SetCountDownTimer(time);

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

	// r(t) = a t^2 + b t + c
	// with a =
	movingParameters[9] = 2.0f/(time*time)*((float)r2+(float)r0-2.0f*(float)r1);
	// with b =
	movingParameters[10] = ((float)(4*r1-3*r0-r2))/time;
	// with c =
	movingParameters[11] = (float)r0;

	// g(t) = a t^2 + b t + c
	// with a =
	movingParameters[12] = 2.0f/(time*time)*((float)g2+(float)g0-2.0f*(float)g1);
	// with b =
	movingParameters[13] = ((float)(4*g1-3*g0-g2))/time;
	// with c =
	movingParameters[14] = (float)g0;

	// b(t) = a t^2 + b t + c
	// with a =
	movingParameters[15] = 2.0f/(time*time)*((float)b2+(float)b0-2.0f*(float)b1);
	// with b =
	movingParameters[16] = ((float)(4*b1-3*b0-b2))/time;
	// with c =
	movingParameters[17] = (float)b0;

	// resizeX(t) = a t^2 + b t + c
	// with a =
	movingParameters[18] = 2.0f/(time*time)*((float)resizeX2+(float)resizeX0-2.0f*(float)resizeX1);
	// with b =
	movingParameters[19] = ((float)(4*resizeX1-3*resizeX0-resizeX2))/time;
	// with c =
	movingParameters[20] = (float)resizeX0;

	// resizeY(t) = a t^2 + b t + c
	// with a =
	movingParameters[21] = 2.0f/(time*time)*((float)resizeY2+(float)resizeY0-2.0f*(float)resizeY1);
	// with b =
	movingParameters[22] = ((float)(4*resizeY1-3*resizeY0-resizeY2))/time;
	// with c =
	movingParameters[23] = (float)resizeY0;

	// angle(t) = a t^2 + b t + c
	// with a =
	movingParameters[24] = 2.0f/(time*time)*((float)angle2+(float)angle0-2.0f*(float)angle1);
	// with b =
	movingParameters[25] = ((float)(4*angle1-3*angle0-angle2))/time;
	// with c =
	movingParameters[26] = (float)angle0;


	movingType = KD_QUADRATIC_MOVE;
}

/*void KD_MovableEvent::setGravityMove(int x, int y, float speedX, float speedY, float gravity, int r0, int g0, int b0, int alpha0, int r1, int g1, int b1, int alpha1, float angleSpeed, float time)
{
	assert(time);
	SetCountDownTimer(time);

	this->x = x;
	this->y = y;

	// in order for x and y to be handled as float, they will be part of the movingParameters.
	movingParameters[0] = (float)x;
	movingParameters[1] = (float)y;
	movingParameters[2] = speedX;
	movingParameters[3] = speedY;
	movingParameters[4] = gravity;

	movingParameters[5] = (float)r0;
	movingParameters[6] = (float)g0;
	movingParameters[7] = (float)b0;
	movingParameters[8] = (float)alpha0;

	movingParameters[9] = (float)r1;
	movingParameters[10] = (float)g1;
	movingParameters[11] = (float)b1;
	movingParameters[12] = (float)alpha1;

	movingParameters[13] = angleSpeed;

	movingType = KD_GRAVITY_MOVE;
}*/

// FOOL OF ME! GRAVITY MOVE IS ONLY A SPECIAL KIND OF QUADRATIC MOVE WITH Ax=0!
void KD_MovableEvent::setGravityMove(int x, int y, float speedX, float speedY, float gravity, int r0, int g0, int b0, int alpha0, int r1, int g1, int b1, int alpha1, float angleSpeed, float time)
{
	assert(time);
	SetCountDownTimer(time);

	this->x = x;
	this->y = y;

	movingParameters[0] = 0;
	movingParameters[1] = speedX;
	movingParameters[2] = (float)x;

	movingParameters[3] = gravity;
	movingParameters[4] = speedY;
	movingParameters[5] = (float)y;

		// alpha(t) = a t^2 + b t + c
	// with a =
	movingParameters[6] = 0;
	// with b =
	movingParameters[7] = ((float)(alpha1-alpha0))/time;
	// with c =
	movingParameters[8] = (float)alpha0;

	// r(t) = a t^2 + b t + c
	// with a =
	movingParameters[9] = 0;
	// with b =
	movingParameters[10] = ((float)(r1-r0))/time;
	// with c =
	movingParameters[11] = (float)r0;

	// g(t) = a t^2 + b t + c
	// with a =
	movingParameters[12] = 0;
	// with b =
	movingParameters[13] = ((float)(g1-g0))/time;
	// with c =
	movingParameters[14] = (float)g0;

	// b(t) = a t^2 + b t + c
	// with a =
	movingParameters[15] = 0;
	// with b =
	movingParameters[16] = ((float)(b1-b0))/time;
	// with c =
	movingParameters[17] = (float)b0;

	// resizeX(t) = a t^2 + b t + c
	// with a =
	movingParameters[18] = 0;
	// with b =
	movingParameters[19] = 0;
	// with c =
	movingParameters[20] = 1;

	// resizeY(t) = a t^2 + b t + c
	// with a =
	movingParameters[21] = 0;
	// with b =
	movingParameters[22] = 0;
	// with c =
	movingParameters[23] = 1;

	// angle(t) = a t^2 + b t + c
	// with a =
	movingParameters[24] = 0;
	// with b =
	movingParameters[25] = angleSpeed;
	// with c =
	movingParameters[26] = 0;


	movingType = KD_QUADRATIC_MOVE;
}

void KD_MovableEvent::setBlinking(float timeVisible, float timeInvisible)
{
	this->timeVisible = (int)(timeVisible*1000.0f);
	this->timeInvisible = (int)(timeInvisible*1000.0f);
}
