/*
 *  VS_Entity.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 12/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Entity.h"
#include "VS_Screen.h"
#include "VS_Layer.h"

vsEntity::vsEntity():
m_visible(true),
m_parent(NULL),
m_child(NULL)
{
	m_next = m_prev = this;
}

vsEntity::~vsEntity()
{
	if ( m_parent )
		m_parent->RemoveChild(this);
	else
		Extract();
	
	// now, delete our children.  Remember that they'll be extracting themselves from our child list as they're deleted,
	// so we need to check what the next child is before we delete the previous child!
	
	vsEntity *child = m_child;
	
	while(child)
	{
		vsEntity *next = child->m_next;
		
		if ( next == child )
			next = NULL;
		
		delete child;
		child = next;
	}
}

void
vsEntity::Append( vsEntity *sprite )
{
	sprite->m_next = m_next;
	sprite->m_prev = this;
	
	m_next->m_prev = sprite;
	m_next = sprite;
}

void
vsEntity::Prepend( vsEntity *sprite )
{
	sprite->m_next = this;
	sprite->m_prev = m_prev;
	
	m_prev->m_next = sprite;
	m_prev = sprite;
}

void
vsEntity::AddChild( vsEntity *sprite )
{
	sprite->m_parent = this;
	sprite->m_next = m_child;
	sprite->m_prev = NULL;
	
	if ( m_child )
		m_child->m_prev = sprite;
	
	m_child = sprite;
}

void
vsEntity::RemoveChild( vsEntity *sprite )
{
	if ( m_child == sprite )
	{
		m_child = m_child->m_next;
	}
	else
	{
		sprite->Extract();
	}
}

void
vsEntity::Extract()
{
	if ( m_prev )
		m_prev->m_next = m_next;
	if ( m_next )
		m_next->m_prev = m_prev;
	
	m_prev = m_next = this;
}

void
vsEntity::DrawChildren( vsDisplayList *list )
{
	vsEntity *child = m_child;
		
	while ( child )
	{
		child->Draw( list );
		child = child->m_next;
	}
}

void
vsEntity::Draw( vsDisplayList *list )
{
	if ( m_visible )
	{
		_Draw(list);
		DrawChildren(list);
	}
}

void
vsEntity::Update( float timeStep )
{
	vsEntity *child = m_child;
	
	while ( child )
	{
		vsEntity *next = child->m_next;
		
		child->Update( timeStep );

		child = next;
	}
}

void
vsEntity::RegisterOnLayer(int layer)
{
	vsScreen::Instance()->GetLayer(layer)->RegisterEntityOnTop(this);
}

#if defined(_DEBUG)

void
vsEntity::RegisterOnDebugLayer()
{
	vsScreen::Instance()->GetDebugLayer()->RegisterEntityOnTop(this);
}

#endif // _DEBUG
