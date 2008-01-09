/*
 *  VS_Math.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/05/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_MATH_H
#define VS_MATH_H

#define vsMin(a,b) ((a<b)?a:b)
#define vsMax(a,b) ((a>b)?a:b)
inline float vsClamp( float a, float min, float max ) { return vsMin( max, vsMax( min, a ) ); }

inline float vsFabs( float in ) { return (in >= 0.f)?in:-in; }

float vsSqrt( float in );

float vsCos( float theta );
float vsSin( float theta );
float vsTan( float theta );
float vsATan2( float o, float a );

#endif // VS_MATH_H


