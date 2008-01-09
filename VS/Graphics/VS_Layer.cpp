/*
 *  VS_Layer.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 12/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Screen.h"

#include "VS_Camera.h"
#include "VS_DisplayList.h"
#include "VS_Entity.h"
#include "VS_Layer.h"
//#include "VS_Transform.h"

#include <SDL/SDL_opengl.h>

vsLayer::vsLayer()
{
	m_entityList = new vsEntity();
//	m_displayList = new vsDisplayList(40000);
	m_defaultCamera = new vsCamera;
	m_camera = m_defaultCamera;
}

vsLayer::~vsLayer()
{
	delete m_defaultCamera;
	delete m_entityList;
//	delete m_displayList;
}

void
vsLayer::SetCamera( vsCamera *camera )
{
	if ( camera )
		m_camera = camera;
	else
		m_camera = m_defaultCamera;
}

float
vsLayer::GetFOV()
{
	return m_camera->GetFOV();
}

void
vsLayer::Update( float timeStep )
{
	if ( m_camera )
	{
		m_camera->Update( timeStep );
	}

	vsEntity *entity = m_entityList->GetNext();
	vsEntity *next;
	while ( entity != m_entityList )
	{
		next = entity->GetNext();		// entities might remove themselves during their Update, so pre-cache the next entity
		
		entity->Update( timeStep );
		
		if ( entity->GetNext() == entity )
			entity = next;
		else
			entity = entity->GetNext();
	}
}

void
vsLayer::Draw( vsDisplayList *list )
{
//	m_displayList->Clear();
	
	if ( m_camera )
		list->SetCameraTransform( m_camera->GetCameraTransform() );
	
	// now we need to push the inverse of our camera info onto the display list, so our entities get drawn relative to the camera
	
	vsEntity *entity = m_entityList->GetNext();
	while ( entity != m_entityList )
	{
		entity->Draw( list );
		entity = entity->GetNext();
	}
}

void
vsLayer::RegisterEntityOnBottom( vsEntity *sprite )
{
	sprite->Extract();
	m_entityList->Append(sprite);
}

void
vsLayer::RegisterEntityOnTop( vsEntity *sprite )
{
	sprite->Extract();
	m_entityList->Prepend(sprite);
}

vsVector2D
vsLayer::GetCorner(bool bottom, bool right)
{
	vsVector2D pos = vsVector2D::Zero;
	// okay.  First, let's start by grabbing the coordinate of our requested corner,
	// as though we had no camera.  Later on, we'll correct for the camera.
	
	float fov = GetFOV();			// fov is measured VERTICALLY.  So our height is FOV.
	float halfFov = 0.5f * fov;		// since we assume that '0,0' is in the middle, our coords vertically are [-halfFov .. halfFov]
	
	if ( bottom )
		pos.y = halfFov;
	else	// top
		pos.y = -halfFov;
	
	// now, to figure out where the edge is, we need to know our screen aspect ratio, which is the ratio of horizontal pixels to vertical pixels.
	float aspectRatio = vsScreen::Instance()->GetAspectRatio();
	
	if ( right )
		pos.x = halfFov * aspectRatio;
	else	// left
		pos.x = -halfFov * aspectRatio;
	
	
	// Okay!  Now we have the corner of our screen.  Now we just need to figure out where this camera-relative coordinate sits in world-space
	// to do this, we take the world-to-camera transform off the camera, and then apply its inverse to our position.
	
	vsTransform worldToCamera;
	worldToCamera.m_position = m_camera->GetPosition();
	worldToCamera.m_angle = m_camera->GetAngle();
	
	pos = worldToCamera.ApplyInverseTo(pos);
	
	return pos;
}

#if defined(_DEBUG)

class vsDebugCamera : public vsCamera
{
public:
	vsDebugCamera() : vsCamera()
	{
	}
	
	void				Init()
	{
		m_fov = 1.0f;
		m_position.Set( 0.5f * vsScreen::Instance()->GetAspectRatio() , 0.5f);
	}
};

static vsDebugCamera s_debugCamera;

void
vsLayer::SetDebugCamera()
{
	s_debugCamera.Init();
	SetCamera( &s_debugCamera );
}

#endif // _DEBUG


