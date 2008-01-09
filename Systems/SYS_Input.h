/*
 *  SYS_Input.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef SYS_INPUT_H
#define SYS_INPUT_H

#include "CORE_GameSystem.h"
#include "VS_Vector.h"
#include <SDL/SDL.h>

enum ControlID
{
	CID_UpDownLAxis,
	CID_LeftRightLAxis,
	CID_LUp,
	CID_LDown,
	CID_LLeft,
	CID_LRight,

	CID_UpDownRAxis,
	CID_LeftRightRAxis,
	CID_RUp,
	CID_RDown,
	CID_RLeft,
	CID_RRight,

	CID_Up,
	CID_Down,
	CID_Left,
	CID_Right,
	
	CID_A,
	CID_B,
	
	CID_MAX
};

class sysInput : public coreGameSystem
{
	SDL_Joystick	*m_joystick;
	
	float			m_keyControlState[CID_MAX];

	float			m_controlState[CID_MAX];
	float			m_lastControlState[CID_MAX];
	
	void			ReadAxis( int axisID, int neg, int pos );
	void			ReadButton( int buttonID, int cid );
	
	bool			WasDown( ControlID id );
public:
	
	virtual void	Init();
	virtual void	Deinit();
	
	virtual void	Update( float timeStep );
	
	vsVector2D		GetLeftStick() { return vsVector2D( GetState(CID_LeftRightLAxis), GetState(CID_UpDownLAxis) ); }
	vsVector2D		GetRightStick() { return vsVector2D( GetState(CID_LeftRightRAxis), GetState(CID_UpDownRAxis) ); }
	
	float			GetState( ControlID id ) { return m_controlState[id]; }
	bool			IsUp( ControlID id ) { return !IsDown( id ); }
	bool			IsDown( ControlID id );
	bool			WasPressed( ControlID id );
	bool			WasReleased( ControlID id );
};

#endif // SYS_INPUT_H
