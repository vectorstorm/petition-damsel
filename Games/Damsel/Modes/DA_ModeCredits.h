/*
 *  DA_ModeCredits.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_MODECREDITS_H
#define DA_MODECREDITS_H

#include "CORE_GameMode.h"

class daGame;
class vsSprite;
class fsFile;

#define MAX_CREDIT_LINES (25)

class daModeCredits : public coreGameMode
{
	daGame *	m_game;
	fsFile *	m_credits;
	
	vsSprite *	m_line[MAX_CREDIT_LINES];
	int			m_lineId[MAX_CREDIT_LINES];
	bool		m_lineUsed[MAX_CREDIT_LINES];
	int			m_lineCount;
	bool		m_creditsDone;
	
	bool		m_fadingOut;
	bool		m_inittedFade;
	float		m_fadingTimer;
	
	
	float		m_creditsPos;
	
	void		LoadLine( int i );	// load a line into slot 'i'.
	
public:
	
	daModeCredits( daGame *game );
	virtual ~daModeCredits();
	
	virtual void Init();
	virtual void Deinit();
	
	virtual void Update(float timeStep);
};


#endif // DA_MODECREDITS_H
