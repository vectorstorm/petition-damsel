/*
 *  VS_Layer.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 12/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_LAYER_H
#define VS_LAYER_H

#include "VS_Vector.h"

class vsEntity;
class vsDisplayList;
class vsCamera;

class vsLayer
{
	vsEntity *		m_entityList;	// our sprites
//	vsDisplayList *	m_displayList;
	vsCamera *		m_camera;
	vsCamera *		m_defaultCamera;
	
public:
		
					vsLayer();
	virtual			~vsLayer();
	
	float			GetFOV();
	
	vsVector2D		GetCorner(bool bottom, bool right);
	
	vsVector2D		GetTopLeftCorner() { return GetCorner(false,false); }
	vsVector2D		GetTopRightCorner() { return GetCorner(false,true); }
	vsVector2D		GetBottomLeftCorner() { return GetCorner(true,false); }
	vsVector2D		GetBottomRightCorner() { return GetCorner(true,true); }
	
//	vsDisplayList *	GetDisplayList() { return m_displayList; }
	vsCamera *		GetCamera() { return m_camera; }
	void			SetCamera( vsCamera *camera );

#if defined(_DEBUG)
	void			SetDebugCamera();
#endif // _DEBUG
	
	void			Update( float timeStep );
	void			Draw( vsDisplayList *list );
	
	void			RegisterEntityOnTop( vsEntity *sprite );
	void			RegisterEntityOnBottom( vsEntity *sprite );
};

#endif // VS_LAYER_H
