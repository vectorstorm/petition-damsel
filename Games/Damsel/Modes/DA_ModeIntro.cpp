/*
 *  DA_ModeIntro.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_ModeIntro.h"

#include "DA_Game.h"

#include "SYS_Input.h"

#include "VS_Font.h"
#include "VS_Sprite.h"

/*		Beauteous damsel, your petition is ill-timed, 
 *		for I am prevented from embarking on any other 
 *		adventure until I have brought the one to which 
 *		I have pledged myself to a successful conclusion.
 *
 *			Cervantes, Don Quixote
 *
 */

daModeIntro::daModeIntro( daGame *game ):
	m_game(game)
{
	vsString fakeWord[FAKE_WORDS] =
	{
		"\"Beauteous ",
		"damsel",
		", your ",
		"petition",
		" is ",
		"ill-timed",
		","
	};

	vsString lineText[INTRO_LINES] = {
//		"\"Beauteous damsel, your petition is ill-timed,",
		"For I am prevented from embarking on any other",
		"adventure until I have brought the one to which I",
		"have pledged myself to a successful conclusion;",
		"- Cervantes, Don Quixote"
	};
	
	vsVector2D fakeLineWordPos( 600.0f, -200.0f );
	for ( int i = FAKE_WORDS-1; i >= 0; i-- )
	{
		float width = vsBuiltInFont::GetStringWidth(fakeWord[i], 20.0f, 25.0f);
		
		m_wordOrigPos[i] = fakeLineWordPos - vsVector2D(width*0.5f,0.0f);
		
		m_word[i] = new vsSprite(vsBuiltInFont::CreateString(fakeWord[i], 20.0f, 25.0f, Justification_Center));
		m_word[i]->SetColor( vsColor::Black );
		m_word[i]->SetPosition(m_wordOrigPos[i]);
		m_word[i]->SetScale(1.0f);
		m_word[i]->RegisterOnLayer(1);
		
		fakeLineWordPos.x -= width;
	}
	
	for ( int i = 0; i < INTRO_LINES; i++ )
	{
		m_line[i] = new vsSprite( vsBuiltInFont::CreateString(lineText[i], 20.0f, 25.0f, Justification_Right) );
		m_line[i]->SetColor( vsColor::Black );
		m_line[i]->RegisterOnLayer(1);
		
		vsVector2D pos( 600.0f, -140.0f + (60.0f * i) );
		
		if ( i == INTRO_LINES-1 )
			pos.y += 60.0f;	// extra whitespace above the attribution.
		
		m_line[i]->SetPosition( pos );
	}
}

daModeIntro::~daModeIntro()
{
	for ( int i = 0; i < INTRO_LINES; i++ )
		vsDelete(m_line[i]);

	for ( int i = 0; i < FAKE_WORDS; i++ )
		vsDelete(m_word[i]);
}

void
daModeIntro::Init()
{
	m_introTimer = 0.f;
	m_game->PlayMusic( daGame::Music_Quote );
	m_state = State_Quote;


	for ( int i = FAKE_WORDS-1; i >= 0; i-- )
	{
		m_word[i]->SetColor( vsColor::Black );
		m_word[i]->SetPosition(m_wordOrigPos[i]);
		m_word[i]->SetScale(1.0f);
		m_word[i]->SetAngle(vsAngle::Zero);
	}
	
	for ( int i = 0; i < INTRO_LINES; i++ )
	{
		m_line[i]->SetColor( vsColor::Black );
	}
}

void
daModeIntro::Deinit()
{
}

void
daModeIntro::Update( float timeStep )
{
	switch( m_state )
	{
		case State_Quote:
			Quote( timeStep );
			break;
		case State_TransitionIn:
			TransitionIn( timeStep );
			break;
		case State_Title:
			Title( timeStep );
			break;
	}
}

void
daModeIntro::Quote( float timeStep )
{
	const float c_timeBetweenLines = 3.0f;
	
	m_introTimer += timeStep;
	
	for ( int i = 0; i < INTRO_LINES+1; i++ )
	{
		float lineTime = m_introTimer - (c_timeBetweenLines * i);
		vsColor c = vsColor::Black;
		
		if ( lineTime >= c_timeBetweenLines )
			c = vsColor::LightBlue;
		else if ( lineTime > 0.f )
			c = vsInterpolate( lineTime / c_timeBetweenLines, vsColor::Black, vsColor::LightBlue );
		
		if ( i == 0 )
		{
			for ( int j = 0; j < FAKE_WORDS; j++ )
				m_word[j]->SetColor(c);
		}
		else
			m_line[i-1]->SetColor(c);
	}
	
	if ( /*m_introTimer >= (INTRO_LINES+1.f) &&*/ m_game->GetInput()->WasPressed(CID_A) )
	{
		m_game->PlayMusic( daGame::Music_Title );
		m_state = State_TransitionIn;
		m_introTimer = 0.f;
	}
}

void
daModeIntro::TransitionIn( float timeStep )
{
	m_introTimer += timeStep;
	
	const float c_startFadeTime = 3.7f;					// when do we start fading the three special words?
	const float c_endFadeTime = c_startFadeTime + 1.f;	// when do the three special words finish fading?
	const float c_fadeOutTime = 3.0f;
	vsColor c = vsColor::Black;
	
	if ( m_introTimer < c_fadeOutTime )
	{
		c = vsInterpolate( m_introTimer / c_fadeOutTime, vsColor::LightBlue, vsColor::Black );
	}
	
	for ( int i = 0; i < INTRO_LINES; i++ )
		m_line[i]->SetColor(c);
	for ( int i = 0; i < FAKE_WORDS; i+=2 )
		m_word[i]->SetColor(c);
	
	// handle title words
	{
		float wordScale = 3.0f;
		if ( m_introTimer < c_fadeOutTime )
		{
			wordScale = vsInterpolate( m_introTimer / c_fadeOutTime, 1.0f, 3.0f );
		}
		
		for ( int i = 1; i < FAKE_WORDS; i+=2 )
			m_word[i]->SetScale(wordScale);
		
		vsVector2D newWordPos(0.0f,0.0f);
		int wordId = 2;
		for ( int i = 1; i < FAKE_WORDS; i+=2 )
		{
			float frac = m_introTimer / c_fadeOutTime;
			m_word[i]->SetPosition( vsInterpolate(frac, m_wordOrigPos[i], newWordPos) );

			c = vsColor::Red;
			
			if ( frac < 1.0f + (0.1f * wordId) )
			{
				if ( frac < 1.0f )
					c = vsInterpolate(frac, vsColor::LightBlue, vsColor::White);
				else
					c = vsColor::White;
			}
			else if ( m_introTimer > c_startFadeTime )
			{
				frac = m_introTimer-c_startFadeTime;
				if ( frac < 1.f )
					c = vsInterpolate(frac, vsColor::Red, vsColor::Black);
				else
				{
					c = vsColor::Black;
					m_game->SetMode( daGame::Mode_TitleScreen );
				}
			}
			m_word[i]->SetColor(c);
			
			
			frac = m_introTimer / c_endFadeTime;
			vsAngle finalAngle(DEGREES(-15.0f));
			frac += wordId * 0.05f;
			
			vsAngle nowAngle = vsAngle::Zero;
			
			if ( frac > 0.f )
				nowAngle = vsInterpolate(frac, vsAngle::Zero, finalAngle);
			m_word[i]->SetAngle(nowAngle);
			
			newWordPos.y -= 100.0f;
			
			wordId--;
		}
	}
}

void
daModeIntro::Title( float timeStep )
{
	UNUSED(timeStep);
}

