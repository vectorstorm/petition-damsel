/*
 *  SYS_Input.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "SYS_Input.h"

#include "Core.h"
#include "CORE_Game.h"

#include <SDL/SDL.h>

void
sysInput::Init()
{
	m_joystick = NULL;
	for ( int i = 0; i < CID_MAX; i++ )
	{
		m_lastControlState[i] = 0.f;
		m_controlState[i] = 0.f;
	}
	
	// try to get a joystick
	
	int joystickCount = SDL_NumJoysticks();
	
	if ( joystickCount )
	{
		printf("Found %d joysticks.\n", joystickCount);
		m_joystick = SDL_JoystickOpen(0);
		printf("Using %s, %d axes, %d buttons, %d hats, %d balls\n", SDL_JoystickName(0), SDL_JoystickNumAxes(m_joystick), 
			   SDL_JoystickNumButtons(m_joystick), SDL_JoystickNumHats(m_joystick), SDL_JoystickNumBalls(m_joystick));
		
		//SDL_JoystickEventState(SDL_ENABLE);
	}
	else
		printf("No joystick found.  Using keyboard input.\n");
	
	Update(0.0f);	// run a zero-time update, so we can correctly identify key presses vs. holds on the first frame.
}

void
sysInput::Deinit()
{
}

void
sysInput::Update(float timeStep)
{
	UNUSED(timeStep);
	
	SDL_Event event;
	
	for ( int i = 0; i < CID_MAX; i++ )
		m_lastControlState[i] = m_controlState[i];
	
	//		int i, j;
	while( SDL_PollEvent( &event ) ){
		switch( event.type ){
			case SDL_KEYDOWN:
			{
				switch( event.key.keysym.sym ){
					case SDLK_q:
						core::SetExitToMenu();
						break;
					case SDLK_ESCAPE:
						core::SetExit();
						break;
					case SDLK_UP:
						m_keyControlState[CID_Up] = 1.0f;
						break;
					case SDLK_DOWN:
						m_keyControlState[CID_Down] = 1.0f;
						break;
					case SDLK_LEFT:
						m_keyControlState[CID_Left] = 1.0f;
						break;
					case SDLK_RIGHT:
						m_keyControlState[CID_Right] = 1.0f;
						break;
					case SDLK_SPACE:
						m_keyControlState[CID_A] = 1.0f;
						break;
					case SDLK_LALT:
						m_keyControlState[CID_B] = 1.0f;
						break;
					default:
						break;
				}
				break;
			}
			case SDL_KEYUP:
			{
				switch( event.key.keysym.sym ){
					case SDLK_UP:
						m_keyControlState[CID_Up] = 0.0f;
						break;
					case SDLK_DOWN:
						m_keyControlState[CID_Down] = 0.0f;
						break;
					case SDLK_LEFT:
						m_keyControlState[CID_Left] = 0.0f;
						break;
					case SDLK_RIGHT:
						m_keyControlState[CID_Right] = 0.0f;
						break;
					case SDLK_SPACE:
						m_keyControlState[CID_A] = 0.0f;
						break;
					case SDLK_LALT:
						m_keyControlState[CID_B] = 0.0f;
						break;
					default:
						break;
				}
				break;
			}
			case SDL_QUIT:
				core::SetExit();
				break;
			/*case SDL_JOYAXISMOTION:
				if( event.jaxis.which == 0)
				{
					float axisValue = event.jaxis.value / 32767.0f;
					switch( event.jaxis.axis )
					{
						case 0:
							printf("Value %f\n", axisValue);
							if ( axisValue < 0.f )
							{
								m_controlState[CID_Left] = -axisValue;
								m_controlState[CID_Right] = 0.f;
							}
							else
							{
								m_controlState[CID_Left] = 0.f;
								m_controlState[CID_Right] = axisValue;
							}
							break;
						case 1:
							break;
						default:
							break;
					}
				}
				break;*/
			default:
				break;
		}
	}
	
	if ( m_joystick )
	{
		SDL_JoystickUpdate();
		ReadAxis(0, CID_LLeft, CID_Right);
		ReadAxis(1, CID_LUp, CID_Down);

		ReadAxis(2, CID_RLeft, CID_RRight);
		ReadAxis(3, CID_RUp, CID_RDown);
		
		ReadButton( 2, CID_A );
		ReadButton( 3, CID_B );
		
		ReadButton( 12, CID_Up );
		ReadButton( 14, CID_Down );
		ReadButton( 15, CID_Left );
		ReadButton( 13, CID_Right );
	}
	else
	{
		for ( int i = 0; i < CID_MAX; i++ )
			m_controlState[i] = 0.f;
	}
	
	for ( int i = 0; i < CID_MAX; i++ )
	{
		if ( m_keyControlState[i] > m_controlState[i] )
			m_controlState[i] = m_keyControlState[i];
	}
	
	m_controlState[CID_LeftRightLAxis] = m_controlState[CID_LRight] - m_controlState[CID_LLeft];
	m_controlState[CID_UpDownLAxis] = m_controlState[CID_LUp] - m_controlState[CID_LDown];

	m_controlState[CID_LeftRightRAxis] = m_controlState[CID_RRight] - m_controlState[CID_RLeft];
	m_controlState[CID_UpDownRAxis] = m_controlState[CID_RUp] - m_controlState[CID_RDown];
}

void
sysInput::ReadAxis( int axisID, int neg, int pos )
{
	float axisValue = SDL_JoystickGetAxis(m_joystick, axisID) / 32767.0f;
	if ( vsFabs(axisValue) < 0.2f )
		axisValue = 0.f;
	else
	{
		if ( axisValue > 0.f )
			axisValue -= 0.2f;
		else
			axisValue += 0.2f;
		axisValue *= (1.0f / 0.8f);
	}

	if ( axisValue < 0.f )
	{
		m_controlState[neg] = -axisValue;
		m_controlState[pos] = 0.f;
	}
	else
	{
		m_controlState[neg] = 0.f;
		m_controlState[pos] = axisValue;
	}
}

void
sysInput::ReadButton( int buttonID, int cid )
{
	bool buttonDown = !!SDL_JoystickGetButton(m_joystick, buttonID);
	
	if ( buttonDown )
		m_controlState[cid] = 1.0f;
	else
		m_controlState[cid] = 0.0f;
}

bool
sysInput::IsDown( ControlID id )
{
	return (m_controlState[id] != 0.f);
}

bool
sysInput::WasDown( ControlID id )
{
	return (m_lastControlState[id] != 0.f);
}

bool
sysInput::WasPressed( ControlID id )
{
	return (IsDown(id) && !WasDown(id));
}

bool
sysInput::WasReleased( ControlID id )
{
	return (!IsDown(id) && WasDown(id));
}

