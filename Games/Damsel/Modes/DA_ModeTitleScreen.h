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
class daLevelBackground;
class utMenu;
class sndSample;
class vsSprite;


class daModeTitleScreen : public coreGameMode
{
	daGame *		m_game;
	
	daLevelBackground *	m_background;
	
	sndSample *	m_menuThump;
	
	vsSprite *	m_title;
	utMenu *	m_menu;
	
	vsVector2D	m_iniTitlePos;
	vsVector2D	m_iniMenuPos;
	
	int			m_menuItemSelected;
	
	float		m_transitionTimer;
	
	bool		m_transitioningIn;
	bool		m_transitioningOut;
	
	int			m_transitioningOutTo;
	
	void		StartTransitionOut();
	
public:
	
				daModeTitleScreen( daGame *game );
	virtual		~daModeTitleScreen();
	
	void Init();
	void Deinit();
	
	void Update( float timeStep );
};

#endif // DA_MODE_TITLESCREEN_H

