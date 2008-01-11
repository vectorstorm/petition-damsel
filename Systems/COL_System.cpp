/*
 *  COL_System.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 23/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "COL_System.h"
#include "COL_Object.h"

#include "VS_Vector.h"

// Calculate the time at which a moving circle intersects another moving circle

static int circleCircleCollision( const colCircle &one, const vsVector2D &va, 
					   const colCircle &two, const vsVector2D &vb,
					   float *t1, float *t2 )
{
	float r = one.radius + two.radius;
	
	float c0 = one.center.x;
	float d0 = one.center.y;
	float dc = va.x;
	float dd = va.y;
	
	float x0 = two.center.x;
	float y0 = two.center.y;
	float dx = vb.x;
	float dy = vb.y;
	
	{
		// The solution is a quadratic on t : at*t + b*t + c = 0;
		float a = dc*dc + dx*dx + dd*dd + dy*dy - dx*dc*2 - dy*dd*2;
		float b = ( c0*dc - c0*dx - dc*x0 + dx*x0 + d0*dd - d0*dy - y0*dd + dy*y0 ) * 2;
		float c = c0*c0 + x0*x0 + d0*d0 + y0*y0 - r*r - c0 * x0 * 2 - d0 * y0 * 2;
		
		if( vsFabs( a ) == 0.0f )
		{
			return 0;
		}
		
		// Calculate the determinant
		float det = b*b - a*c*4;
		
		//        cout << "test(0)=" << _test( a.center.x, a.center.y, va.x, va.y, b.center.x, b.center.y, dx, dy, r, 0 ) << endl;
		//        cout << "test2(0)=" << _test2( a, b, c, 0 ) << endl;
		
		if( det < 0.0f )
		{
			// no solution!
			return 0;        
		}
		else if( det == 0.0f )
		{
			// one solution
			*t1 = -b / ( a * 2 );
			
			//                cout << "test(t1)=" << _test( a.center.x, a.center.y, va.x, va.y, b.center.x, b.center.y, dx, dy, r, *t1 ) << endl;
			//                cout << "test2(t1)=" << _test2( a, b, c, *t1 ) << endl;
			
			return 1;
		}
		else // two solutions
		{
			float d = vsSqrt( det );
			
			*t1 = ( -b - d ) / ( a * 2 );
			*t2 = ( -b + d ) / ( a * 2 );
			
			//                cout << "test(t1)=" << _test( a.center.x, a.center.y, va.x, va.y, b.center.x, b.center.y, dx, dy, r, *t1 ) << endl;
			//                cout << "test2(t1)=" << _test2( a, b, c, *t1 ) << endl;
			//                cout << "test(t2)=" << _test( a.center.x, a.center.y, va.x, va.y, b.center.x, b.center.y, dx, dy, r, *t2 ) << endl;
			//                cout << "test2(t2)=" << _test2( a, b, c, *t2 ) << endl;
			
			return 2;
		}
	}
}

void
colSystem::Init()
{
	m_objectList = new colObject(0,0);
	m_pendingObjectList = new colObject(0,0);
	m_inCollisionTest = false;
}

void
colSystem::Deinit()
{
	delete m_objectList;
	delete m_pendingObjectList;
}

void
colSystem::RegisterObject( colObject *object )
{
	m_pendingObjectList->Append(object);
}

void
colSystem::PostUpdate(float timeStep)
{
	// pull everything out of our pending list, and drop it into our real list
	
	while ( m_pendingObjectList->GetNext() != NULL )
	{
		colObject *obj = m_pendingObjectList->GetNext();
		obj->Extract();
		m_objectList->Append(obj);
	}

	if ( timeStep <= 0.f )	// if no time is passing, don't bother checking for collisions
		return;
	
	m_inCollisionTest = true;
	
	colObject *a, *aNext;
	colObject *b, *bNext;
	
	bool foundCollision = false;
	int passesLeft = 5;			// ensure we don't do more than five passes, just for CPU sake;  if we haven't resolved all collisions by the end of five passes, give up and let stuff pass through each other.
	
	do
	{
		passesLeft--;
		foundCollision = false;

		a = m_objectList->GetNext();
		while(a)
		{
			a->ComputeVelocity(timeStep);
			a = a->GetNext();
		}
		
		a = m_objectList->GetNext();
		while(a)
		{
			aNext = a->GetNext();
			vsAssert( aNext != a, "Collision object list corrupted!" );
			
			b = a->GetNext();
			while ( b )
			{
				bNext = b->GetNext();
				vsAssert( bNext != b, "Collision object list corrupted!" );
				
				if ( Test(a,b, timeStep) )
					foundCollision = true;
				
				b = bNext;
			}
			
			a = aNext;
		}
	}while( foundCollision && passesLeft > 0 );

	m_inCollisionTest = false;

	a = m_objectList->GetNext();
	while(a)
	{
		aNext = a->GetNext();
		a->EndPhysicsFrame();
		a = aNext;
	}
}

bool
colSystem::Test( colObject *a, colObject *b, float timeStep )
{
	bool aVsB = a->TestsAgainst(b);
	bool bVsA = b->TestsAgainst(a);
	bool collided = false;
	
	if ( aVsB || bVsA )
	{
		colCircle aC = a->GetBoundingCircle();
		colCircle bC = b->GetBoundingCircle();
		
		// assume uniform scaling on collision object
		aC.radius *= vsFabs(a->GetTransform().m_scale.x);
		bC.radius *= vsFabs(b->GetTransform().m_scale.x);
		
		aC.center += a->GetLastTransform().m_position;
		bC.center += b->GetLastTransform().m_position;
		
		const vsVector2D &aV = a->GetVelocity();
		const vsVector2D &bV = b->GetVelocity();
		float speedA = aV.Length();
		float speedB = bV.Length();
		
		vsVector2D delta = aC.center - bC.center;
		float sqRad = (aC.radius + bC.radius);
		sqRad += (speedA + speedB);
		sqRad *= sqRad;
		
		if ( delta.Length() > sqRad )
			return false;
		
		//			printf("Player ship at: %0.2f, %0.2f -> %0.2f, %0.2f\n", bC.center.x, bC.center.y, b->GetTransform().m_position.x, b->GetTransform().m_position.y);
		
		float t1, t2;
		int collCount = circleCircleCollision( aC, aV, bC, bV, &t1, &t2 );
		
		vsTuneable float t_slop = 0.0f;
		
		if ( collCount >= 1 && t1 >= -t_slop && t1 < timeStep )		// at least one collision and we enter the object during this timeStep
		{
			// coll happened!  We need to know the final transform and normal
			HandleCollision( a, b, t1 - t_slop, timeStep, aVsB, bVsA );
			collided = true;
		}
		else if ( (collCount >= 2 && t1 < 0.f && t2 > 0.f ) )			// at least two collisions, and we entered the object some time in the past.
		{
			// we seem to be embedded;  we must not have properly popped out last time?
			HandleCollision( a, b, 0.f, timeStep, aVsB, bVsA );
			collided = true;
		}
		else if ( collCount >= 2 && t2 >= 0.f && t2 < timeStep )
		{
			//HandleCollision( a, b, t2, timeStep, aVsB, bVsA );	// ignore collisions as we pass out of each other.
		}
	}
	
	return collided;
}

void
colSystem::HandleCollision( colObject *a, colObject *b, float t, float timeStep, bool aVsB, bool bVsA )
{
	float fractionThroughFrame = t / timeStep;

	colCircle aC = a->GetBoundingCircle();
	colCircle bC = b->GetBoundingCircle();
	
	// assume uniform scaling on collision object
	aC.radius *= vsFabs(a->GetTransform().m_scale.x);
	bC.radius *= vsFabs(b->GetTransform().m_scale.x);
	
	aC.center += a->GetLastTransform().m_position;
	bC.center += b->GetLastTransform().m_position;
	
	vsTransform finalA = vsInterpolate( fractionThroughFrame, a->GetLastTransform(), a->GetTransform() );
	vsTransform finalB = vsInterpolate( fractionThroughFrame, b->GetLastTransform(), b->GetTransform() );
/*	
	float timeAgo = timeStep - t;
	
	finalA.m_position = a->GetTransform().m_position;
	finalA.m_position -= a->GetVelocity() * timeAgo;
//	finalA.m_position += a->GetVelocity() * t;

	finalB.m_position = b->GetTransform().m_position;//bC.center;
	finalB.m_position -= b->GetVelocity() * timeAgo;
//	finalB.m_position += b->GetVelocity() * t;
*/
	vsVector2D  normalA = (finalB.m_position - finalA.m_position);
	normalA.Normalise();
	vsVector2D	normalB = -normalA;
	
	vsVector2D delta = (finalB.m_position - finalA.m_position);
	float deltaLen = delta.Length();
	
	float ar = aC.radius;
	float br = bC.radius;

	if ( deltaLen < (ar+br) )	// we aren't pushed far enough apart to stop intersecting!
	{
		float aMoveFraction = 0.5f;
		vsVector2D midpoint = finalA.m_position + (0.5f * delta);
		delta.Normalise();
		vsTuneable float t_slop = 0.01f;
		delta *= (ar+br+t_slop);
		
		if ( aVsB && bVsA )
			aMoveFraction = 0.5f;
		else if ( aVsB )
			aMoveFraction = 1.0f;
		else
			aMoveFraction = 0.0f;

		float bMoveFraction = 1.0f - aMoveFraction;
		
		finalA.m_position = ( midpoint - (delta * aMoveFraction) );
		finalB.m_position = ( midpoint + (delta * bMoveFraction) );
	}
	
	float radiusAFraction = ar / (ar+br);
	
	delta *= radiusAFraction;
	vsVector2D collisionPoint = finalA.m_position + delta;
	
//	vsVector2D aV = a->GetVelocity();
//	vsVector2D bV = b->GetVelocity();
	vsVector2D aV = a->GetVelocityOfPoint( collisionPoint );
	vsVector2D bV = b->GetVelocityOfPoint( collisionPoint );
	
	vsVector2D forceA = bV - aV;
	vsVector2D forceB = aV - bV;
//	float force = normalA.Dot( bV - aV );	// here's how much force has been imparted by the collision
	
	float aMassFraction = 1.0f;
	float bMassFraction = 1.0f;
	
	if ( aVsB && bVsA )
	{
		float aMass = a->GetMass();
		float bMass = b->GetMass();
		float totalMass = aMass + bMass;
	
		aMassFraction = aMass / totalMass;
		bMassFraction = bMass / totalMass;
	}
//	float pressureOnNormalA = normalA.Dot( forceA ) * bMassFraction;
//	float pressureOnNormalB = normalB.Dot( forceB ) * aMassFraction;
	
	float totalElasticity = (a->GetElasticity() + b->GetElasticity());
	
//	pressureOnNormalA *= totalElasticity;	// temp;  1.0f == inelastic collision, 2.0f == elastic collision
//	pressureOnNormalB *= totalElasticity;
	
	float timeRemaining = timeStep - t;
	
	colEvent aE;
	colEvent bE;
	
	if ( aVsB )
	{
		aE.t = timeRemaining;
		aE.collisionTransform = finalA;
		aE.collisionNormal = normalA;//forceA;//normalA * pressureOnNormalA;
		aE.collisionPoint = collisionPoint;
		aE.collisionVelocity = forceA * bMassFraction * totalElasticity;//pressureOnNormalA * normalA;
		aE.colFlags = b->GetCollisionFlags();
		
		a->CollisionCallback(aE);
	}
	
	if ( bVsA )
	{
		bE.t = timeRemaining;
		bE.collisionTransform = finalB;
		bE.collisionNormal = normalB;//forceB;//normalB * pressureOnNormalB;
		bE.collisionPoint = collisionPoint;
		bE.collisionVelocity = forceB * aMassFraction * totalElasticity;//pressureOnNormalB * normalB;
		bE.colFlags = a->GetCollisionFlags();
	
		b->CollisionCallback(bE);
	}
}

bool
colSystem::CollisionTest( const vsVector2D &a, const vsVector2D &b, float r, int testFlags )
{
	colCircle c;
	c.center = a;
	c.radius = r;
	
	vsVector2D velocity = b - a;
	colObject *object = m_objectList->GetNext();
	
	while ( object )
	{
		if ( object->CollisionsActive() && !!(object->GetCollisionFlags() & testFlags ) )
		{
			// this is an object we need to test against!
			colCircle bC = object->GetBoundingCircle();
			bC.center += object->GetTransform().m_position;
			vsVector2D bV = object->GetVelocity();
			float t1, t2;
			
			int collCount = circleCircleCollision( c, velocity, bC, bV, &t1, &t2 );
			
			if ( collCount >= 1 )
			{
				if ( t1 >= 0.f && t1 <= 1.f )
					return true;	// we hit this object!
			}
			if ( collCount >= 2 )
			{
				if ( t1 <= 0.f && t2 >= 0.f )
					return true;	// we're in the middle of this object!
				if ( t2 >= 0.f && t2 <= 1.f )
					return true;	// we hit this object!
			}
		}
		object = object->GetNext();
	}
	
	return false;
}
