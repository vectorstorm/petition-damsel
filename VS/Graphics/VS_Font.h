/*
 *  VS_Font.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 30/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_FONT_H
#define VS_FONT_H

class vsDisplayList;
class vsVector2D;

enum JustificationType
{
	Justification_Left,
	Justification_Right,
	Justification_Center
};

class vsBuiltInFont
{
	static void AppendCharacterToList( char c, vsDisplayList *list, const vsVector2D &offset, float size );
	static vsDisplayList * CreateString_Internal(const char* string, float size, float capSize, JustificationType j);

	static void BuildDisplayListFromString(vsDisplayList * list, const char* string, float size, float capSize=0.f, JustificationType j = Justification_Left);
	static void BuildDisplayListFromCharacter( vsDisplayList *list, char c, float size, float capSize=0.f );
	
public:
	
	static vsDisplayList * CreateString(const vsString &string, float size, float capSize=0.f, JustificationType j = Justification_Left);
	static vsDisplayList * CreateCharacter(char letter, float size, float capSize=0.f);
	
	static float			GetStringWidth(const vsString &string, float size, float capSize=0.f);
};

#endif // VS_FONT_H
