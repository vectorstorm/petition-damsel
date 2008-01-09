/*
 *  CORE_Game.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 12/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "CORE_Game.h"
#include "CORE_GameMode.h"
#include "CORE_GameRegistry.h"

#include "SYS_Input.h"
#include "SYS_Timer.h"
#include "COL_System.h"
#include "SND_System.h"

#include "VS_Layer.h"
#include "VS_Screen.h"
#include "VS_Sprite.h"

//REGISTER_GAME("Empty", coreGame)

coreGameSystem	*coreGame::s_system[GameSystem_MAX];

//coreGame *coreGame::s_instance = NULL;

coreGame::coreGame()
{
	m_layerCount = 1;
	m_currentMode = NULL;
}

coreGame::~coreGame()
{
}

void
coreGame::Init()
{
	const vsString &name = coreGameRegistry::GetGameName(this);
	printf(" -- Initialising game \"%s\"\n", name.c_str());
	vsScreen::Instance()->CreateLayers(m_layerCount);	// just one layer for now
	
	ConfigureGameSystems();
	InitGameSystems();
	
	m_firstFrame = true;

	m_framesRendered = 0;
}

void
coreGame::Deinit()
{
	const vsString &name = coreGameRegistry::GetGameName(this);
	printf(" -- Deinitialising game \"%s\"\n", name.c_str());
	DeinitGameSystems();
	
	vsScreen::Instance()->DestroyLayers();	// just one layer for now
	printf(" -- Exitting game \"%s\"\n", name.c_str());
}

void
coreGame::SetGameMode( coreGameMode *newMode )
{
	if ( m_currentMode )
		m_currentMode->Deinit();
	m_currentMode = newMode;
	if ( m_currentMode )
		m_currentMode->Init();
}

void
coreGame::StartTimer()
{
	sysTimer::Instance()->Init();	// reinit our timer, so as not to have missed frames during our load sequence tallied.
	m_startTicks = sysTimer::Instance()->GetCurrentMillis();	
}

void
coreGame::StopTimer()
{
	float fps = m_framesRendered / ((sysTimer::Instance()->GetCurrentMillis()-m_startTicks)/1000.0f);
	
	printf(" ## Frames rendered: %ld\n ## Average FPS: %f\n", m_framesRendered, fps);
	
	int missedFrames = sysTimer::Instance()->GetMissedFrameCount();		// how many times did we miss rendering at 60?
	
	printf(" ## Frames missed:  %d\n", missedFrames);
}

void
coreGame::CreateGameSystems()
{
	s_system[ GameSystem_Timer ] = new sysTimer;
	s_system[ GameSystem_Input ] = new sysInput;
	s_system[ GameSystem_Collision ] = new colSystem;
	s_system[ GameSystem_Sound ] = new sndSystem;
}


// By default, we have all game systems, in the standard order.  Other games
// can reconfigure this to get a different set of systems, or a different update
// order if they really want to.  (I can't imagine why they'd want to, but I'm
// sure somebody'll come up with a good reason)
void
coreGame::ConfigureGameSystems()
{
	m_systemCount = GameSystem_MAX;
	
	for ( int i = 0; i < GameSystem_MAX; i++ )
		m_system[i] = s_system[i];
}

void
coreGame::InitGameSystems()
{
	for ( int i = 0; i < GameSystem_MAX; i++ )
	{
		m_system[i]->Init();
		m_system[i]->Activate();
	}
}

void
coreGame::DeinitGameSystems()
{
	for ( int i = 0; i < GameSystem_MAX; i++ )
	{
		m_system[i]->Deinit();
//		delete m_system[i];
//		m_system[i] = NULL;
	}
}

void
coreGame::Go()
{
	m_framesRendered++;
		
	for ( int i = 0; i < GameSystem_MAX; i++ )
		if ( m_system[i]->IsActive() )
			m_system[i]->Update( m_timeStep );
		
	Update( m_timeStep );
		
	if ( m_currentMode )
		m_currentMode->Update( m_timeStep );
	
	vsScreen::Singleton().Update( m_timeStep );

	for ( int i = 0; i < GameSystem_MAX; i++ )
		if ( m_system[i]->IsActive() )
			m_system[i]->PostUpdate( m_timeStep );
		
	vsScreen::Singleton().Draw();
}

sysInput *
coreGame::GetInput()
{
	return (sysInput *)s_system[GameSystem_Input];
}

colSystem *
coreGame::GetCollision()
{
	return (colSystem *)s_system[GameSystem_Collision];
}

sndSystem *
coreGame::GetSound()
{
	return (sndSystem *)s_system[GameSystem_Sound];
}

