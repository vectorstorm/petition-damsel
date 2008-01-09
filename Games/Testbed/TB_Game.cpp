/*
 *  TB_Game.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/09/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "TB_Game.h"

#include "CORE_GameRegistry.h"

#include "VS_Screen.h"
#include "VS_Layer.h"

#include "SYS_Input.h"

#if defined(_DEBUG)
REGISTER_GAME("Testbed", tbGame)

tbGame::tbGame():
	m_a(NULL),
	m_b(NULL)
{
}

void
tbGame::Init()
{
	Parent::Init();
	
	// load ourselves a couple asteroids and a sprite graphic.
	m_a = physSprite::Load("Asteroid.vec", ColFlag_Enemy, ColFlag_Enemy);
	m_b = physSprite::Load("Asteroid.vec", ColFlag_Particle, ColFlag_Enemy);
	m_e = new physEmitter("Thrust.vec", 1.0f, 500);
	
	vsScreen::Instance()->GetLayer(0)->RegisterEntityOnTop(m_a);
	m_a->SetCollisionsActive(true);
		
//	vsScreen::Instance()->GetLayer(0)->RegisterEntityOnTop(m_b);
//	m_b->SetCollisionsActive(true);
}

void
tbGame::Deinit()
{
	delete m_a;
	delete m_b;
	delete m_e;
	
	m_a = NULL;
	m_b = NULL;
	m_e = NULL;

	Parent::Deinit();
}

void
tbGame::Update( float timeStep )
{
	UNUSED(timeStep);
	
	sysInput *input = GetInput();
	
	if ( input->WasPressed(CID_A) )
	{
		m_a->SetVelocity( vsVector2D::Zero );
		m_b->SetVelocity( vsVector2D::Zero );
		m_a->SetAngularVelocity( 0.f );
		m_b->SetAngularVelocity( 0.f );
		
		m_a->SetPosition( vsVector2D(-100.0f, 0.0f) );
		m_b->SetPosition( vsVector2D(100.0f, 0.0f) );
		
		m_e->SetSpawnPosition( vsVector2D(100.0f, 0.0f) );
		m_e->SetSpawnRate( 100.0f );
		vsTuneable vsVector2D partVel(-400.0f, 0.0f);
		m_e->SetSpawnVelocity( partVel, 100.0f );
		
		vsTuneable vsVector2D impulseA(-2000.0f,0.0f);
		vsTuneable vsVector2D impulseB(-30000.0f,12000.0f);
		
		m_a->SetMass(5.0f);
		m_b->SetMass(5.0f);
		
		m_a->AddForce(impulseA);
		m_b->AddForce(impulseB);
		
		vsTuneable float torqueAmt = 500.0f;
		
		m_a->AddTorque(-torqueAmt);
		m_b->AddTorque(torqueAmt);
	}
	
	m_e->Update(timeStep);
}

#endif // _DEBUG
