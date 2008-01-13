/*
 *  DA_Splat.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 13/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_Splat.h"

#include "VS_DisplayList.h"
#include "VS_Sprite.h"

daSplat::daSplat():
	vsSprite( vsDisplayList::Load("Splat.vec") )
{
}

void
daSplat::Update( float timeStep )
{
	m_timer -= timeStep;
	
	SetScale(vsVector2D(1.2f,1.2f));
	
	if ( m_timer <= 0.f )
	{
		Extract();	// despawn us.
	}
	else
	{
		vsColor grey(0.5f,0.5f,0.5f,1.0f);
		if ( m_timer > 1.0f )
			SetColor(grey);
		else
			SetColor( vsInterpolate( m_timer, vsColor::Black, grey ) );
	}
}

void 
daSplat::Spawn( const vsVector2D &where )
{
	RegisterOnLayer(0);
	SetPosition(where);
	m_timer = 5.0f;
}
