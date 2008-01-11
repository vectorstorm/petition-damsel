/*
 *  PHYS_Sprite.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 4/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef PHYS_SPRITE_H
#define PHYS_SPRITE_H

#include "COL_Sprite.h"

class physSprite : public colSprite
{
	typedef colSprite Parent;
	
	vsVector2D		m_velocity;
	float			m_angularVelocity;
	
	float			m_angularMass;
	
	vsVector2D		m_force;
	float			m_torque;
	
	bool			m_angleLocked;
	
	float			m_collBounciness;

protected:
	
public:
	static physSprite * Load(const vsString &filename, int colFlags, int testFlags);
	
	physSprite( vsDisplayList *list, int colFlags, int testFlags );
	virtual ~physSprite();
	
	void				SetVelocity(const vsVector2D &v) { m_velocity = v; }	// these two functions should only be called when initially spawning an object!  Calling at any other time can lead to collision troubles!
	void				SetAngularVelocity(float v) { m_angularVelocity = v; }	// 

	
	void			SetMass(float mass) { m_colObject->SetMass(mass); }
	void			SetAngleLocked(bool locked) { m_angleLocked = locked; }
	
	void			AddForce( const vsVector2D &force ) { m_force += force; }
	void			AddTorque( float torque ) { m_torque += torque; }
	
	const vsVector2D &	GetVelocity() { return m_velocity; }
	float				GetAngularVelocity() { return m_angularVelocity; }
	
	void				ApplyImpulse( const vsVector2D &where, const vsVector2D &impulse );
	vsVector2D			GetVelocityOfPoint( const vsVector2D &point );	// assume that 'point' is part of me;  get its linear velocity.
	vsVector2D			GetVelocityDirOfPoint( const vsVector2D &point );	// assume that 'point' is part of me;  get its linear velocity.
	
	
	virtual bool	CollisionCallback( const colEvent &collision );
	virtual void	DestroyCallback();
	
	virtual void	Update( float timeStep );
	virtual void	Draw( vsDisplayList *list );
};

#endif // PHYS_SPRITE_H
