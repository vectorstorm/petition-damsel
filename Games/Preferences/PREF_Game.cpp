/*
 *  Pref_Game.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/12/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "PREF_Game.h"

#include "CORE_GameRegistry.h"

#include "UT_Menu.h"

#include "VS_System.h"

REGISTER_GAME("Preferences", prefGame)


prefGame::prefGame()
{
}

enum topItems
{
	Top_Screen,
	Top_Sound,
	Top_Controls
};

enum screenItems
{
	Screen_Resolution,
	Screen_Fullscreen,
	Screen_Bloom,
	Screen_Cancel,
	Screen_Accept
};

enum soundItems
{
	Sound_EffectVolume,
	Sound_MusicVolume,
	Sound_Cancel,
	Sound_Accept
};

void
prefGame::Init()
{
	Parent::Init();
	
	m_state = Top;

	vsVector2D menuPos(-500,-300);
	
	m_topMenu = new utMenu(3);
	
	m_topMenu->SetItemLabel(Top_Screen, "Screen");
	m_topMenu->SetItemLabel(Top_Sound, "Sound");
	m_topMenu->SetItemLabel(Top_Controls, "Controls");
	
	m_topMenu->RegisterOnLayer(0);
	m_topMenu->SetPosition(menuPos);


	m_screenMenu = new utMenu(5);
	
	m_screenMenu->SetItemLabel(Screen_Resolution, "Resolution");
	m_screenMenu->SetItemLabel(Screen_Fullscreen, "Fullscreen");
	m_screenMenu->SetItemLabel(Screen_Bloom, "Bloom Filter");
	m_screenMenu->SetItemLabel(Screen_Cancel, "Cancel");
	m_screenMenu->SetItemLabel(Screen_Accept, "Accept");
	
	m_screenMenu->RegisterOnLayer(0);
	m_screenMenu->SetPosition(menuPos);
	m_screenMenu->SetVisible(false);
	
	m_soundMenu = new utMenu(4);
	
	m_soundMenu->SetItemLabel(Sound_EffectVolume, "Effect Volume");
	m_soundMenu->SetItemLabel(Sound_MusicVolume, "Music Volume");
	m_soundMenu->SetItemLabel(Sound_Cancel, "Cancel");
	m_soundMenu->SetItemLabel(Sound_Accept, "Accept");
	
	m_soundMenu->RegisterOnLayer(0);
	m_soundMenu->SetPosition(menuPos);
	m_soundMenu->SetVisible(false);
	
	ResetValues();
}

void
prefGame::Deinit()
{
	delete m_topMenu;
	delete m_screenMenu;
	delete m_soundMenu;
	
	Parent::Deinit();
}

void
prefGame::Update(float timeStep)
{
	utMenuAction action;
	switch(m_state)
	{
		case Top:
			m_topMenu->SetVisible(true);
			m_screenMenu->SetVisible(false);
			m_soundMenu->SetVisible(false);
			
			action = m_topMenu->GetAction();
			if ( action.type == utMenuAction::Type_Select )
			{
				if ( action.menuItemId == Top_Screen )
				{
					m_state = Screen;
					m_screenMenu->Enter();
				}
				if ( action.menuItemId == Top_Sound )
				{
					m_state = Sound;
					m_soundMenu->Enter();
				}
			}
			break;
		case Screen:
			m_topMenu->SetVisible(false);
			m_screenMenu->SetVisible(true);
			
			action = m_screenMenu->GetAction();
			if ( action.type == utMenuAction::Type_Cancel ||
				action.type == utMenuAction::Type_Select && action.menuItemId == Screen_Cancel )
			{
				ResetValues();	// reset our values, if we're cancelling our changes!
				m_state = Top;
			}
			else if ( action.type == utMenuAction::Type_Select && action.menuItemId == Screen_Accept )
			{
				UpdateValues();
				vsSystem::Instance()->UpdateVideoMode();
				m_state = Top;
			}
			else if ( action.type == utMenuAction::Type_Left )
			{
				if ( action.menuItemId == Screen_Resolution )
					m_resolution = vsMax( m_resolution-1, 0 );
				else if ( action.menuItemId == Screen_Fullscreen )
					m_fullscreen = !m_fullscreen;
				else if ( action.menuItemId == Screen_Bloom )
					m_bloom = !m_bloom;
				
				SetValues();
			}
			else if ( action.type == utMenuAction::Type_Right )
			{
				if ( action.menuItemId == Screen_Resolution )
					m_resolution = vsMin( m_resolution+1, m_resolutionCount-1 );
				else if ( action.menuItemId == Screen_Fullscreen )
					m_fullscreen = !m_fullscreen;
				else if ( action.menuItemId == Screen_Bloom )
					m_bloom = !m_bloom;
				
				SetValues();
			}
			break;
		case Sound:
			m_topMenu->SetVisible(false);
			m_soundMenu->SetVisible(true);
			
			action = m_soundMenu->GetAction();
			if ( action.type == utMenuAction::Type_Cancel ||
				action.type == utMenuAction::Type_Select && action.menuItemId == Sound_Cancel )
			{
				ResetValues();	// reset our values, if we're cancelling our changes!
				m_state = Top;
			}
			else if ( action.type == utMenuAction::Type_Select && action.menuItemId == Sound_Accept )
			{
				UpdateValues();
				//vsSystem::Instance()->UpdateVideoMode();
				m_state = Top;
			}
			else if ( action.type == utMenuAction::Type_Left )
			{
				if ( action.menuItemId == Sound_EffectVolume )
					m_effectVolume = vsMax( m_effectVolume-1, 0 );
				else if ( action.menuItemId == Sound_MusicVolume )
					m_musicVolume = vsMax( m_musicVolume-1, 0 );
				
				SetValues();
			}
			else if ( action.type == utMenuAction::Type_Right )
			{
				if ( action.menuItemId == Sound_EffectVolume )
					m_effectVolume = vsMin( m_effectVolume+1, 10 );
				else if ( action.menuItemId == Sound_MusicVolume )
					m_musicVolume = vsMin( m_musicVolume+1, 10 );
				
				SetValues();
			}
			break;
		default:
			break;
	}
	
	Parent::Update(timeStep);
}

void
prefGame::ResetValues()
{
	vsSystemPreferences *p = vsSystem::Instance()->GetPreferences();
	
	m_fullscreen = p->GetFullscreen();
	m_resolution = p->GetResolutionId();
	m_resolutionCount = p->GetSupportedResolutionCount();
	m_bloom = p->GetBloom();
	
	m_musicVolume = p->GetMusicVolume();
	m_effectVolume = p->GetEffectVolume();
	
	SetValues();
}

vsString
prefGame::MakeVolumeString(int value)
{
	vsString result = "";
	if ( value > 0 )
		result += "<";
	else
		result += " ";
	
	result += vsFormatString("%d",value);
	
	if ( value < 10 )
		result += ">";
		
	return result;
}

void
prefGame::SetValues()
{
	vsSystemPreferences *p = vsSystem::Instance()->GetPreferences();

	vsString resolutionValue;
	Resolution * resArray = p->GetSupportedResolutions();
	
	resolutionValue = vsFormatString("%dx%d", resArray[m_resolution].width, resArray[m_resolution].height);
	if ( m_resolution > 0 )
		resolutionValue = "<" + resolutionValue;
	else
		resolutionValue = " " + resolutionValue;
	if ( m_resolution < m_resolutionCount-1 )
		resolutionValue += ">";
	
	m_screenMenu->SetItemValue(Screen_Resolution, resolutionValue);
	
	
	vsString fullscreenValue = m_fullscreen?"<ON>":"<OFF>";
	m_screenMenu->SetItemValue(Screen_Fullscreen, fullscreenValue);

	
	vsString bloomValue = m_bloom?"<ON>":"<OFF>";
	m_screenMenu->SetItemValue(Screen_Bloom, bloomValue);
	
	
	m_soundMenu->SetItemValue(Sound_EffectVolume, MakeVolumeString(m_effectVolume));
	m_soundMenu->SetItemValue(Sound_MusicVolume, MakeVolumeString(m_musicVolume));
}

void
prefGame::UpdateValues()
{
	vsSystemPreferences *p = vsSystem::Instance()->GetPreferences();
	
	p->SetResolutionId(m_resolution);
	p->SetFullscreen(m_fullscreen);
	p->SetBloom(m_bloom);
	
	p->SetMusicVolume(m_musicVolume);
	p->SetEffectVolume(m_effectVolume);
	
	p->Save();
}


