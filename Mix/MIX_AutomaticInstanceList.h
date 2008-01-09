/*
 *  MIX_AutomaticInstanceList.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 31/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef MIX_AUTOMATICINSTANCELIST_H
#define MIX_AUTOMATICINSTANCELIST_H

/** An automatically maintained list of instances for a type.
*
*  To add the ability to enumerate through all instances of a particular type,
*  derive that type from this template. 
*
*  This class will work perfectly with multiple classes in a (single or multiple)
*  inheritance tree requiring an automatic instance list. 
*  You will just need to provide the appropriate scope resolution to identiy
*  which automatic list you are referring to (as you would expect!).
*
*\code
*   class SomeClass : mixAutomaticInstanceList<SomeClass> {};
*   class DerivedClass : public SomeClass, public
mixAutomaticInstanceList<DerivedClass> {};
*
*   SomeClass a, b, c;
*   DerivedClass d, e, f;
*
*   for( SomeClass *instance = SomeClass::getFirstInstance(); 
		 *        instance != NULL; 
		 *        instance = instance->getNextInstance() )
*   {
	*      // ...
	*   }
*
*   SomeClass *someClassInstance =
mixAutomaticInstanceList<SomeClass>::getFirstInstance;
*   DerivedClass *derivedClassInstance =
mixAutomaticInstanceList<DerivedClass>::getFirstInstance;
*\endcode
*
*\ingroup qcl_types_utility
*/
template <class T>
class mixAutomaticInstanceList
{
public:
    mixAutomaticInstanceList();
    mixAutomaticInstanceList( const mixAutomaticInstanceList<T> &otherObject );
    virtual ~mixAutomaticInstanceList();
	mixAutomaticInstanceList<T> &operator =( const mixAutomaticInstanceList<T> &otherObject );
	
    T *GetNextInstance() const;
    T *GetPreviousInstance() const;
	
	static T      *GetFirstInstance();
    static uint32  GetInstanceCount();
	
private:
		static T *s_firstInstance;
    T *m_previousInstance;
    T *m_nextInstance;
};

template<class T> T *mixAutomaticInstanceList<T>::s_firstInstance;
/**< Static variable used to hold the head of the list.
*/



/** Adds this instance to the automatic instance list for this type.
*/
template<class T>
mixAutomaticInstanceList<T>::mixAutomaticInstanceList()
{
	const size_t offset = (size_t)(T*)1 - (size_t)(mixAutomaticInstanceList<T>*)(T*)1;
	T *thisT = (T*)((size_t)this + offset);
	
	m_previousInstance = NULL;
	m_nextInstance = s_firstInstance;
	if( m_nextInstance )
	{
		m_nextInstance->m_previousInstance = thisT;
	}
	
	s_firstInstance = thisT;
}

/** Adds this instance to the automatic instance list for this type.
*/
template<class T>
mixAutomaticInstanceList<T>::mixAutomaticInstanceList( const mixAutomaticInstanceList<T>
												   &otherObject )
{
	const size_t offset = (size_t)(T*)1 - (size_t)(mixAutomaticInstanceList<T>*)(T*)1;
	T *thisT = (T*)((size_t)this + offset);
	
	m_previousInstance = NULL;
	m_nextInstance = s_firstInstance;
	if( m_nextInstance )
	{
		m_nextInstance->m_previousInstance = thisT;
	}
	
	s_firstInstance = thisT;
}

/** Prevent the compiler-generated assignment operator from trashing the instance
links.
*/
template<class T>
mixAutomaticInstanceList<T> &
mixAutomaticInstanceList<T>::operator =( const mixAutomaticInstanceList<T> &otherObject )
{
	// nothing to do
	return *this;
}


/** Remove this instance to the automatic instance list for this type.
*/
template<class T>
mixAutomaticInstanceList<T>::~mixAutomaticInstanceList()
{
	const size_t offset = (size_t)(T*)1 - (size_t)(mixAutomaticInstanceList<T>*)(T*)1;
	T *thisT = (T*)((size_t)this + offset);
	
	// must be at least one instance if *we're* destructing!
	assert( s_firstInstance != NULL );
	
	//
	// unhook ourselves from the list
	//
	if( m_previousInstance )
	{
		assert( thisT == m_previousInstance->m_nextInstance );
		m_previousInstance->m_nextInstance = m_nextInstance;
	}
	else
	{
		assert( thisT == T::s_firstInstance );
		T::s_firstInstance = m_nextInstance;
	}
	if( m_nextInstance )
	{
		assert( m_nextInstance->m_previousInstance == thisT );
		m_nextInstance->m_previousInstance = m_previousInstance;
	}
}


/** Returns the first instance of type T, or NULL if no instances exist.
*
*  Note that the "first" instance only corresponds to the instance's
*  position in the instance list, and bears no guaranteed relation to the age
*  of the instance in relation to other instances.
*/
template<class T> T*
mixAutomaticInstanceList<T>::GetFirstInstance()
{
	return s_firstInstance;
}

/** Returns the next instance of type T, or NULL if this is the last instance in the
list.
*/
template<class T> T* 
mixAutomaticInstanceList<T>::GetNextInstance() const
{ 
	return m_nextInstance;
}



/** Returns the previous instance of type T, or NULL if this is the first instance
in the list.
*/
template<class T> T* 
mixAutomaticInstanceList<T>::GetPreviousInstance() const
{ 
	return m_previousInstance;
}

/** Returns how many instances are in the list. 
*
*\note this function iterates through the entire list, 
*      so don't call it every time through your loop!!!
*/
template<class T> uint32
mixAutomaticInstanceList<T>::GetInstanceCount()
{
	uint32 count = 0;
	T *instance = getFirstInstance();
	while( instance )
	{
		count++;
		instance = instance->getNextInstance();
	}
	return count;
}

#endif // MIX_AUTOMATICINSTANCELIST_H
