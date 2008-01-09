/*
 *  VS_Log.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/05/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Log.h"

#include <stdarg.h>

void vsLog(const char *format, ...)
{
	char sz[1024];
	va_list marker;
	
	va_start(marker, format);
	vsprintf(sz, format, marker);
	va_end(marker);
	
	vsString str = sz;
	
	vsLog(str);
}

void vsLog(const vsString &str)
{
	printf("%s\n", str.c_str() );
}


