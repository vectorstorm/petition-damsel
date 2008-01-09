/*
 *  SST_Fireworks.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 1/05/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef SST_FIREWORKS_H
#define SST_FIREWORKS_H

#include "VS_Color.h"
#include "VS_Entity.h"
#include "VS_Vector.h"

class sstFireworkParticle
{
public:
	sstFireworkParticle();
	
	vsVector2D	m_position;
	vsVector2D	m_velocity;
	vsColor		m_color;
	
	float		m_life;			// how long have I been alive?
	float		m_lifeTime;		// what's my full lifetime?
	
	sstFireworkParticle *	m_next;
	sstFireworkParticle *	m_prev;

	void			Append( sstFireworkParticle *other );
	void			Extract();
};

class sstFireworks : public vsEntity
{
	typedef vsEntity Parent;
	
	sstFireworkParticle	*m_usedList;
	sstFireworkParticle *m_unusedList;
	
	sstFireworkParticle *m_particle;
	int					m_particleCount;
	
	void				Spawn(const vsVector2D &pos, const vsVector2D &vel, const vsColor &color, float lifeTime);

public:
	
	sstFireworks( int count );
	virtual ~sstFireworks();
	
	virtual void	Update( float timeStep );
	virtual void	Draw(vsDisplayList *list);

	void			SpawnImmediate( int spawnCount, const vsVector2D &pos, const vsVector2D &vel, float speed, float radius = 0.f, const vsColor &color = vsColor::White, float lifetime = 1.0f );
	void			SpawnImmediateBurst( int spawnCount, const vsVector2D &pos, const vsVector2D &vel, float speed, float radius = 0.f, const vsColor &color = vsColor::White, float lifetime = 1.0f );
};


#endif // SST_FIREWORKS_H

