/*
 *  VS_Vector.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 12/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Vector.h"
//#include <math.h>

vsVector2D vsVector2D::Zero(0.0f,0.0f);

float vsVector2D::Length() const
{
	return vsSqrt(x*x+y*y);
}

float vsVector2D::SqLength() const
{
	return (x*x+y*y);
}

void vsVector2D::Normalise()
{
	*this *= (1.0f/Length());
}

float vsVector3D::Length()
{
	return vsSqrt(x*x+y*y+z*z);
}

float vsVector3D::SqLength()
{
	return (x*x+y*y+z*z);
}

void vsVector3D::Normalise()
{
	*this *= (1.0f/Length());
}

vsVector2D operator*(float b, const vsVector2D &vec) { return vec * b; }
vsVector3D operator*(float b, const vsVector3D &vec) { return vec * b; }

vsVector2D operator-(const vsVector2D &vec) { return vsVector2D(-vec.x,-vec.y); }
vsVector3D operator-(const vsVector3D &vec) { return vsVector3D(-vec.x,-vec.y,-vec.z); }

float vsInterpolate( float alpha, float a, float b )
{
	return ((1.0f-alpha)*a) + (alpha*b);
}

vsVector2D vsInterpolate( float alpha, const vsVector2D &a, const vsVector2D &b )
{
	return ((1.0f-alpha)*a) + (alpha*b);
}

vsVector3D vsInterpolate( float alpha, const vsVector3D &a, const vsVector3D &b )
{
	return ((1.0f-alpha)*a) + (alpha*b);
}

