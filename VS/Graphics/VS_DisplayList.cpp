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
#include "FS_Record.h"
#include "MEM_Store.h"

static vsString g_opCodeName[vsDisplayList::OpCode_MAX] =
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
vsDisplayList::Load( const vsString &filename )
{
	vsDisplayList *loader = new vsDisplayList(1024 * 50);	// 50k should be enough to load in just about anything.  (famous  last words)
	
	fsFile *file = new fsFile(filename);
	fsRecord r;
	
	while( file->Record(&r) )
	{
		vsString label = r.GetLabel();
		
		for ( int i = 0; i < OpCode_MAX; i++ )
		{
			if ( label == g_opCodeName[i] )
			{
				OpCode code = (OpCode)i;
								
				switch(code)
				{
					case OpCode_SetColor:
					{
						vsColor color = r.Color();
						loader->SetColor(color);
						break;
					}
					case OpCode_MoveTo:
					{
						vsVector2D pos = r.Vector2D();
						loader->MoveTo(pos);
						break;
					}
					case OpCode_LineTo:
					{
						vsVector2D pos = r.Vector2D();
						loader->LineTo(pos);
						break;
					}
					case OpCode_DrawPoint:
					{
						vsVector2D pos = r.Vector2D();
						loader->DrawPoint(pos);
						break;
					}
					case OpCode_PushTransform:
					case OpCode_PopTransform:
					case OpCode_SetCameraTransform:
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

vsDisplayList::vsDisplayList( int memSize ):
/*	m_instructionCount(0),
	m_nextInstruction(0),
	m_maxInstructions(maxInstructions),*/
	m_instanceParent(NULL),
	m_instanceCount(0)
{
	if ( memSize )
	{
		m_fifo = new memStore(memSize);
//		m_instruction = new op[maxInstructions];
		
		Clear();
	}
	else
		m_fifo = NULL;
//		m_instruction = NULL;
}

vsDisplayList::~vsDisplayList()
{
	vsAssert( m_instanceCount == 0, "Deleted a display list while something was still referencing it!" );

	if ( m_fifo )
	{
		delete m_fifo;
		m_fifo = NULL;
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

int
vsDisplayList::GetSize() const
{
	return m_fifo->Length();
}

int
vsDisplayList::GetMaxSize() const
{
	return m_fifo->BufferLength();
}

void
vsDisplayList::Clear()
{
	m_fifo->Clear();
}

void
vsDisplayList::Rewind()
{
	m_fifo->Rewind();
}

void
vsDisplayList::SetColor( const vsColor &color )
{
	m_fifo->WriteUint8( OpCode_SetColor );
	m_fifo->WriteColor( color );
}

void
vsDisplayList::MoveTo( const vsVector2D &pos )
{
	m_fifo->WriteUint8( OpCode_MoveTo );
	m_fifo->WriteVector2D( pos );
}

void
vsDisplayList::LineTo( const vsVector2D &pos )
{
	m_fifo->WriteUint8( OpCode_LineTo );
	m_fifo->WriteVector2D( pos );
}

void
vsDisplayList::DrawPoint( const vsVector2D &pos )
{
	m_fifo->WriteUint8( OpCode_DrawPoint );
	m_fifo->WriteVector2D( pos );
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
		m_fifo->Append( list.m_fifo );
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
	m_fifo->WriteUint8( OpCode_PushTransform );
	m_fifo->WriteTransform( t );
}

void
vsDisplayList::SetCameraTransform( const vsTransform &t )
{
	m_fifo->WriteUint8( OpCode_SetCameraTransform );
	m_fifo->WriteTransform( t );
}

void
vsDisplayList::PopTransform()
{
	m_fifo->WriteUint8( OpCode_PopTransform );
}

vsDisplayList::op *
vsDisplayList::PopOp()
{
	if ( !m_fifo->AtEnd() )
	{
		m_currentOp.type = (OpCode)m_fifo->ReadUint8();
		
		switch( m_currentOp.type )
		{
			case OpCode_SetColor:
				m_currentOp.data.Set( m_fifo->ReadColor() );
				break;
			case OpCode_MoveTo:
			case OpCode_LineTo:
			case OpCode_DrawPoint:
				m_currentOp.data.Set( m_fifo->ReadVector2D() );
				break;
			case OpCode_PushTransform:
			case OpCode_SetCameraTransform:
				m_currentOp.data.Set( m_fifo->ReadTransform() );
				break;
			case OpCode_PopTransform:
			default:
				break;
		}
		
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


