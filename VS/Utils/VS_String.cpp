/*
 *  VS_String.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/05/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_String.h"

//#include <string>
#include <stdarg.h>

vsString vsEmptyString = "";

vsString vsFormatString( const char* format, ... )
{
	char sz[1024];
	va_list marker;
	
	va_start(marker, format);
	vsprintf(sz, format, marker);
	va_end(marker);
	
	return sz;
}

