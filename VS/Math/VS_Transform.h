/*
 *  VS_Transform.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_TRANSFORM_H
#define VS_TRANSFORM_H

#include "VS_Angle.h"
#include "VS_Vector.h"

class vsTransform
{
public:
	
	static vsTransform	Zero;
	
	vsVector2D		m_position;
	vsAngle			m_angle;
	vsVector2D		m_scale;
	
	vsTransform();
	vsTransform(const vsVector2D &pos, const vsAngle &angle);
	vsTransform(const vsVector2D &pos, const vsAngle &angle, const vsVector2D &scale);
	
	vsVector2D		ApplyTo( const vsVector2D &v );
	vsVector2D		ApplyInverseTo( const vsVector2D &v );
};

vsTransform vsInterpolate( float alpha, const vsTransform &a, const vsTransform &b );

#endif // VS_TRANSFORM_H
