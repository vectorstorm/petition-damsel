/*
 *  VS_Transform.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Transform.h"

vsTransform vsTransform::Zero;

vsTransform::vsTransform():
	m_position( vsVector2D::Zero ),
	m_angle( vsAngle::Zero ),
	m_scale(1.0f, 1.0f)
{
}

vsTransform::vsTransform( const vsVector2D &pos, const vsAngle &angle ):
	m_position(pos),
	m_angle(angle),
	m_scale(1.f, 1.f)
{
}

vsTransform::vsTransform( const vsVector2D &pos, const vsAngle &angle, const vsVector2D &scale ):
	m_position(pos),
	m_angle(angle),
	m_scale(scale)
{
}

vsTransform vsInterpolate( float alpha, const vsTransform &a, const vsTransform &b )
{
	vsTransform result;
	
	result.m_position = vsInterpolate( alpha, a.m_position, b.m_position );
	result.m_angle = vsInterpolate( alpha, a.m_angle, b.m_angle );
	result.m_scale = vsInterpolate( alpha, a.m_scale, b.m_scale );
	
	return result;
}

vsVector2D
vsTransform::ApplyTo( const vsVector2D &v )
{
	vsVector2D result = m_angle.ApplyRotationTo(v);
	result.x *= m_scale.x;
	result.y *= m_scale.y;
	result += m_position;
	
	return result;
}

vsVector2D
vsTransform::ApplyInverseTo( const vsVector2D &v )
{
	vsVector2D result = v;
	vsAngle inverseAngle = -m_angle;
	
	result -= m_position;
	result.x /= m_scale.x;
	result.y /= m_scale.y;
	result = inverseAngle.ApplyRotationTo(result);
	
	return result;
}

