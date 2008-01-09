/*
 *  Core.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 25/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef CORE_H
#define CORE_H

class coreGame;
class vsPreferences;
class memHeap;

class core
{
	static memHeap *	s_gameHeap;
	static coreGame *	s_game;
	static coreGame *	s_nextGame;

	static vsPreferences * s_preferences;
	
	static bool			s_exit;
	
public:
	
	static void			Init();
	static void			Deinit();
	
	static void			SetGame( coreGame *game );		// this sets the game which will be activated NEXT FRAME.
	static coreGame *	GetGame( );						// get the current game
	static const vsString &		GetGameName( );					// get the name of the current game
	
	static void			Go();
	
	static void			SetExitToMenu();	// exit to the main menu
	static void			SetExit();			// exit program entirely
};

#endif //CORE_H
