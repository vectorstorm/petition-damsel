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
	
	vsVector2D topLeft = vsScreen::Instance()->GetLayer(0)->GetTopLeftCorner();
	vsVector2D bottomRight = vsScreen::Instance()->GetLayer(0)->GetBottomRightCorner();
	for ( int i = 0; i < 10; i++ )
	{
		vsDisplayList *list = vsDisplayList::Load("Grass.vec");
		//list->Compile();
		vsSprite *grassSprite = new vsSprite(list);
		grassSprite->SetColor(vsColor::Green);
		vsVector2D pos;
		do
		{
			pos = vsRandom::GetVector2D(topLeft,bottomRight);
		}while ( vsFabs(pos.x) < 100.f || vsFabs(pos.y) < 100.f );
		
		grassSprite->SetPosition(pos);
		AddChild(grassSprite);
	}
}

daLevelBackground::~daLevelBackground()
{
//	delete m_grassDisplayList;
}
