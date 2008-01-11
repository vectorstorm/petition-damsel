/*
 *  DA_LevelBackground.h
 *  Damsel
 *
 *  Created by Trevor Powell on 11/01/08.
 *  Copyright 2008 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef DA_LEVELBACKGROUND_H
#define DA_LEVELBACKGROUND_H

#include "VS_Sprite.h"

class daLevelBackground : public vsSprite
{
	vsDisplayList *m_grassDisplayList;
public:
	
					daLevelBackground();
	virtual			~daLevelBackground();
	
};

#endif // DA_LEVELBACKGROUND_H
