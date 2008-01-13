/*
 *  DA_LevelBackground.cpp
 *  Damsel
 *
 *  Created by Trevor Powell on 11/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#include "DA_LevelBackground.h"

#include "VS_DisplayList.h"
#include "VS_Layer.h"
#include "VS_Random.h"
#include "VS_Screen.h"

daLevelBackground::daLevelBackground():
	vsSprite( vsDisplayList::Load("LevelBackground.vec") )
{
//	m_grassDisplayList = new vsDisplayList("Grass.vec");
	
	SetScale( vsVector2D(2.0f,2.0f) );
	SetColor( vsColor::White );
	
	m_fadeTimer = 0.f;
	m_fadingIn = false;
	
	vsVector2D topLeft = vsScreen::Instance()->GetLayer(0)->GetTopLeftCorner();
	vsVector2D bottomRight = vsScreen::Instance()->GetLayer(0)->GetBottomRightCorner();
	for ( int i = 0; i < MAX_GRASS; i++ )
	{
		vsDisplayList *list = vsDisplayList::Load("Grass.vec");
		//list->Compile();
		m_grass[i] = new vsSprite(list);
		m_grass[i]->SetColor(vsColor::Green);
		vsVector2D pos;
		do
		{
			pos = vsRandom::GetVector2D(topLeft,bottomRight);
		}while ( vsFabs(pos.x) < 100.f || vsFabs(pos.y) < 100.f );
		
		m_grass[i]->SetPosition(pos);
		AddChild(m_grass[i]);
	}
	
	m_roadMarkings = vsSprite::Load("RoadMarkings.vec");
	m_roadMarkings->SetColor(vsColor::Yellow);
	AddChild(m_roadMarkings);
	
	Update(0.0f);
}

daLevelBackground::~daLevelBackground()
{
//	delete m_grassDisplayList;
}

void
daLevelBackground::Update( float timeStep )
{
	vsTuneable float c_fadeTime = 1.0f;
	
	if ( m_fadingIn )
		m_fadeTimer += timeStep / m_fadeTime;
	else
		m_fadeTimer -= timeStep / m_fadeTime;
	
	m_fadeTimer = vsClamp( m_fadeTimer, 0.f, c_fadeTime );
	
	float f = m_fadeTimer / c_fadeTime;
	
	f = (3.0f * f * f) - (2.0f * f * f * f);
	
	vsColor grey(0.3f,0.3f,0.3f,1.0f);
	SetColor( vsInterpolate( f, vsColor::Black, grey ) );
	m_roadMarkings->SetColor( vsInterpolate( f, vsColor::Black, vsColor::Yellow ) );
	
	for ( int i = 0; i < MAX_GRASS; i++ )
		m_grass[i]->SetColor( vsInterpolate( f, vsColor::Black, vsColor::Green ) );
}

void
daLevelBackground::FadeIn(float fadeTime)
{
	m_fadingIn = true;
	m_fadeTime = fadeTime;
}

void
daLevelBackground::FadeOut(float fadeTime)
{
	m_fadingIn = false;
	m_fadeTime = fadeTime;
}
