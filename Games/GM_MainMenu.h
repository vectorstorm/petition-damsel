/*
 *  GM_MainMenu.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef GM_MAINMENU_H
#define GM_MAINMENU_H

#include "CORE_Game.h"

#include "VS_Entity.h"
#include "VS_Sprite.h"

//class vsSprite;
class vsFont;
class sndMusic;
class sndSample;

/*
class gmMainMenuEntity : public vsEntity
{
	vsFont *m_font;
	vsDisplayList *m_cacheString;
	
	virtual void	_Draw( vsDisplayList *list );
	
public:
	
	void Init();
	void Deinit();
};*/

class mmStretchSprite : public vsSprite
{
	float			m_timer;
	
public:
	mmStretchSprite( vsDisplayList *list, float delay );

	virtual void	Update(float timeStep);
};

class gmMainMenu : public coreGame
{
	typedef coreGame Parent;
	
	vsSprite *	m_title;
	vsSprite *	m_letter[11];
	
	vsSprite *	m_build;
	vsSprite *	m_buildName;
	
	vsSprite *	m_stripes[2];
	
	vsSprite **	m_menuSprite;
	
	sndMusic *	m_music;
	sndSample *	m_tickSound;
	
	float		m_pulseTimer;
	
	int			m_menuSpriteCount;
	int			m_selection;
	
	void				BuildTitleSprite();
	
public:
	
				gmMainMenu();
//	virtual		~gmMainMenu();
		
	virtual void		Update(float timeStep);
	
	virtual void		Init();
	virtual void		Deinit();
};

#endif // GM_MAINMENU_H
