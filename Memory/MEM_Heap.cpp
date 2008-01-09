/*
 *  MEM_Heap.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 8/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "MEM_Heap.h"

memHeap gGlobalHeap(1024 * 1024 * 8);	// 8 meg

memHeap * memHeap::s_current = NULL;

#define MAX_HEAP_STACK (4)
static memHeap *	s_stack[MAX_HEAP_STACK] = {NULL,NULL,NULL,NULL};


memHeap::memHeap(size_t size)
{
	if ( s_current )
		m_startOfMemory = s_current->Alloc(size, __FILE__, __LINE__, Type_Static);
	else
	{
		m_startOfMemory = malloc(size);
		s_current = this;
		Push(this);
	}
	
	m_endOfMemory = (void *)((char *)m_startOfMemory + size);
	m_memorySize = size;
	
	m_memoryUsed = 0;
	m_highWaterMark = 0;
	m_totalAllocations = 0;
	
	m_leakMark = 0;
	
//	for ( int i = 0; i < MAX_ALLOCATIONS; i++ )
//	{
//		m_unusedBlockList.Append( &m_blockStore[i] );
//	}
	
	memBlock *iniBlock = (memBlock *)m_startOfMemory;
	iniBlock->m_start = m_startOfMemory;
	iniBlock->m_end = m_endOfMemory;
	iniBlock->m_size = m_memorySize;
	iniBlock->m_used = false;
	iniBlock->m_next = NULL;
	iniBlock->m_prev = NULL;
	
	m_blockList.Append( iniBlock );
}

memHeap::~memHeap()
{
}

void
memHeap::Push( memHeap *newCurrent )
{
	vsAssert( s_stack[MAX_HEAP_STACK-1] == NULL, "Overflowed stack of memHeaps!" );
	
	for ( int i = MAX_HEAP_STACK-1; i > 0; i-- )
		s_stack[i] = s_stack[i-1];
	s_stack[0] = newCurrent;
		
	s_current = s_stack[0];
}

void
memHeap::Pop( memHeap *oldCurrent /* == NULL */ )
{
	vsAssert( s_stack[1] != NULL, "Underflowed stack of memHeaps!" );

	// Optional safety measure:
	//
	// if someone has specified which memHeap they expect to be popping off,
	// then validate that it's actually the heap they expect it to be.
	if ( oldCurrent != NULL )
		vsAssert( s_stack[0] == oldCurrent, "Expected oldCurrent!" );
	
	for ( int i = 0; i < MAX_HEAP_STACK-1; i++ )
		s_stack[i] = s_stack[i+1];
	s_stack[MAX_HEAP_STACK-1] = NULL;
	
	s_current = s_stack[0];
}

memBlock *
memHeap::FindFreeMemBlockOfSize( size_t size )
{
	memBlock *block = m_blockList.m_next;
	bool foundMemBlockForAlloc = false;
	
	while(block)
	{
		if ( block->m_used == false && block->m_size >= size )
		{
			return block;
		}
		
		block = block->m_next;
	}
	
	vsAssert( foundMemBlockForAlloc, "Out of memory!" );	// if this breaks, we're out of memory, or are suffering from memory fragmentation!
	return NULL;
}

void *
memHeap::Alloc(size_t size, const char *file, int line, int allocType)
{
	size += sizeof( memBlock ) + sizeof( unsigned long );		// we need to allocate enough space for our new 'memBlock' header, and some bytes on the end.
	size = (size+31) & 0xffffffe0;								// round 'size' up to the nearest 32 bytes, to force alignment.
	
	memBlock *block = FindFreeMemBlockOfSize(size);
	
	void * end = (void *)((char *)block->m_start + size);
	
	if ( block->m_size > (int)(size + sizeof( memBlock )) )	// enough extra unused space to allocate another memblock out of it?
	{
		// block is larger than we asked for;  we need to split it.
		memBlock *split = (memBlock *)end;
		split->m_start = end;
		split->m_end = block->m_end;
		split->m_size = block->m_size - size;
		split->m_used = false;
		
		block->m_end = end;
		block->m_size = size;
		
		block->Append(split);
	}
	
	strncpy( block->m_filename, file, 128 );
	block->m_line = line;
	block->m_blockId = m_totalAllocations++;
	block->m_allocType = allocType;
	
	block->m_used = true;
	void *result = (void *)((char *)block->m_start + sizeof(memBlock));
	
	m_memoryUsed += block->m_size;
	if ( m_memoryUsed > m_highWaterMark )
	{
		m_highWaterMark = m_memoryUsed;
		/*if ( m_highWaterMark > 1024 * 1024 )
			TraceMemoryBlocks();*/
	}
	
	// overwrite everything in the user area, to make it really obvious what
	// memory hasn't been initialised by the user.
	memset(result, 0xcdcdcdcd, block->m_size - sizeof( memBlock ) );
	
	// write a special code just past the end of what the user thinks we're
	// giving them.  We'll check that the user hasn't written over our special
	// code when they free the block, as that would indicate that they've overwritten
	// an array or somesuch.  
	void * safety = (void *)((char *)block->m_end - sizeof(unsigned long));
	unsigned long *safetyLong = (unsigned long *)safety;
	*safetyLong = 0xeeeeeeee;
		
	return result;
}

void
memHeap::Free(void *p, int allocType)
{
	p = (void *)((char *)p - sizeof(memBlock));	// adjust pointer to point to the start of its memBlock header
	
	memBlock *block = m_blockList.m_next;
	bool foundBlockToFree = false;
	
	while ( block )
	{
		if ( block->m_start == p )
		{
			// make sure the user hasn't overwritten our code past the end of their memory block.
			void * safety = (void *)((char *)block->m_end - sizeof(unsigned long));
			unsigned long *safetyLong = (unsigned long *)safety;
			vsAssert( *safetyLong == 0xeeeeeeee, "Buffer overflow detected!" );	// if we hit this assert, someone has overwritten the bounds of this memory buffer!
			
			if( block->m_allocType != allocType )
			{
				const char *allocFunction[] =
				{
					"Static alloc",
					"malloc",
					"new",
					"new []"
				};
				const char *freeFunction[] =
				{
					"None",
					"free",
					"delete",
					"delete []"
				};
				printf("Error:  Allocation from %s line %d was allocated using %s\n", block->m_filename, block->m_line, allocFunction[block->m_allocType]);
				printf("Error:   but was freed using %s;  should have been %s!\n", freeFunction[allocType], freeFunction[block->m_allocType]);
			}
			
			// check if we can merge together with the prev or the next block.
			void *	userArea = (void *)((char *)block->m_start + sizeof(memBlock));
			size_t	userSize = block->m_size - sizeof(memBlock);
			memset(userArea, 0xdddddddd, userSize );
			block->m_used = false;
			m_memoryUsed -= block->m_size;
			
			if ( block->m_next && !block->m_next->m_used )
			{
				memBlock *nextBlock = block->m_next;
				
				// next block isn't being used;  let's merge it into us!
				block->m_end = nextBlock->m_end;
				block->m_size += nextBlock->m_size;
				nextBlock->Extract();
			}
			if ( block->m_prev && block->m_prev != &m_blockList && !block->m_prev->m_used )
			{
				// previous block isn't being used;  let's merge ourself into it!
				block->m_prev->m_end = block->m_end;
				block->m_prev->m_size += block->m_size;
				block->Extract();
			}
			foundBlockToFree = true;
			return;
		}
		block = block->m_next;
	}
	
	vsAssert(foundBlockToFree, "Tried to free a block we didn't allocate!");	// tried to free a block we didn't know about!
}

void
memHeap::PrintStatus()
{
	printf(" >> MEMORY STATUS\n");
	printf(" >> Heap current usage %lu / %lu (%0.2f%% usage)\n", m_memoryUsed, m_memorySize, 100.0f*m_memoryUsed/m_memorySize);
	printf(" >> Heap highwater usage %lu / %lu (%0.2f%% usage)\n", m_highWaterMark, m_memorySize, 100.0f*m_highWaterMark/m_memorySize);
	
	size_t bytesFree = m_memorySize - m_memoryUsed;
	size_t largestBlock = 0;
	
	memBlock *block = m_blockList.m_next;
	
	while ( block )
	{
		if ( !block->m_used && block->m_size > largestBlock )
			largestBlock = block->m_size;
		
		block = block->m_next;
	}
	
	printf(" >> Heap largest free block %lu / %lu bytes free (%0.2f%% fragmentation)\n", largestBlock, bytesFree, 100.0f - (100.0f*largestBlock / bytesFree) );
}

void
memHeap::CheckForLeaks()
{
	bool foundLeak = false;
	memBlock *block = m_blockList.m_next;
	
	while ( block )
	{
		if ( block->m_used && block->m_blockId > m_leakMark )
		{
			if ( !foundLeak )
			{
				vsLog("\nERROR:  LEAKS DETECTED!\n-------------------\nLeaked blocks follow:\n");
				foundLeak = true;
			}
			 vsLog("[%d] %s:%d : %d bytes", block->m_blockId, block->m_filename, block->m_line, block->m_size);
		}
		block = block->m_next;
	}
	
	vsAssert(!foundLeak, "Memory leaks found!  Details to stdout.");
}

void
memHeap::TraceMemoryBlocks()
{
	memBlock *block = m_blockList.m_next;
	
	while ( block )
	{
		if ( block->m_used )
		{
			vsLog("[%d] %s:%d : %lu bytes", block->m_blockId, block->m_filename, block->m_line, block->m_size);
		}
		block = block->m_next;
	}
}

void
memHeap::PrintBlockList()
{
	memBlock *block = m_blockList.m_next;
	
	while ( block )
	{
		printf("[%d] %s:%d : %lu bytes\n", block->m_blockId, block->m_filename, block->m_line, block->m_size);
		block = block->m_next;
	}
}

memBlock::memBlock():
m_start(0),
m_end(0),
m_size(0),
m_used(false),
m_next(NULL),
m_prev(NULL)
{
}


void
memBlock::Append( memBlock *block )
{
	block->m_next = m_next;
	block->m_prev = this;
	
	if ( m_next )
		m_next->m_prev = block;
	m_next = block;
}

void
memBlock::Extract()
{
	if ( m_next )
		m_next->m_prev = m_prev;
	if ( m_prev )
		m_prev->m_next = m_next;
}


#undef new
#undef INGAME_NEW



#undef malloc
#undef free
void * MyMalloc(size_t size, const char *fileName, int lineNumber, int allocType)
{
	void * result;
	
	if ( memHeap::GetCurrent() )
	{
		result = memHeap::GetCurrent()->Alloc(size, fileName, lineNumber, allocType);
//		printf("Allocating %s:%d, 0x%x\n", fileName, lineNumber, (unsigned int)result);
		return result;
	}
	
	return malloc(size);
}

void MyFree(void *p, int allocType)
{
//	printf("Deallocating 0x%x\n", (unsigned int)p);
	if ( memHeap::GetCurrent() )
	{
		if ( memHeap::GetCurrent()->Contains(p) )
			return memHeap::GetCurrent()->Free(p, allocType);
	}
	
	return free(p);
}


void* operator new (size_t size, const char *file, int line)
{
	void *result = MyMalloc(size, file, line, Type_New); 
	return result;
}

void* operator new[] (size_t size, const char *file, int line)
{
	size_t actualSize = size;	// four extra bytes at start, and four at end
	
	void *result = MyMalloc(actualSize, file, line, Type_NewArray); 
	return result;
}

void operator delete (void *ptr, const char *file, int line)
{
	UNUSED(file);
	UNUSED(line);
	
	MyFree(ptr, Type_New); 
}

void operator delete (void *ptr)
{
	MyFree(ptr, Type_New); 
}

void operator delete[] (void *ptr)
{
	MyFree(ptr, Type_NewArray); 
}

void operator delete[] (void *ptr, const char *file, int line)
{
	UNUSED(file);
	UNUSED(line);

	MyFree(ptr, Type_NewArray); 
}
