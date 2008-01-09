/*
 *  COL_System.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 23/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef COL_SYSTEM_H
#define COL_SYSTEM_H

#include "CORE_GameSystem.h"
#include "VS_Vector.h"

class colObject;

class colSystem : public coreGameSystem
{
	colObject *		m_objectList;
	colObject *		m_pendingObjectList;
	
	bool			m_inCollisionTest;
	
	bool Test( colObject *a, colObject *b, float timeStep );
	void HandleCollision( colObject *a, colObject *b, float t, float timeStep, bool aVsB, bool bVsA );
	
public:
	
	virtual void	Init();
	virtual void	Deinit();
	
	bool			InCollisionTest() { return m_inCollisionTest; }
	
	void			PostUpdate( float timeStep );
	
	void			RegisterObject( colObject *object );
	
	bool			CollisionTest( const vsVector2D &a, const vsVector2D &b, float r, int testFlags );
};

#endif // COL_SYSTEM_H
