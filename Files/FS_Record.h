/*
 *  FS_Record.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 27/12/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef FS_RECORD_H
#define FS_RECORD_H

//#define MAX_LABEL_LENGTH (64)
#define MAX_PARAMETERS   (12)


class vsVector2D;
class vsColor;


class fsRecord
{
		
	enum ArgType
	{
		Arg_Float,
		Arg_Int
	};
	
	vsString	m_label;
	int			m_iArg[MAX_PARAMETERS];
	float		m_fArg[MAX_PARAMETERS];
	
	ArgType		m_argType[MAX_PARAMETERS];
	int			m_argCount;
	
	bool		m_writeType;
	
	float		GetArg(int i);
	
public:
	fsRecord();
		
	void		Init();
		
	bool		FromString( const vsString &s );	// attempt to fill out this fsRecord from a vsString
	vsString	ToString();							// convert this fsRecord into a vsString.
	
	void		SetLabel( const vsString &l );
	void		SetFloat( float f ) { m_fArg[0] = f; m_argCount = 1; m_argType[0] = Arg_Float; }
	void		SetInt( int i ) { m_iArg[0] = i; m_argCount = 1; m_argType[0] = Arg_Int; }
	
	const vsString &	GetLabel() { return m_label; }
	int					GetArgCount() { return m_argCount; }
	
	int					GetArgAsInt( int i ) { return (int)GetArg(i); }
	float				GetArgAsFloat( int i ) { return GetArg(i); }

	void				SetArgAsInt( int i, int value ) { m_argType[i] = Arg_Int; m_iArg[i] = value; }
	void				SetArgAsFloat( int i, float value ) { m_argType[i] = Arg_Float; m_fArg[i] = value; }
	void				SetArgCount(int count) { m_argCount = count; }
	
	vsVector2D	Vector2D();
	vsColor		Color();
};

#endif // FS_RECORD_H

