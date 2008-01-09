/*
 *  VS_String.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/05/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_STRING_H
#define VS_STRING_H

typedef std::string vsString;

vsString vsFormatString( const char* format, ... );

#define STR vsFormatString

extern vsString vsEmptyString;

#endif // VS_STRING_H

