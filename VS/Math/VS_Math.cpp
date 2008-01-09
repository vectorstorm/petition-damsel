/*
 *  VS_Math.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/05/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Math.h"
#include <math.h>

float vsCos(float theta)
{
	return cosf(theta);
}

float vsSin(float theta)
{
	return sinf(theta);
}

float vsATan2(float opp, float adj)
{
	return atan2f(opp, adj);
}

float vsSqrt(float in)
{
	return sqrtf(in);
}


