/*
 *  SND_Sample.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "SND_Sample.h"
#include "SND_System.h"

#include "FS_File.h"

sndSample::sndSample(const vsString &filename_in)
{
	const vsString &filename = fsFile::GetFullFilename(filename_in);
	m_chunk=Mix_LoadWAV(filename.c_str());
	if(!m_chunk) {
		printf("Mix_LoadWAV(\"%s\"): %s\n", filename.c_str(), Mix_GetError());
		// this might be a critical error...
	}
}

sndSample::~sndSample()
{
	sndSystem::Instance()->CancelDeferredSounds(this);
}

void
sndSample::Play()
{
	sndSystem::Instance()->PlaySound(this);
}

void
sndSample::PlayDeferred(float fuse)
{
	sndSystem::Instance()->PlaySoundDeferred(this, fuse);
}


