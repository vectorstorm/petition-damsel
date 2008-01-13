/*
 *  DA_Petition.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 11/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_Petition.h"
#include "DA_ModeInGame.h"

#include "SND_Sample.h"

#include "VS_DisplayList.h"
#include "VS_Font.h"
#include "VS_Transform.h"

daPetition::daPetition(daModeInGame *mode, int maxSignatures):
	vsSprite(vsDisplayList::Load("Petition.vec")),
	m_mode(mode),
	m_signatures(0),
	m_maxSignatures(maxSignatures),
	m_state(daPetition::Dead)
{
	SetScale(3.0f);
	
	m_verts = new vsVector2D[m_maxSignatures];
	
	m_exploding = false;
	
	vsVector2D top(0.0f,12.0f);
	
	vsAngle ang = 0.f;
	float incr = TWOPI / (m_maxSignatures-1);
	for ( int i = 0; i < m_maxSignatures; i++ )
	{
		ang += incr;
		m_verts[i] = ang.ApplyRotationTo(top);
	}
	
	SetColor(vsColor::Yellow);
	
	if ( !m_mode )
		m_signatures = 3;
	
	m_beep = new sndSample("beep_b.wav");
	m_explodeBeep = new sndSample("3beeps1000.wav");
}

daPetition::~daPetition()
{
	vsDelete( m_explodeBeep );
	vsDelete( m_beep );
	vsDeleteArray(m_verts);
}

void
daPetition::Update( float timeStep )
{
	if ( !m_mode ) return;
	
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
	
	if ( m_exploding )
	{
		m_explodeTimer -= timeStep;
		
		if ( m_explodeTimer < 0.f )
			Despawn();
		
	}
	else if ( m_state == Held || m_state == Dropping || m_state == Dropped )
	{
		m_timer -= timeStep;
		
		
		if ( m_timer < 0.0f )
		{
			m_exploding = true;
			m_explodeTimer = 3.0f;
			m_pulseTimer = 0.f;
			m_explodeBeep->Play();
			
			m_mode->SpawnMadCar( GetPositionInLevel() );
		}
		else if ( (int)m_timer < m_beepedAt )
		{
			m_beepedAt = (int)m_timer;
			
			m_beep->Play();
		}
		
	}
}

void
daPetition::_Draw( vsDisplayList *list )
{
	// draw lines around us, to denote how close we are to fully signed
	
	if ( m_exploding )
	{
		SetColor( vsColor::Red );
		
		list->MoveTo(m_verts[0]*5.0f);
		
		for ( int i = 1; i < m_maxSignatures; i++ )
		{
			list->LineTo(m_verts[i]*5.0f);
		}
	}
	else if ( !m_mode || m_state == Held || m_state == Dropping || m_state == Dropped )
	{
		vsTransform t;
		if ( m_state == Held && m_player->GetScale().x < 0.f )	// flipped!
		{
			t.m_scale.Set(-1.0f,1.0f);
		}
		list->PushTransform(t);

		vsColor darkRed(0.5f,0.0f,0.0f,1.0f);
		vsColor green(0.0f,0.6f,0.0f,1.0f);
		bool enoughSignatures = true;
		if ( m_signatures )
			list->SetColor( green );
		else
			list->SetColor( darkRed );
		list->MoveTo(m_verts[0]);
		
		for ( int i = 1; i < m_maxSignatures; i++ )
		{
			if ( enoughSignatures && i >= m_signatures )
			{
				enoughSignatures = false;
				list->SetColor( darkRed );
			}
			list->LineTo(m_verts[i]);
		}
		list->PopTransform();
		
		if ( m_mode )
		{
			vsString timerString = vsFormatString("%d", (int)m_timer);
			vsDisplayList *timerList = vsBuiltInFont::CreateString(timerString, 20.f, 20.f, Justification_Center);
			t.m_position.Set(0.f,-40.f);
			list->PushTransform(t);
			list->Append(*timerList);
			list->PopTransform();
			delete timerList;
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
	SetColor( vsColor::Yellow );
	SetPosition(pos);
	RegisterOnLayer(0);
	m_state = Pickup;
	m_signatures = 0;
	m_exploding = false;
}

void
daPetition::Despawn()
{
	if ( m_maxSignatures == m_signatures )
		m_mode->AddTimeLeft(5.0f);
	if ( m_parent )
		m_parent->RemoveChild(this);
	Extract();
	m_state = Dead;
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
	if ( m_state != Held || m_player != player )
	{
		m_player = player;
		m_state = Held;	// don't register with the layer;  we're 
		// going to be a child of the player, in this state.
		
		m_timer = 10.0f;	// ten second activity, max
		m_beepedAt = 9;
	}
}

void
daPetition::Thrown(vsVector2D source, vsVector2D destination)
{
	SetPosition(source);
	m_dropDestination = destination;
	RegisterOnLayer(0);
	m_state = Dropping;
}

void
daPetition::Sign()
{
	if ( m_signatures < m_maxSignatures )
	{
		m_signatures++;
		
		m_mode->AddSignature();
		m_mode->AddTimeLeft(1.0f);
	}
}

