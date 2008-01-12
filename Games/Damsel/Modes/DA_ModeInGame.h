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
class daGame;
class daLevelBackground;
class daPlayer;
class daPedestrian;
class daPetition;
class daHud;

#define MAX_PEDESTRIANS (50)
#define MAX_CARS		(10)
#define MAX_PETITIONS   (5)

class daModeInGame : public coreGameMode
{
	daGame *				m_game;
	daPlayer *				m_player;
	daPedestrian *			m_pedestrian[MAX_PEDESTRIANS];
	daCar *					m_car[MAX_CARS];
	daCamera *				m_camera;

	daPetition *		m_petition[MAX_PETITIONS];

	daLevelBackground *		m_background;
	daHud *					m_hud;
	
	int						m_score;
	int						m_highScore;
	
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
	
	virtual void	Update(float timeStep);
	
	
	daPetition *		FindAvailablePetition( const vsVector2D &where );
};

#endif // DA_MODEINGAME_H

