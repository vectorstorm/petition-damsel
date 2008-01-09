/*
 *  AST_PlayerShip.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 25/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "AST_PlayerShip.h"
#include "AST_PlayerShot.h"
#include "AST_Game.h"

#include "Core.h"
#include "CORE_Game.h"

#include "COL_Object.h"
#include "COL_System.h"

#include "PHYS_Emitter.h"

#include "VS_DisplayList.h"
#include "VS_Layer.h"
#include "VS_Screen.h"

#include "SYS_Input.h"

astPlayerShip::astPlayerShip( physEmitter *thrustEmitter ):
physSprite( vsDisplayList::Load("PlayerShip.vec"), ColFlag_Player, ColFlag_Enemy )
{
	m_thrust = vsSprite::Load("PlayerThrust.vec");
	m_leftAttitude = vsSprite::Load("PlayerAttitude.vec");
	m_rightAttitude = vsSprite::Load("PlayerAttitude.vec");
	m_leftRearAttitude = vsSprite::Load("PlayerAttitude.vec");
	m_rightRearAttitude = vsSprite::Load("PlayerAttitude.vec");
	
	m_colObject->SetCollisionsActive(false);

	for ( int i = 0; i < Thruster_MAX; i++ )
	{
		m_thrustAmt[i] = 0.f;
	}
	
	SetColor(vsColor::Blue);
	
	m_thrust->SetColor(vsColor::Red);
	AddChild( m_thrust );
	m_thrust->m_transform.m_position.Set(0,9);	
	
	AddChild( m_leftAttitude );
	AddChild( m_rightAttitude );
	AddChild( m_leftRearAttitude );
	AddChild( m_rightRearAttitude );

	m_leftAttitude->m_transform.m_position.Set( 5.0f, -8.0f );
	m_leftAttitude->m_transform.m_angle.Set( DEGREES(30.0f) );
	m_leftRearAttitude->m_transform.m_position.Set( 12.0f, 8.0f );
	m_leftRearAttitude->m_transform.m_angle.Set( DEGREES(-30.0f) );
//	m_leftAttitude->m_transform.m_position.Set( 7.5, 0 );
	
	m_rightAttitude->m_transform.m_position.Set( -5.0f, -8.0f );
	m_rightAttitude->m_transform.m_angle.Set( DEGREES(-30.0f) );
//	m_rightAttitude->m_transform.m_position.Set( -7.5, 0 );
	m_rightAttitude->m_transform.m_scale.Set(-1.0f,1.0f);

	m_rightRearAttitude->m_transform.m_position.Set( -12.0f, 8.0f );
	m_rightRearAttitude->m_transform.m_scale.Set(-1.0f,1.0f);
	m_rightRearAttitude->m_transform.m_angle.Set( DEGREES(30.0f) );

	for ( int i = 0; i < c_maxShots; i++ )
	{
		m_shotList[i] = new astPlayerShot();
		m_shotList[i]->SetColor( vsColor::Yellow );
	}
	
	m_isSpawned = false;
	
	m_emitter = thrustEmitter;
}

astPlayerShip::~astPlayerShip()
{
	delete m_thrust;
	delete m_leftAttitude;
	delete m_rightAttitude;
	delete m_leftRearAttitude;
	delete m_rightRearAttitude;

	for ( int i = 0; i < c_maxShots; i++ )
		delete m_shotList[i];
}

void
astPlayerShip::Spawn( const vsVector2D &pos_in )
{
	SetPosition( pos_in );
	SetVelocity( vsVector2D::Zero );
	SetAngularVelocity( 0.f );
	RegisterOnLayer(0);
	m_spawnInvulnerable = true;
	m_colObject->Teleport();
	
	m_timeSinceSpawn = 0.f;
	
	m_isSpawned = true;
}

void
astPlayerShip::Despawn()
{
	Extract();
	m_colObject->SetCollisionsActive(false);
	
	m_emitter->SetSpawnRate(0.0f);
	m_emitter->SpawnImmediateBurst(60, GetPosition(), 100.0f, 30.0f, vsColor::Blue);
	
	m_isSpawned = false;
	m_delayBeforeSpawn = 2.0f;	// no respawning for at least two seconds.
	m_timeSinceSpawn = 0.f;
}

bool
astPlayerShip::AttemptSpawn(float timeStep, const vsVector2D &position)
{
	m_delayBeforeSpawn -= timeStep;
	
	if ( m_delayBeforeSpawn <= 0.f )
	{
		//float radius = m_colObject->GetBoundingCircle().radius * 4.0f;
		//vsVector2D nextPos = position + vsVector2D(20.0f,20.0f);
		//if (!core::GetGame()->GetCollision()->CollisionTest( position, nextPos, radius, m_colObject->GetTestFlags() ) )
			Spawn(position);
	}
	return m_isSpawned;
}

void
astPlayerShip::HandleThrusters( float timeStep )
{
	UNUSED(timeStep);
	
	sysInput *input = core::GetGame()->GetInput();
	
	ControlID thrusterControl[Thruster_MAX] =
	{
		CID_B,
		CID_Left,
		CID_Right
	};
	
	/*
	 SetColor 1.0 1.0 1.0 0.8
	 MoveTo -0.75 -1.8
	 LineTo 7.5 -3.0
	 LineTo 0.75 1.8
	 */
	
	for( int i = 0; i < Thruster_MAX; i++ )
	{
		float desired = input->GetState( thrusterControl[i] );
		float current = m_thrustAmt[i];
		
		m_thrustAmt[i] = vsInterpolate( 0.15f, current, desired );
	}
	
	m_thrust->m_transform.m_scale.Set( m_thrustAmt[Thruster_Main], m_thrustAmt[Thruster_Main] );
	m_leftAttitude->m_transform.m_scale.Set( m_thrustAmt[Thruster_Left], m_thrustAmt[Thruster_Left] );
	m_rightAttitude->m_transform.m_scale.Set( -m_thrustAmt[Thruster_Right], m_thrustAmt[Thruster_Right] );

	m_rightRearAttitude->m_transform.m_scale.Set( -m_thrustAmt[Thruster_Left], m_thrustAmt[Thruster_Left] );
	m_leftRearAttitude->m_transform.m_scale.Set( m_thrustAmt[Thruster_Right], m_thrustAmt[Thruster_Right] );
}

void
astPlayerShip::HandleSpawnTimer( float timeStep )
{
	m_timeSinceSpawn += timeStep;
	
	const float c_invulnerableTime = 3.0f;	// three seconds of invulnerability
	
	if ( m_spawnInvulnerable )
	{
		float scale = 1.0f;
		vsColor c = vsColor::Blue;
		if ( m_timeSinceSpawn > c_invulnerableTime )
		{
			m_colObject->SetCollisionsActive(true);
			m_spawnInvulnerable = false;
		}
		else if ( m_timeSinceSpawn > 2.0f )
		{
			float frac = (m_timeSinceSpawn - 2.0f);
			
			float theta = frac * TWOPI;
			float pulseAmt = vsFabs(vsSin(theta));
			
			c = vsInterpolate(pulseAmt, vsColor::LightBlue, vsColor::White);
		}
		else if ( m_timeSinceSpawn > 1.0f )
		{
			float frac = (m_timeSinceSpawn - 1.0f);
			
			float theta = frac * TWOPI;
			float pulseAmt = vsFabs(vsSin(theta));
			
			c = vsInterpolate(pulseAmt, vsColor::LightBlue, vsColor::White);
		}
		else
		{
			c = vsInterpolate(m_timeSinceSpawn, vsColor::Black, vsColor::LightBlue);
		}
		
		const float c_scaleTime = 3.0f;
		if ( m_timeSinceSpawn < c_scaleTime )
		{
			float frac = m_timeSinceSpawn / c_scaleTime;
			frac = 1.0f - ((frac-1.f) * (frac-1.f));									// slam in, ease out
			//frac = (3.0f * frac * frac) - (2.0f * frac * frac * frac);			// ease in, ease out
			
			scale = vsInterpolate( frac, 5.0f, 1.0f );
		}
		
		SetColor(c);
		m_transform.m_scale = vsVector2D(scale,scale);
	}
}

void
astPlayerShip::Update( float timeStep )
{
	sysInput *input = core::GetGame()->GetInput();
	
	//vsVector2D lStick = input->GetLeftStick();
	float steering = (float)(input->IsDown(CID_Right) - input->IsDown(CID_Left));
	
	AddTorque( GetAngularVelocity() * -6.0f );		// angular damping
	AddTorque( DEGREES(1080.0f) * steering );		// rotation from attitude jets
	
	AddForce( GetVelocity() * -0.3f );				// damping
	
	//if ( lStick.y > 0.f )
	if ( input->IsDown(CID_B) )
	{
		float acceleration = 300.0f;
		AddForce( m_transform.m_angle.GetForwardVector() * acceleration /* lStick.y*/ );	// thrust
		m_emitter->SetSpawnRate( 30.0f );
	}
	else
	{
		m_emitter->SetSpawnRate(0.0f);
	}
	
	HandleSpawnTimer(timeStep);
	
	bool shooting = (!m_spawnInvulnerable && input->WasPressed(CID_A));
	
	if ( shooting )
	{
		for ( int i = 0; i < c_maxShots; i++ )
		{
			if ( !m_shotList[i]->IsSpawned() )
			{
				vsVector2D forwardVec = m_transform.m_angle.GetForwardVector();
				vsVector2D muzzlePos = GetPosition() + (forwardVec * 18.0f);
				vsVector2D shotVelocity = GetVelocity() + (forwardVec * 400.0f);
				m_shotList[i]->Spawn( muzzlePos, shotVelocity );
				break;
			}
		}
	}
	
	HandleThrusters(timeStep);
	
	Parent::Update( timeStep );
	
	vsVector2D forwardVec = m_transform.m_angle.GetForwardVector();
	m_emitter->SetSpawnPosition( GetPosition() - forwardVec * 30.0f, 10.0f );
	m_emitter->SetSpawnColor( vsColor::Red );

	if ( input->IsDown(CID_B) )
		m_emitter->SetSpawnVelocity( forwardVec * -200.0f, 60.0f );
}

bool	
astPlayerShip::CollisionCallback( const colEvent &event )
{
	if ( event.colFlags & ColFlag_Enemy )
	{
		SetDestroyed();
		return true;
	}
	
	return false;
}

void
astPlayerShip::DestroyCallback()
{
	Despawn();
}
