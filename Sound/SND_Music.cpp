/*
 *  SND_Music.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "SND_Music.h"
#include "FS_File.h"

#include "SYS_Timer.h"

#include <SDL_mixer.h>

#include <SDL.h>

int music_buffer_pos=-1;
int decrypted_bytes =-1;

sndMusic::sndMusic( const vsString &filename_in, bool looping )
{
	const vsString &filename = fsFile::GetFullFilename(filename_in);
	
	m_music=Mix_LoadMUS(filename.c_str());
	if(!m_music) {
		printf("Mix_LoadMUS(\"%s\"): %s\n", filename.c_str(), Mix_GetError());
		// this might be a critical error...
	}
	m_playing = false;
	m_looping = looping;
}

sndMusic::~sndMusic()
{
	Stop();
	Mix_FreeMusic(m_music);
}

void
sndMusic::Start()
{
	int loops = (m_looping)?-1:1;
	if ( m_playing )
		Mix_HaltMusic();
	if(Mix_PlayMusic(m_music, loops)!=-1) 
	{
		m_playing = true;
		
		Mix_RewindMusic();	// this is stupid, but it's the only way I've found to
		Mix_RewindMusic();	// make SDL_Mixer start playing at a reliable place and time.
		Mix_RewindMusic();	// Why three calls?  I don't know.  But it works, as a hack.  :(
							// (Without this, there was up to a 1 second uncertainty in when music actually started)
		
		m_startTick = sysTimer::Instance()->GetMicroseconds();
	}
	else
		printf("Mix_PlayMusic: %s\n", Mix_GetError()); // well, there's no music, but most games don't break without music, so don't assert.
}

void
sndMusic::Stop()
{
	if ( m_playing )
	{
		Mix_HaltMusic();
		m_playing = false;
	}
}

float
sndMusic::GetTime()
{
	float result = 0.f;
	if ( m_playing )
	{
		unsigned long deltaMicros = sysTimer::Instance()->GetMicroseconds() - m_startTick;
		
		result = deltaMicros / 1000000.0f;
	}
	
	return result;
}

void
sndMusic::Rewind()
{
	if ( m_playing )
	{
		Mix_RewindMusic();
		m_startTick = sysTimer::Instance()->GetMicroseconds();
	}
}

void
sndMusic::GoToTime(float time)
{
	UNUSED(time);
	if ( m_playing )
	{
		Stop();
		//bool isMp3 = (Mix_GetMusicType(m_music) == MUS_MP3);
		//if ( isMp3 )
		//	Rewind();
		//Mix_SetMusicPosition(time);
		int loops = (m_looping)?-1:1;
		Mix_FadeInMusicPos( m_music, loops, 0, time );
		m_playing = true;
	}
}

bool
sndMusic::IsPlaying()
{
	return m_playing;
}

bool
sndMusic::IsActuallyPlaying()
{
	return (m_playing && Mix_PlayingMusic());
}

