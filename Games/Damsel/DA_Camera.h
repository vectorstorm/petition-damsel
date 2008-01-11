/*
 *  DA_Camera.h
 *  Damsel
 *
 *  Created by Trevor Powell on 11/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_CAMERA_H
#define DA_CAMERA_H

#include "VS_Camera.h"

class vsSprite;

class daCamera : public vsCamera
{
	vsSprite *	m_followSprite;
public:
				daCamera();
	virtual		~daCamera();
	
	void		Init() { m_followSprite = NULL; }
	void		Deinit() { m_followSprite = NULL; }
	
	virtual void		Update( float timeStep );
	
	void		FollowSprite( vsSprite *sprite ) { m_followSprite = sprite; }
};

#endif // DA_CAMERA_H
