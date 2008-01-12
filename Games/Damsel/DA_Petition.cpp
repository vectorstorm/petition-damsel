/*
 *  DA_Petition.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 11/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_Petition.h"

#include "VS_DisplayList.h"

daPetition::daPetition(int maxSignatures):
	vsSprite(vsDisplayList::Load("Petition.vec")),
	m_signatures(0),
	m_maxSignatures(maxSignatures),
	m_state(daPetition::Dead)
{
	SetScale(3.0f);
	
	m_verts = new vsVector2D[m_maxSignatures];
	
	
	vsVector2D top(0.0f,12.0f);
	
	vsAngle ang = 0.f;
	float incr = TWOPI / (m_maxSignatures-1);
	for ( int i = 0; i < m_maxSignatures; i++ )
	{
		ang += incr;
		m_verts[i] = ang.ApplyRotationTo(top);
	}
	
	SetColor(vsColor::Yellow);
}

daPetition::~daPetition()
{
	vsDeleteArray(m_verts);
}

void
daPetition::Update( float timeStep )
{
	if ( m_state == Dropping )
	{
		vsTuneable float dropSpeed = 800.0f;
		float distanceThisFrame = dropSpeed * timeStep;
		vsVector2D delta = m_dropDestination - GetPosition();
		
		if ( delta.Magnitude() > distanceThisFrame )
		{
			delta.Normalise();
			delta *= distanceThisFrame;
		}
		else
			m_state = Dropped;
		
		SetPosition( GetPosition() + delta );
	}
}

void
daPetition::_Draw( vsDisplayList *list )
{
	// draw lines around us, to denote how close we are to fully signed
	
	if ( m_state == Held || m_state == Dropped )
	{
		bool enoughSignatures = true;
		if ( m_signatures )
			list->SetColor( vsColor::Green );
		else
			list->SetColor( vsColor::Red );
		list->MoveTo(m_verts[0]);
		
		for ( int i = 1; i < m_maxSignatures; i++ )
		{
			if ( enoughSignatures && i >= m_signatures )
			{
				enoughSignatures = false;
				list->SetColor( vsColor::Red );
			}
			list->LineTo(m_verts[i]);
		}
	}
}

vsVector2D
daPetition::GetPositionInLevel()
{
	vsVector2D pos = GetPosition();
	
	if ( m_state == Held )
		pos += m_player->GetPosition();
	
	return pos;
}

void
daPetition::Spawn( const vsVector2D &pos )
{
	SetPosition(pos);
	RegisterOnLayer(0);
	m_state = Pickup;
}

void
daPetition::PickedUp()
{
	m_state = Inventory;
	Extract();	// not drawn visibly on screen when we're in the inventory
}

void
daPetition::HeldUp( vsSprite *player )
{
	m_player = player;
	m_state = Held;	// don't register with the layer;  we're 
					// going to be a child of the player, in this state.
}

void
daPetition::Thrown(vsVector2D source, vsVector2D destination)
{
	SetPosition(source);
	m_dropDestination = destination;
	RegisterOnLayer(0);
	m_state = Dropping;
}


