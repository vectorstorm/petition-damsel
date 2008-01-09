/*
 *  SND_Music.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef SND_MUSIC_H
#define SND_MUSIC_H

#include "SDL_mixer.h"

class sndMusic
{
	Mix_Music *		m_music;
	
	unsigned long	m_startTick;
	
	bool			m_looping;
	bool			m_playing;
	
public:
	
	sndMusic( const vsString &filename_in, bool looping = true );
	~sndMusic();
	
	void	Start();
	void	Stop();
	
	float	GetTime();	 // how far through this music have we gotten?
	
	void	Rewind();
	void	GoToTime(float time);
	
	bool	IsPlaying();
	bool	IsActuallyPlaying();
	
	friend class sndSystem;
};

#endif // SND_MUSIC_H
