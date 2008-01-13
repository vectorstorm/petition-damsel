/*
 *  DA_LevelBackground.h
 *  Damsel
 *
 *  Created by Trevor Powell on 11/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_LEVELBACKGROUND_H
#define DA_LEVELBACKGROUND_H

#include "VS_Sprite.h"

class daLevelBackground : public vsSprite
{
#define MAX_GRASS (10)
	vsSprite *		m_grass[MAX_GRASS];
	vsSprite *		m_roadMarkings;
	
	float			m_fadeTimer;
	float			m_fadeTime;		// over how long are we supposed to fade?
	bool			m_fadingIn;
public:
	
					daLevelBackground();
	virtual			~daLevelBackground();
	
	virtual void	Update(float timeStep);
	
	void			FadeIn(float fadeTime = 1.0f);
	void			FadeOut(float fadeTime = 1.0f);
	
};

#endif // DA_LEVELBACKGROUND_H
