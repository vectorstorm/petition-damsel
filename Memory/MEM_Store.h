/*
 *  MEM_Fifo.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 12/05/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef MEM_STORE_H
#define MEM_STORE_H

class vsColor;
class vsTransform;
class vsVector2D;

class memStore
{
	char *		m_buffer;
	int			m_bufferLength;	// how long is this buffer, really?
	char *		m_bufferEnd;
	
	char *		m_readHead;
	char *		m_writeHead;
	
	bool		m_bufferIsExternal;
	
	inline int			BytesLeftForWriting() { return (int)(m_bufferEnd - m_writeHead); }
	inline int			BytesLeftForReading() { return (int)(m_bufferEnd - m_readHead); }
	
public:
			memStore( int maxSize );
			memStore( char *buffer, int bufferLength );
	virtual ~memStore();
	
	bool	AtEnd()			{ return (m_readHead == m_writeHead); }
	int		Length()		{ return (int)(m_writeHead - m_buffer); }
	int		BufferLength()	{ return m_bufferLength; }
	void	Rewind();	// rewind to the start
	void	Clear();	// rewind to the start, and erase our contents.
	
	void	Append( memStore *o );		// add contents of 'o' to my buffer.
	
	void	WriteInt8(int8 value);
	void	WriteUint8(uint8 value);

	void	WriteInt16(int16 value);
	void	WriteUint16(uint16 value);

	void	WriteInt32(int32 value);
	void	WriteUint32(uint32 value);

	void	WriteFloat(float value);
	
	int8	ReadInt8();
	uint8	ReadUint8();
	
	int16	ReadInt16();
	uint16	ReadUint16();

	int32	ReadInt32();
	uint32	ReadUint32();

	float	ReadFloat();
	
	
	void		WriteVector2D(const vsVector2D &v);
	vsVector2D	ReadVector2D();
	
	void		WriteColor(const vsColor &v);
	vsColor		ReadColor();

	void		WriteTransform(const vsTransform &v);
	vsTransform	ReadTransform();
};

#endif // MEM_FIFO_H

