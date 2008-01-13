/*
 *  DA_Cop.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 13/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_Cop.h"

#include "DA_ModeInGame.h"
#include "DA_Petition.h"
#include "DA_Player.h"

#include "Core.h"
#include "CORE_Game.h"

#include "COL_System.h"

#include "VS_DisplayList.h"
#include "VS_Layer.h"
#include "VS_Random.h"
#include "VS_Screen.h"

daCop::daCop( daModeInGame *mode ):
daBasicPerson( false ),
m_mode(mode),
m_spawned(false),
m_homingOnTarget(false),
m_focusedPetition(NULL)
{
	colCircle circle;
	circle.center = vsVector2D::Zero;
	circle.radius = 15.0f;
	m_colObject->SetBoundingCircle( circle );
}

void
daCop::Update(float timeStep)
{
	if ( !m_mode ) return;

	if ( m_focusedPetition )
	{
		m_exitting = false;
		if ( !m_focusedPetition->AttractsPedestrians() )
			m_focusedPetition = NULL;
	}
	
	if ( !m_attemptedSigning && !m_focusedPetition )
		m_focusedPetition = m_mode->FindAvailablePetition( GetPosition() );
	
	if ( !m_homingOnTarget )
		PickTarget();
	else
		GoToTarget();
	
	Parent::Update(timeStep);
}

void
daCop::PickTarget()
{
	// check where I'm going to, vs. where I am.
	
	vsVector2D toExit = m_exit - GetPosition();
	
	if ( vsFabs(toExit.x) < 50.0f || vsFabs(toExit.y) < 50.0f )
	{
		m_target = m_exit;
		m_homingOnTarget = true;
	}
	else
	{
		m_target = GetPosition();
		
		if ( vsFabs(m_exit.x) > vsFabs(m_exit.y) )	// side exit, so move vertically first
			m_target.y = m_exit.y;
		else
			m_target.x = m_exit.x;
		
		m_homingOnTarget = true;
	}
}

void
daCop::GoToTarget()
{
	vsVector2D delta = m_target - GetPosition();
	vsVector2D deltaExit = m_exit - GetPosition();
	
	if ( m_exitting )
	{
		// make straight for nearest edge of screen.
		
		vsVector2D exits[4];
		
		for ( int i = 0; i < 4; i++ )
			exits[i] = GetPosition();
		
		exits[0].x = -3200.0f;
		exits[1].x = 3200.0f;
		exits[2].y = -1600.0f;
		exits[3].y = 1600.0f;
		
		int nearest = 0;
		float nearestDist = 100000.0f;
		
		for ( int i = 0; i < 4; i++ )
		{
			float dist = (exits[i] - GetPosition()).Length();
			
			if ( dist < nearestDist )
			{
				nearestDist = dist;
				nearest = i;
			}
		}
		
		delta = exits[nearest] - GetPosition();
	}
	else if ( m_homingOnPlayer )
	{
		if ( !m_mode->GetPlayer()->IsSpawned() )
			m_exitting = true;
		else
			delta = m_mode->GetPlayer()->GetPosition() - GetPosition();
	}
	else if ( m_focusedPetition )
	{
		vsVector2D petitionPosition = m_focusedPetition->GetPositionInLevel();
		vsVector2D directionToPetition = petitionPosition - GetPosition();
		
		delta = directionToPetition;
	}
	
	if ( m_homingOnPlayer && delta.Length() < 60.0f )
	{
		m_mode->GetPlayer()->Die();
		m_homingOnPlayer = false;
		m_exitting = true;
	}
	else if ( m_exitting && delta.Length() < 200.0f )
	{
		Despawn();
	}
	else
	{
		float distance = delta.Length();
		delta.Normalise();
		
		if ( distance < 30.0f )
			delta *= (1.0f / 30.0f);
		
		vsVector2D desiredVelocity = delta;
		
		vsTuneable float s_walkSpeed = 300.0f;
		desiredVelocity *= s_walkSpeed;
		
		// Let's compare my desired velocity against my actual velocity.
		vsVector2D actualVelocity = GetVelocity();
		
		vsVector2D deltaVelocity = desiredVelocity - actualVelocity;
		
		vsTuneable float s_accelAmount = 2.0f;
		AddForce( deltaVelocity * s_accelAmount );
	}
}

void
daCop::WaitAtTarget()
{
}

void
daCop::Spawn(const vsVector2D &where_in, const vsVector2D &where_out, float radius_in, bool noOverlap)
{
	vsAssert( !m_spawned, "Tried to spawn an asteroid that was already spawned!" );
	
	int tries = 0;
	
	while ( !m_spawned )
	{
		float rx = vsRandom::GetFloat( -radius_in, radius_in );
		float ry = vsRandom::GetFloat( -radius_in, radius_in );
		rx += where_in.x;
		ry += where_in.y;
		
		vsVector2D where(rx,ry);
		vsVector2D nextPos = where + vsVector2D(20.0f,20.0f);
		float radius = m_colObject->GetBoundingCircle().radius;
		radius *= 2.0f;
		
		if ( !noOverlap || tries++ > 10 ||
			!core::GetGame()->GetCollision()->CollisionTest( where, nextPos, radius, m_colObject->GetTestFlags() ) )
		{
			SetColor( vsColor(1.0f, 0.5f, 0.5f, 1.0f) );
			SetPosition(where);
			m_exit = where_out;
			
			SetAngleLocked(true);
			SetAngularVelocity(0.0f);
			SetVelocity( vsVector2D::Zero );
			
			m_colObject->SetTransform( m_transform );
			m_colObject->Teleport();
			m_colObject->SetMass( 0.1f * radius );	// we're 5 ships heavy  (A player ship is '1' unit heavy)
			m_colObject->SetCollisionsActive(true);
			
			RegisterOnLayer(0);
			
			m_spawned = true;
			m_homingOnTarget = true;
			m_homingOnPlayer = true;
			m_focusedPetition = NULL;
			m_attemptedSigning = false;
			m_exitting = false;
		}
	}
}

void
daCop::Despawn()
{
	m_colObject->SetCollisionsActive(false);
	Extract();
	m_spawned = false;
}


bool
daCop::CollisionCallback( const colEvent &collision )
{
	if ( collision.colFlags & ColFlag_Shot )
	{
		SetDestroyed();
		
		return true;
	}
	
	return Parent::CollisionCallback( collision );
}

void
daCop::DestroyCallback()
{
	m_mode->SpawnCop( GetPosition() );		// also some law enforcement.
	m_mode->Splat( GetPosition() );
	Despawn();
}
