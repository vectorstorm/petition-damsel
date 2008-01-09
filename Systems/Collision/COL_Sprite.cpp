/*
 *  COL_Sprite.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 3/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "COL_Sprite.h"

#include "COL_Object.h"
#include "COL_System.h"
#include "Core.h"
#include "CORE_Game.h"

#include "VS_DisplayList.h"

colSprite *
colSprite::Load(const vsString &filename, int colFlags, int testFlags)
{
	return new colSprite( vsDisplayList::Load(filename), colFlags, testFlags );
}

colSprite::colSprite(vsDisplayList *list, int colFlags, int testFlags) : vsSprite(list), m_colObject(NULL)
{
	m_colObject = new colObject(colFlags, testFlags);
	m_colObject->SetResponder(this);
	colCircle circle;
	
	list->GetBoundingCircle( circle.center, circle.radius );
	circle.radius *= 0.8f;
	m_colObject->SetBoundingCircle( circle );
}

colSprite::~colSprite()
{
	if ( m_colObject )
		delete m_colObject;
}

void
colSprite::Teleport()
{
	m_colObject->Teleport();
}

void
colSprite::Update( float timeStep )
{
	UNUSED(timeStep);
	m_colObject->SetTransform( m_transform );
}

bool
colSprite::CollisionCallback( const colEvent &collision )
{
	m_transform = collision.collisionTransform;
	m_colObject->SetTransform(m_transform);
	m_colObject->Teleport();
	
	return true;
}

void
colSprite::DestroyCallback()
{
}
