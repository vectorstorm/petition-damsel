/*
 *  DA_PetitionPickup.h
 *  Damsel
 *
 *  Created by Trevor Powell on 11/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_PETITIONPICKUP_H
#define DA_PETITIONPICKUP_H

#include "VS_Sprite.h"

class daPetitionPickup : public vsSprite
{
	enum State
	{
		Pickup,		// inert pickup;  waiting for player to grab me
		Inventory,	// player has picked me up;  don't currently exist on the game board
		Held,		// being held above player's head right now;  actively attracting people!
		Dropped,	// been dropped by player;  passively attracting people
		Dead		// Been used, not yet been respawned as a pickup.
	};
	
	vsVector2D	*m_verts;
	
	int			m_signatures;
	int			m_maxSignatures;
	
	State		m_state;
	
	virtual void _Draw( vsDisplayList *list );
public:
				daPetitionPickup(int maxSignatures);
	virtual		~daPetitionPickup();
	
	bool		AttractsPedestrians() { return (m_state == Held || m_state == Dropped); }
	bool		ActiveAttractsPedestrians() { return (m_state == Held); }
	
	bool		AvailableForSpawn() { return (m_state == Dead); }
	
	void		Spawn(const vsVector2D &where);
};

#endif // DA_PETITIONPICKUP_H
