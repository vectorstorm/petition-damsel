/*
 *  DA_ModeTitleScreen.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 9/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_ModeTitleScreen.h"

#include "DA_Game.h"

#include "Core.h"

#include "UT_Menu.h"

#include "VS_Font.h"
#include "VS_Layer.h"
#include "VS_Screen.h"

enum
{
	NewGame,
	HowToPlay,
	Options,
	Credits,
	Quit,
	MENU_MAX
};

static vsVector2D titlePos(600.0f,-380.0f);
static vsVector2D menuPos(-500.0f,100.0f);

static const float c_transitionDuration = 1.0f;

daModeTitleScreen::daModeTitleScreen( daGame *game ):
m_game(game)
{
	m_menu = new utMenu( MENU_MAX, 25.0f, 35.0f, 20.0f );
	m_menu->SetItemLabel(NewGame, "New Game");
	m_menu->SetItemLabel(HowToPlay, "How To Play");
	m_menu->SetItemLabel(Options, "Options");
	m_menu->SetItemLabel(Credits, "Credits");
	m_menu->SetItemLabel(Quit, "Quit");
	
	m_title = new vsSprite( NULL );
	
	vsSprite *timed = new vsSprite(vsBuiltInFont::CreateString(vsString("Ill-Timed"), 30.0f, 80.0f, Justification_Right));
	vsSprite *petition = new vsSprite(vsBuiltInFont::CreateString(vsString("Petition"), 60.0f, 80.0f, Justification_Right));
	vsSprite *damsel = new vsSprite(vsBuiltInFont::CreateString(vsString("Damsel"), 90.0f, 90.0f, Justification_Right));
	
	timed->SetPosition( vsVector2D(0.0f,0.0f) );
	petition->SetPosition( vsVector2D(0.0f,80.0f) );
	damsel->SetPosition( vsVector2D(0.0f,180.0f) );
	
	m_title->AddChild(timed);
	m_title->AddChild(petition);
	m_title->AddChild(damsel);
}

daModeTitleScreen::~daModeTitleScreen()
{
	delete m_title;
	delete m_menu;
}

void
daModeTitleScreen::Init()
{
	m_title->RegisterOnLayer(1);
	
	m_menu->Enter();
	m_menu->RegisterOnLayer(1);
		
	m_transitioningIn = true;
	m_transitioningOut = false;
	m_transitionTimer = 0.f;
	
	titlePos.x = vsScreen::Instance()->GetLayer(1)->GetTopRightCorner().x - 100.f;

	m_iniTitlePos = titlePos;
	m_iniMenuPos = menuPos;
	
	m_iniTitlePos.y = vsScreen::Instance()->GetLayer(1)->GetTopLeftCorner().y - 300.f;
	m_iniMenuPos.x = vsScreen::Instance()->GetLayer(1)->GetTopLeftCorner().x - 400.0f;
	
	m_menu->SetPosition(m_iniMenuPos);
	m_title->SetPosition(m_iniTitlePos);
	m_title->SetColor(vsColor::Black);
	
	//m_game->PlayMusic( daGame::Music_Title );
}

void
daModeTitleScreen::Deinit()
{
	m_title->Extract();
	m_menu->Extract();
	
	m_transitioningIn = false;
	m_transitioningOut = false;
}

void
daModeTitleScreen::Update( float timeStep )
{
	UNUSED(timeStep);
	
	if ( m_transitioningIn || m_transitioningOut )
	{
		
		m_transitionTimer += timeStep;
		
		if ( m_transitionTimer > c_transitionDuration )
		{
			m_transitionTimer = c_transitionDuration;
			if ( m_transitioningOut )
			{
				if ( m_transitioningOutTo == NewGame )
					m_game->SetMode(daGame::Mode_InGame);	// Mode_LevelSelect?  Are levels appropriate for this game?
				if ( m_transitioningOutTo == Options )
					m_game->GoToPreferences();
				if ( m_transitioningOutTo == Credits )
					m_game->SetMode(daGame::Mode_Credits);
//				else if ( m_transitioningOutTo == HowToPlay )
//					m_game->SetMode(daGame::Mode_Tutorial);
			}
			m_transitioningIn = false;
			m_transitioningOut = false;
		}
		
		float timer = m_transitionTimer;
		if ( m_transitioningOut )
			timer = c_transitionDuration - m_transitionTimer;
		
		float transitionFraction = timer / c_transitionDuration;
		transitionFraction = (3.0f * transitionFraction * transitionFraction) - (2.0f * transitionFraction * transitionFraction * transitionFraction);
		
		m_menu->SetPosition( vsInterpolate(transitionFraction, m_iniMenuPos, menuPos) );
		m_title->SetPosition( vsInterpolate(transitionFraction, m_iniTitlePos, titlePos) );
		m_title->SetColor( vsInterpolate(transitionFraction, vsColor::Black, vsColor::Red) );
	}
	
	if ( !m_transitioningIn && !m_transitioningOut )
	{
		if ( m_menu->WasActionTaken() )
		{
			utMenuAction a = m_menu->GetAction();
			
			if ( a.type == utMenuAction::Type_Select )
			{
				switch( a.menuItemId )
				{
					case NewGame:
						m_transitioningOut = true;
						m_transitionTimer = 0.f;
						m_transitioningOutTo = NewGame;
						break;
					case HowToPlay:
						m_transitioningOut = true;
						m_transitionTimer = 0.f;
						m_transitioningOutTo = HowToPlay;
						//m_game->GetHowToPlayScreen()->Show(true);
						m_game->FadeOutMusic(c_transitionDuration);
						break;
					case Options:
						m_transitioningOut = true;
						m_transitionTimer = 0.f;
						m_transitioningOutTo = Options;
						m_game->FadeOutMusic(c_transitionDuration);
						break;
					case Credits:
						m_transitioningOut = true;
						m_transitionTimer = 0.f;
						m_transitioningOutTo = Credits;
						m_game->FadeOutMusic(c_transitionDuration);
						break;
					case Quit:
						core::SetExitToMenu();
						break;
					default:
						break;
				}
			}
		}
	}
}


