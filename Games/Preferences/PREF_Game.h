/*
 *  PREF_Game.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/12/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef PREF_GAME_H
#define PREF_GAME_H

#include "CORE_Game.h"

class utMenu;

class prefGame: public coreGame
{
	typedef coreGame Parent;
	
	enum State
	{
		Top,
		Screen,
		Sound,
		Control
	};
	
	int					m_state;
	
	utMenu *			m_topMenu;
	utMenu *			m_screenMenu;
	utMenu *			m_soundMenu;
	utMenu *			m_controlMenu;
	
	int					m_resolution;
	int					m_resolutionCount;
	bool				m_fullscreen;
	bool				m_bloom;
	
	int					m_effectVolume;
	int					m_musicVolume;
	
	void				ResetValues();		// set local values from the system preferences
	void				SetValues();		// set menu values to match our local values.
	void				UpdateValues();		// save our local values back into the system preferences
	
	vsString			MakeVolumeString( int value );
	
public:
	
	prefGame();
	
	virtual void		Update(float timeStep);
	
	virtual void		Init();
	virtual void		Deinit();
};

#endif // PREF_GAME_H

