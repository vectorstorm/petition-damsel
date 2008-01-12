/*
 *  DA_ModeInGame.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_ModeInGame.h"

#include "DA_Camera.h"
#include "DA_Car.h"
#include "DA_Game.h"
#include "DA_LevelBackground.h"
#include "DA_Hud.h"
#include "DA_Player.h"
#include "DA_Pedestrian.h"
#include "DA_Petition.h"
#include "SYS_Input.h"

#include "VS_Layer.h"
#include "VS_Screen.h"
#include "VS_Random.h"

daModeInGame::daModeInGame( daGame *game ):
	m_game(game),
	m_score(0),
	m_highScore(0)
{
}

daModeInGame::~daModeInGame()
{
}

void
daModeInGame::Init()
{
	m_player = new daPlayer(this);
	m_player->SetColor(vsColor::Red);
	m_player->RegisterOnLayer(0);
	
	m_timeLimit = 60.0f;
	
	for ( int i = 0; i < MAX_PEDESTRIANS; i++ )
	{
		m_pedestrian[i] = new daPedestrian(this);
		m_pedestrian[i]->SetColor(vsColor::Blue);
		//m_pedestrian[i]->Spawn( vsVector2D::Zero, 600.0f, true);
	}
	
	for ( int i = 0; i < MAX_CARS; i++ )
	{
		m_car[i] = new daCar;
		m_car[i]->SetColor(vsColor::White);
	}
	
	for ( int i = 0; i < MAX_PETITIONS; i++ )
	{
		m_petition[i] = new daPetition(this,10);
		m_petition[i]->SetColor(vsColor::Yellow);
	}
	
	m_hud = new daHud(this);
	m_hud->RegisterOnLayer(1);
	m_hud->FadeIn();
	
	m_background = new daLevelBackground;
	m_background->RegisterOnLayer(0);
	
	//m_game->PlayMusic( daGame::Music_Title );

	m_score = 0;

	m_petitionSpawnTimer = 0.0f;
	m_pedestrianSpawnTimer = 2.0f;
	m_carSpawnTimer = 5.0f;
	
	m_camera = new daCamera();
	m_camera->FollowSprite( m_player );
	vsScreen::Instance()->GetLayer(0)->SetCamera(m_camera);
}

void
daModeInGame::Deinit()
{
	vsScreen::Instance()->GetLayer(0)->SetCamera(NULL);
	vsDelete(m_camera);
	
	vsDelete(m_background);
	vsDelete(m_hud);
	
	for ( int i = 0; i < MAX_PEDESTRIANS; i++ )
		vsDelete( m_pedestrian[i] );
	for ( int i = 0; i < MAX_PICKUPS; i++ )
		vsDelete( m_petition[i] );
	vsDelete( m_player );
}

void
daModeInGame::Update( float timeStep )
{
	vsVector2D playerPos = m_player->GetPosition();
	// check for whether the player has moved over a petition pickup.
	for ( int i = 0; i < MAX_PETITIONS; i++ )
	{
		if ( m_petition[i]->AvailableForPickup() )
		{
			const float c_pickupDistance = 20.0f;
			vsVector2D delta = playerPos - m_petition[i]->GetPosition();
			if ( delta.Length() < c_pickupDistance )
			{
				m_petition[i]->PickedUp();
			}
		}
	}
	
	
	
	m_petitionSpawnTimer -= timeStep;
	if ( m_petitionSpawnTimer <= 0.f )
	{
		AttemptToSpawnPetition();
		
		m_petitionSpawnTimer = vsRandom::GetFloat(10.0f, 20.0f);
	}
	
	m_pedestrianSpawnTimer -= timeStep;
	if ( m_pedestrianSpawnTimer <= 0.f )
	{
		SpawnPedestrian();
		//m_pedestrianSpawnTimer = 2000.0f;
		m_pedestrianSpawnTimer = vsRandom::GetFloat(0.2f, 1.0f);
	}
	
	m_carSpawnTimer -= timeStep;
	if ( m_carSpawnTimer <= 0.f )
	{
		SpawnCar();
		m_carSpawnTimer = vsRandom::GetFloat(3.0f, 8.0f);
	}

	if ( m_game->GetInput()->WasPressed(CID_B) )
		m_game->SetMode(daGame::Mode_TitleScreen);
	
	m_timeLimit -= timeStep;
	if ( m_timeLimit < 0.f )
	{
		m_timeLimit = 0.f;
		// do gameover sequence here
		
		m_game->SetMode(daGame::Mode_TitleScreen);
	}
	
}

void
daModeInGame::SpawnCar()
{
	for ( int i = 0; i < MAX_CARS; i++ )
	{
		if ( !m_car[i]->IsSpawned() )
		{
			m_car[i]->Spawn( vsVector2D(-1600,75), vsVector2D(1600,75), 10.0f, true );
			return;
		}
	}
}

void
daModeInGame::SpawnPedestrian()
{
	/*vsVector2D legalSpawnPositions[8] =
	{
		vsVector2D(-800, -100),
		vsVector2D(800,-100),
		vsVector2D(-800, 100),
		vsVector2D(800, 100),
		vsVector2D(100, 500),
		vsVector2D(-100, 500),
		vsVector2D(100, -500),
		vsVector2D(-100, -500)
	};*/
	
	vsVector2D corner[2] = {
		vsVector2D(1600,200),
		vsVector2D(200,1000)
	};
	
	for ( int i = 0; i < MAX_PEDESTRIANS; i++ )
	{
		if ( !m_pedestrian[i]->IsSpawned() )
		{
			bool enterSide = vsRandom::GetBool();
			
			if ( vsRandom::GetBool() )
			{
				corner[0].x *= -1;
				corner[1].x *= -1;
			}
			if ( vsRandom::GetBool() )
			{
				corner[0].y *= -1;
				corner[1].y *= -1;
			}
			
			if ( enterSide )
			{
				vsVector2D swap = corner[1];
				corner[1] = corner[0];
				corner[0] = swap;
			}
			m_pedestrian[i]->Spawn( corner[0], corner[1], 40.f, true );
			return;
		}
	}
}

daPetition *
daModeInGame::FindAvailablePetition( const vsVector2D &where )
{
	float bestAttraction = 1.0f;
	daPetition *bestResult = NULL;
	
	
	for ( int i = 0; i < MAX_PICKUPS; i++ )
	{
		if ( m_petition[i]->AttractsPedestrians() )
		{
			vsTuneable float s_normalAttractDistance = 100.0f;
			vsTuneable float s_activeAttractDistance = 300.0f;
			
			vsVector2D petitionPos = m_petition[i]->GetPositionInLevel();//m_petition[i]->GetPosition();
			float effectiveAttractDistance = s_normalAttractDistance;			
			
			if ( m_petition[i]->ActiveAttractsPedestrians() )	// petition being held by the player.
			{
				effectiveAttractDistance = s_activeAttractDistance;
//				petitionPos += m_player->GetPosition();
			}
			
			float distance = (petitionPos - where).Magnitude();
			float attraction = distance / effectiveAttractDistance;
			if ( attraction < bestAttraction )
			{
				bestAttraction = attraction;
				bestResult = m_petition[i];
			}
		}
	}
	
	return bestResult;
}

void
daModeInGame::AttemptToSpawnPetition()
{
	for ( int i = 0; i < MAX_PETITIONS; i++ )
	{
		if ( m_petition[i]->AvailableForSpawn() )
		{
			vsLayer *l = vsScreen::Instance()->GetLayer(0);
			
			m_petition[i]->Spawn( vsRandom::GetVector2D( l->GetTopLeftCorner(), l->GetBottomRightCorner() ) );
			return;
		}
	}
}

int
daModeInGame::GetPetitionsInHand()
{
	int count = 0;
	
	for ( int i = 0; i < MAX_PETITIONS; i++ )
	{
		if ( m_petition[i]->InInventory() )
			count++;
	}
	
	return count;
}

daPetition *
daModeInGame::GetPetitionFromInventory()
{
	for ( int i = 0; i < MAX_PETITIONS; i++ )
	{
		if ( m_petition[i]->InInventory() )
			return m_petition[i];
	}
	
	return NULL;
}

void
daModeInGame::AddSignature()
{
	m_score++;
	
	m_timeLimit += 5.0f;
}


