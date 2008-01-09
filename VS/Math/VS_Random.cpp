/*
 *  VS_Random.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Random.h"

#include "VS_Angle.h"
#include "VS_Color.h"
#include "VS_Random.h"
#include "VS_Vector.h"

#include <time.h>

void
vsRandom::Init()
{
	srand( (unsigned int)time(NULL) );
}

float
vsRandom::GetFloat(float maxValue)
{
	float result = rand() / (float)RAND_MAX;
	result *= maxValue;
	return result;
}

float
vsRandom::GetFloat(float min, float max)
{
	float delta = max - min;
	
	return GetFloat(delta) + min;
}

int
vsRandom::GetInt(int maxValue)
{
	float result = rand() / (float)RAND_MAX;
	result *= maxValue;
	return (int)result;
}

int
vsRandom::GetInt(int min, int max)
{
	int delta = max - min;
	
	return GetInt(delta) + min;
}


vsVector2D
vsRandom::GetVector2D(float maxLength)
{
	vsVector2D result( GetFloat(-1.0f,1.0f), GetFloat(-1.0f,1.0f) );
	
	if ( result.SqLength() > 1.0f )
		result.Normalise();
	
	result *= maxLength;
	
	return result;
}

vsColor
vsRandom::GetColor(float min, float max)
{
	vsVector3D result( GetFloat(0.f,100.f), GetFloat(0.f,100.f), GetFloat(0.f,100.f) );
	
	if ( result.SqLength() > 1.0f )
		result.Normalise();
	
	float brightness = GetFloat(min,max);
	result *= brightness;
	
	return vsColor( result.x, result.y, result.z, 1.0f );
}
