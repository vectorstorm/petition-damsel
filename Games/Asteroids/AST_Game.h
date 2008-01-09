/*
 *  AST_GameMode.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 25/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef AST_GAMEMODE_H
#define AST_GAMEMODE_H

#include "CORE_Game.h"
#include "MIX_Singleton.h"

//class astModeHowTo;
//class astModeInGame;
//class astModeGameOver;
//class astModeTitleScreen;
class astPlayerShip;
class astPlayerShot;
class astAsteroid;
class astAsteroidPool;
class astCamera;
class astHowToPlay;
class astHud;
class astStar;
class colSprite;
class coreGameMode;
class physEmitter;
class vsSprite;
class sndMusic;

class astGame : public coreGame, public mixSingleton<astGame>
{
	typedef coreGame Parent;
	
//	astModeTitleScreen		*m_titleScreen;
//	astModeInGame			*m_inGame;
//	astModeGameOver			*m_gameOver;
//	astModeHowTo			*m_howTo;
	
	enum
	{
		MODE_Title,
		MODE_InGame,
		MODE_GameOver,
		MODE_HowTo,
		MODE_MAX
	};
	
	coreGameMode *			m_mode[MODE_MAX];
	
	astHowToPlay *			m_howToPlayScreen;
	
	astPlayerShip			*m_player;
	astPlayerShot			*m_shot;
//	astAsteroid				**m_asteroid;
//	int						m_asteroidCount;
	astAsteroidPool *		m_asteroids;
	astStar					**m_star;
	int						m_starCount;
	
	physEmitter				*m_sparkEmitter;
	physEmitter				*m_shrapnelEmitter;
	
	astHud					*m_hud;
	astCamera				*m_camera;
	
	sndMusic				*m_music;
	
	int						m_score;
	int						m_highScore;
	int						m_playersLeft;
	
public:
	
	astGame();
		
	virtual void	Init();
	virtual void	Deinit();
	
	virtual void	Update( float timeStep );
	
	void			StartLevel();
	
	void			SetModeInGame();
	void			SetModeGameOver();
	void			SetModeTitleScreen();
	void			SetModeHowTo();
	
	bool			IsPlayerSpawned();
	bool			AttemptPlayerSpawn(float timeStep);
	
	void			ShowHUD(bool show);
	void			ShowAsteroids(bool show);
	astHowToPlay *	GetHowToPlayScreen() { return m_howToPlayScreen; }
	void			RespawnAsteroids();
	
	int				GetScore() { return m_score; }
	int				GetHighScore() { return m_highScore; }
	int				GetPlayerLivesRemaining() { return m_playersLeft; }
	
	physEmitter *	GetShrapnelEmitter() { return m_shrapnelEmitter; }
	
	void			AddScore(int points);
	
};

#endif // AST_GAMEMODE_H
