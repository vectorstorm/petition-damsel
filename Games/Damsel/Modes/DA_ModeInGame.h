/*
 *  DA_ModeInGame.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_MODEINGAME_H
#define DA_MODEINGAME_H

#include "CORE_GameMode.h"

#include "VS_Vector.h"

class daCamera;
class daCar;
class daCop;
class daGame;
class daLevelBackground;
class daPlayer;
class daPedestrian;
class daPetition;
class daSplat;
class daHud;
class vsSprite;

#define MAX_PEDESTRIANS (50)
#define MAX_CARS		(10)
#define MAX_PETITIONS   (5)
#define MAX_SPLATS		(10)
#define MAX_COPS		(10)

class daModeInGame : public coreGameMode
{
	daGame *				m_game;
	daPlayer *				m_player;
	daPedestrian *			m_pedestrian[MAX_PEDESTRIANS];
	daCar *					m_car[MAX_CARS];
	daSplat *				m_splat[MAX_SPLATS];
	daCop *					m_cop[MAX_COPS];
	daCamera *				m_camera;
	
	int						m_nextSplat;

	daPetition *			m_petition[MAX_PETITIONS];
	
	daLevelBackground *		m_background;
	daHud *					m_hud;
	
	vsSprite *				m_gameOverSprite;
	bool					m_gameOver;
	float					m_gameOverTimer;
	
	int						m_score;
	int						m_highScore;
	
	float					m_timeLimit;
	float					m_respawnTimer;
	
	float					m_carSpawnTimer;
	float					m_petitionSpawnTimer;
	float					m_pedestrianSpawnTimer;
	
	void			AttemptToSpawnPetition();
	void			SpawnPedestrian();
	void			SpawnCar();
	
public:
					daModeInGame( daGame *game );
	virtual			~daModeInGame();
	
	virtual void	Init();
	virtual void	Deinit();
	
	int				GetScore() { return m_score; }
	int				GetHighScore() { return m_highScore; }
	int				GetPetitionsInHand();
	daPetition *	GetPetitionFromInventory();
	
	daPlayer *		GetPlayer() { return m_player; }
	
	void			AddSignature();
	void			Splat( const vsVector2D &where );
	void			SpawnMadCar( const vsVector2D &dest );
	void			SpawnCop( const vsVector2D &target );
	
	void			AddTimeLeft(float amt) { m_timeLimit += amt; }
	float			GetTimeLeft() { return m_timeLimit; }
	
	virtual void	Update(float timeStep);
	
	
	daPetition *		FindAvailablePetition( const vsVector2D &where );
};

#endif // DA_MODEINGAME_H

