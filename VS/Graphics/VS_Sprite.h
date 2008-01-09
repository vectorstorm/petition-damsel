/*
 *  VS_Sprite.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 12/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_SPRITE_H
#define VS_SPRITE_H

#include "VS_Color.h"
#include "VS_Entity.h"
#include "VS_Transform.h"

class vsSprite : public vsEntity
{
protected:
	vsDisplayList	*m_displayList;
	
	float			m_boundingRadius;
	vsColor			m_color;
	bool			m_useColor;
	
	typedef vsEntity Parent;
	
public:
	vsTransform		m_transform;
	
	static vsSprite * Load(const vsString &filename);
	
					vsSprite( vsDisplayList *displayList = NULL );
	virtual			~vsSprite();
					
	void			SetColor( vsColor c ) { m_color = c; m_useColor = true; }
	
	void			SetPosition( const vsVector2D &pos ) { m_transform.m_position = pos; }
	vsVector2D		GetPosition() { return m_transform.m_position; }
	void			SetAngle( const vsAngle &angle ) { m_transform.m_angle = angle; }
	vsAngle			GetAngle() { return m_transform.m_angle; }
	void			SetScale( float scale ) { m_transform.m_scale = vsVector2D(scale,scale); }
	void			SetScale( const vsVector2D &scale ) { m_transform.m_scale = scale; }
	vsVector2D		GetScale() { return m_transform.m_scale; }
	
	float			GetBoundingRadius() { return m_boundingRadius; }
	
	void			Rotate( float angle ) { m_transform.m_angle.Rotate(angle); }
	
	virtual void	Draw( vsDisplayList *list );
};

#endif // VS_SPRITE_H
