/*
 *  DA_PetitionPickup.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 11/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_PetitionPickup.h"

#include "VS_DisplayList.h"

daPetitionPickup::daPetitionPickup(int maxSignatures):
	vsSprite(vsDisplayList::Load("Petition.vec")),
	m_signatures(0),
	m_maxSignatures(maxSignatures),
	m_state(daPetitionPickup::Dead)
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

daPetitionPickup::~daPetitionPickup()
{
	vsDeleteArray(m_verts);
}

void
daPetitionPickup::_Draw( vsDisplayList *list )
{
	// draw lines around us, to denote how close we are to fully signed
	
	if ( m_state == Held || m_state == Dropped )
	{
		bool enoughSignatures = true;
		list->SetColor( vsColor::Green );
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

void
daPetitionPickup::Spawn( const vsVector2D &pos )
{
	SetPosition(pos);
	RegisterOnLayer(0);
	m_state = Pickup;
}
