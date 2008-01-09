/*
 *  MEM_Store.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 12/05/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *q
 */

#include "MEM_Store.h"

#include "VS_Angle.h"
#include "VS_Color.h"
#include "VS_Transform.h"
#include "VS_Vector.h"

#if defined(_WIN32)
#include <winsock2.h>
#elif defined(__GNUC__)
#include <netinet/in.h> // for access to ntohl, et al
#endif

memStore::memStore( int maxSize )
{
	m_buffer = new char[maxSize];
	m_bufferLength = maxSize;
	m_bufferEnd = &m_buffer[m_bufferLength];
	
	m_readHead = m_writeHead = m_buffer;
	m_bufferIsExternal = false;
}

memStore::memStore( char *buffer, int bufferLength )
{
	m_buffer = buffer;
	m_bufferLength = bufferLength;
	m_bufferEnd = &m_buffer[m_bufferLength];

	m_readHead  = m_buffer;
	m_writeHead = m_bufferEnd;
	m_bufferIsExternal = true;
}

memStore::~memStore()
{
	if( !m_bufferIsExternal )
		delete [] m_buffer;
	m_buffer = NULL;
}
/*
int
memStore::BytesLeftForReading()
{
	return ;
}

int
memStore::BytesLeftForWriting()
{
	
}*/

void
memStore::Rewind()
{
	m_readHead = m_buffer;
}

void
memStore::Clear()
{
	m_writeHead = m_readHead = m_buffer;
}

void
memStore::Append( memStore *o )
{
	vsAssert( BytesLeftForWriting() >= o->Length(), "Tried to write past the end of the memStore!" );
	
	memcpy( m_writeHead, o->m_buffer, o->Length() );
	m_writeHead += o->Length();
}

void
memStore::WriteInt8( int8 v )
{
	vsAssert( BytesLeftForWriting() >= sizeof(v), "Tried to write past the end of the memStore!" );
	
	memcpy( m_writeHead, &v, sizeof(v) );
	m_writeHead += sizeof(v);
}

void
memStore::WriteUint8( uint8 v )
{
	vsAssert( BytesLeftForWriting() >= sizeof(v), "Tried to write past the end of the memStore!" );
	
	//memcpy( m_writeHead, &v, sizeof(v) );
	*(uint8*)m_writeHead = v;
	m_writeHead += sizeof(v);
}

void
memStore::WriteInt16( int16 v )
{
	vsAssert( BytesLeftForWriting() >= sizeof(v), "Tried to write past the end of the memStore!" );
	
	v = htons(v);
	memcpy( m_writeHead, &v, sizeof(v) );
	m_writeHead += sizeof(v);
}

void
memStore::WriteUint16( uint16 v )
{
	vsAssert( BytesLeftForWriting() >= sizeof(v), "Tried to write past the end of the memStore!" );
	
	v = htons(v);
	memcpy( m_writeHead, &v, sizeof(v) );
	m_writeHead += sizeof(v);
}

void
memStore::WriteInt32( int32 v )
{
	vsAssert( BytesLeftForWriting() >= sizeof(v), "Tried to write past the end of the memStore!" );
	
	v = htonl(v);
	memcpy( m_writeHead, &v, sizeof(v) );
	m_writeHead += sizeof(v);
}

void
memStore::WriteUint32( uint32 v )
{
	vsAssert( BytesLeftForWriting() >= sizeof(v), "Tried to write past the end of the memStore!" );
	
	v = htonl(v);
	memcpy( m_writeHead, &v, sizeof(v) );
	m_writeHead += sizeof(v);
}

void
memStore::WriteFloat( float v )
{
	vsAssert( BytesLeftForWriting() >= sizeof(v), "Tried to write past the end of the memStore!" );
	
	memcpy( m_writeHead, &v, sizeof(v) );
	m_writeHead += sizeof(v);
}

int8
memStore::ReadInt8()
{
	int8 v;
	vsAssert( BytesLeftForReading() >= sizeof(v), "Tried to read past the end of the memStore!" );
	
	memcpy( &v, m_readHead, sizeof(v) );
	m_readHead += sizeof(v);
	
	return v;
}

uint8
memStore::ReadUint8()
{
	uint8 v;
	vsAssert( BytesLeftForReading() >= sizeof(v), "Tried to read past the end of the memStore!" );
	
	//memcpy( &v, m_readHead, sizeof(v) );
	v = *(uint8 *)m_readHead;
	m_readHead += sizeof(v);
	
	return v;
}


int16
memStore::ReadInt16()
{
	int16 v;
	vsAssert( BytesLeftForReading() >= sizeof(v), "Tried to read past the end of the memStore!" );
	
	memcpy( &v, m_readHead, sizeof(v) );
	v = ntohs(v);
	m_readHead += sizeof(v);
	
	return v;
}


uint16
memStore::ReadUint16()
{
	uint16 v;
	vsAssert( BytesLeftForReading() >= sizeof(v), "Tried to read past the end of the memStore!" );
	
	memcpy( &v, m_readHead, sizeof(v) );
	v = ntohs(v);
	m_readHead += sizeof(v);
	
	return v;
}


int32
memStore::ReadInt32()
{
	int32 v;
	vsAssert( BytesLeftForReading() >= sizeof(v), "Tried to read past the end of the memStore!" );
	
	memcpy( &v, m_readHead, sizeof(v) );
	v = ntohl(v);
	m_readHead += sizeof(v);
	
	return v;
}


uint32
memStore::ReadUint32()
{
	uint32 v;
	vsAssert( BytesLeftForReading() >= sizeof(v), "Tried to read past the end of the memStore!" );
	
	memcpy( &v, m_readHead, sizeof(v) );
	v = ntohl(v);
	m_readHead += sizeof(v);
	
	return v;
}


float
memStore::ReadFloat()
{
	float v;
	vsAssert( BytesLeftForReading() >= sizeof(v), "Tried to read past the end of the memStore!" );
		
	memcpy( &v, m_readHead, sizeof(v) );
	m_readHead += sizeof(v);
	
	return v;
}

void
memStore::WriteVector2D(const vsVector2D &v)
{
	WriteFloat( v.x );
	WriteFloat( v.y );
}

vsVector2D	
memStore::ReadVector2D()
{
	// important note:  when the results of function calls are used as arguments to a function or constructor,
	// their order of operation is NOT DEFINED;  that means that they could be called in any order!  Usually,
	// that doesn't matter.  However, here we might be tempted to just create a result vector using
	// a constructor like this:  result( ReadFloat(), ReadFloat() );.  This might work with some compilers, but
	// will break with others, and yield swapped x and y coordinates!  For this reason, we must assign these values
	// to intermediate values, before reconstructing our vector!
	
	float x = ReadFloat();
	float y = ReadFloat();
	vsVector2D result( x, y );
	
	return result;
}

void
memStore::WriteColor(const vsColor &v)
{
	WriteUint8( (uint8)(255.f * v.r) );
	WriteUint8( (uint8)(255.f * v.g) );
	WriteUint8( (uint8)(255.f * v.b) );
	WriteUint8( (uint8)(255.f * v.a) );
}

vsColor
memStore::ReadColor()
{
	float inv = 1.f / 255.0f;
	
	float r = ReadUint8() * inv;
	float g = ReadUint8() * inv;
	float b = ReadUint8() * inv;
	float a = ReadUint8() * inv;
	
	vsColor v( r, g, b, a );
	
	return v;
}

void
memStore::WriteTransform(const vsTransform &v)
{
	WriteVector2D( v.m_position );
	WriteFloat( v.m_angle.Get() );
	WriteVector2D( v.m_scale );
}

vsTransform	
memStore::ReadTransform()
{
	vsTransform result;
	result.m_position = ReadVector2D();
	result.m_angle.Set( ReadFloat() );
	result.m_scale = ReadVector2D();
	
	return result;
}
