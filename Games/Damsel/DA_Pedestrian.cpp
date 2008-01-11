/*
 *  DA_Pedestrian.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 10/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_Pedestrian.h"

#include "Core.h"
#include "CORE_Game.h"

#include "COL_System.h"

#include "VS_DisplayList.h"
#include "VS_Layer.h"
#include "VS_Random.h"
#include "VS_Screen.h"

daPedestrian::daPedestrian():
	daBasicPerson( false ),
	m_spawned(false),
	m_homingOnTarget(false)
{
	colCircle circle;
	circle.center = vsVector2D::Zero;
	circle.radius = 15.0f;
	m_colObject->SetBoundingCircle( circle );
}

void
daPedestrian::Update(float timeStep)
{
	if ( !m_homingOnTarget )
		PickTarget();
	else
		GoToTarget();
	 
	Parent::Update(timeStep);
}

void
daPedestrian::PickTarget()
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
daPedestrian::GoToTarget()
{
	vsVector2D delta = m_target - GetPosition();
	vsVector2D deltaExit = m_exit - GetPosition();
	if ( deltaExit.Length() < 60.0f )
	{
		Despawn();
	}
	else if ( delta.Length() < 50.0f )
	{
		m_homingOnTarget = false;
	}
	else
	{
		float distance = delta.Length();
		delta.Normalise();
		
		if ( distance < 30.0f )
			delta *= (1.0f / 30.0f);
		
		vsVector2D desiredVelocity = delta;

		vsTuneable float s_walkSpeed = 100.0f;
		desiredVelocity *= s_walkSpeed;
		
		// Let's compare my desired velocity against my actual velocity.
		vsVector2D actualVelocity = GetVelocity();
		
		vsVector2D deltaVelocity = desiredVelocity - actualVelocity;
		
		vsTuneable float s_accelAmount = 5.0f;
		AddForce( deltaVelocity * s_accelAmount );
	}
}

void
daPedestrian::WaitAtTarget()
{
}

void
daPedestrian::Spawn(const vsVector2D &where_in, const vsVector2D &where_out, float radius_in, bool noOverlap)
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
			m_homingOnTarget = false;
		}
	}
}

void
daPedestrian::Despawn()
{
	m_colObject->SetCollisionsActive(false);
	Extract();
	m_spawned = false;
}


