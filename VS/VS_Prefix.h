/*
 *  VS_Prefix.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 8/04/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_PREFIX_H
#define VS_PREFIX_H


#ifndef NULL
#define NULL (0L)
#endif //

#define BIT(x) (1<<x)

#ifdef UNUSED
//#elif defined(__GNUC__)
#elif defined(__cplusplus)
#define UNUSED (void)
//#define UNUSED(x) x __attribute__((unused))
//# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
# define UNUSED(x) x
#endif

// Define our 'vsTuneable' type, which lets us change tuning values at a breakpoint,
// while still compiling down to 'const' in release builds.
#if defined(_DEBUG)
#define vsTuneable static
#else
#define vsTuneable const
#endif

#if defined(__cplusplus)
#include <string>
#include <exception> // for std::bad_alloc
#include "VS_Debug.h"
#include "VS_Math.h"
#include "VS_String.h"
#include "VS_Log.h"

void * MyMalloc(size_t size, const char*fileName, int lineNumber, int allocType = 1);	// 1 == Malloc type.  We can ignore this.  :)
void MyFree(void *p, int allocType = 1);

void* operator new (size_t size, const char *file, int line);
void* operator new[] (size_t size, const char *file, int line);
void operator delete (void *ptr, const char *file, int line);
void operator delete[] (void *ptr, const char *file, int line);
void operator delete (void *ptr);
void operator delete[] (void *ptr);

#define vsDelete(x) { delete x; x = NULL; }

#define vsDeleteArray(x) { delete [] x; x = NULL; }

#define INGAME_NEW new(__FILE__, __LINE__)
#define new INGAME_NEW

#define malloc(x) MyMalloc(x, __FILE__, __LINE__)
#define free(p) MyFree(__FILE__, __LINE__, (p))


#if defined(_WIN32)

typedef signed char int8;
typedef unsigned char uint8;
typedef signed int int16;
typedef unsigned int uint16;
typedef signed long int32;
typedef unsigned long uint32;

#else // __GNUC__, __APPLE_CC__, and probably everything else?

typedef signed char int8;
typedef unsigned char uint8;
typedef signed int int16;
typedef unsigned int uint16;
typedef signed long int32;
typedef unsigned long uint32;

#endif	// normal Linux/BSD/etc section

#endif // __cplusplus

#endif // VS_PREFIX_H
