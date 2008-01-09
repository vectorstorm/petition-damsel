/*
 *  VS_DisplayList.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_DISPLAYLIST_H
#define VS_DISPLAYLIST_H

#include "VS_Angle.h"
#include "VS_Color.h"
#include "VS_Vector.h"
#include "VS_Transform.h"

class vsDisplayList
{
public:
	enum OpCode
	{
		OpCode_SetColor,
		OpCode_MoveTo,
		OpCode_LineTo,
		OpCode_DrawPoint,
		OpCode_PushTransform,
		OpCode_PopTransform,
		OpCode_SetCameraTransform,
		OpCode_MAX
	};
	
	struct Data
	{
		float x,y,z,w,theta;
		void Set(const vsVector2D & in) {x=in.x;y=in.y;z=0.f;w=0.f;};
		void Set(const vsColor & in) {x=in.r,y=in.g,z=in.b,w=in.a;};
		void Set(const vsTransform &t) {x=t.m_position.x;y=t.m_position.y;z=t.m_scale.x;w=t.m_scale.y;theta=t.m_angle.Get();}
		
		vsVector2D GetVector2D() {return vsVector2D(x,y);}
		vsColor GetColor() {return vsColor(x,y,z,w); }
		vsTransform	GetTransform() {return vsTransform(vsVector2D(x,y), vsAngle(theta), vsVector2D(z,w));}
	};
	
	struct op
	{
		OpCode	type;
		Data	data;
	};
	
private:
	
	
	char *	m_buffer;
	int		m_bufferSize;
	int		m_bufferPos;
	
	op *m_instruction;
	int	m_maxInstructions;
	int m_instructionCount;
	int	m_nextInstruction;
	
	op  m_currentOp;
	
	vsDisplayList *	m_instanceParent;		// if set, I'm an instance of this other vsDisplayList, and contain no actual data myself
	int				m_instanceCount;		// The number of instances that have been derived off of me.  If this value isn't zero, assert if someone tries to delete me.
	
	int		GetSize() const { return m_instructionCount; }
	int		GetFreeOps() const { return m_maxInstructions - m_instructionCount; }
	
	op *	AddOp(OpCode type);
	
public:
	
	static vsDisplayList *	Load(const char *filename);
	
			vsDisplayList(int maxInstructions = 1024);
//			vsDisplayList(const vsDisplayList &list);
	virtual	~vsDisplayList();
	
	vsDisplayList *	CreateInstance();
	
	int		GetOpCount() const { return m_instructionCount; }
	int		GetMaxOpCount() const { return m_maxInstructions; }
	void	Clear();
	void	Rewind();	// reset to the start of the display list
	
	 //	The following functions do not draw primitives immediately;  instead, they add them to this display list.
	 // Instructions given using these functions will be carried out when this vsDisplayList is passed to a vsRenderer,
	 // using the DrawDisplayList() function.  (Normally, this happens automatically at the end of processing each frame)
	
	void	SetColor( const vsColor &color );
	void	MoveTo( const vsVector2D &pos );
	void	LineTo( const vsVector2D &pos );
	void	DrawPoint( const vsVector2D &pos );
	void	DrawLine( const vsVector2D &from, const vsVector2D &to );
	void	PushTransform( const vsTransform &t );
	void	PopTransform();
	void	SetCameraTransform( const vsTransform &t );	// no stack of camera transforms;  they an only be set absolutely!
	
	void	GetBoundingCircle( vsVector2D &center, float &radius );
	float	GetBoundingRadius();
	
	void	Append( const vsDisplayList &list );	// appends the passed display list onto us.
	
	op *	PopOp();
	
	void operator= ( const vsDisplayList &list ) { Clear(); Append(list); }
};

#endif // VS_DISPLAYLIST
