/*
 *  VS_Color.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Color.h"

vsColor vsColor::White(1.0f,1.0f,1.0f,0.8f);
vsColor vsColor::Blue(0.2f,0.2f,1.0f,0.8f);
vsColor vsColor::LightBlue(0.5f,0.5f,1.0f,0.8f);
vsColor vsColor::Red(1.0f,0.2f,0.2f,0.8f);
vsColor vsColor::Green(0.2f,1.0f,0.2f,0.8f);
vsColor vsColor::Yellow(1.0f,1.0f,0.2f,0.8f);
vsColor vsColor::Black(0.0f,0.0f,0.0f,1.0f);

vsColor  operator*( float scalar, const vsColor &color ) {return color * scalar;}


vsColor vsInterpolate( float alpha, const vsColor &a, const vsColor &b )
{
	return ((1.0f-alpha)*a) + (alpha*b);
}
