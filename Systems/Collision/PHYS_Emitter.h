/*
 *  PHYS_Emitter.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef PHYS_EMITTER_H
#define PHYS_EMITTER_H

#include "VS_Color.h"
#include "VS_Vector.h"

class physSprite;
class vsDisplayList;

class physEmitter
{
	vsDisplayList *	m_baseList;
	
	physSprite **	m_particle;
	vsColor *		m_particleColor;
	bool *			m_particleInUse;
	float *			m_particleLife;
	
	int				m_particleCount;
	
	vsVector2D		m_position;
	float			m_radius;
	
	vsColor			m_color;
	vsVector2D		m_velocity;
	float			m_velRadius;

	float			m_spawnRate;
	float			m_hose;
	
	float			m_particleLifetime;
	
	void			Spawn(const vsVector2D &pos, const vsVector2D &vel, const vsColor &color = vsColor::White);
	
public:
	
	physEmitter(const vsString &filename, float mass, int maxParticleCount);
	~physEmitter();
	
	void	SetSpawnPosition( const vsVector2D &pos, float radius = 0.f ) { m_position = pos; m_radius = radius; }
	void	SetSpawnVelocity( const vsVector2D &vel, float radius = 0.f ) { m_velocity = vel; m_velRadius = radius; }
	void	SetSpawnColor( const vsColor &c ) { m_color = c; }
	
	void	SetSpawnRate( float particlesPerSecond ) { m_spawnRate = particlesPerSecond; }
	void	SpawnImmediateBurst( int spawnCount, const vsVector2D &pos, const vsVector2D &vel, float speed, float radius = 0.f, const vsColor &color = vsColor::White );
	void	SpawnImmediateBurst( int spawnCount, const vsVector2D &pos, float speed, float radius = 0.f, const vsColor &color = vsColor::White );
	void	Update( float timeStep );
};

#endif // PHYS_EMITTER_H
