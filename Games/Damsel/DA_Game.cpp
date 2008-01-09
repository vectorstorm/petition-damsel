/*
 *  DA_Game.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_Game.h"

#include "Core.h"
#include "CORE_GameRegistry.h"

#include "DA_ModeCredits.h"
#include "DA_ModeIntro.h"
#include "DA_ModeTitleScreen.h"

#include "SND_Music.h"

REGISTER_GAME("Damsel", daGame)	// by naming us 'MainMenu', we're the game that gets started automatically.

daGame::daGame()
{
	m_layerCount = 2;	// we need two layers;  one for the game, one for our HUD.
}

daGame::~daGame()
{
}

void
daGame::Init()
{
	Parent::Init();
	
	m_mode[Mode_Intro] = new daModeIntro(this);

	m_mode[Mode_TitleScreen] = new daModeTitleScreen(this);
	//m_mode[Mode_LevelSelect] = new daModeIntro(this);
	//m_mode[Mode_InGame] = new daModeIntro(this);
	m_mode[Mode_Credits] = new daModeCredits(this);
	
	m_music[Music_Quote] = new sndMusic("Leaving the theatre.mp3");
	m_music[Music_Title] = new sndMusic("Poppin Pills.mp3");
	
	SetGameMode( m_mode[Mode_Intro] );
}

void
daGame::Deinit()
{
	for ( int i = 0; i < MODE_MAX; i++ )
		vsDelete(m_mode[i]);
	
	for ( int i = 0; i < MUSIC_MAX; i++ )
		vsDelete(m_music[i]);
	
	Parent::Deinit();
}

void
daGame::Update( float timeStep )
{
	UNUSED(timeStep);
}

void
daGame::SetMode( GameMode mode )
{
	SetGameMode( m_mode[mode] );
}

void
daGame::PlayMusic( GameMusic type )
{
	m_music[type]->Start();
}

void
daGame::GoToPreferences()
{
	coreGame *prefs = coreGameRegistry::FindGame("Preferences");
	
	core::SetGame(prefs);
}

