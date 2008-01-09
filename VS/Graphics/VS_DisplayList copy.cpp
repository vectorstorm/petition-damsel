/*
 *  VS_DisplayList.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 17/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_DisplayList.h"
#include "FS_File.h"
#include <assert.h>
#include <string.h>

static char * g_opCodeName[vsDisplayList::OpCode_MAX] =
{
	"SetColor",
	"MoveTo",
	"LineTo",
	"DrawPoint",
	"PushTransform",
	"PopTransform",
	"CameraTransform"
};

vsDisplayList *
vsDisplayList::Load( const char *filename )
{
	vsDisplayList *loader = new vsDisplayList(256);
	
	fsFile *file = new fsFile(filename);
	fsRecord r;
	
	while( file->Record(r) )
	{
		char *label = r.label;
		
		for ( int i = 0; i < OpCode_MAX; i++ )
		{
			if ( 0 == strcmp( label, g_opCodeName[i] ) )
			{
				OpCode code = (OpCode)i;
				
				op *o = loader->AddOp( code );
				
				switch(code)
				{
					case OpCode_SetColor:
					{
						vsColor color = r.Color();
						o->data.Set(color);
						break;
					}
					case OpCode_MoveTo:
					case OpCode_LineTo:
					case OpCode_DrawPoint:
					{
						vsVector2D pos = r.Vector2D();
						o->data.Set(pos);
						break;
					}
					case OpCode_PushTransform:
					{
					}
					default:
						break;
				}
			}
		}
	}
	
	delete file;
	
	vsDisplayList *result = new vsDisplayList( loader->GetSize() );
	result->Append(*loader);
	delete loader;
	
	return result;
}

vsDisplayList::vsDisplayList( int maxInstructions ):
	m_instructionCount(0),
	m_nextInstruction(0),
	m_maxInstructions(maxInstructions),
	m_instanceParent(NULL),
	m_instanceCount(0)
{
	if ( maxInstructions )
	{
		m_instruction = new op[maxInstructions];
		
		Clear();
	}
	else
		m_instruction = NULL;
}

vsDisplayList::~vsDisplayList()
{
	assert( m_instanceCount == 0 );

	if ( m_instruction )
	{
		delete [] m_instruction;
		m_instruction = NULL;
	}
	else if ( m_instanceParent )
	{
		m_instanceParent->m_instanceCount--;
	}
}

vsDisplayList *
vsDisplayList::CreateInstance()
{
	vsDisplayList *result = new vsDisplayList(0);
	
	result->m_instanceParent = this;
	m_instanceCount++;
	
	return result;
}

void
vsDisplayList::Clear()
{
	m_instructionCount = 0;
	m_nextInstruction = 0;
}

void
vsDisplayList::Rewind()
{
	m_nextInstruction = 0;
}

vsDisplayList::op *
vsDisplayList::AddOp(OpCode type)
{
	assert( m_instructionCount < m_maxInstructions );	// if we hit this assertion, we have too many instructions in this display list!
	
	op *result = &m_instruction[ m_instructionCount++ ];
	
	result->type = type;
	return result;
}

void
vsDisplayList::SetColor( const vsColor &color )
{
	op* o = AddOp( OpCode_SetColor );
	o->data.Set(color);
}

void
vsDisplayList::MoveTo( const vsVector2D &pos )
{
	op* o = AddOp( OpCode_MoveTo );
	o->data.Set(pos);
}

void
vsDisplayList::LineTo( const vsVector2D &pos )
{
	op* o = AddOp( OpCode_LineTo );
	o->data.Set(pos);
}

void
vsDisplayList::DrawPoint( const vsVector2D &pos )
{
	op* o = AddOp( OpCode_DrawPoint );
	o->data.Set(pos);
}

void
vsDisplayList::Append( const vsDisplayList &list )
{
	if ( list.m_instanceParent )
	{
		Append( *list.m_instanceParent );
	}
	else
	{
		int newElements = list.GetSize();
		assert( newElements <= GetFreeOps() );	// overflowing this list?
		
		for( int i = 0; i < newElements; i++ )
		{
			m_instruction[ m_instructionCount++ ] = list.m_instruction[ i ];
		}
	}
}

void
vsDisplayList::DrawLine( const vsVector2D &from, const vsVector2D &to )
{
	MoveTo(from);
	LineTo(to);
}

void
vsDisplayList::PushTransform( const vsTransform &t )
{
	op* o = AddOp( OpCode_PushTransform );
	o->data.Set(t);
}

void
vsDisplayList::SetCameraTransform( const vsTransform &t )
{
	op* o = AddOp( OpCode_SetCameraTransform );
	o->data.Set(t);
}

void
vsDisplayList::PopTransform()
{
	AddOp( OpCode_PopTransform );
}

vsDisplayList::op *
vsDisplayList::PopOp()
{
	if ( m_nextInstruction < m_instructionCount )
	{
		memcpy( &m_currentOp, &m_instruction[m_nextInstruction++], sizeof(op) );
		return &m_currentOp;
	}
	
	return NULL;
}

void
vsDisplayList::GetBoundingCircle(vsVector2D &center, float &radius)
{
	if ( m_instanceParent )
	{
		m_instanceParent->GetBoundingCircle(center, radius);
	}
	else
	{
		vsVector2D min(1000000.0f,1000000.0f);
		vsVector2D max(-1000000.0f, -1000000.0f);
		
		vsTransform currentTransform;
		Rewind();
		op *o = PopOp();
		
		while(o)
		{
			if ( o->type == OpCode_MoveTo || o->type == OpCode_LineTo )
			{
				vsVector2D pos = o->data.GetVector2D();
				
				if ( pos.x > max.x )
					max.x = pos.x;
				if ( pos.y > max.y )
					max.y = pos.y;
				if ( pos.x < min.x )
					min.x = pos.x;
				if ( pos.y < min.y )
					min.y = pos.y;
			}
			o = PopOp();
		}
		
		center = 0.5f * (max + min);
		radius = (max-min).Length() * 0.5f;
	}
}

float
vsDisplayList::GetBoundingRadius()
{
	float maxDistance = 0.f;
	
	if ( m_instanceParent )
	{
		return m_instanceParent->GetBoundingRadius();
	}
	else
	{
		vsTransform currentTransform;
		
		Rewind();
		op *o = PopOp();
		
		while(o)
		{
			if ( o->type == OpCode_MoveTo || o->type == OpCode_LineTo )
			{
				vsVector2D pos = o->data.GetVector2D();
				
				float dist = pos.Length();
				
				if ( dist > maxDistance )
					maxDistance = dist;
			}
			
			o = PopOp();
		}
	}
	return maxDistance;
}


