/*
 *  DA_Hud.h
 *  Damsel
 *
 *  Created by Trevor Powell on 10/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_HUD_H
#define DA_HUD_H

#include "VS_Entity.h"

class daModeInGame;
class vsDisplayList;

class daHud : public vsEntity
{
	daModeInGame *		m_gameMode;
	vsDisplayList *		m_signatures;
	vsDisplayList *		m_timeRemaining;
	vsDisplayList *		m_scoreList;
	vsDisplayList *		m_timeList;
	vsDisplayList *		m_petitionList;
	
	int					m_score;		// this is the score that we've built into 'm_scoreList';  we don't need to rebuild that display list except when this value changes!
	int					m_seconds;
	
	float				m_opacity;
	float				m_fadingFrom;
	float				m_fadingTo;
	float				m_fadeTimer;
	bool				m_fading;
	
	void			BuildScoreLists();
	
	void			FadeTo(float opacity) { m_fading = true; m_fadingFrom = m_opacity; m_fadingTo = opacity; m_fadeTimer = 0.f; }
	
protected:
	virtual void	_Draw( vsDisplayList *list );
public:
	
	daHud( daModeInGame *mode );
	virtual ~daHud();
	
	virtual void	Update(float timeStep);
	
	void			FadeIn() { FadeTo(1.0f); }
	void			FadeOut() { FadeTo(0.0f); }
	
};

#endif // DA_HUD_H

