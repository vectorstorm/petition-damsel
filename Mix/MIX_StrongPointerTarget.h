#ifndef MIX_TYPES_POINTERS_STRONGPOINTERTARGET_H
#define MIX_TYPES_POINTERS_STRONGPOINTERTARGET_H

/** Parent class for objects that want to be pointed at by a strong pointer.
 *
 *  Strong pointers keep the object they are pointing to alive.
 *
 *  A class needs to derive from mixStrongPointerTarget if it wants to be referred to
 *  by a strong pointer, and is happy that the instance will disappear when the
 *  last strong pointer loses scope. (If no strong pointer is ever created,
 *  the instance can delete itself some other way, like via the stack).
 *
 *  Strong pointers include: mixStrongPointer
 *
 *\ingroup mix_types_pointers
 */

class mixStrongPointerTarget
{
public:
	mixStrongPointerTarget() { m_referenceCount = 0; }
			///< Initialize the reference count to 0.

	mixStrongPointerTarget( const mixStrongPointerTarget &otherCount ) { *this = otherCount; }
			///< Copy constructor. Note that this initialize the reference count to 0!

	mixStrongPointerTarget &operator=( const mixStrongPointerTarget &otherCount ) { if( this != &otherCount ) { m_referenceCount = 0; }  return *this; }
			///< Copy constructor. Note that this initialize the reference count to 0!

	virtual ~mixStrongPointerTarget() { assert( m_referenceCount == 0 ); }
			///< Destructor. Asserts the reference count is 0.

	int addReference() const { assert( m_referenceCount >= 0 ); return ++ m_referenceCount; }
			///< Adds a reference, returns the new reference count.

	int releaseReference() const { assert( m_referenceCount > 0 ); return -- m_referenceCount; }
			///< Removes a reference, returns the new reference count.

	int getReferenceCount() const { assert( m_referenceCount >= 0 ); return m_referenceCount; }
			///< Returns the reference count (obscure!)
	
private:
	mutable int m_referenceCount;
};

#endif // MIX_TYPES_POINTERS_STRONGPOINTERTARGET_H
