////////////////// CLASSE LGPL!!!!!!!!!!!!!!! A REPROGRAMMER!!!!!!!!!!!!!!!!!!!!!!!
/*
	$Id$

	------------------------------------------------------------------------
	ClanLib, the platform independent game SDK.

	This library is distributed under the GNU LIBRARY GENERAL PUBLIC LICENSE
	version 2. See COPYING for details.

	For a total list of contributers see CREDITS.

	See http://www.clanlib.org
	------------------------------------------------------------------------

	1999/06/19 Daniel Vogel
	
	totally replaced old CL_Vector with this code
*/

//! clanCore="Math"
//! header=core.h

#include "../Tools/defines.h"

#ifndef header_Vector
#define header_Vector
#include <iostream>

//class Matrix;

/** KD_Vector class
 This class provides basic functions and operators for working with vectors
 */
class DllExport KD_Vector
{
public:
//! Variables:
	//: x coordinate
	float x;
	//: y coordinate
	float y;
	//: z coordinate
	float z;
	//: w coordinate
	float w;
      
public:
	
//! Construction:
	//: Constructor that Initializes a vector
	KD_Vector(float x = 0.0, float y = 0.0, float z = 0.0, float w = 1.0);

	//: Copy Constructor
	//- other - vector to copy
	KD_Vector(const KD_Vector &other);

//! Attributes:
	//: Returns the (euclid) norm of the vector. It only uses the x,y,z ordinates! 
	//- Returns - the euclid norm of the vector (in R^3)
	float norm() const;				// not using w component

	//: Normalizes the vector (not taking into account the w ordinate!)
	void normalize();				// not using w component
	
	//: Returns the dot product of the current vector and v
	//- v - other vector 
	//- Returns - the dot product of the two vectors
	float dot(const KD_Vector& v) const;
	
	//: Returns the angle between the current vector and v
	//- v - other vector 
	//- Returns - the angle between the two vectors
	float angle(const KD_Vector& v) const;

	//: Returns the cross product of the current vector and v
	//- v - other vector 
	//- Returns - the cross product of the two vectors
	KD_Vector cross(const KD_Vector& v) const;
	
	//: Returns the current vector rotated around vector a and angle angle
	//- angle - angle to rotate
	//- a - rotation axis
	//- Returns - the rotated vector
	KD_Vector rotate(float angle, const KD_Vector& a) const;

	//: rounds all components
	void round();

//! Operators:
	//: Scalar product (vector * scalar)
	//- Returns - the scalar product
	KD_Vector operator * (float s);  

	//: Scalar product (scalar * vector)
	//- Returns - the scalar product
	friend KD_Vector operator *  (float s, const KD_Vector& v);

	//: += operator
	void operator += (const KD_Vector& v);

	//: -= operator
	void operator -= (const KD_Vector& v);  

	//: *= operator (scalar multiplication)
	void operator *= (float s);  

	//: + operator
	KD_Vector operator + (const KD_Vector& v);
	
	//: - operator
	KD_Vector operator - (const KD_Vector& v);
	
	//: unary - operator
	KD_Vector operator - ();

	//: assignment operator
	KD_Vector& operator = (const KD_Vector& v);

	//: Returns true if current vector equals v
	//- v - other vector
	//- Returns - true if v equals the current vector, false otherwise
	int operator == (const KD_Vector& v) const;
    	
	//: Returns false if current vector equals v
	//- v - other vector
	//- Returns - false if v equals the current vector, true otherwise
	int operator != (const KD_Vector& v) const;
	
	//: Returns reference to n-th ordinate (0. == x, 1. == y, ...)
	//- n - number of ordinate (starting with 0)
	//- Returns - reference to the n-th ordinate
	float& operator [] (int n);
	
	//: cout's the x,y,z ordinates (meant for debugging)
	friend std::ostream& operator << (std::ostream&, KD_Vector& v);



	//friend KD_Vector operator* (const Matrix &m1, const KD_Vector &v2);
};

#endif
