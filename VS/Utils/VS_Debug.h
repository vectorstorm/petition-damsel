/*
 *  VS_Debug.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/05/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_DEBUG_H
#define VS_DEBUG_H


#if defined(_DEBUG)
#include "VS_String.h"
#include "VS_Log.h"

#define vsAssert(x,y) if(!(x)){ vsFailedAssert(#x, y,__FILE__, __LINE__); }

void vsFailedAssert( const vsString &conditionStr, const vsString &msg, const char *file, int line );


#else
#define vsAssert(x,y) {UNUSED(x); UNUSED(y);}
#endif


#endif // VS_DEBUG_H

