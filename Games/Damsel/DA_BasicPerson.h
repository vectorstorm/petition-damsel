/*
 *  DA_BasicPerson.h
 *  Damsel
 *
 *  Created by Trevor Powell on 11/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_BASICPERSON_H
#define DA_BASICPERSON_H

#include "PHYS_Sprite.h"

class daBasicPerson : public physSprite
{
	typedef physSprite Parent;
	
	bool		m_player;
		
#define LIMB_COUNT (2)	// swinging objects, synched to walk cycle
		
	vsSprite *	m_limb[LIMB_COUNT];
		
	float		m_limbSwingCycle;	// [0..1)
public:
		
	daBasicPerson(bool player);
		
	virtual void	Update(float timeStep);
};


#endif // DA_BASICPERSON_H

