/*
 *  COL_Object.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 31/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef COL_OBJECT_H
#define COL_OBJECT_H

#include "VS_Transform.h"

class colModel;

enum{
	ColFlag_Player		= BIT(0),
	ColFlag_Shot		= BIT(1),
	ColFlag_Enemy		= BIT(2),
	ColFlag_Particle	= BIT(3),
	ColFlag_Reserved0	= BIT(4),
	ColFlag_Reserved1	= BIT(5),
	ColFlag_Reserved2	= BIT(6),
	ColFlag_Reserved3	= BIT(7),
	ColFlag_Reserved4	= BIT(8),

	ColFlag_User0		= BIT(9),
	ColFlag_User1		= BIT(10),
	ColFlag_User2		= BIT(11),
	ColFlag_User3		= BIT(12),
	ColFlag_User4		= BIT(13),
	ColFlag_User5		= BIT(14),
	ColFlag_User6		= BIT(15)
};



struct colCircle
{
	vsVector2D	center;
	float		radius;
};

struct colEvent
{
	vsTransform collisionTransform;		// object transform at time of collision
	vsVector2D collisionPoint;			// precise point where collision happened
	vsVector2D collisionNormal;			// collision force direction
	vsVector2D collisionVelocity;		// velocity of other object
	float t;							// time through the frame at which the collision happened
	int colFlags;						// what collision type was the other object?
};

class colResponder
{
public:
	virtual ~colResponder() {}
	
	virtual bool		CollisionCallback( const colEvent &event ) = 0;
	virtual void		DestroyCallback() = 0;
};

class colObject
{
	colModel *		m_model;
	colCircle		m_boundingCircle;
	vsTransform		m_transform;
	vsTransform		m_lastTransform;
	float			m_mass;
	float			m_elasticity;
	
	vsVector2D		m_velocity;
	float			m_angularVelocity;
	
	bool			m_velocitiesSet;
	
	colResponder *	m_responder;
	
	bool			m_teleport;
	
	int				m_colFlags;		// what type of object am I?
	int				m_testFlags;	// what type of objects should I test against?
	
	colObject *	m_next;
	colObject * m_prev;
	
	bool		m_active;
	bool		m_destroyed;
	
public:
				colObject(int colFlags, int testFlags);
	virtual		~colObject();
	
	void		Init();
	void		Deinit();
	
	void		SetVelocity( const vsVector2D &velocity, float angularVelocity );
	
	virtual bool		CollisionCallback( const colEvent &collision );
	virtual void		DestroyCallback();
	
	void				SetResponder( colResponder *responder ) { m_responder = responder; }
	
	const colCircle &	GetBoundingCircle() { return m_boundingCircle; }
	void				SetBoundingCircle( const colCircle &circle );
	float				GetMass() { return m_mass; }
	void				SetMass( float mass ) { m_mass = mass; }
	
	float				GetElasticity() { return m_elasticity; }
	void				SetElasticity(float e) { m_elasticity = e; }
	
	bool				TestsAgainst( colObject * b ) { return (CollisionsActive() && b->CollisionsActive()) && !!(m_testFlags & b->m_colFlags); }	// returns true if we should test against this other object
	
	int					GetCollisionFlags() { return m_colFlags; }
	int					GetTestFlags() { return m_testFlags; }
	
	bool				CollisionsActive() { return m_active; }
	void				SetCollisionsActive(bool active=true);
	
	void				SetTransform( const vsTransform &t ) { m_transform = t; }
	const vsTransform &	GetTransform() { return m_transform; }
	
	const vsTransform &	GetLastTransform() { return m_lastTransform; }
	
	void				SetVelocity( const vsVector2D &velocity ) { m_velocity = velocity; }
	void				SetAngularVelocity( float rv ) { m_angularVelocity = rv; }
	
	void				ComputeVelocity(float timeStep);
	void				EndPhysicsFrame(); 
	const vsVector2D &	GetVelocity() { return m_velocity; }
	
	vsVector2D			GetVelocityOfPoint( const vsVector2D &point );	// assume that 'point' is part of me;  get its linear velocity.
	vsVector2D			GetVelocityDirOfPoint( const vsVector2D &point );	// assume that 'point' is part of me;  get its linear velocity.
	
	void				Teleport() { m_teleport = true; }
	bool				HasTeleported() { return m_teleport; }
	
	void				SetDestroyed() { m_active = false;  m_destroyed = true; }
	
	colObject *			GetNext() { return m_next; }
	colObject *			GetPrev() { return m_prev; }
	void				Append( colObject *object );
	void				Extract();
};

#endif // COL_OBJECT_H
