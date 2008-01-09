/*
 *  GM_MainMenu.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "Core.h"
#include "CORE_GameRegistry.h"
#include "CORE_Version.h"

#include "GM_MainMenu.h"

#include "SYS_Input.h"

#include "SND_System.h"
#include "SND_Sample.h"
#include "SND_Music.h"

#include "VS_DisplayList.h"
#include "VS_Font.h"
#include "VS_Layer.h"
#include "VS_Screen.h"
#include "VS_Sprite.h"

REGISTER_GAME("MainMenu", gmMainMenu)

/*
void
gmMainMenuEntity::Init()
{
	m_font = new vsFont;
	m_cacheString = new vsDisplayList(1024);
	m_cacheString->Clear();
	m_font->BuildDisplayListForString(m_cacheString, "VectorStorm");
}

void
gmMainMenuEntity::Deinit()
{
	delete m_font;
	delete m_cacheString;
}

void
gmMainMenuEntity::_Draw( vsDisplayList *list )
{
	list->Append( *m_cacheString );
}
*/

gmMainMenu::gmMainMenu()
{
	m_selection = 0;
}

void
gmMainMenu::Init()
{
	Parent::Init();
	
	m_music = new sndMusic("TicTacTic.mp3");
//	m_music = new sndMusic("tt2.ogg");
	m_tickSound = new sndSample("thump_4.wav");
	
	BuildTitleSprite();
	
	m_menuSpriteCount = coreGameRegistry::GetGameCount();
	m_menuSprite = new vsSprite *[m_menuSpriteCount];
	
	for ( int i = 0; i < m_menuSpriteCount; i++ )
	{
		m_menuSprite[i] = vsSprite::Load("MenuFrame.vec");
		vsScreen::Instance()->GetLayer(0)->RegisterEntityOnTop( m_menuSprite[i] );
		
		m_menuSprite[i]->m_transform.m_position.Set(0.0f, -100.0f + (90.0f * i));
		m_menuSprite[i]->m_transform.m_scale.Set(1.5f, 1.5f);
		
		vsSprite *buttonImage = new vsSprite( vsBuiltInFont::CreateString( coreGameRegistry::GetGameName(i), 15.0f, 25.0f ) );
		buttonImage->m_transform.m_position.Set(-190.0f, 12.0f);
		m_menuSprite[i]->AddChild(buttonImage);
	}
	
	m_build = new vsSprite( 
			vsBuiltInFont::CreateString("Build " + coreVersion::GetVersionString() + " - " + coreVersion::GetVersionNameString(), 15.0f, 25.0f, Justification_Center) );
	m_build->m_transform.m_position.Set(400.0f,385.0f);
	m_build->m_transform.m_angle.Set(DEGREES(10.0f));
	m_build->SetColor(vsColor(0.0f,0.0f,0.75f));
	m_build->RegisterOnLayer(0);
/*
	m_buildName = new vsSprite( 
						   vsBuiltInFont::CreateString(coreVersion::GetVersionNameString(), 15.0f, 25.0f, Justification_Center) );
	m_buildName->m_transform.m_position.Set(480.0f,340.0f);
	m_buildName->m_transform.m_angle.Set(DEGREES(10.0f));
	m_buildName->SetColor(vsColor(0.0f,0.0f,1.0f));
	m_buildName->RegisterOnLayer(0);
*/	
	m_pulseTimer = 0.f;
	
	m_music->Start();
}

void
gmMainMenu::Deinit()
{
	delete m_title;
	
	for ( int i = 0; i < m_menuSpriteCount; i++ )
		delete m_menuSprite[i];
	delete [] m_menuSprite;
	
	delete m_music;
	delete m_tickSound;
	
	delete m_stripes[0];
	delete m_stripes[1];
	
	delete m_build;
//	delete m_buildName;
	
	Parent::Deinit();
}

void
gmMainMenu::BuildTitleSprite()
{
	m_title = new vsSprite();
	
	char title[12] = "VectorStorm";
	m_title->m_transform.m_position.Set(-550.f, -350.f);
	vsScreen::Instance()->GetLayer(0)->RegisterEntityOnTop( m_title );
	
	for ( int i = 0; i < 11; i++ )
	{
		// move objects around based on which character it is.
		
		if ( i == 0 )		// in 'V'
		{
			m_letter[i] = new vsSprite( vsBuiltInFont::CreateCharacter(title[i], 30.0f, 120.0f) );
			
			m_letter[i]->m_transform.m_position.Set(0.0f, 0.0f);
			m_letter[i]->SetColor( vsColor::Blue );
		}
		else if ( i < 6 )	// in 'ector'
		{
			m_letter[i] = new vsSprite( vsBuiltInFont::CreateCharacter(title[i], 30.0f, 80.0f)/*, i * 0.2f*/ );
			m_letter[i]->m_transform.m_position.Set(50.0f + i*35.0f,-70.0f);
			m_letter[i]->SetColor( vsColor::Blue );
		}
		else if ( i == 6 )	// in 'S'
		{
			m_letter[i] = new vsSprite( vsBuiltInFont::CreateCharacter(title[i], 30.0f, 120.0f) );
			m_letter[i]->m_transform.m_position.Set(40.0f, 40.0f);
			m_letter[i]->SetColor( vsColor::LightBlue );
		}
		else // in 'torm'
		{
			m_letter[i] = new mmStretchSprite( vsBuiltInFont::CreateCharacter(title[i], 40.0f, 80.0f), i * 0.2f );
			int StormChar = i - 6;
			m_letter[i]->m_transform.m_position.Set(60.0f + StormChar*50.0f, -30.0f);
			m_letter[i]->SetColor( vsColor::LightBlue );
		}
		
		
		m_title->AddChild( m_letter[i] );
	}
	for ( int i = 0; i < 2; i++ )
	{
		m_stripes[i] = vsSprite::Load("Stripes.vec");
		m_stripes[i]->SetColor( vsColor::Yellow );
		m_stripes[i]->m_transform.m_position.Set( -250.0f + (500.0f*i), 470.0f );
		m_stripes[i]->m_transform.m_angle.Set( DEGREES(10.0f) );
		vsScreen::Instance()->GetLayer(0)->RegisterEntityOnTop( m_stripes[i] );
	}
}

#define PULSE_DURATION (2.0f)

void
gmMainMenu::Update(float timeStep)
{
	sysInput *input = GetInput();
	
	if ( input->WasPressed( CID_Down ) )
	{
		coreGame::GetSound()->PlaySound(m_tickSound);
		m_selection++;
		if ( m_selection >= m_menuSpriteCount )
			m_selection = 0;
	}
	if ( input->WasPressed( CID_Up ) )
	{
		coreGame::GetSound()->PlaySound(m_tickSound);
		m_selection--;
		if ( m_selection < 0 )
			m_selection = m_menuSpriteCount-1;
	}
	if ( input->WasPressed( CID_A ) )
		core::SetGame( coreGameRegistry::GetGame(m_selection) );
	
	m_pulseTimer += timeStep;
	if ( m_pulseTimer > PULSE_DURATION )
		m_pulseTimer -= PULSE_DURATION;
	
	float frac = m_pulseTimer / PULSE_DURATION;
	
	float pulseAmt = vsCos(TWOPI * frac);	// [ -1..1 ]
	pulseAmt = (pulseAmt * 0.5f) + 0.5f;	// [ 0..1 ]
	
	for ( int i = 0; i < m_menuSpriteCount; i++ )
	{
		if ( i == m_selection )
		{
			vsColor lightBlue(0.5f,0.5f,1.0f,0.8f);
			vsColor pulseColor = vsInterpolate( pulseAmt, lightBlue, vsColor::White );
			m_menuSprite[i]->SetColor( pulseColor );
		}
		else
			m_menuSprite[i]->SetColor( vsColor::Blue );
	}
}

mmStretchSprite::mmStretchSprite( vsDisplayList *list, float delay ): vsSprite(list), m_timer(-delay)
{
}

void
mmStretchSprite::Update( float timeStep )
{
	m_timer += timeStep;
	
	if ( m_timer > 2.0f )
		m_timer -= 2.0f;
	
	float scaleVal = 0.5f * (vsCos( m_timer * PI ) + 1.0f);	// [0..1]
	scaleVal += 1.5f;										// [1.5 .. 2.5]
	scaleVal *= 0.5f;										// [0.75 .. 1.25]
	
	m_transform.m_scale.Set(1.0f,scaleVal);
}
