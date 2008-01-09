/*
 *  BNG_Game.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "SST_Game.h"
#include "SST_Fireworks.h"
#include "SST_Hud.h"
#include "SST_ModeInGame.h"
#include "SST_Rocket.h"
#include "SST_Sequence.h"

#include "FS_File.h"
#include "FS_Record.h"

#include "PHYS_Emitter.h"

#include "SND_System.h"
#include "SND_Music.h"
#include "SND_Sample.h"

#include "SYS_Timer.h"
#include "SYS_Input.h"

#include "Core.h"
#include "CORE_GameRegistry.h"

#include "VS_Layer.h"
#include "VS_Random.h"
#include "VS_Screen.h"


//#define RECORDING_ALLOWED		// disabled in most builds, so people don't accidentally tromp over the initial data when they're trying to figure out how to play.

REGISTER_GAME("StarShot", sstGame)


sstGame::sstGame()
{
	m_layerCount = 2;
}

void
sstGame::Init()
{
	Parent::Init();
	
	m_modeInGame = new sstModeInGame(this);
	
	SetGameMode( m_modeInGame );
}

void
sstGame::Deinit()
{
	vsDelete( m_modeInGame );
	
	Parent::Deinit();
}

void
sstGame::Update(float timeStep)
{
	UNUSED(timeStep);
}
