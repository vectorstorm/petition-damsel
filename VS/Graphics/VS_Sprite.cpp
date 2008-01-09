/*
 *  VS_Sprite.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 12/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Sprite.h"

#include "VS_DisplayList.h"

vsSprite *
vsSprite::Load(const vsString &filename)
{
	return new vsSprite( vsDisplayList::Load(filename) );
}

vsSprite::vsSprite( vsDisplayList *list ) : vsEntity(),
	m_transform(),
	m_color(vsColor::White),
	m_useColor(false),
	m_boundingRadius(0.f)
{
	m_displayList = list;
	
	if ( m_displayList )
		m_boundingRadius = m_displayList->GetBoundingRadius();
}

vsSprite::~vsSprite()
{
	if ( m_displayList )
		delete m_displayList;
}

void
vsSprite::Draw( vsDisplayList *list )
{
	if ( GetVisible() )
	{
		if ( m_useColor )
			list->SetColor( m_color );
		
		list->PushTransform( m_transform );
		
		if ( m_displayList )
			list->Append( *m_displayList );
		
		_Draw( list );
		
		Parent::Draw(list);
		
		list->PopTransform();
	}
}
