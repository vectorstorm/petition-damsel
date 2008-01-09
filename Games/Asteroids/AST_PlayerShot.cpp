/*
 *  AST_PlayerShot.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 14/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "AST_PlayerShot.h"
#include "AST_Game.h"

#include "VS_DisplayList.h"
#include "VS_Layer.h"
#include "VS_Screen.h"

static const float c_shotLifetime = 2.0f;

astPlayerShot::astPlayerShot():
colSprite( vsDisplayList::Load("Shot.vec"), ColFlag_Shot, ColFlag_Enemy ),
m_velocity(vsVector2D::Zero),
m_timer(0.f),
m_spawned(false)
{
}

void
astPlayerShot::Spawn( const vsVector2D &where, const vsVector2D &velocity )
{
	vsAssert( !m_spawned, "Tried to spawn a shot that was already spawned!" );

	SetPosition(where);
	m_velocity = velocity;
	
	m_colObject->SetCollisionsActive(true);
	m_colObject->SetTransform( m_transform );
	m_colObject->Teleport();
	
	RegisterOnLayer(0);
	//vsScreen::Instance()->GetLayer(0)->RegisterEntityOnBottom( this );
	
	m_spawned = true;
	
	m_timer = c_shotLifetime;
}

void
astPlayerShot::Despawn()
{ 
	vsAssert( m_spawned, "Tried to despawn a shot that wasn't spawned!" );
	
	m_colObject->SetCollisionsActive(false);
	Extract();
	
	m_spawned = false;
}

void
astPlayerShot::Update(float timeStep)
{
	m_timer -= timeStep;
	SetPosition( GetPosition() + (m_velocity * timeStep) );
	if ( m_spawned )
	{
		Parent::Update(timeStep);
	}
	
	if ( m_timer <= 0.f )
		Despawn();
}

bool	
astPlayerShot::CollisionCallback( const colEvent &event )
{
	UNUSED(event);
	SetDestroyed();
	
	return true;
}

void
astPlayerShot::DestroyCallback()
{
	Despawn();
}

