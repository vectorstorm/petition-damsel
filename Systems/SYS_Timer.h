/*
 *  SYS_FrameRateCap.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef SYS_TIMER_H
#define SYS_TIMER_H

#include "CORE_GameSystem.h"
#include "MIX_Singleton.h"

#include "VS_Entity.h"


class sysTimerEntity : public vsEntity
{
public:
	
	virtual void Draw( vsDisplayList *list );
};

class sysTimer : public coreGameSystem, public mixSingleton<sysTimer>
{
	unsigned long		m_startCpu;
	unsigned long		m_startRender;
	unsigned long		m_startGpu;
	unsigned int		m_missedFrames;
	
	unsigned long		m_gpuTime;
	unsigned long		m_renderTime;
	unsigned long		m_cpuTime;

#if defined(_DEBUG)
	sysTimerEntity *	m_entity;
#endif // _DEBUG
	
	bool				m_firstFrame;
	
public:
		
		sysTimer();
	
	virtual void		Init();
	virtual void		Deinit();
	
	unsigned long		GetMicroseconds();
	
	virtual void		Update( float timeStep );
	virtual void		PostUpdate(float timeStep);
	virtual void		EndRenderTime();	// we've finished processing our display lists
	virtual void		EndGPUTime();		// OpenGL has returned control to our app
	
	unsigned long		GetCurrentMillis() { return m_startCpu / 1000; }
	unsigned int		GetMissedFrameCount() { return m_missedFrames / 1000; }
	
	unsigned long		GetGPUTime() { return m_gpuTime / 1000; }
	unsigned long		GetRenderTime() { return m_renderTime / 1000; }
	unsigned long		GetCPUTime() { return m_cpuTime / 1000; }
};

#endif // SYS_FRAMERATECAP_H
