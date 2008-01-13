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
m_seconds(-1),
m_scoreList(NULL),
m_timeList(NULL),
m_opacity(0.f),
m_fadingFrom(0.f),
m_fadingTo(0.f),
m_fading(false)
{
//	m_playerShip = vsDisplayList::Load("PlayerShip.vec");
	
	m_petitionList = vsDisplayList::Load("Petition.vec");
	
	m_signatures = vsBuiltInFont::CreateString("Political Points: ", 20.0f, 25.0f, Justification_Right);
	m_timeRemaining = vsBuiltInFont::CreateString("Time left: ", 20.0f, 25.0f, Justification_Right);
	
	BuildScoreLists();
}

daHud::~daHud()
{
	delete m_petitionList;
	delete m_signatures;
	delete m_timeRemaining;
	if ( m_scoreList )
		delete m_scoreList;
	if ( m_timeList )
		delete m_timeList;
}

void
daHud::BuildScoreLists()
{
	int newScore = m_gameMode->GetScore();
	int seconds = (int)m_gameMode->GetTimeLeft();
	
	if ( seconds < 0 )
		seconds = 0;
	
	if ( !m_scoreList || newScore != m_score )
	{
		if ( m_scoreList )
			delete m_scoreList;
		
		vsString scoreString = vsFormatString("%d", newScore);
		m_scoreList = vsBuiltInFont::CreateString( scoreString, 30.0f );
		m_score = newScore;
	}
	if ( !m_timeList || seconds != m_seconds )
	{
		if ( m_timeList )
			delete m_timeList;
		
		int minutes = seconds/60;
		seconds -= minutes*60;
		
		vsString timeString = vsFormatString("%01d:%02d", minutes, seconds);
		m_timeList = vsBuiltInFont::CreateString( timeString, 20.0f );
		m_seconds = seconds;
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
		t.m_position.Set(-100.f, -440.f);
		list->PushTransform(t);
		list->Append(*m_signatures);
		list->Append(*m_scoreList);
		list->PopTransform();
	}
	
	if ( m_timeList )
	{
		t.m_position.Set(450, -440.f);
		list->PushTransform(t);
		list->Append(*m_timeRemaining);
		list->Append(*m_timeList);
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

