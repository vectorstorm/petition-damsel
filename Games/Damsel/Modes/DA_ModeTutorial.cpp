/*
 *  DA_ModeTutorial.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 13/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_ModeTutorial.h"

#include "DA_Car.h"
#include "DA_Cop.h"
#include "DA_Game.h"
#include "DA_Pedestrian.h"
#include "DA_Petition.h"
#include "DA_Player.h"

#include "SYS_Input.h"

#include "VS_Font.h"
#include "VS_Layer.h"
#include "VS_Screen.h"
#include "VS_Sprite.h"


daModeTutorial::daModeTutorial( daGame *game ):
	m_game(game)
{
}

daModeTutorial::~daModeTutorial()
{
}

void
daModeTutorial::Init()
{
	m_currentPage = 0;
	m_title = new vsSprite(vsBuiltInFont::CreateString("How to Play", 45.f, 55.f, Justification_Center));
	m_title->SetPosition( vsVector2D(0.0f, -400.0f) );
	m_title->RegisterOnLayer(1);
	m_title->SetColor(vsColor::White);
	
	m_back = new vsSprite(vsBuiltInFont::CreateString("Press <LEFT> or <RIGHT> to switch pages.  Press <A> to return", 15.f, 20.f, Justification_Right));
	m_back->SetPosition( vsScreen::Instance()->GetLayer(1)->GetBottomRightCorner() + vsVector2D(-50.f,-50.f) );
	m_back->SetColor(vsColor::Blue);
	m_back->RegisterOnLayer(1);
	
	for ( int i = 0; i < MAX_PAGES; i++ )
	{
		m_page[i] = new vsSprite(NULL);
	}
	
	m_player = new daPlayer(NULL);
	m_player->SetPosition( vsVector2D(-0.0f, -250.0f) );
	m_player->SetColor(vsColor::Red);
	m_page[1]->AddChild(m_player);

	m_pedestrian = new daPedestrian(NULL);
	m_pedestrian->SetPosition( vsVector2D(-0.0f, -250.0f) );
	m_pedestrian->SetColor(vsColor::Blue);
	m_page[2]->AddChild(m_pedestrian);
	
	m_petition = new daPetition(NULL,10);
	m_petition->SetPosition( vsVector2D(-0.0f, -250.0f) );
	m_petition->SetColor(vsColor::Yellow);
	m_page[3]->AddChild(m_petition);
	
	m_cop = new daCop(NULL);
	m_cop->SetPosition( vsVector2D(-0.0f, -250.0f) );
	m_cop->SetColor(vsColor(0.5f,0.0f,0.0f,1.0f));
	m_page[4]->AddChild(m_cop);

	m_car = new daCar(NULL);
	m_car->SetPosition( vsVector2D(-0.0f, -250.0f) );
	m_car->SetAngle(vsAngle(DEGREES(90.0f)));
	m_page[0]->AddChild(m_car);
	
	
	const int c_pageOneLines = 14;
	
	vsString pageOneLines[c_pageOneLines] = {
		"This is you, Petition Damsel.",
		"Your goal is to get people to sign your petitions",
		"to lower the local speed limit, and make the streets",
		"safer.",  
		"",
		"Each signed petition is worth political",
		"points and extra petitioning time!",
		"",
		"Pedestrians who die for your petition are worth",
		"double political points, but no time.",
		"",
		"",
		"",
		"<Page 2/5>"
	};
	
	vsString pageTwoLines[c_pageOneLines] = {
		"This is a pedestrian. He would love to sign your",
		"petition. After he signs your petition he will", 
		"darken and will not sign any more petitions.",
		"",
		"But his tragic death could still be worth extra",
		"political points.",
		"",
		"I mean, just speaking theoretically.",
		"",
		"Because getting someone hurt would be wrong.",
		"",
		"Very, very wrong.",
		"",
		"<Page 3/5>"
	};
	
	vsString pageThreeLines[c_pageOneLines] = {
		"This is a petition. Search the level to find them!",
		"",
		"When you have one, hold the <A> button to hold it",
		"over your head and attract nearby pedestrians.",
		"Release the <A> button to drop or throw it on the",
		"ground.  The green part of the circle shows how many", 
		"signatures it has received.",
		"",
		"Pedestrians are more likely to notice and sign your",
		"petitions while you hold it.  However, bad things",
		"tend to happen when petitions expire, so don't hold",
		"on for too long!",
		"",
		"<Page 4/5>"
	};
	
	vsString pageFourLines[c_pageOneLines] = {
		"This is a cop.  He doesn't like deaths on his beat.",
		"",
		"He also doesn't like political activists, and will",
		"do his best to get rid of you.  Avoid him!",
		"",
		"He could probably be bought off with donuts, if there",
		"were any donuts in this game.",
		"",
		"",
		"Oh well.",
		"",
		"",
		"",
		"<Page 5/5>"
	};
	
	vsString pageFiveLines[c_pageOneLines] = {
		"This is a car.",
		"",
		"These are very dangerous, and zoom along the highway",
		"at reckless speeds, hurting innocent pedestrians,",
		"destroying the environment, and being a general",
		"nuisance.",  
		"",
		"Someone should do something about them.",
		"",
		"And that someone is you.",
		"",
		"(press RIGHT to advance to the next page)",
		"",
		"<Page 1/5>"
	};
	
	const float c_size = 20.0f;
	const float c_capSize = 30.0f;
	
	for ( int i = 0; i < c_pageOneLines; i++ )
	{
		vsSprite *text;
		if ( pageFiveLines[i][0] == '(' )
		{
			text = new vsSprite(vsBuiltInFont::CreateString(pageFiveLines[i], 15.f, 20.f, Justification_Center));
			text->SetColor(vsColor::Blue);
		}
		else
		{
			text = new vsSprite(vsBuiltInFont::CreateString(pageFiveLines[i], c_size, c_capSize, Justification_Center));
			text->SetColor(vsColor::LightBlue);
		}
		text->SetPosition( vsVector2D(0.0f, -140.0f + (40.0f * i)) );
		m_page[0]->AddChild(text);
	}

	for ( int i = 0; i < c_pageOneLines; i++ )
	{
		vsSprite *text = new vsSprite(vsBuiltInFont::CreateString(pageOneLines[i], c_size, c_capSize, Justification_Center));
		text->SetPosition( vsVector2D(0.0f, -140.0f + (40.0f * i)) );
		text->SetColor(vsColor::LightBlue);
		m_page[1]->AddChild(text);
	}

	for ( int i = 0; i < c_pageOneLines; i++ )
	{
		vsSprite *text = new vsSprite(vsBuiltInFont::CreateString(pageTwoLines[i], c_size, c_capSize, Justification_Center));
		text->SetPosition( vsVector2D(0.0f, -140.0f + (40.0f * i)) );
		text->SetColor(vsColor::LightBlue);
		m_page[2]->AddChild(text);
	}
	for ( int i = 0; i < c_pageOneLines; i++ )
	{
		vsSprite *text = new vsSprite(vsBuiltInFont::CreateString(pageThreeLines[i], c_size, c_capSize, Justification_Center));
		text->SetPosition( vsVector2D(0.0f, -140.0f + (40.0f * i)) );
		text->SetColor(vsColor::LightBlue);
		m_page[3]->AddChild(text);
	}
	for ( int i = 0; i < c_pageOneLines; i++ )
	{
		vsSprite *text = new vsSprite(vsBuiltInFont::CreateString(pageFourLines[i], c_size, c_capSize, Justification_Center));
		text->SetPosition( vsVector2D(0.0f, -140.0f + (40.0f * i)) );
		text->SetColor(vsColor::LightBlue);
		m_page[4]->AddChild(text);
	}
	
	ShowPage( 0 );
	
	m_transitioningIn = true;
	m_transitioningOut = false;
	m_transitionTimer = 0.f;
	
	m_game->PlayMusic(daGame::Music_Quote);
	
	Update(0.0f);
}

void
daModeTutorial::Deinit()
{
	vsDelete(m_title);
	vsDelete(m_back);
	
	for ( int i = 0; i < MAX_PAGES; i++ )
		vsDelete(m_page[i]);
}

void
daModeTutorial::Update( float timeStep )
{
	if ( m_transitioningIn || m_transitioningOut )
	{
		if ( m_transitioningOut )
		{
			m_transitionTimer -= timeStep;
			if ( m_transitionTimer <= 0.0f )
			{
				m_transitionTimer = 0.0f;
				m_transitioningOut = false;
				
				m_game->SetMode( daGame::Mode_TitleScreen );
				return;
			}
		}
		else if ( m_transitioningIn )
		{
			m_transitionTimer += timeStep;
			
			if ( m_transitionTimer > 1.0f )
			{
				m_transitionTimer = 1.0f;
				m_transitioningIn = false;
			}
		}
	}
	
	if ( m_game->GetInput()->WasPressed(CID_Left) )
	{
		m_currentPage = vsMax(0, m_currentPage-1);
		ShowPage( m_currentPage );
	}
	if ( m_game->GetInput()->WasPressed(CID_Right) )
	{
		m_currentPage = vsMin(4, m_currentPage+1);
		ShowPage( m_currentPage );
	}
	if ( m_game->GetInput()->WasPressed(CID_A) )
	{
		m_transitioningOut = true;
	}
	
	float f = m_transitionTimer;
	f = (3.0f * f * f) - (2.0f * f * f * f);

	vsTuneable vsVector2D c_offscreenPos(-2000.0f,0.0f);
	m_page[m_currentPage]->SetPosition( vsInterpolate(f, c_offscreenPos, vsVector2D::Zero) );
	m_title->SetColor( vsInterpolate( f, vsColor::Black, vsColor::White ) );
	m_back->SetColor( vsInterpolate( f, vsColor::Black, vsColor::Blue ) );
}

void
daModeTutorial::ShowPage( int pageNumber )
{
	for ( int i = 0; i < MAX_PAGES; i++ )
	{
		if ( i == pageNumber )
			m_page[i]->RegisterOnLayer(1);
		else
			m_page[i]->Extract();
	}
}

