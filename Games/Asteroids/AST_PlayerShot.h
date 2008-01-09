/*
 *  AST_PlayerShot.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 14/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef AST_PLAYERSHOT_H
#define AST_PLAYERSHOT_H

#include "COL_Sprite.h"

class astPlayerShot : public colSprite
{
	typedef colSprite Parent;
	
	vsVector2D		m_velocity;
	float			m_timer;

	bool m_spawned;
	
public:
	astPlayerShot();
	
	void Spawn( const vsVector2D & where, const vsVector2D & velocity );
	void Despawn();
	
	bool IsSpawned() { return m_spawned; }
	
	virtual void Update( float timeStep );
	virtual bool	CollisionCallback( const colEvent &event );
	virtual void	DestroyCallback();
};

#endif // AST_PLAYERSHOT_H
