/*
 *  DA_BasicPerson.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 11/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_BasicPerson.h"

#include "VS_DisplayList.h"

daBasicPerson::daBasicPerson(bool player):
physSprite( vsDisplayList::Load(player?"Player.vec":"Pedestrian.vec"), (player)?ColFlag_Player:ColFlag_Enemy, ColFlag_Player|ColFlag_Enemy|ColFlag_Shot )
{
	//m_displayList->Compile();
	
	if ( player )
		SetMass(50.0f);
	
	m_player = player;
	
	SetAngleLocked(true);

	m_limbSwingCycle = 0.f;
	
	for ( int i = 0; i < LIMB_COUNT; i++ )
	{
		vsDisplayList *list = vsDisplayList::Load(player?"PlayerLeg.vec":"PedestrianLeg.vec");
		//list->Compile();
		m_limb[i] = new vsSprite(list);
		m_limb[i]->SetPosition( vsVector2D(5.0f,0.0f) );
		AddChild( m_limb[i] );
	}
	
	colCircle circle;
	circle.center = vsVector2D::Zero;
	circle.radius = 30.0f;
	m_colObject->SetBoundingCircle( circle );
}

void
daBasicPerson::Update( float timeStep )
{
	float s_walkSpeed = 300.0f;
	float s_limbSwingCycleLength = 0.3f;
	
	vsVector2D actualVelocity = GetVelocity();
	
	float speedFraction = actualVelocity.Length() / s_walkSpeed;
	
	m_limbSwingCycle += (speedFraction * timeStep) / s_limbSwingCycleLength;
	
	if ( m_limbSwingCycle > 1.f )
		m_limbSwingCycle -= 1.f;
	
	float ang = DEGREES(45.0f);
	if ( !m_player )
	{
		ang = DEGREES(20.0f);
		s_limbSwingCycleLength = 0.1f;
	}
	
	vsAngle angle = vsCos(m_limbSwingCycle * PI) * ang;
	
	
	m_limb[0]->SetAngle(angle);
	m_limb[1]->SetAngle(-angle);
	
	if ( actualVelocity.x > 2.f )
		SetScale(vsVector2D(1.0f,1.0f));
	else if ( actualVelocity.x < -2.f )
		SetScale(vsVector2D(-1.0f,1.0f));	// flip horizontally if we're going left.
	
	Parent::Update( timeStep );
}
