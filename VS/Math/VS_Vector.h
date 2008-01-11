/*
 *  VS_Vector.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 12/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_VECTOR_H
#define VS_VECTOR_H

		//  Standard 2D vector type
class vsVector2D
{
public:
	static vsVector2D Zero;
	
	float x;
	float y;
	
	vsVector2D() {x=0;y=0;}
	vsVector2D(float x_in, float y_in) { x = x_in; y = y_in; }
	
	vsVector2D	operator=( const vsVector2D &b ) {x=b.x; y=b.y; return *this;}
	vsVector2D  operator+( vsVector2D b ) const {return vsVector2D(x+b.x, y+b.y);}
	vsVector2D  operator-( vsVector2D b ) const {return vsVector2D(x-b.x, y-b.y);}
	vsVector2D  operator*( float b ) const {return vsVector2D(x*b,y*b);}
	vsVector2D  operator*=( float b ) {x*=b; y*=b; return *this;}
	vsVector2D  operator+=( vsVector2D b ) {*this = *this+b; return *this; }
	vsVector2D  operator-=( vsVector2D b ) {*this = *this-b; return *this; }
	
	bool operator==(const vsVector2D &b) const { return (this->x == b.x && this->y == b.y); }
	bool operator!=(const vsVector2D &b) const { return !(*this==b); }
		
	float	Length() const;
	float	SqLength() const;
	float	Magnitude() const { return Length(); }
	float	SqMagnitude() const { return SqLength(); }
	void	Normalise();
	
	float	Dot( const vsVector2D &b ) const { return ( x*b.x +
										   y*b.y ); }
	
	void	Set(float x_in, float y_in) {x=x_in; y=y_in;}
};



// VectorStorm is a 2D game engine, but if you have some strange need for a 3D
// vector, we've got a ready-made one.
struct vsVector3D
{
	float x;
	float y;
	float z;
	
	vsVector3D() {x=0;y=0;z=0;}
	vsVector3D(const vsVector2D &b) {x=b.x;y=b.y;z=0;}
	vsVector3D(float x_in, float y_in, float z_in) { x = x_in; y = y_in; z = z_in; }
	
	vsVector3D	operator=( const vsVector3D &b ) {x=b.x; y=b.y; z = b.z; return *this;}
	vsVector3D  operator+( vsVector3D b ) const {return vsVector3D(x+b.x, y+b.y, z+b.z);}
	vsVector3D  operator-( vsVector3D b ) const {return vsVector3D(x-b.x, y-b.y, z-b.z);}
	vsVector3D  operator*( float b ) const {return vsVector3D(x*b,y*b,z*b);}
	vsVector3D  operator*=( float b ) {x*=b; y*=b; z*=b; return *this;}
	vsVector3D  operator+=( vsVector3D b ) {*this = *this+b; return *this; }
	vsVector3D  operator-=( vsVector3D b ) {*this = *this-b; return *this; }
	
	bool operator==(const vsVector3D &b) { return (this->x == b.x && this->y == b.y && this->z == b.z); }
	bool operator!=(const vsVector3D &b) { return !(*this==b); }
		
		float	Length();
	float	SqLength();
	void	Normalise();
	
	vsVector3D	Cross( const vsVector3D &b ) const { return vsVector3D( y*b.z - z*b.y,
														   z*b.x - x*b.z,
														   x*b.y - y*b.x ); }
	
	float	Dot( const vsVector3D &b ) const { return ( x*b.x +
										   y*b.y +
										   z*b.z ); }
	
	void	Set(float x_in, float y_in, float z_in) {x=x_in; y=y_in;z=z_in;}
};



vsVector2D operator*(float b, const vsVector2D &vec);
vsVector3D operator*(float b, const vsVector3D &vec);

vsVector2D operator-(const vsVector2D &vec);
vsVector3D operator-(const vsVector3D &vec);

float vsInterpolate( float alpha, float a, float b );

vsVector2D vsInterpolate( float alpha, const vsVector2D &a, const vsVector2D &b );
vsVector3D vsInterpolate( float alpha, const vsVector3D &a, const vsVector3D &b );

#endif // VS_VECTOR_H
