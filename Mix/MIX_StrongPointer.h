#ifndef MIX_TYPES_POINTERS_STRONGPOINTER_H
#define MIX_TYPES_POINTERS_STRONGPOINTER_H

#include "MIX_StrongPointerTarget.h"

/** A smart pointer to class T with strong semantics.
 *
 *  The T type must have been derived from mixStrongPointerTarget.
 *
 *  This type of pointer:
 *    - reference count the objects they point at
 *    - automatically delete objects when the last strong pointer disappears
 *
 *  Apart from that, they behave just like an ordinary pointer. Enjoy!
 *
 *	Be wary of using a mixStrongPointer in conjunction with a qCopyOnWritePointer!
 *
 *\ingroup qcl_types_pointers
 */
template< class T >
class mixStrongPointer
{
public:
				mixStrongPointer( T *object = NULL );
				mixStrongPointer( const mixStrongPointer<T> &sp );
	virtual    ~mixStrongPointer();

	mixStrongPointer<T> &	operator = ( const mixStrongPointer<T> &sp );
	mixStrongPointer<T> &	operator = ( T *object );

				operator const T *() const;
				operator       T *();

			T *	operator->();
	  const T *	operator->() const;


private:
	T *m_target;
};




/** Construct a new mixStrongPointer from a dumb pointer.
 *  The object being attached do won't necessarily have a 0 reference count!
 */
template< class T >
mixStrongPointer<T>::mixStrongPointer( T *object )
 :	m_target( object )
{
	if( m_target )
	{
		m_target->mixStrongPointerTarget::addReference();
	}
}


/** Copy constructor.
 */
template< class T >
mixStrongPointer<T>::mixStrongPointer( const mixStrongPointer<T> &sp )
:	m_target( sp.m_target )
{
	if( m_target )
	{
		m_target->mixStrongPointerTarget::addReference();
	}
}

/** Destructor. Will release the object if this is the last pointer!
 */
template< class T >
mixStrongPointer<T>::~mixStrongPointer()
{
	if( m_target )
	{
		if( m_target->mixStrongPointerTarget::releaseReference() == 0 )
		{
			// last object, bye!
			delete m_target;
		}
	}
}


/** Standard assignment.
 *   If this pointer was the last pointing to its object,
 *   that object will be deleted when the new pointer is assigned.
 */
template< class T >
mixStrongPointer<T> &
mixStrongPointer<T>::operator=( const mixStrongPointer &sp ) 
{ 
	if( this == &sp )
	{
		// self assignment, sod off!
		return *this;
	}

	// release the old reference
	if( m_target )
	{
		if( m_target->mixStrongPointerTarget::releaseReference() == 0 )
		{
			// last pointer, bye!
			delete m_target;
		}
	}

	// get the new reference
	m_target = sp.m_target;
	if( m_target )
	{
		m_target->mixStrongPointerTarget::addReference();
	}

	return *this; 
}


/**  Standard assignment.
 *   If this pointer was the last pointing to its object,
 *   that object will be deleted when the new pointer is assigned.
 */
template< class T >
mixStrongPointer<T> &
mixStrongPointer<T>::operator=( T *object ) 
{ 
	// release the old reference
	if( m_target )
	{
		if( m_target->mixStrongPointerTarget::releaseReference() == 0 )
		{
			// last object, bye!
			delete m_target;
		}
	}

	// get the new reference
	m_target = object;
	if( m_target )
	{
		m_target->mixStrongPointerTarget::addReference();
	}

	return *this; 
}


/**  Writable access to the object. 
 *   Requires a non-const mixStrongPointer.
 */
template< class T >
mixStrongPointer<T>::operator T *() 
{ 
	return m_target;
}


/**  Read-only access to the object. 
 */
template< class T >
mixStrongPointer<T>::operator const T *() const
{ 
	return m_target;
}


/**  Arrow operator - write access to the members of the object pointed at. 
 *   Only safe to use on non-NULL pointers.
 *   Requires a non-const mixStrongPointer.
 */
template< class T >
T *
mixStrongPointer<T>::operator->() 
{ 
	assert( m_target != NULL );
	return m_target; 
}

/**  Const arrow operator - read access to the members of the object pointed at. 
 *   Only safe to use on non-NULL pointers.
 */
template< class T >
const T *
mixStrongPointer<T>::operator->() const
{ 
	assert( m_target != NULL );
	return m_target; 
}


#endif // MIX_TYPES_POINTERS_STRONGPOINTER_H
