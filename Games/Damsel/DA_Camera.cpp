/*
 *  DA_Camera.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 11/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_Camera.h"

#include "VS_Screen.h"
#include "VS_Sprite.h"

daCamera::daCamera()
{
	m_followSprite = NULL;
	m_fov = 1500.0f;	// we see 800 units around the position.  (vertically)
}

daCamera::~daCamera()
{
}

void
daCamera::Update( float timeStep )
{
	UNUSED(timeStep);
	
	if ( m_followSprite )
	{
		//  don't show outside our bounds!
		
		float left = -1600.0f;
		float right = 1600.0f;
		float top = -1000.0f;
		float bottom = 1000.0f;
		float horiFOV = m_fov * vsScreen::Instance()->GetAspectRatio();
		float halfFOV = m_fov * 0.5f;
		float halfHoriFOV = horiFOV * 0.5f;
		
		m_position = vsInterpolate( 0.01f, m_position, m_followSprite->m_transform.m_position );
		
		m_position.x = vsClamp( m_position.x, left + halfHoriFOV, right - halfHoriFOV );
		m_position.y = vsClamp( m_position.y, top + halfFOV, bottom - halfFOV );
	}
}

