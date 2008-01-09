/*
 *  CORE_GameRegistry.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 25/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "CORE_GameRegistry.h"

coreGameEntry *					coreGameRegistry::s_mainMenu = NULL;
coreGameEntry *					coreGameRegistry::s_game[MAX_GAMES];
int								coreGameRegistry::s_gameEntries = 0;

coreGameEntry::coreGameEntry( const vsString & name, coreGame *game )
{
	m_name = name;
	m_game = game;
	
	coreGameRegistry::RegisterGame(this);
}

void
coreGameRegistry::RegisterGame( coreGameEntry *entry )
{
	vsAssert( s_gameEntries < MAX_GAMES, "Too many games registered!" );	// if this assert fires, we need to increase the value of MAX_GAMES
	
	if ( entry->m_name == "Damsel" )	// Damsel is our 'main menu' in this custom build.
	{
		s_mainMenu = entry;
	}
	else
	{
		// insertion sort our entry into place.
		
		int index = 0;	// store which index the game should be sorted into
		
		for ( index = 0; index < s_gameEntries; index++ )
		{
			if ( entry->m_name < s_game[index]->m_name )
				break;
		}
		
		// push all later games further down our list
		for ( int i = s_gameEntries; i > index; i-- )
			s_game[i] = s_game[i-1];
		
		// store this game in its sorted location
		s_game[index] = entry;
		s_gameEntries++;
	}
}

coreGame *
coreGameRegistry::FindGame( const vsString &name )
{
	for ( int i = 0; i < s_gameEntries; i++ )
	{
		if ( name == s_game[i]->m_name )
			return s_game[i]->m_game;
	}
	
	return NULL;
}

coreGame *
coreGameRegistry::GetGame( int i )
{
	vsAssert( i >= 0 && i < s_gameEntries, "Illegal GameID requested!" );
	
	return s_game[i]->m_game;
}

const vsString &
coreGameRegistry::GetGameName( int i )
{
	vsAssert( i >= 0 && i < s_gameEntries, "Illegal GameID requested!" );
	
	return s_game[i]->m_name;
}

const vsString &
coreGameRegistry::GetGameName( coreGame *game )
{
	for ( int i = 0; i < s_gameEntries; i++ )
		if ( s_game[i]->m_game == game )
			return s_game[i]->m_name;
	
	if ( game == s_mainMenu->m_game )
		return s_mainMenu->m_name;
	
	return vsEmptyString;
}
