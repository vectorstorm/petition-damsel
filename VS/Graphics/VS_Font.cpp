/*
 *  VS_Font.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 30/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Font.h"

#include "VS_DisplayList.h"
#include "VS_Vector.h"

static int lookup_letterIndex( char letter );



int lookup_letterIndex( char letter )
{
	char upperLetter = toupper( letter );
	if( upperLetter >= 'A' && upperLetter <= 'Z' )
	{
		return int( upperLetter - 'A' );
	}
	else if( isdigit( letter ) )
	{
		return 26 + int( letter - '0' );
	}
	else switch( letter )
	{
		case ':': return 36;
		case '.': return 37;
		case '-': return 38;
		case '%': return 39;
		case '+': return 40;
		case '@': return 41;
		case '(': return 42;
		case ')': return 43;
		case '/': return 44;
		case '\\': return 45;
		case '=' : return 46;
		case '!' : return 47;
		case '_' : return 48;
		case '<' : return 49;
		case '>' : return 50;
		case ',' : return 51;
		case '"' : return 52;
		case ';' : return 53;	// actually back-quotes.  Hope I never need to use a semicolon!
		case '?' : return 54;
		case '\'' : return 55;
		default:  return -1;
	}		
}

#define W	(0.5f)
#define W2  (W * 0.5f)
#define H	(-0.25f)
#define H2	(2.0f * H)
#define H3	(3.0f * H)

vsVector2D baseline(0.f,-H);

// The points.
vsVector2D P [24] = {
	vsVector2D::Zero,
	
	vsVector2D(-W,H3),			vsVector2D(0,H3),        	vsVector2D(W,H3),
	vsVector2D(-W,H2),			vsVector2D(0,H2),			vsVector2D(W,H2),
	vsVector2D(-W,H),			vsVector2D(0,H),			vsVector2D(W,H),
	vsVector2D(-W,0),			vsVector2D(0,0),        	vsVector2D(W,0),
	vsVector2D(-W,-H),			vsVector2D(0,-H),        	vsVector2D(W,-H),
	vsVector2D(-W,-H2),        	vsVector2D(0,-H2),        	vsVector2D(W,-H2),
	vsVector2D(-W,-H3),        	vsVector2D(0,-H3),        	vsVector2D(W,-H3),
	              vsVector2D(-W2,-H3),			vsVector2D(W2,-H3)
	
};
// Vector strokes (indexed into P).
static int st_nick53[56][15] = {
	/* A */ { 13,4,2,6,15,0,10,6,-1 },
	/* B */ { 13,1,2,6,8,12,14,13,-1 },
	/* C */ { 12,14,10,4,2,6,-1 },
	/* D */ { 13,1,2,6,12,14,13,-1 },
	/* E */ { 12,14,10,4,2,6,10,-1 },
	/* F */ { 13,4,2,6,0,7,8,-1 },
	/* G */ { 11,12,14,10,4,2,6,-1 },
	/* H */ { 13,1,0,15,3,0,10,6,-1 },
	/* I */ { 14,2,-1 },
	/* J */ { 10,14,12,3,-1 },
	/* K */ { 13,1,0,7,8,6,3,0,8,12,15,-1 },
	/* L */ { 15,13,1,-1 },
	/* M */ { 13,1,5,3,15,-1 },
	/* N */ { 13,1,9,0,15,3,-1 },
	/* O */ { 14,10,4,2,6,12,14,-1 },
	/* P */ { 13,4,2,6,10,-1 },
	/* Q */ { 14,10,4,2,6,12,14,0,11,15,-1 },
	/* R */ { 13,4,2,6,10,0,8,12,15,-1 },
	/* S */ { 10,14,12,4,2,6,-1 },
	/* T */ { 14,2,0,4,2,6,-1 },
	/* U */ { 1,10,14,12,3,-1 },
	/* V */ { 1,7,14,9,3,-1 },
	/* W */ { 1,13,11,15,3,-1 },
	/* X */ { 1,4,12,15,0,3,6,10,13,-1 },
	/* Y */ { 14,5,0,1,5,3,-1 },
	/* Z */ { 1,3,7,13,15,-1 },
	
	/* 0 */ { 14,10,4,2,6,12,14,0,10,6,-1 },
	/* 1 */ { 13,15,0,14,2,4,-1 },
	/* 2 */ { 15,13,9,6,2,4,-1 },
	/* 3 */ { 4,2,6,8,12,14,10,-1 },
	/* 4 */ { 14,8,0,12,10,4,2,-1 },
	/* 5 */ { 3,1,7,5,9,12,14,10,-1 },
	/* 6 */ { 3,2,4,10,14,12,9,5,7,-1 },
	/* 7 */ { 1,3,9,13,0,9,8,-1 },
	/* 8 */ { 2,4,12,14,10,6,2,-1 },
	/* 9 */ { 15,6,2,4,7,11,9,-1 },
	
	/* : */ { 2,4,5,2,0,10,11,13,10,-1 },
	/* . */ { 11,13,14,11,-1 },
	/* - */ { 10,9,-1 },
	/* % */ { 1,2,4,1,0,12,14,15,12,0,13,3,-1 },
	/* + */ { 10,9,0,5,11,-1 },
	/* @ */ { 12,5,11,12,6,2,4,10,14,15,-1 },
	/* ( */ { 2,4,7,10,14,-1 },
	/* ) */ { 2,6,9,12,14,-1 },
	/* / */ { 10,3,-1 },
	/* \ */ { 1,12,-1 },
	/* = */ { 4,6,0,7,9,-1 },
	/* ! */ { 11,13,14,11,0,8,2,-1 },
	/* _ */ { 13,15,-1 },
	/* < */ { 6,7,12,-1 },
	/* > */ { 4,9,10,-1 },
	/* , */ { 11,13,20,11,-1 },
	/* " */ { 1,2,5,1,0,2,3,6,2,-1 },
	/* ; */ { 1,2,4,1,0,2,3,5,2,-1 },
	/* ? */ { 4,2,6,8,0,11,13,14,11,-1 },
	/* ' */ { 1, 2, 4, 1, -1 }
	///* @ */ { 12,5,11,6,2,4,10,14,15,-1 }
	///* @ */ { 12,5,11,12,9,2,4,10,14,15,-1 }
};

static const float c_kerningFactor = 0.6f;
static const float c_spaceKerning = 0.8f;	// space width, compared to a regular character


void
vsBuiltInFont::BuildDisplayListFromCharacter( vsDisplayList *list, char c, float size, float capSize )
{
	char thisChar = c;
	char upperChar = toupper(thisChar);
	float thisSize = size;
	
	if ( thisChar == ' ' )
	{
		// half width spaces, because that looks better.
		thisSize *= c_spaceKerning;
	}
	else if ( upperChar >= 'A' && upperChar <= 'Z' && thisChar == upperChar && capSize > 0.f )
	{
		thisSize = capSize;
	}
	
	AppendCharacterToList( thisChar, list, vsVector2D::Zero, thisSize );
}


void
vsBuiltInFont::BuildDisplayListFromString( vsDisplayList *list, const char *string, float size, float capSize, JustificationType j )
{
	vsVector2D offset = vsVector2D::Zero;
	size_t len = strlen(string);
	
	if ( j != Justification_Left )
	{
		float width = GetStringWidth(string, size, capSize);
		
		if ( j == Justification_Right )
			offset.Set(-width, 0.f);
		if ( j == Justification_Center )
			offset.Set(-(width*0.5f), 0.f);
	}
	
	for ( size_t i = 0; i < len; i++ )
	{
		char thisChar = string[i];
		char upperChar = toupper(thisChar);
		float thisSize = size;
		
		if ( thisChar == ' ' )
		{
			// half width spaces, because that looks better.
			thisSize *= c_spaceKerning;
		}
		else if ( upperChar >= 'A' && upperChar <= 'Z' && thisChar == upperChar && capSize > 0.f )
		{
			thisSize = capSize;
		}
		
		offset.x += c_kerningFactor * thisSize;
		AppendCharacterToList( string[i], list, offset, thisSize );
		offset.x += c_kerningFactor * thisSize;
	}
}

vsDisplayList * 
vsBuiltInFont::CreateString_Internal(const char* string, float size, float capSize, JustificationType j)
{
	vsDisplayList *result = NULL;
	vsDisplayList loader(1024 * 10);
	
	BuildDisplayListFromString( &loader, string, size, capSize, j );
	
	int displayListSize = loader.GetSize();
	if ( displayListSize )
	{
		result = new vsDisplayList( displayListSize );
		result->Append(loader);
	}
	
	return result;
}

vsDisplayList *
vsBuiltInFont::CreateString(const vsString &string, float size, float capSize, JustificationType j)
{
	return CreateString_Internal( string.c_str(), size, capSize, j );
}

vsDisplayList * 
vsBuiltInFont::CreateCharacter(char c, float size, float capSize)
{
	vsDisplayList loader(1024);
	
	BuildDisplayListFromCharacter( &loader, c, size, capSize );
	
	vsDisplayList *result = new vsDisplayList( loader.GetSize() );
	result->Append(loader);
	
	return result;
}

void 
vsBuiltInFont::AppendCharacterToList( char c, vsDisplayList *list, const vsVector2D &offset, float size )
{
	int index = lookup_letterIndex(c);
	
	if ( index >= 0 )
	{
        int *strokes = st_nick53[index];
		int z = 0;
		bool	inLineStrip = false;
		
		while ( strokes[z] >= 0 )
		{
			int newVertID = strokes[z++];
			
			if ( newVertID == 0 )
			{
				inLineStrip = false;
			}
			else
			{
				if ( inLineStrip )
				{
					list->LineTo( offset + (size * (P[newVertID]-baseline)) );
				}
				else
				{
					list->MoveTo( offset + (size * (P[newVertID]-baseline)) );
					inLineStrip = true;
				}
			}
		}
	}
}

float
vsBuiltInFont::GetStringWidth( const vsString &string, float size, float capSize )
{
	if ( capSize == 0.f )
		capSize = size;
	
	float width = 0.f;
	size_t len = strlen(string.c_str());
	
	for ( size_t i = 0; i < len; i++ )
	{
		char thisChar = string[i];
		char upperChar = toupper(thisChar);
		float thisSize = size;
		
		if ( thisChar == ' ' )
		{
			// half width spaces, because that looks better.
			thisSize *= c_spaceKerning;
		}
		else if ( upperChar >= 'A' && upperChar <= 'Z' && thisChar == upperChar && capSize > 0.f )
		{
			thisSize = capSize;
		}
		
		width += 2.0f * c_kerningFactor * thisSize;
	}
	
	return width;
}
