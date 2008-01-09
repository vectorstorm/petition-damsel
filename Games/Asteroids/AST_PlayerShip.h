/*
 *  AST_PlayerShip.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 25/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef AST_PLAYERSHIP_H
#define AST_PLAYERSHIP_H

#include "PHYS_Sprite.h"

class astPlayerShot;
class physEmitter;

class astPlayerShip : public physSprite
{
	typedef physSprite Parent;
	static const int	c_maxShots = 10;
	
	astPlayerShot	*m_shotList[c_maxShots];
	
	physEmitter		*m_emitter;
	
	vsSprite		*m_thrust;
	vsSprite		*m_leftAttitude;
	vsSprite		*m_rightAttitude;
	vsSprite		*m_leftRearAttitude;
	vsSprite		*m_rightRearAttitude;
	
	float			m_delayBeforeSpawn;
	float			m_timeSinceSpawn;
	bool			m_isSpawned;
	bool			m_spawnInvulnerable;
	
	enum{
		Thruster_Main,
		Thruster_Left,
		Thruster_Right,
		Thruster_MAX
	};
	
	float			m_thrustAmt[Thruster_MAX];
	
	void			HandleThrusters( float timeStep );
	void			HandleSpawnTimer( float timeStep );
	
public:
		astPlayerShip(physEmitter *thrustEmitter);
	virtual ~astPlayerShip();
	
	void			Spawn(const vsVector2D &position);
	void			Despawn();
	bool			AttemptSpawn( float timeStep, const vsVector2D &position);
	bool			IsSpawned() { return m_isSpawned; }
	
	virtual void	Update( float timeStep );
	virtual bool	CollisionCallback( const colEvent &event );
	virtual void	DestroyCallback();
};

#endif // AST_PLAYERSHIP_H
