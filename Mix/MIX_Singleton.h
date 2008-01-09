/*
 *  MIX_Singleton.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 31/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef MIX_SINGLETON_H
#define MIX_SINGLETON_H

#include <assert.h>

/** Base mix-in class for singleton types.
*
*  A singleton is a type that only allows a single instance.
*
*  YourType must derive from mixSingleton<YourType>.
*
*\ingroup vs_types_mixins
*/
template <typename T> 
class mixSingleton
{
public:
	/** Default constructor. Asserts if not the only instance. */
    mixSingleton()
{
        assert( !s_instance );
        const size_t offset = (size_t)(T*)1 - (size_t)(mixSingleton<T>*)(T*)1;
        s_instance = (T*)((size_t)this + offset);
}

/** Default destructor. Asserts if not destructing the expected only instance. */
~mixSingleton()
{  
	const size_t offset = (size_t)(T*)1 - (size_t)(mixSingleton<T>*)(T*)1;
	assert( s_instance == (T*)((size_t)this + offset) );  
	s_instance = 0;  
}

/** Returns true if the instance exists. */
static bool Exists()
{
	return s_instance != NULL;
}

/** Returns a reference to the instance. 
*  It is not legal to call this function if the instance does not exist. 
*/
static T& Singleton()
{  
	assert( s_instance );  
	return *s_instance;  
}

/** Returns a pointer to the instance.  
*  Returns NULL if the instance does not exist.
*  This is legal to do, as opposed to GetSingleton(), which is not legal to call
*  when the instance does not exist.
*/ 
static T* Instance()
{  
	return s_instance;  
}

private:
static T *s_instance;
};

template <typename T> T* mixSingleton <T>::s_instance = 0;

#endif // MIX_SINGLETON_H
