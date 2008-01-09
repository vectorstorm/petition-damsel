/*
 *  DA_ModeTitleScreen.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_MODE_TITLESCREEN_H
#define DA_MODE_TITLESCREEN_H

#include "CORE_GameMode.h"

#include "VS_Vector.h"

class daGame;
class utMenu;
class vsSprite;

class daModeTitleScreen : public coreGameMode
{
	daGame *		m_game;
	
	vsSprite *	m_title;
	utMenu *	m_menu;
	
	vsVector2D	m_iniTitlePos;
	vsVector2D	m_iniMenuPos;
	
	float		m_transitionTimer;
	
	bool		m_transitioningIn;
	bool		m_transitioningOut;
	
	int			m_transitioningOutTo;
	
public:
	
				daModeTitleScreen( daGame *game );
	virtual		~daModeTitleScreen();
	
	void Init();
	void Deinit();
	
	void Update( float timeStep );
};

#endif // DA_MODE_TITLESCREEN_H

