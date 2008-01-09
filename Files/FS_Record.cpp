/*
 *  FS_Record.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 27/12/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "FS_Record.h"

#include "VS_Color.h"
#include "VS_Vector.h"


fsRecord::fsRecord()
{
	Init();
}

void
fsRecord::Init()
{
	m_label = vsEmptyString;
	m_argCount = 0;
}

void
fsRecord::SetLabel( const vsString &l )
{
	m_label = l;
}

float
fsRecord::GetArg( int i )
{
	if ( m_argType[i] == Arg_Float )
		return m_fArg[i];
	else if ( m_argType[i] == Arg_Int )
		return m_iArg[i];
	
	vsAssert(0, "Unknown arg type!");
	
	return 0.0f;
}

vsVector2D
fsRecord::Vector2D()
{
	vsAssert( m_argCount >= 2, "Wrong number of arguments supplied for Vector2D" );
	return vsVector2D( GetArg(0), GetArg(1) );
}

vsColor
fsRecord::Color()
{
	vsAssert( m_argCount >= 4, "Wrong number of arguments supplied for Color" );
	return vsColor( GetArg(0), GetArg(1), GetArg(2), GetArg(3) );
}

vsString
fsRecord::ToString()
{
	vsString result = m_label;
	
	result += " ";
	
	for ( int i = 0; i < m_argCount; i++ )
	{
		if ( m_argType[i] == Arg_Float )
			result += vsFormatString("%f ", m_fArg[i]);
		else if ( m_argType[i] == Arg_Int )
			result += vsFormatString("%d ", m_iArg[i] );
		else
			vsAssert(0, "Unknown arg type!");
	}
	return result;
}

static vsString ExtractStringToken( vsString &string )
{
	vsString token;
	size_t index = string.find(' ');
	
	if ( index == vsString::npos )
	{
		token = string;
		string = "";
	}
	else
	{
		token = string.substr(0, index);
		string.erase(0,index+1);
//		string = string.substr(index, vsString::npos);
	}
	
	return token;
}

bool
fsRecord::FromString( const vsString &string )
{
	vsString parseString = string;
	vsString token;
	
	// remove any trailing \n from the string.
	{
		size_t i = parseString.find('\n');
		if ( i != parseString.npos )
			parseString.erase(i,1);
	}
	
	Init();
	
	token = ExtractStringToken( parseString );
	
	if ( token.empty() )
		return false;
	
	m_label = token;
	
	float f = 0.f;
	int i = 0;
	
	while(!parseString.empty())
	{
		token = ExtractStringToken( parseString );
		
		// in order for it to be a float, it must contain a decimal point.
		// so if there's no decimal point, then it's not a floating point value.
		
		vsAssert(m_argCount < MAX_PARAMETERS, "Too many arguments!" );
		
		bool isAFloat = ( token.find('.') != token.npos );
		
		if ( isAFloat )
		{
			vsAssert(sscanf( token.c_str(), "%f", &f ), "Couldn't find a floating value where we expected one?");
			m_argType[m_argCount] = Arg_Float;
			m_fArg[m_argCount++] = f;
		}
		else
		{
			vsAssert(sscanf( token.c_str(), "%d", &i), "Couldn't find an integer value?" );
			m_argType[m_argCount] = Arg_Int;
			m_iArg[m_argCount++] = i;
		}
	}
	
	return true;
}

