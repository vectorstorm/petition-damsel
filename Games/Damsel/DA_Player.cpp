/*
 *  DA_Player.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 10/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_Player.h"

#include "DA_ModeInGame.h"
#include "DA_Petition.h"

#include "Core.h"
#include "CORE_Game.h"

#include "VS_DisplayList.h"

#include "SYS_Input.h"

daPlayer::daPlayer( daModeInGame *mode ):
	daBasicPerson( true ),
	m_mode(mode),
	m_petitionHeld(NULL)
{
	SetCollisionsActive(true);
}

void
daPlayer::Update(float timeStep)
{
	sysInput *input = core::GetGame()->GetInput();

	vsVector2D desiredVelocity = input->GetLeftStick();
	desiredVelocity.y *= -1.f;
	vsTuneable float s_walkSpeed = 300.0f;
	
	if (desiredVelocity.Length() > 1.f )
		desiredVelocity.Normalise();
	desiredVelocity *= s_walkSpeed;
	
	// Let's compare my desired velocity against my actual velocity.
	vsVector2D actualVelocity = GetVelocity();
	
	vsVector2D deltaVelocity = desiredVelocity - actualVelocity;
	
	vsTuneable float s_accelAmount = 5.0f;
	AddForce( deltaVelocity * s_accelAmount );
	
	Parent::Update(timeStep);


	// let our parent move us first.
	
	if ( !m_petitionHeld && m_mode->GetPetitionsInHand() && input->WasPressed( CID_A ) )
		AcquirePetition();
	
	if ( m_petitionHeld )
		HandlePetition();
}

vsTuneable float s_petitionHeldOffset = -80.f;

void
daPlayer::HandlePetition()
{
	sysInput *input = core::GetGame()->GetInput();
	if ( !input->IsDown( CID_A ) )	// When the 'a' button is down, we throw our petition
	{
		vsVector2D source = GetPosition() + vsVector2D(0.f,s_petitionHeldOffset);
		vsVector2D destination = GetPosition();
		
		vsVector2D desiredDirection = input->GetLeftStick();
		if ( desiredDirection.SqMagnitude() > 1.0f )
			desiredDirection.Normalise();
		
		vsTuneable float s_throwDistance = 500.0f;
		
		destination += desiredDirection * s_throwDistance;
		
		RemoveChild(m_petitionHeld);
		m_petitionHeld->Thrown( source, destination );
		
		m_petitionHeld = NULL;
	}
}

void
daPlayer::AcquirePetition()
{
	m_petitionHeld = m_mode->GetPetitionFromInventory();
	m_petitionHeld->HeldUp(this);
	
	AddChild(m_petitionHeld);
	m_petitionHeld->SetPosition( vsVector2D( 0.0f, s_petitionHeldOffset ) );
}

