#ifndef Color_H
#define Color_H

#include "../Tools/defines.h"

/**
	Class representing a color
  */
class DllExport KD_Color
{
public:
	// red, between 0 and 1
	float r;
	// green, between 0 and 1
	float g;
	// blue, between 0 and 1
	float b;

	KD_Color();
	KD_Color(const KD_Color &color);
	KD_Color(float _r, float _g, float _b);
	~KD_Color();

	//KD_Color& operator = (const KD_Color& color);

	bool operator == (const KD_Color &color) const;
	
	// Adds another color
	KD_Color operator+( const KD_Color &other );

	// Adds another color
	void operator+=( const KD_Color &other );

	// Adds another color
	//friend KD_Color operator+( KD_Color &c1 , KD_Color &c2 );
	
	// Multiply by a constant
	KD_Color operator*( float lambda );

	// Multiply by another color
	KD_Color KD_Color::operator*( const KD_Color &other );

	// Sets all the components between 0 and 1
	void normalize();
};

#endif
