/*
 *  SND_Sample.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef SND_SAMPLE_H
#define SND_SAMPLE_H

#include "SDL_mixer.h"

class sndSample
{
	Mix_Chunk *	m_chunk;
	
public:
	
	sndSample( const vsString &filename_in );
	~sndSample();
	
	void		Play();
	void		PlayDeferred( float fuse );
	
	friend class sndSystem;
};

#endif // SND_SAMPLE_H
