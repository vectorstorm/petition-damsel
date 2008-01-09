/*
 *  COL_Sprite.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 3/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef COL_SPRITE_H
#define COL_SPRITE_H

#include "COL_Object.h"
#include "VS_Sprite.h"

class colSprite : public vsSprite, public colResponder
{
protected:
	colObject *		m_colObject;
public:
	static colSprite * Load(const vsString &filename, int colFlags, int testFlags);
	
	colSprite( vsDisplayList *list, int colFlags, int testFlags );
	virtual ~colSprite();
	
	void			SetCollisionsActive(bool active) { m_colObject->SetCollisionsActive(active); }
	
	void			Teleport();
	void			SetDestroyed() { m_colObject->SetDestroyed(); }
	
	virtual bool	CollisionCallback( const colEvent &collision );
	virtual void	DestroyCallback();

	virtual void	Update( float timeStep );
};


#endif // COL_SPRITE_H
