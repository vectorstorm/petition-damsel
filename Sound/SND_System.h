/*
 *  SND_System.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 8/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef SND_SYSTEM_H
#define SND_SYSTEM_H

#include "CORE_GameSystem.h"
#include "MIX_Singleton.h"

class sndMusic;
class sndSample;

struct sndDeferredSample
{
	sndSample *	m_sample;
	float		m_fuse;		// how long until the sample gets played?
};

#define MAX_DEFERRED_SAMPLES (10)

class sndSystem : public coreGameSystem, public mixSingleton<sndSystem>
{
	int		m_channelCount;
	int		m_maxChannelsInUse;
	int		m_channelsInUse;
	
	sndDeferredSample	m_deferredSample[MAX_DEFERRED_SAMPLES];
	
public:
	
	static void ChannelFinished( int channel );
	
	sndSystem();
	~sndSystem();
	
	void Init();
	void Deinit();
	
	virtual void	Update( float timeStep );
	
	void	PlayMusic( sndMusic *music );
	void	StopMusic();
	
	void	PlaySound( sndSample *sound );
	void	PlaySoundDeferred( sndSample *sound, float fuse );
	void	CancelDeferredSounds( sndSample *sound );
};

#endif // SND_SYSTEM_H
