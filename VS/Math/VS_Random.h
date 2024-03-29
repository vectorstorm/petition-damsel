/*
 *  VS_Random.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_RANDOM_H
#define VS_RANDOM_H

class vsVector2D;
class vsColor;

class vsRandom
{
public:
	static void Init();
	
	static float GetFloat(float maxValue);
	static float GetFloat(float min, float max);
	static int			GetInt(int max);
	static int			GetInt(int min, int max);
	static bool			GetBool() { return !!GetInt(2); }
	
	static vsVector2D	GetVector2D(float maxLength);
	static vsColor		GetColor(float minBrightness, float maxBrightness);
	
	static vsVector2D	GetVector2D(const vsVector2D &topLeft, const vsVector2D &bottomRight);
};

#endif // VS_RANDOM_H
