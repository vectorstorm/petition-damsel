/*
 *  DA_Player.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 10/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_Player.h"

#include "DA_PetitionPickup.h"

#include "Core.h"
#include "CORE_Game.h"

#include "VS_DisplayList.h"

#include "SYS_Input.h"

daPlayer::daPlayer():
daBasicPerson( true )
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
}

