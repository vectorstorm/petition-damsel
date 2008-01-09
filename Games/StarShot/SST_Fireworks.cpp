/*
 *  SST_Fireworks.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 1/05/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "SST_Fireworks.h"

#include "VS_DisplayList.h"
#include "VS_Random.h"

sstFireworks::sstFireworks( int count ):
vsEntity()
{
	m_particle = new sstFireworkParticle[count];
	m_particleCount = count;
	
//	for ( int i = 0; i < count; i++ )
//		m_particle[i].m_used = false;
	
	m_usedList = new sstFireworkParticle;
	m_unusedList = new sstFireworkParticle;

	for ( int i = 0; i < count; i++ )
		m_unusedList->Append(&m_particle[i]);
}

sstFireworks::~sstFireworks()
{
	delete m_usedList;
	delete m_unusedList;
	delete [] m_particle;
	m_particle = NULL;
}

void
sstFireworks::Update( float timeStep )
{
	sstFireworkParticle *p = m_usedList->m_next;
	
	while( p && p != m_usedList )
	{
		sstFireworkParticle *next = p->m_next;
		
		p->m_position += p->m_velocity * timeStep;
		p->m_velocity += vsVector2D(0.0f,30.81f) * timeStep;
		
		p->m_life += timeStep;
		
		if ( p->m_life > p->m_lifeTime )
		{
			p->Extract();
			m_unusedList->Append(p);
		}
		p = next;
	}
}

void
sstFireworks::Draw( vsDisplayList *list )
{
/*	vsVector2D tl(-3.f,-3.f);
	vsVector2D tr(3.f,-3.f);
	vsVector2D bl(-3.f,3.f);
	vsVector2D br(3.f,3.f);*/
	
	vsColor c;
	vsVector2D pos;
	vsVector2D dest;
	
	sstFireworkParticle *p = m_usedList->m_next;
	
	while( p && p != m_usedList )
	{
		float backVelTime = 0.5f;
		if ( p->m_life < backVelTime )
			backVelTime = p->m_life;
		pos = p->m_position;
		dest = pos - (p->m_velocity * backVelTime);
		float lifeFraction = p->m_life / p->m_lifeTime;
		
		c = vsInterpolate( lifeFraction, p->m_color, vsColor(0.f,0.f,0.f,1.0f) );
		
		list->SetColor( c );
		
		list->MoveTo( pos );
		c.a = 0.0f;
		list->SetColor( c );
		list->LineTo( dest );
		
		p = p->m_next;
	}
}

void
sstFireworks::Spawn(const vsVector2D &pos, const vsVector2D &vel, const vsColor &color, float lifeTime)
{
	sstFireworkParticle *p = m_unusedList->m_next;
	
	if ( p && p != m_unusedList )
	{
		p->m_life = 0.f;
		p->m_lifeTime = lifeTime;
		p->m_color = color;
		p->m_position = pos;
		p->m_velocity = vel;
		
		p->Extract();
		m_usedList->Append(p);
	}
}

void
sstFireworks::SpawnImmediate( int spawnCount, const vsVector2D &pos, const vsVector2D &vel, float speed, float radius, const vsColor &color, float lifeTime )
{
	for ( int i = 0; i < spawnCount; i++ )
	{
		vsVector2D offsetFromCenter = vsRandom::GetVector2D( 1.0f );
		vsVector2D position = pos + (radius * offsetFromCenter);
		vsVector2D velocity = vel + (vsRandom::GetVector2D(radius) * speed);
		
		Spawn( position, velocity, color, lifeTime );
	}
}


void
sstFireworks::SpawnImmediateBurst( int spawnCount, const vsVector2D &pos, const vsVector2D &vel, float speed, float radius, const vsColor &color, float lifeTime )
{
	for ( int i = 0; i < spawnCount; i++ )
	{
		vsVector2D offsetFromCenter = vsRandom::GetVector2D( 1.0f );
		vsVector2D position = pos + (radius * offsetFromCenter);
		vsVector2D velocity = vel + (offsetFromCenter * speed);
		
		Spawn( position, velocity, color, lifeTime );
	}
}

sstFireworkParticle::sstFireworkParticle()
{
	m_next = m_prev = this;
}


void
sstFireworkParticle::Append( sstFireworkParticle *other )
{
	other->m_next = m_next;
	other->m_prev = this;
	
	m_next->m_prev = other;
	m_next = other;
}

void
sstFireworkParticle::Extract()
{
	if ( m_prev )
		m_prev->m_next = m_next;
	if ( m_next )
		m_next->m_prev = m_prev;
	
	m_prev = m_next = this;
}


