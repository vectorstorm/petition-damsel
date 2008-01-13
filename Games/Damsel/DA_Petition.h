/*
 *  DA_Petition.h
 *  Damsel
 *
 *  Created by Trevor Powell on 11/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_PETITION_H
#define DA_PETITION_H

#include "VS_Sprite.h"

class daModeInGame;

class daPetition : public vsSprite
{
	enum State
	{
		Pickup,		// inert pickup;  waiting for player to grab me
		Inventory,	// player has picked me up;  don't currently exist on the game board
		Held,		// being held above player's head right now;  actively attracting people!
		Dropping,	// been dropped by player;  passively attracting people
		Dropped,	// been dropped by player;  passively attracting people
		Dead		// Been used, not yet been respawned as a pickup.
	};
	
	daModeInGame *	m_mode;
	vsSprite *	m_player;
	
	vsVector2D	*m_verts;
	
	int			m_signatures;
	int			m_maxSignatures;
	
	float		m_timer;
	bool		m_exploding;
	float		m_explodeTimer;
	float		m_pulseTimer;
	
	vsVector2D	m_dropDestination;
	
	State		m_state;
	
	virtual void _Draw( vsDisplayList *list );
public:
				daPetition(daModeInGame *mode, int maxSignatures);
	virtual		~daPetition();
	
	virtual void	Update( float timeStep );
	
	vsVector2D	GetPositionInLevel();
	
	bool		AttractsPedestrians() { return (m_state == Held || m_state == Dropped); }
	bool		ActiveAttractsPedestrians() { return (m_state == Held); }
	
	bool		AvailableForSpawn() { return (m_state == Dead); }
	bool		AvailableForPickup() { return (m_state == Pickup); }
	bool		InInventory() { return (m_state == Inventory); }
	
	void		Sign();
	
	void		PickedUp();
	void		HeldUp( vsSprite *player );
	void		DroppedAt( vsVector2D pos );
	void		Thrown( vsVector2D source, vsVector2D destination );
	void		Spawn(const vsVector2D &where);
	void		Despawn();
};

#endif // DA_PETITION_H
