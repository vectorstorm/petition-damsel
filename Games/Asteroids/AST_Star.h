/*
 *  AST_Star.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 10/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef AST_STAR_H
#define AST_STAR_H

#include "VS_Sprite.h"

class vsCamera;

class astStar : public vsSprite
{
	vsCamera *m_camera;
	
public:
	
	astStar( vsCamera *camera );
	
	virtual void	Update(float timeStep);
};

#endif // AST_STAR_H
