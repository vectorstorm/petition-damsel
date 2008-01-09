/*
 *  AST_Camera.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 25/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "AST_Camera.h"

#include "VS_Sprite.h"
#include  <assert.h>


astCamera::astCamera()
{
	m_position.Set(0.0f,0.0f);
	m_fov = 800.0f;	// we see 800 units around the position.  (vertically)
	m_trackSprite = NULL;
}

astCamera::~astCamera()
{
}

void
astCamera::Update(float timeStep)
{
	UNUSED(timeStep);
	
	static bool s_trackPlayer = false;
	
	if ( s_trackPlayer && m_trackSprite )
	{
		m_position = vsInterpolate( 0.05f, m_position, m_trackSprite->m_transform.m_position );
		m_angle = vsInterpolate( 0.05f, m_angle, m_trackSprite->m_transform.m_angle );
	}
}
