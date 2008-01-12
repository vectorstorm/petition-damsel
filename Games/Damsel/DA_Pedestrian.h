/*
 *  DA_Pedestrian.h
 *  Damsel
 *
 *  Created by Trevor Powell on 10/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_PEDESTRIAN_H
#define DA_PEDESTRIAN_H

#include "DA_BasicPerson.h"

class daModeInGame;
class daPetition;

class daPedestrian: public daBasicPerson
{
	typedef daBasicPerson Parent;
	
	daModeInGame	*m_mode;
	
	daPetition		*m_focusedPetition;
	float			m_postSigningPetitionInterest;
	bool			m_attemptedSigning;
	
	vsVector2D		m_exit;
	
	vsVector2D		m_target;
	bool			m_homingOnTarget;
	
	bool			m_spawned;
	
	void			PickTarget();
	void			GoToTarget();
	void			WaitAtTarget();
	
public:
	
					daPedestrian( daModeInGame *mode );
	virtual void	Update( float timeStep );
	
	virtual bool	CollisionCallback( const colEvent &collision );
	virtual void	DestroyCallback();

	bool			IsSpawned() { return m_spawned; }
	void			Spawn(const vsVector2D &where_in, const vsVector2D &where_out, float radius_in, bool noOverlap);
	void			Despawn();
};

#endif // DA_PEDESTRIAN_H
