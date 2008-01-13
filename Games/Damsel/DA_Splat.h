/*
 *  DA_Splat.h
 *  Damsel
 *
 *  Created by Trevor Powell on 13/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_SPLAT_H
#define DA_SPLAT_H

#include "VS_Sprite.h"

class daSplat : public vsSprite
{
	float m_timer;
public:
	daSplat();
	
	virtual void Update(float timeStep);
	
	void Spawn( const vsVector2D &where );
};

#endif // DA_SPLAT_H
