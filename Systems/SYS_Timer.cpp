/*
 *  SYS_FrameRateCap.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "SYS_Timer.h"
#include "CORE_Game.h"
#include "Core.h"

#include "VS_DisplayList.h"
#include "VS_Layer.h"
#include "VS_Screen.h"

#include <SDL/SDL.h>

#define TICK_INTERVAL (1)	// 16 ticks per frame;  slightly over 60fps


#if defined(_WIN32)

#include <windows.h>

static LARGE_INTEGER g_liFrequency;
static LARGE_INTEGER g_liCurrent;

#else

#include <sys/time.h>	

#endif

void
sysTimerEntity::Draw( vsDisplayList *list )
{
	// TODO:  Fix, so we're not casting to floats any more;  should be able to do this entirely with
	// integers.  (Used to use float timing, but now it's done with integers;  need to update the
	// sysTimerEntity to render using the new integer method)
	float offsetPerMilli = 0.01f;
	vsVector2D startPoint(0.1f, 0.9f);
	vsVector2D cpuPos(offsetPerMilli, 0.0f);
	cpuPos *= (float)sysTimer::Instance()->GetCPUTime();
	cpuPos += startPoint;
	vsVector2D renderPos(offsetPerMilli, 0.0f);
	renderPos *= (float)sysTimer::Instance()->GetRenderTime();
	renderPos += cpuPos;
	vsVector2D gpuPos(offsetPerMilli, 0.0f);
	gpuPos *= (float)sysTimer::Instance()->GetGPUTime();
	gpuPos += renderPos;

	list->SetColor( vsColor::Blue );
	list->MoveTo( startPoint );
	list->LineTo( cpuPos );

	list->SetColor( vsColor::Green );
	list->MoveTo( cpuPos );
	list->LineTo( renderPos );
	
	list->SetColor( vsColor::Red );
	list->MoveTo( renderPos );
	list->LineTo( gpuPos );
	
	vsVector2D sixtyUp( offsetPerMilli * 16.666f, -0.02f );
	vsVector2D thirtyUp( offsetPerMilli * 33.333f, -0.02f );

	vsVector2D sixtyDown( sixtyUp.x, 0.02f );
	vsVector2D thirtyDown( thirtyUp.x, 0.02f );
	
	list->SetColor( vsColor::Blue );
	list->MoveTo( sixtyUp + startPoint );
	list->LineTo( sixtyDown + startPoint );
	
	list->MoveTo( thirtyUp + startPoint );
	list->LineTo( thirtyDown + startPoint );
}

sysTimer::sysTimer()
{
#if defined(_DEBUG)
	m_entity = NULL;
#endif // _DEBUG
}

void
sysTimer::Init()
{
#if defined(_WIN32)
   if (!QueryPerformanceFrequency(&g_liFrequency))
      printf("QPF() failed with error %d\n", GetLastError());
#endif

//	m_startCpu = SDL_GetTicks();
//	m_startGpu = SDL_GetTicks();
	m_startCpu = GetMicroseconds();
	m_startRender = GetMicroseconds();
	m_startGpu = GetMicroseconds();
	m_missedFrames = 0;
	m_firstFrame = true;
	
#if defined(_DEBUG)
	if ( !m_entity )	// we get 'initted' multiple times;  make sure we don't re-allocate this!
	{
		m_entity = new sysTimerEntity;
		vsScreen::Instance()->GetDebugLayer()->RegisterEntityOnTop( m_entity );
	}
#endif // _DEBUG
}

void
sysTimer::Deinit()
{
#if defined(_DEBUG)
	vsDelete(m_entity);
#endif // _DEBUG
}

unsigned long
sysTimer::GetMicroseconds()
{
#if defined(_WIN32)
	if ( !QueryPerformanceCounter(&g_liCurrent) )
         printf("QPC() failed with error %d\n", GetLastError());

	return (g_liCurrent.QuadPart * 1000000) / g_liFrequency.QuadPart;
#else	// !_WIN32
	struct timeval time;
	gettimeofday(&time, NULL);
	
	return (time.tv_sec * 1000000) + (time.tv_usec);
#endif	// !_WIN32
}

#define MAX_TIME_PER_FRAME (2.0f/60.0f)		// 60fps.
#define MIN_TIME_PER_FRAME (1.0f/60.0f)

void
sysTimer::Update( float timeStep )
{	
	UNUSED(timeStep);

//	unsigned long now = SDL_GetTicks();
	unsigned long now = GetMicroseconds();
	
	unsigned long roundTime = now - m_startCpu;
	
	if ( roundTime > 100000 )	// probably hit a breakpoint or something
		roundTime = m_startCpu = now - 16000;
	
	/*while ( now < m_nextTick )
	{
		//printf("Delaying %d ticks.\n", nextTicks-now);
		SDL_Delay(m_nextTick-now);
		now = SDL_GetTicks();
	}*/
	
	if ( roundTime < 1000 )
	{
		// less than a millisecond between frames?  Looks like we have a driver that's refusing to wait for a vsync!
	}
	
	float actualTimeStep = (roundTime) / 1000000.0f;
	if ( m_firstFrame )	// first frame.
	{
		actualTimeStep = MIN_TIME_PER_FRAME;
		m_firstFrame = false;
	}
	if ( actualTimeStep > MAX_TIME_PER_FRAME )
	{
		actualTimeStep = MAX_TIME_PER_FRAME;
		m_missedFrames++;
	}
	
	core::GetGame()->SetTimeStep( actualTimeStep );
	
	m_startCpu = now;
}

void
sysTimer::EndRenderTime()
{
	//	unsigned long now = SDL_GetTicks();
	unsigned long now = GetMicroseconds();
	m_renderTime = (now - m_startRender);
	m_cpuTime = (m_startRender - m_startCpu);
	m_startGpu = now;
}
	
	void
sysTimer::EndGPUTime()
{
//	unsigned long now = SDL_GetTicks();
	unsigned long now = GetMicroseconds();
	m_gpuTime = (now - m_startGpu);
}

void
sysTimer::PostUpdate( float timeStep )
{
	UNUSED(timeStep);
//	unsigned long now = SDL_GetTicks();
	unsigned long now = GetMicroseconds();
	
	m_startRender = now;
}
