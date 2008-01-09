/*
 *  VS_Debug.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/05/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Debug.h"
#include "VS_DisplayList.h"
#include "VS_Font.h"
#include "VS_Layer.h"
#include "VS_Screen.h"
#include "VS_Sprite.h"

#include <assert.h>

#if defined(_DEBUG)

static bool bAsserted = false;

void vsFailedAssert( const vsString &conditionStr, const vsString &msg, const char *file, int line )
{
	if ( !bAsserted )
	{
		// failed assertion..  render and go into an infinite loop.
		bAsserted = true;
		vsString trimmedFile(file);
		size_t pos = trimmedFile.rfind('/');
		if ( pos )
		{
			trimmedFile = trimmedFile.substr(pos+1);
		}
		
		vsLog("Failed assertion:  %s", msg.c_str());
		vsLog("Failed condition: (%s)", conditionStr.c_str());
		vsLog("at %s:%d", trimmedFile.c_str(), line);
		//	assert(0);
		//	exit(1);
		
		vsString loc = vsFormatString("(%s) %s:%d", conditionStr.c_str(), trimmedFile.c_str(), line);
		
		vsDisplayList *m = vsBuiltInFont::CreateString(msg, 12.0f, 15.0f, Justification_Center);
		vsDisplayList *l = vsBuiltInFont::CreateString(loc, 12.0f, 15.0f, Justification_Center);
		
		vsSprite *mSprite = new vsSprite(m);
		vsSprite *lSprite = new vsSprite(l);
		
		mSprite->SetPosition( vsVector2D(0.f, -30.f) );
		lSprite->SetPosition( vsVector2D(0.f, 30.f) );
		
		mSprite->SetColor( vsColor::White );
		lSprite->SetColor( vsColor::White );
		
		vsScreen::Instance()->GetLayer(0)->SetCamera(NULL);
		vsScreen::Instance()->GetLayer(0)->RegisterEntityOnTop( mSprite );
		vsScreen::Instance()->GetLayer(0)->RegisterEntityOnTop( lSprite );
		
		vsScreen::Instance()->Draw();
		vsScreen::Instance()->Draw();
		vsScreen::Instance()->Draw();
		
		// Now we intentionally try to write to NULL, to trigger debuggers to stop, 
		// so we can perhaps try to debug whatever went wrong.
		//
		// If there's no debugger running, then this will probably yield a segfault.
		// If we're running on something primitive enough that it doesn't even segfault,
		// then just explicitly exit.
		{
			char *ptr = NULL;
			*ptr = 0;
			
			exit(1);
		}
	}
	else
	{
		vsLog("Error:  Asserted while handling assertion!");
	}
}

#endif

