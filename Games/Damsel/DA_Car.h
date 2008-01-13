/*
 *  DA_Car.h
 *  Damsel
 *
 *  Created by Trevor Powell on 12/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_CAR_H
#define DA_CAR_H

#include "PHYS_Sprite.h"

class daModeInGame;

class daCar : public physSprite
{
	typedef physSprite Parent;
	
	daModeInGame *	m_mode;
	
	vsVector2D		m_target;
	vsVector2D		m_desiredVelocity;
	
	bool			m_spawned;
	
	void			PickTarget();
	void			GoToTarget();
	void			WaitAtTarget();
	
public:
	
	daCar( daModeInGame *mode );
	virtual void	Update( float timeStep );
	
	bool			IsSpawned() { return m_spawned; }
	void			Spawn(const vsVector2D &where_in, const vsVector2D &where_out, float radius_in, bool noOverlap);
	void			Despawn();
};

#endif // DA_CAR_H
