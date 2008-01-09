/*
 *  DA_ModeIntro.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_MODEINTRO_H
#define DA_MODEINTRO_H

#include "CORE_GameMode.h"

#include "VS_Vector.h"

class vsSprite;
class daGame;

class daModeIntro : public coreGameMode
{
#define INTRO_LINES (4)
#define FAKE_WORDS (7)
	daGame *		m_game;
	
	vsSprite *		m_line[INTRO_LINES];
	vsSprite *		m_word[FAKE_WORDS];
	vsVector2D		m_wordOrigPos[FAKE_WORDS];
	
	float			m_introTimer;
	
	enum State
	{
		State_Quote,
		State_TransitionIn,
		State_Title
	};
	State			m_state;
	
public:
	
					daModeIntro( daGame *game );
	virtual			~daModeIntro();
	
	virtual void	Init();
	virtual void	Deinit();
	
	virtual void	Update(float timeStep);
	
	virtual void	Quote(float timeStep);
	virtual void	TransitionIn(float timeStep);
	virtual void	Title(float timeStep);
};

#endif // DA_MODEINTRO_H

