/*
 *  COL_Object.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 31/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "Core.h"
#include "CORE_Game.h"

#include "COL_System.h"

#include "COL_Object.h"

colObject::colObject( int colFlags, int testFlags ): 
m_model(NULL),
m_transform(vsTransform::Zero),
m_lastTransform(vsTransform::Zero),
m_teleport(true),
m_colFlags(colFlags),
m_testFlags(testFlags),
m_next(NULL), 
m_prev(NULL),	
m_active(false)
{
	m_boundingCircle.center = vsVector2D::Zero;
	m_boundingCircle.radius = 0.f;
	
	m_mass = 1.0f;
	m_elasticity = 0.9f;	// pretty elastic, by default, but not perfectly elastic.
	m_velocitiesSet = false;
	m_destroyed = false;
}

colObject::~colObject()
{
}

void
colObject::Init()
{
	m_teleport = true;
}

void
colObject::Deinit()
{
	m_active = false;
}

void
colObject::SetCollisionsActive(bool active)
{
	if ( active != m_active )
	{
		m_active = active;
		
		if ( active )
		{
			core::GetGame()->GetCollision()->RegisterObject( this );
			m_destroyed = false;
		}
		else
			Extract();
	}
}

void
colObject::SetBoundingCircle( const colCircle &circle )
{
	m_boundingCircle = circle;
}

void
colObject::SetVelocity( const vsVector2D &velocity, float angularVelocity )
{
	m_velocity = velocity;
	m_angularVelocity = angularVelocity;
	m_velocitiesSet = true;
}

void
colObject::ComputeVelocity( float timeStep )
{
	if ( m_velocitiesSet )
	{
		if ( m_teleport )
			m_lastTransform = m_transform;
	}
	else// ( !m_velocitiesSet )
	{
		if ( m_teleport )
		{
			m_velocity = vsVector2D::Zero;
			m_angularVelocity = 0.f;
			m_lastTransform = m_transform;
		}
		else
		{
			m_velocity = (m_transform.m_position - m_lastTransform.m_position) * (1.0f / timeStep);
			m_angularVelocity = (m_transform.m_angle - m_lastTransform.m_angle).Get() * (1.0f / timeStep);
		}
	}
}

void
colObject::Append( colObject *object )
{
	object->m_next = m_next;
	object->m_prev = this;
	
	if ( m_next )
		m_next->m_prev = object;
	
	m_next = object;
}

void
colObject::Extract()
{
	if ( m_prev )
		m_prev->m_next = m_next;
	if ( m_next )
		m_next->m_prev = m_prev;
	
	m_prev = m_next = this;
}

bool
colObject::CollisionCallback(const colEvent &collision)
{
	bool handled = false;
	
	if ( m_responder )
	{
		handled = m_responder->CollisionCallback( collision );
	}
	
	if ( !handled )
	{
		SetTransform(collision.collisionTransform);
		Teleport();
		handled = true;
	}
	
	return handled;
}

void
colObject::DestroyCallback()
{
	if ( m_responder )
		m_responder->DestroyCallback();
}

void
colObject::EndPhysicsFrame()
{
	if ( m_destroyed )
	{
		Extract();
		DestroyCallback();
	}
	else
	{ 
		m_velocitiesSet = false;  
		m_teleport = false; 
		m_lastTransform = m_transform; 
	}
}

vsVector2D
colObject::GetVelocityOfPoint( const vsVector2D &point )
{
	vsVector2D velocityFromRotation = point - m_transform.m_position;						// vector from 'point' to our center
	velocityFromRotation.Set( -velocityFromRotation.y, velocityFromRotation.x );	// rotate 90 degrees
	velocityFromRotation *= m_angularVelocity;
	
	vsVector2D result = velocityFromRotation + m_velocity;
	
	return result;
}

