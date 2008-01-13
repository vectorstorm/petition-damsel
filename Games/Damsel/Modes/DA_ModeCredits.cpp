/*
 *  DA_ModeCredits.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_ModeCredits.h"
#include "DA_Game.h"

#include "FS_File.h"

#include "SYS_Input.h"

#include "VS_Font.h"
#include "VS_Layer.h"
#include "VS_Screen.h"
#include "VS_Sprite.h"


daModeCredits::daModeCredits( daGame *game ):
	m_game(game)
{
}

daModeCredits::~daModeCredits()
{
}

void
daModeCredits::Init()
{
	m_credits = new fsFile("Credits.txt");
	for ( int i = 0; i < MAX_CREDIT_LINES; i++ )
	{
		m_line[i] = NULL;
		m_lineUsed[i] = false;
	}
	m_creditsDone = false;
	m_lineCount = 0;
	m_creditsPos = -650.f;
	
	m_fadingOut = false;
	m_fadingTimer = 0.f;
	m_inittedFade = false;
	
	m_game->PlayMusic( daGame::Music_Quote );
}

void
daModeCredits::Deinit()
{
	vsDelete(m_credits);
	
	for ( int i = 0; i < MAX_CREDIT_LINES; i++ )
	{
		if ( m_line[i] )
			vsDelete(m_line[i]);
	}
}

void
daModeCredits::LoadLine( int i )
{
	vsString creditString;
	m_creditsDone = !m_credits->ReadLine(&creditString);
	m_lineUsed[i] = false;
	
	if ( m_line[i] )
		vsDelete(m_line[i]);
	
	if ( !m_creditsDone )
	{
		if ( creditString[0] == '-' )
			m_line[i] = new vsSprite( vsBuiltInFont::CreateString(creditString, 35.0f, 60.0f, Justification_Center) );
		else
			m_line[i] = new vsSprite( vsBuiltInFont::CreateString(creditString, 15.0f, 20.0f, Justification_Center) );
		m_line[i]->SetColor( vsColor(vsColor::LightBlue) );
		m_line[i]->SetPosition( vsVector2D(0.f, -700.f) );
		m_line[i]->RegisterOnLayer(1);
		m_lineUsed[i] = true;
		m_lineId[i] = m_lineCount++;
	}
}

void
daModeCredits::Update( float timeStep )
{
	int activeLines = 0;
	for ( int i = 0; i < MAX_CREDIT_LINES; i++ )
	{
		if ( m_lineUsed[i] )
			activeLines++;
	}
	
	const float c_creditsScrollSpeed = 50.0f;	// 50 'units' per second
	const float c_creditLineSpacing = 50.0f;
	float top = vsScreen::Instance()->GetLayer(1)->GetTopLeftCorner().y - 50.0f;
	
	m_creditsPos += (timeStep * c_creditsScrollSpeed);
	
	for ( int i = 0; i < MAX_CREDIT_LINES; i++ )
	{
		if ( m_lineUsed[i] )
		{
			float yCoord = m_lineId[i] * c_creditLineSpacing;
			yCoord -= m_creditsPos;
			
			if ( m_creditsDone && m_lineId[i] == m_lineCount-1 && yCoord < top )
				m_fadingOut = true;
				
			if ( m_creditsDone && m_lineId[i] == m_lineCount-1 )	// last line
				yCoord = vsMax( yCoord, 0.f );	// last line doesn't go past halfway.
			
			if ( yCoord < top )
				LoadLine(i);
			else
				m_line[i]->SetPosition( vsVector2D(0.f, yCoord) );
		}
		else if ( !m_creditsDone )
			LoadLine(i);
	}
	
	if ( m_game->GetInput()->WasPressed(CID_A) )
		m_fadingOut = true;
	
	if ( m_fadingOut )
	{
		m_fadingTimer += timeStep;
		const float c_fadeDuration = 2.0f;
		float frac = m_fadingTimer / c_fadeDuration;
		vsColor c = vsColor::Black;
		
		if ( !m_inittedFade )
		{
			m_game->FadeOutMusic( c_fadeDuration );
			m_inittedFade = true;
		}

		if ( frac < 1.0f )
		{
			c = vsInterpolate( frac, vsColor::LightBlue, vsColor::Black );

			for ( int i = 0; i < MAX_CREDIT_LINES; i++ )
			{
				if ( m_lineUsed[i] )
				{
					m_line[i]->SetColor(c);
				}
			}
		}
		else
		{
			return m_game->SetMode(daGame::Mode_TitleScreen);
		}
	}
}

