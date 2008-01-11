/*
 *  DA_Hud.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 10/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_Hud.h"

#include "DA_ModeInGame.h"

#include "VS_Color.h"
#include "VS_DisplayList.h"
#include "VS_Font.h"

// HUD is actually an acronym for 'Heads Up Display'.  But I can't be
// fussed to hold down the shift key for that long, so this class is
// now and forever known as 'daHud'.

daHud::daHud( daModeInGame *mode ):
m_gameMode(mode),
m_score(-1),
m_highScore(-1),
m_scoreList(NULL),
m_highScoreList(NULL),
m_opacity(0.f),
m_fadingFrom(0.f),
m_fadingTo(0.f),
m_fading(false)
{
//	m_playerShip = vsDisplayList::Load("PlayerShip.vec");
	
	m_petitionList = vsDisplayList::Load("Petition.vec");
	
	BuildScoreLists();
}

daHud::~daHud()
{
	delete m_petitionList;
	if ( m_scoreList )
		delete m_scoreList;
	if ( m_highScoreList )
		delete m_highScoreList;
}

void
daHud::BuildScoreLists()
{
	int newScore = m_gameMode->GetScore();
	int newHighScore = m_gameMode->GetHighScore();
	
	if ( !m_scoreList || newScore != m_score )
	{
		if ( m_scoreList )
			delete m_scoreList;
		
		vsString scoreString = vsFormatString("%08d", newScore);
		m_scoreList = vsBuiltInFont::CreateString( scoreString, 30.0f );
		m_score = newScore;
	}
	if ( !m_highScoreList || newHighScore != m_highScore )
	{
		if ( m_highScoreList )
			delete m_highScoreList;
		
		vsString highscoreString = vsFormatString("%08d", newHighScore);
		m_highScoreList = vsBuiltInFont::CreateString( highscoreString, 20.0f );
		m_highScore = newHighScore;
	}
}

void
daHud::_Draw( vsDisplayList *list )
{
	// by default, our viewport extends -500 -> 500 vertically, and something more than that horizontally (based on aspect ratio)
	
	BuildScoreLists();
	
	vsTransform t = vsTransform::Zero;
	t.m_position.Set(-460.f, -400.f);
	t.m_scale.Set(2.0f,2.0f);
	
	list->SetColor( vsColor::Yellow * m_opacity * m_opacity );	// ease in, slam out
	
	int petitionsRemaining = m_gameMode->GetPetitionsInHand();
	if ( petitionsRemaining > 5 )
		petitionsRemaining = 5;	// only draw up to five icons, max.
	for ( int i = 0; i < petitionsRemaining; i++ )	// draw each of our player icons
	{
		list->PushTransform(t);
		list->Append(*m_petitionList);
		list->PopTransform();
		
		t.m_position += vsVector2D(50.f,0.f);
	}
	
	t.m_scale.Set(1.0f,1.0f);
	
	list->SetColor( vsColor::Blue * m_opacity );
	if ( m_scoreList )
	{
		t.m_position.Set(-520.f, -440.f);
		list->PushTransform(t);
		list->Append(*m_scoreList);
		list->PopTransform();
	}
	
	if ( m_highScoreList )
	{
		t.m_position.Set(-40.f, -440.f);
		list->PushTransform(t);
		list->Append(*m_highScoreList);
		list->PopTransform();
	}
}

void
daHud::Update(float timeStep)
{
	if ( m_fading )
	{
		m_fadeTimer += timeStep;
		
		if ( m_fadeTimer > 1.0f )
		{
			m_fadeTimer = 1.0f;
			m_fading = false;
		}
		
		m_opacity = vsInterpolate( m_fadeTimer, m_fadingFrom, m_fadingTo );
	}
}

