/*
 *  VS_Camera.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 20/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_CAMERA_H
#define VS_CAMERA_H

#include "VS_Vector.h"
#include "VS_Angle.h"
#include "VS_Transform.h"

class vsCamera
{
protected:
	vsVector2D	m_position;
	vsAngle		m_angle;
	vsAngle		m_reverseAngle;
	float		m_fov;
	
public:
	
				vsCamera();
	virtual		~vsCamera();
	
	virtual void		Update( float /*timeStep*/ ) {}
	
	const vsVector2D &	GetPosition() { return m_position; }
	const vsAngle &		GetAngle() { return m_angle; }
	float				GetFOV() { return m_fov; }
	
	vsTransform			GetCameraTransform() { return vsTransform(m_position, m_angle, vsVector2D(m_fov,m_fov)); }
	
	bool				IsPositionVisible( const vsVector2D &pos, float r=0.f );
	bool				WrapAround( vsVector2D &pos, float r=0.f );
};

#endif // VS_CAMERA_H
