/*
 *  TB_Game.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/09/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

// The TestBed game is just a simple place for quick testing.  I've been using it as a
// place to do testing of the collision system, so I can quickly set up collision test
// cases and run them over and over again as I fix the collision and physics systems.
//
// The TestBed game only exists in Debug builds of VectorStorm, as it's not a real game.

#ifndef TB_GAME_H
#define TB_GAME_H

#if defined(_DEBUG)

#include "CORE_Game.h"

#include "PHYS_Sprite.h"
#include "PHYS_Emitter.h"

class tbGame : public coreGame
{
	typedef coreGame Parent;

	physSprite *m_a;
	physSprite *m_b;
	
	physEmitter *m_e;

public:
		
		tbGame();
	
	virtual void	Init();
	virtual void	Deinit();
	
	virtual void	Update( float timeStep );
};

#endif // _DEBUG

#endif // TB_GAME_H

