/*
 *  MEM_Heap.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 8/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef MEM_HEAP_H
#define MEM_HEAP_H

class memBlock
{
public:
	
	void *		m_start;
	void *		m_end;
	size_t		m_size;
	int			m_blockId;
	
	char		m_filename[128];
	int			m_line;
	
	char		m_allocType;
	
	bool		m_used;
	
	memBlock *	m_next;
	memBlock *	m_prev;
	
	memBlock();
	
	void Extract();
	void Append( memBlock *block );
};

enum
{
	Type_Static,
	Type_Malloc,
	Type_New,
	Type_NewArray
};

class memHeap
{
#define MAX_ALLOCATIONS (4096)
	
	void *	m_startOfMemory;
	void *	m_endOfMemory;
	size_t	m_memorySize;
	
	size_t	m_memoryUsed;
	size_t	m_highWaterMark;
	int		m_totalAllocations;
	
	int		m_leakMark;
	
	memBlock	m_blockList;
//	memBlock	m_unusedBlockList;
//	memBlock	m_blockStore[MAX_ALLOCATIONS];
	
	static memHeap * s_current;
	
	memBlock *	FindFreeMemBlockOfSize(size_t size);
//	memBlock *	GetUnusedMemBlock();
	
public:
	memHeap(size_t bufferSize);
	memHeap(void *buffer, int bufferSize);
	~memHeap();
	
	static memHeap *	GetCurrent() { return s_current; }
	bool				Contains(void *p) { return (p >= m_startOfMemory && p < m_endOfMemory); }
	
	void *	Alloc(size_t size, const char *fileName, int line, int allocType);
	void	Free(void *ptr, int allocType);
	
	static void	Push( memHeap *newCurrent );	// push a new allocator context
	static void	Pop( memHeap *oldCurrent = NULL );							// pop it off.
	
	void	PrintStatus();
	void	PrintBlockList();
	void	CheckForLeaks();
	void	TraceMemoryBlocks();
	
	void	SetMarkForLeakTesting() { m_leakMark = m_totalAllocations; }
};


#endif // MEM_HEAP_H
