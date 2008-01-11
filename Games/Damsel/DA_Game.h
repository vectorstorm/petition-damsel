/*
 *  DA_Game.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_GAME_H
#define DA_GAME_H

#include "CORE_Game.h"

class coreGameMode;
class sndMusic;

class daGame : public coreGame
{
	typedef coreGame Parent;
public:
	enum GameMode
	{
		Mode_Intro,
		Mode_TitleScreen,
		//Mode_LevelSelect,
		Mode_InGame,
		Mode_Credits,
		MODE_MAX
	};
	enum GameMusic
	{
		Music_Quote,
		Music_Title,
		MUSIC_MAX
	};
private:	
	
	coreGameMode *		m_mode[MODE_MAX];
	
	sndMusic *			m_music[MUSIC_MAX];
	sndMusic *			m_currentMusic;
	
public:
					daGame();
	virtual			~daGame();
	
	virtual void	Init();
	virtual void	Deinit();
	
	virtual void	Update( float timeStep );
	
	void			GoToPreferences();
	
	void			SetMode( GameMode mode );
	void			PlayMusic( GameMusic type );
	void			FadeOutMusic( float time );
};

#endif // DA_GAME_H

