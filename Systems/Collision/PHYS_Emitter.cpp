/*
 *  PHYS_Emitter.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "PHYS_Emitter.h"
#include "PHYS_Sprite.h"

#include "VS_DisplayList.h"
#include "VS_Layer.h"
#include "VS_Random.h"
#include "VS_Screen.h"

physEmitter::physEmitter(const vsString &filename, float mass, int maxParticleCount)
{
	m_baseList = vsDisplayList::Load(filename);
	
	m_particle = new physSprite *[maxParticleCount];
	m_particleInUse = new bool[maxParticleCount];
	m_particleLife = new float[maxParticleCount];
	m_particleColor = new vsColor[maxParticleCount];
	m_particleCount = maxParticleCount;
	
	for ( int i = 0; i < m_particleCount; i++ )
	{
		m_particle[i] = new physSprite( /*vsDisplayList::Load(filename)*/m_baseList->CreateInstance(), ColFlag_Particle, ColFlag_Enemy|ColFlag_Player);
		m_particle[i]->SetMass(mass);
		
		m_particleInUse[i] = false;
		m_particleLife[i] = 0.f;
		m_particleColor[i] = vsColor::White;
	}
	
	m_particleLifetime = 2.0f;
	m_spawnRate = 0;
	m_hose = 0;
	
	m_position = vsVector2D::Zero;
	m_velocity = vsVector2D::Zero;
	m_radius = 0.f;
	m_velRadius = 0.f;
	m_color = vsColor::White;
}

physEmitter::~physEmitter()
{
	for ( int i = 0; i < m_particleCount; i++ )
	{
		delete m_particle[i];
	}
	delete m_baseList;
	delete [] m_particle;
	delete [] m_particleColor;
	delete [] m_particleInUse;
	delete [] m_particleLife;
}

void
physEmitter::Update(float timeStep)
{
	UNUSED(timeStep);
	

	for ( int i = 0; i < m_particleCount; i++ )
	{
		if ( m_particleInUse[i] )
		{
			m_particleLife[i] += timeStep;
			
			if ( m_particleLife[i] > m_particleLifetime )
			{
				m_particle[i]->SetCollisionsActive( false );
				m_particle[i]->Extract();
				
				m_particleLife[i] = 0.f;
				m_particleInUse[i] = false;
			}
			else
			{
				float amt = /*1.0f -*/ (m_particleLife[i] / m_particleLifetime);
				
				vsColor c = m_particleColor[i];
				
				c.a = vsInterpolate( amt, c.a, 0.f );
				
				m_particle[i]->SetColor( c );
			}
		}
	}
	
	
	m_hose += timeStep * m_spawnRate;
	
	while ( m_hose > 1.0f )
	{
		vsVector2D pos = m_position + vsRandom::GetVector2D( m_radius );
		vsVector2D vel = m_velocity + vsRandom::GetVector2D( m_velRadius );
		
		Spawn( pos, vel, m_color );
		
		m_hose -= 1.0f;
	}
}

void
physEmitter::Spawn(const vsVector2D &pos, const vsVector2D &vel, const vsColor &color)
{
	for ( int i = 0; i < m_particleCount; i++ )
	{
		if ( !m_particleInUse[i] )
		{
			m_particleLife[i] = 0.f;
			m_particleInUse[i] = true;
			m_particleColor[i] = color;
			m_particle[i]->SetPosition( pos );
			m_particle[i]->SetVelocity( vel );
			m_particle[i]->SetAngularVelocity( 0.f );
			m_particle[i]->SetColor( color );
			m_particle[i]->Teleport();
			m_particle[i]->SetCollisionsActive( true );

			vsScreen::Instance()->GetLayer(0)->RegisterEntityOnTop( m_particle[i] );
			break;
		}
	}
}

void
physEmitter::SpawnImmediateBurst( int spawnCount, const vsVector2D &pos, float speed, float radius, const vsColor &color )
{
	/*for ( int i = 0; i < spawnCount; i++ )
	{
		vsVector2D offsetFromCenter = vsRandom::GetVector2D( 1.0f );
		vsVector2D position = pos + (radius * offsetFromCenter);
		vsVector2D velocity = offsetFromCenter * speed;
		
		Spawn( position, velocity, color );
	}*/
	
	SpawnImmediateBurst( spawnCount, pos, vsVector2D::Zero, speed, radius, color );
}

void
physEmitter::SpawnImmediateBurst( int spawnCount, const vsVector2D &pos, const vsVector2D &vel, float speed, float radius, const vsColor &color )
{
	for ( int i = 0; i < spawnCount; i++ )
	{
		vsVector2D offsetFromCenter = vsRandom::GetVector2D( 1.0f );
		vsVector2D position = pos + (radius * offsetFromCenter);
		vsVector2D velocity = vel + (offsetFromCenter * speed);
		
		Spawn( position, velocity, color );
	}
}

