/*
	$Id$

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	------------------------------------------------------------------------

	1999/06/19 Daniel Vogel

	totally replaced old KD_Vector with this code
*/

#include "Vector.h"
//#include "API/Core/System/cl_assert.h"
#include <cmath>

//#include "Matrix.h"

KD_Vector::KD_Vector(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

KD_Vector::KD_Vector(const KD_Vector &other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
}

float KD_Vector::norm() const
{
	return sqrt(x*x+y*y+z*z);
}

void KD_Vector::normalize()
{
	float f = norm();
	if (f!=0)
	{
		x /= f;
		y /= f;
		z /= f;
	}
}

float KD_Vector::dot(const KD_Vector& v) const
{
	return x*v.x + y*v.y + z*v.z;
}

float KD_Vector::angle(const KD_Vector& v) const
{
	return acos(dot(v)/(norm()*v.norm()));
}

KD_Vector KD_Vector::cross(const KD_Vector& v) const
{
	KD_Vector tmp = KD_Vector(y * v.z - z * v.y,
				  z * v.x - x * v.z,
				  x * v.y - y * v.x);
	return tmp;  
}

// quick hack, same as glRotatef(angle, a);
KD_Vector KD_Vector::rotate(float angle, const KD_Vector& a) const
{
	KD_Vector tmp = KD_Vector();

	float s = sin(angle);
	float c = cos(angle);

	tmp.x = x*(a.x*a.x*(1-c)+c)     + y*(a.x*a.y*(1-c)-a.z*s) + z*(a.x*a.z*(1-c)+a.y*s);
	tmp.y = x*(a.y*a.x*(1-c)+a.z*s) + y*(a.y*a.y*(1-c)+c)     + z*(a.y*a.z*(1-c)-a.x*s);
	tmp.z = x*(a.x*a.z*(1-c)-a.y*s) + y*(a.y*a.z*(1-c)+a.x*s) + z*(a.z*a.z*(1-c)+c);
	return tmp;  
}

void KD_Vector::round()
{
	x = int(x+0.5f);
	y = int(y+0.5f);
	z = int(z+0.5f);
	w = int(w+0.5f);
}

KD_Vector KD_Vector::operator * (float s)
{
	KD_Vector tmp = KD_Vector(s * x,
				  s * y,
				  s * z,
				  s * w);
	return tmp;
}

KD_Vector operator * (float s, const KD_Vector& v)
{
	KD_Vector tmp = KD_Vector(s * v.x,
				  s * v.y,
				  s * v.z,
				  s * v.w);
	return tmp;
}

void KD_Vector::operator += (const KD_Vector& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.z;
}

void KD_Vector::operator -= (const KD_Vector& v)
{
  	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
}

void KD_Vector::operator *= (float s)
{
	x *= s;
	y *= s;
	z *= s;
	w *= s;
}

KD_Vector KD_Vector::operator + (const KD_Vector& v)
{
	KD_Vector tmp = KD_Vector(x + v.x,
				  y + v.y,
				  z + v.z,
				  w + v.w);
  	return tmp;
}

KD_Vector KD_Vector::operator - (const KD_Vector& v)
{
	KD_Vector tmp = KD_Vector(x - v.x,
				  y - v.y,
				  z - v.z,
				  w - v.z);
  	return tmp;
}

KD_Vector KD_Vector::operator - ()
{
	KD_Vector tmp = KD_Vector(-x,
				  -y,
				  -z,
				  -w);
  	return tmp;
}

KD_Vector& KD_Vector::operator = (const KD_Vector& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

int KD_Vector::operator == (const KD_Vector& v) const
{
	return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w));
}

int KD_Vector::operator != (const KD_Vector& v) const
{
	return !(operator == (v));
}

float & KD_Vector::operator [] (int n)
{
	switch (n)
	{
		case 0:	return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
	}
	//cl_assert(false);
	return x;			// dummy
}

/*std::ostream& operator << (std::ostream& os, KD_Vector& v)
{
	os << v.x << " " << v.y << " " << v.z;
	return os;
} */

/*KD_Vector operator* (const Matrix &m1, const KD_Vector &v2)
{
	KD_Vector res;
	res.w=0;
	
	res.x += m1.matrix[0][0]*v2.x + m1.matrix[0][1]*v2.y + m1.matrix[0][2]*v2.z + m1.matrix[0][3]*v2.w; 
	res.y += m1.matrix[1][0]*v2.x + m1.matrix[1][1]*v2.y + m1.matrix[1][2]*v2.z + m1.matrix[1][3]*v2.w; 
	res.z += m1.matrix[2][0]*v2.x + m1.matrix[2][1]*v2.y + m1.matrix[2][2]*v2.z + m1.matrix[2][3]*v2.w; 
	res.w += m1.matrix[3][0]*v2.x + m1.matrix[3][1]*v2.y + m1.matrix[3][2]*v2.z + m1.matrix[3][3]*v2.w; 

	return res;
}*/
