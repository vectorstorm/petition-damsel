/*
 *  PHYS_Sprite.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 4/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "PHYS_Sprite.h"
#include "VS_DisplayList.h"

physSprite *
physSprite::Load(const vsString &filename, int colFlags, int testFlags)
{
	return new physSprite( vsDisplayList::Load(filename), colFlags, testFlags );
}

physSprite::physSprite(vsDisplayList *list, int colFlags, int testFlags) : colSprite(list, colFlags, testFlags),
m_velocity(vsVector2D::Zero),
m_angularVelocity(0.f),
m_force(vsVector2D::Zero),
m_torque(0.f),
m_collBounciness(1.f)
{
	float radius = 0.f;
	if ( list )
		radius = list->GetBoundingRadius();
	// ordinarily, our angular mass (also known as "moment of inertia" for some reason I don't understand) is defined as
	// ANGULAR_MASS = MASS * (RADIUS*RADIUS).
	m_angularMass = m_colObject->GetMass() /* radius*/ * radius;
}

physSprite::~physSprite()
{
}

void
physSprite::Update( float timeStep )
{
	m_velocity += m_force * timeStep;				// TODO:  Implement mass
	m_angularVelocity += m_torque * timeStep;
	
	m_transform.m_position += m_velocity * timeStep;
	m_transform.m_angle += m_angularVelocity * timeStep;
	
	m_force = vsVector2D::Zero;
	m_torque = 0.f;
	
	m_colObject->SetVelocity( m_velocity, m_angularVelocity );
	
	Parent::Update(timeStep);
}

void
physSprite::Draw( vsDisplayList *list )
{
	Parent::Draw(list);
	
#if defined(_DEBUG)
	
	vsTuneable bool drawVelocity = false;
	
	if ( drawVelocity )
	{
		list->SetColor( vsColor::Red );
		list->MoveTo( GetPosition() );
		list->LineTo( GetPosition() + GetVelocity() );
	}
	
#endif
}

bool
physSprite::CollisionCallback( const colEvent &collision )
{
	//UNUSED(collision);
	/*vsVector2D velocityDirection = m_velocity;
	if ( m_velocity.Length() > 0.f )
	{
		velocityDirection.Normalise();
		
		vsVector2D collisionVelocity;
		float velocityTowardNormal = collisionNormal.Dot( m_velocity );
		if ( velocityTowardNormal > 0.f )
		{
			collisionVelocity = velocityTowardNormal * collisionNormal;
			
			m_velocity -= (2.0f * collisionVelocity);	// perfect elastic collision
		}
	}*/
	//m_velocity += collision.collisionNormal;
	m_transform = collision.collisionTransform;

	ApplyImpulse( collision.collisionPoint, collision.collisionVelocity );
	
	m_transform.m_position += m_velocity * collision.t;
	m_colObject->SetTransform(m_transform);
	m_colObject->SetVelocity( m_velocity, m_angularVelocity );
//	m_colObject->Teleport();
	
	return true;
}

void
physSprite::DestroyCallback()
{
}

void
physSprite::ApplyImpulse( const vsVector2D &where, const vsVector2D &impulse )
{
	
	vsVector2D effectiveImpulse = impulse * (1.0f/m_colObject->GetMass());
//	float impulsePower = effectiveImpulse.Length();
	vsVector2D impulseDirection = effectiveImpulse;
	if ( impulseDirection.Length() > 0.01f )
		impulseDirection.Normalise();
	
	vsVector2D localPoint = where - GetPosition();
	vsVector2D collisionNormal = localPoint;
	if ( collisionNormal.SqLength() > 0.01f )
		collisionNormal.Normalise();
	vsVector2D collisionTangent( -collisionNormal.y, collisionNormal.x );
	
	vsVector2D localVelocity = GetVelocityOfPoint( where );
	vsVector2D localVelocityDir = GetVelocityDirOfPoint( where );
	vsVector2D deltaVelocity = impulse - localVelocity;
	
//	float amt = ( deltaVelocity.Dot( collisionNormal ) );
		
	//if ( amt < 0.0f )
	{
//		float ldn = localVelocityDir.Dot(collisionNormal);
//		float j = ( amt * (1.0f) / (1.0f + ((ldn*ldn)/*/m_colObject->GetMass()*/)));
		
//		m_velocity += j * collisionNormal;
		m_velocity += impulse.Dot( collisionNormal ) * collisionNormal;
		
		if ( localPoint.SqLength() < 1.0f )
			localPoint.Set(1.0f,0.0f);	// fake it, such that we don't add ridiculous speed
		
		m_angularVelocity += impulse.Dot( collisionTangent ) / (m_angularMass * localPoint.Length());

		
//		float fractionAsVelocity = impulseDirection.Dot( collisionNormal );
//		float fractionAsAngular = (1.0f - fractionAsVelocity);
		
//		m_angularVelocity += fractionAsAngular * impulsePower;
//		m_velocity += fractionAsVelocity * effectiveImpulse;
	}
//	m_angularVelocity += velocityDir.Dot(effectiveImpulse);


//	m_velocity += effectiveImpulse;
	
}

vsVector2D
physSprite::GetVelocityOfPoint( const vsVector2D &point )
{
	vsVector2D localPoint = point - GetPosition();						// vector from 'point' to our center
	vsVector2D velocityFromRotation( -localPoint.y, localPoint.x );	// rotate 90 degrees
	velocityFromRotation *= m_angularVelocity;
	
	// note that the above calculation works because m_angularVelocity is in RADIANS, 
	// and so makes a full revolution per second when it reaches a value of TWOPI.
	// The full distance travelled by the passed point around our center point in one
	// full revolution is TWOPI * the point's distance from center.  So at a rotational 
	// velocity of TWOPI, the point also travels at a rate of TWOPI * radius.
	
	vsVector2D result = velocityFromRotation + m_velocity;
	
	return result;
}

vsVector2D
physSprite::GetVelocityDirOfPoint( const vsVector2D &point )
{
	vsVector2D v = GetVelocityOfPoint(point);
	if ( v.Length() > 0.1f )
		v.Normalise();
	return v;
}

