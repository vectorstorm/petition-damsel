/*
 *  DA_ModeTutorial.h
 *  Damsel
 *
 *  Created by Trevor Powell on 13/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_MODETUTORIAL_H
#define DA_MODETUTORIAL_H

#include "CORE_GameMode.h"

class daGame;
class vsSprite;
class vsDisplayList;

#define MAX_PAGES (5)

class daModeTutorial : public coreGameMode
{
	daGame *		m_game;
	
	vsSprite *		m_title;
	vsSprite *		m_page[MAX_PAGES];
	
	vsSprite *		m_player;
	vsSprite *		m_cop;
	vsSprite *		m_pedestrian;
	vsSprite *		m_car;
	vsSprite *		m_petition;
	
	vsSprite *		m_back;
	
	vsSprite *		m_footer;
	
	int				m_currentPage;
	
	float			m_transitionTimer;
	
	bool			m_transitioningIn;
	bool			m_transitioningOut;
	
public:
	daModeTutorial( daGame *game );
	virtual		~daModeTutorial();
	
	virtual void Init();
	virtual void Deinit();
	
	void		ShowPage(int pageId);
	
	virtual void Update( float timeStep );
	
};

#endif // DA_MODETUTORIAL_H

