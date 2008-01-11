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
class daGame;
class daLevelBackground;
class daPlayer;
class daPedestrian;
class daPetitionPickup;
class daHud;

#define MAX_PEDESTRIANS (50)
#define MAX_PICKUPS     (5)

class daModeInGame : public coreGameMode
{
	daGame *				m_game;
	daPlayer *				m_player;
	daPedestrian *			m_pedestrian[MAX_PEDESTRIANS];
	daCamera *				m_camera;

	daPetitionPickup *		m_pickup[MAX_PICKUPS];

	daLevelBackground *		m_background;
	daHud *					m_hud;
	
	int						m_score;
	int						m_highScore;
	int						m_petitionsInHand;
	
	float					m_petitionSpawnTimer;
	float					m_pedestrianSpawnTimer;
	
	void			AttemptToSpawnPetition();
	void			SpawnPedestrian();
	
public:
					daModeInGame( daGame *game );
	virtual			~daModeInGame();
	
	virtual void	Init();
	virtual void	Deinit();
	
	int				GetScore() { return m_score; }
	int				GetHighScore() { return m_highScore; }
	int				GetPetitionsInHand() { return m_petitionsInHand; }
	
	virtual void	Update(float timeStep);
	
	
	daPetitionPickup *		FindAvailablePetition( const vsVector2D &where );
};

#endif // DA_MODEINGAME_H

