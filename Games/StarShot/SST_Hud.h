/*
 *  BNG_Hud.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef BNG_HUD_H
#define BNG_HUD_H

#include "VS_Entity.h"

class sstModeInGame;
class vsDisplayList;

class sstHud : public vsEntity
{
	sstModeInGame *m_parent;
	
	vsDisplayList *m_playing;
	vsDisplayList *m_recording;
	vsDisplayList *m_starting;
	vsDisplayList *m_ended;
	vsDisplayList *m_intermission;
	
	float			m_intermissionBrightness;
public:
	sstHud( sstModeInGame *game );
	~sstHud();
	
	void	Update(float timeStep);
	void	Draw(vsDisplayList *list);
};

#endif // BNG_HUD_H

