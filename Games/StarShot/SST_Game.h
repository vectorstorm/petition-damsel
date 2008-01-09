/*
 *  BNG_Game.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "CORE_Game.h"
#include "MIX_Singleton.h"

class sstModeInGame;

class sstGame : public coreGame, public mixSingleton<sstGame>
{
	typedef coreGame Parent;
	
	sstModeInGame *m_modeInGame;
	
public:
	
	sstGame();
	
	virtual void Init();
	virtual void Deinit();
	
	void	Update(float timeStep);
};

