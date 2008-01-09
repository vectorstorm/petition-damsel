/*
 *  AST_Camera.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 25/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef AST_CAMERA_H
#define AST_CAMERA_H

#include "VS_Camera.h"

class vsSprite;

class astCamera : public vsCamera
{
	vsSprite *		m_trackSprite;
	
public:
	astCamera();
	virtual ~astCamera();
	
	void			SetTrackSprite( vsSprite *track ) { m_trackSprite = track; }
	
	virtual void	Update( float timeStep );
};


#endif // AST_CAMERA_H
