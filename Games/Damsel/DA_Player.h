/*
 *  DA_Player.h
 *  Damsel
 *
 *  Created by Trevor Powell on 10/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_PLAYER_H
#define DA_PLAYER_H

#include "DA_BasicPerson.h"

#define MAX_PICKUPS (5)

class daModeInGame;
class daPetition;

class daPlayer : public daBasicPerson
{
	typedef daBasicPerson Parent;
	
	daModeInGame *		m_mode;
	daPetition *	m_petitionHeld;
	
	bool			m_spawned;
	
	void	HandlePetition();
	void	AcquirePetition();

public:
	
	daPlayer( daModeInGame *mode );
	
	virtual void	Update(float timeStep);
	
	virtual bool	CollisionCallback( const colEvent &collision );
	virtual void	DestroyCallback();
	
	void			Die();
	
	bool			IsSpawned();
	bool			TryRespawn();
};

#endif // DA_PLAYER_H

