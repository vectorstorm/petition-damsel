/*
 *  DA_Car.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 12/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_Car.h"

#include "DA_ModeInGame.h"

#include "Core.h"
#include "CORE_Game.h"

#include "COL_System.h"

#include "VS_DisplayList.h"
#include "VS_Layer.h"
#include "VS_Random.h"
#include "VS_Screen.h"

daCar::daCar( daModeInGame *mode ):
physSprite( vsDisplayList::Load("Car.vec"), ColFlag_Shot, ColFlag_Shot ),
m_mode(mode),
m_spawned(false)
{
	colCircle circle;
	circle.center = vsVector2D::Zero;
	circle.radius = 15.0f;
	m_colObject->SetBoundingCircle( circle );
	
	SetMass(5000.0f);	// huge mass!
}

void
daCar::Update(float timeStep)
{
	if ( !m_mode ) return;

	GoToTarget();
	
	Parent::Update(timeStep);
}

void
daCar::GoToTarget()
{
	// check if we're going off the screen.
	vsVector2D vel = GetVelocity();
	
	if ( vel.x < 0.f && GetPosition().x < -1600.0f )
		Despawn();
	else if ( vel.x > 0.f && GetPosition().x > 1600.0f )
		Despawn();

	{
		
		// Let's compare my desired velocity against my actual velocity.
		vsVector2D actualVelocity = GetVelocity();
		
		vsVector2D deltaVelocity = m_desiredVelocity - actualVelocity;
		
		vsTuneable float s_accelAmount = 5.0f;
		AddForce( deltaVelocity * s_accelAmount );
	}
}

void
daCar::WaitAtTarget()
{
}

void
daCar::Spawn(const vsVector2D &where_in, const vsVector2D &where_out, float radius_in, bool noOverlap)
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
			SetPosition(where);
			m_target = where_out;
			
			vsVector2D dir = m_target - where;
			vsAngle ang = vsAngle::FromForwardVector(dir);
			SetAngle(ang);
			
			dir.Normalise();
			dir *= 1000.0f;
			
			m_desiredVelocity = dir;
			
			SetVelocity(dir);
			
			SetAngleLocked(true);
			SetAngularVelocity(0.0f);
			
			m_colObject->SetTransform( m_transform );
			m_colObject->Teleport();
			m_colObject->SetMass( 0.1f * radius );	// we're 5 ships heavy  (A player ship is '1' unit heavy)
			m_colObject->SetCollisionsActive(true);
			
			RegisterOnLayer(0);
			
			m_spawned = true;
		}
	}
}

void
daCar::Despawn()
{
	m_colObject->SetCollisionsActive(false);
	Extract();
	m_spawned = false;
}


