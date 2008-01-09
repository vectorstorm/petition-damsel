/*
 *  UT_Menu.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 29/12/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "UT_Menu.h"

#include "VS_DisplayList.h"
#include "VS_Font.h"
#include "VS_Sprite.h"

#include "Core.h"
#include "CORE_Game.h"

#include "SYS_Input.h"

#define PULSE_DURATION (2.0f)


utMenu::utMenu(int count, float letterSize, float capSize, float lineSpacing):
	vsSprite(NULL),
	m_itemLabel(NULL),
	m_itemValue(NULL),
	m_itemCount(count),
	m_hilitedId(0),
	m_pulseTimer(0.f),
	m_letterSize(letterSize),
	m_capSize(capSize),
	m_lineSpacing(lineSpacing)
{
	m_itemLabel = new vsSprite*[count];
	m_itemValue = new vsSprite*[count];
	
	for ( int i = 0; i < count; i++ )
	{
		m_itemLabel[i] = NULL;
		m_itemValue[i] = NULL;
	}
}

utMenu::~utMenu()
{
	for ( int i = 0; i < m_itemCount; i++ )
	{
		if ( m_itemLabel[i] )
			delete m_itemLabel[i];
		if ( m_itemValue[i] )
			delete m_itemValue[i];
	}
	
	vsDeleteArray( m_itemLabel );
	vsDeleteArray( m_itemValue );
}

void
utMenu::Update(float timeStep)
{
	if ( !GetVisible() )
		return;
	
	// clear out any actions from the previous frame
	m_action.Clear();
	
	sysInput *input = core::GetGame()->GetInput();
	
	if ( input->WasPressed( CID_Down ) )
	{
		m_hilitedId++;
		//core::GetGame()->GetSound()->PlaySound(m_tickSound);
		if ( m_hilitedId >= m_itemCount )
			m_hilitedId = 0;
	}
	if ( input->WasPressed( CID_Up ) )
	{
		//core::GetGame()->GetSound()->PlaySound(m_tickSound);
		m_hilitedId--;
		if ( m_hilitedId < 0 )
			m_hilitedId = m_itemCount-1;
	}
	if ( input->WasPressed( CID_Left ) )
		m_action.Left(m_hilitedId);
	if ( input->WasPressed( CID_Right ) )
		m_action.Right(m_hilitedId);
	if ( input->WasPressed( CID_A ) )
		m_action.Select(m_hilitedId);
	else if ( input->WasPressed( CID_B ) )
		m_action.Cancel();
	
	
	m_pulseTimer += timeStep;
	if ( m_pulseTimer > PULSE_DURATION )
		m_pulseTimer -= PULSE_DURATION;
	
	float frac = m_pulseTimer / PULSE_DURATION;
	
	float pulseAmt = vsCos(TWOPI * frac);	// [ -1..1 ]
	pulseAmt = (pulseAmt * 0.5f) + 0.5f;	// [ 0..1 ]
	
	for ( int i = 0; i < m_itemCount; i++ )
	{
		if ( m_itemLabel[i] )
		{
			vsColor c = vsColor::Blue;
			
			if ( i == m_hilitedId )
			{
				vsColor lightBlue(0.5f,0.5f,1.0f,0.8f);
				c = vsInterpolate( pulseAmt, lightBlue, vsColor::White );
			}

			m_itemLabel[i]->SetColor( c );
			if ( m_itemValue[i] )
				m_itemValue[i]->SetColor( c );
		}
	}
}

void
utMenu::Draw( vsDisplayList *list )
{
	UNUSED(list);
	ArrangeItems();

	Parent::Draw(list);
}

void
utMenu::ArrangeItems()
{
	int line = 0;
	float maxWidth = 0.f;
	
	// first, figure out what our widest label is
	for ( int i = 0; i < m_itemCount; i++ )
	{
		if ( m_itemLabel[i] )
		{
			float width = m_itemLabel[i]->GetBoundingRadius();
			
			if ( width > maxWidth )
				maxWidth = width;
		}
	}

	// now move our items into place, with the values far enough over that they won't overlap any long labels.
	for ( int i = 0; i < m_itemCount; i++ )
	{
		if ( m_itemLabel[i] )
		{
			vsVector2D pos( 0, line * (m_capSize + m_lineSpacing) );
			vsVector2D vpos( maxWidth + 50.f, line * (m_capSize + m_lineSpacing) );
			m_itemLabel[i]->SetPosition(pos);
			
			if ( m_itemValue[i] )
				m_itemValue[i]->SetPosition(vpos);
			
			line++;
		}
	}
}


void
utMenu::SetItemCount(int count)
{
	m_itemCount = count;
}

void
utMenu::SetItemLabel( int itemId, const vsString & label )
{
	vsAssert(itemId < m_itemCount && itemId >= 0, "itemId out of bounds!");
	if ( m_itemLabel[itemId] )
		delete m_itemLabel[itemId];
	
	m_itemLabel[itemId] = new vsSprite(vsBuiltInFont::CreateString(label, m_letterSize, m_capSize));
	m_itemLabel[itemId]->SetColor(vsColor::Blue);
	AddChild( m_itemLabel[itemId] );
}

void
utMenu::SetItemValue( int itemId, const vsString & value )
{
	vsAssert(itemId < m_itemCount && itemId >= 0, "itemId out of bounds!");
	if ( m_itemValue[itemId] )
		delete m_itemValue[itemId];
	
	m_itemValue[itemId] = new vsSprite(vsBuiltInFont::CreateString(value, m_letterSize, m_capSize));
	m_itemValue[itemId]->SetColor(vsColor::Blue);
	AddChild( m_itemValue[itemId] );
}
