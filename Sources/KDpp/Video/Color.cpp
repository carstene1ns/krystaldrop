#include "Color.h"

KD_Color::KD_Color() : r(0), g(0), b(0)
{

}

KD_Color::KD_Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b)
{

}

KD_Color::KD_Color(const KD_Color &color) : r(color.r), g(color.g), b(color.b)
{
}

KD_Color::~KD_Color()
{
}


bool KD_Color::operator == (const KD_Color &color) const
{
	if (color.r == r && color.g == g && color.b == b)
		return true;
	else
		return false;
}

KD_Color KD_Color::operator+( const KD_Color &other )
{
	return KD_Color(r+other.r, g+other.g, b+other.b);
}

void KD_Color::operator+=( const KD_Color &other )
{
	r+=other.r;
	g+=other.g;
	b+=other.b;
}

KD_Color KD_Color::operator*( float lambda )
{
	return KD_Color(r*lambda, g*lambda, b*lambda);
}

KD_Color KD_Color::operator*( const KD_Color &other )
{
	return KD_Color(r*other.r, g*other.g, b*other.b);
}

void KD_Color::normalize()
{
	if (r<0) r=0;
	else if (r>1) r=1;
	if (g<0) g=0;
	else if (g>1) g=1;
	if (b<0) b=0;
	else if (b>1) b=1;
}
