/*
 *  VS_Camera.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 20/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Camera.h"
#include "VS_Screen.h"

vsCamera::vsCamera():
	m_position( vsVector2D::Zero ),
	m_angle( vsAngle::Zero ),
	m_reverseAngle( vsAngle::Zero ),
	m_fov( 1000.0f )
{
}

vsCamera::~vsCamera()
{
}

bool
vsCamera::IsPositionVisible( const vsVector2D &pos, float r )
{
	float aspectRatio = vsScreen::Instance()->GetAspectRatio();
	vsVector2D finalPos = pos;
	
	if ( m_angle.Get() != 0.f )
	{
		m_reverseAngle.Set( -m_angle.Get() );
		finalPos = m_reverseAngle.ApplyRotationTo(pos);
	}
	finalPos -= m_position;

	float halfFov = m_fov * 0.5f;
	float halfHoriFov = aspectRatio * halfFov;
	bool visible = true;
	
	if ( finalPos.y - r > halfFov || finalPos.y + r < -halfFov )
		visible = false;
	if ( finalPos.x - r > halfHoriFov || finalPos.x + r < -halfHoriFov )
		visible = false;
	
	return visible;
}

bool
vsCamera::WrapAround( vsVector2D &pos_in, float r )
{
	float aspectRatio = vsScreen::Instance()->GetAspectRatio();
	vsVector2D finalPos = pos_in;
	
	finalPos -= m_position;
	if ( m_angle.Get() != 0.f )
	{
		// convert from world to camera-local orientation
		m_reverseAngle.Set( -m_angle.Get() );
		finalPos = m_reverseAngle.ApplyRotationTo(finalPos);
	}
	
	float horiFov = aspectRatio * m_fov;
	float halfFov = m_fov * 0.5f;
	float halfHoriFov = horiFov * 0.5f;
	
	float wrapDist = halfFov + r;
	float horizWrapDist = halfHoriFov + r;
	bool wrapped = false;
	
	if ( finalPos.y - r > halfFov )
	{
		finalPos.y = -wrapDist;
		wrapped = true;
	}
	else if ( finalPos.y + r < -halfFov )
	{
		finalPos.y = wrapDist;
		wrapped = true;
	}
	
	if ( finalPos.x - r > halfHoriFov )
	{
		finalPos.x = -horizWrapDist;
		wrapped = true;
	}
	else if ( finalPos.x + r < -halfHoriFov )
	{
		finalPos.x = horizWrapDist;
		wrapped = true;
	}
	
	if ( wrapped )
	{
		if ( m_angle.Get() != 0.f )
		{
			// convert back from local to world orientation
			finalPos = m_angle.ApplyRotationTo(finalPos);
		}
		finalPos += m_position;
		pos_in = finalPos;
	}
	return wrapped;
}
