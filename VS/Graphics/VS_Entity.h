/*
 *  VS_Entity.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 12/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_ENTITY_H
#define VS_ENTITY_H

class vsDisplayList;

class vsEntity
{
protected:
	
	vsEntity *		m_next;
	vsEntity *		m_prev;
	
	vsEntity *		m_parent;
	vsEntity *		m_child;
	
	bool			m_visible;
	
	void			DrawChildren( vsDisplayList *list );
	
	virtual void	_Draw( vsDisplayList *list ) {UNUSED(list);}
	
public:
		vsEntity();
	virtual ~vsEntity();
	
	virtual void	Update( float timeStep );
	virtual void	Draw( vsDisplayList *list );
	
	void			RegisterOnLayer(int layer);
#if defined(_DEBUG)
	void			RegisterOnDebugLayer();
#endif // _DEBUG
	void			Unregister() { Extract(); }
	
	void			SetVisible(bool visible) { m_visible = visible; }
	bool			GetVisible() { return m_visible; }
	
	vsEntity *		GetNext() { return m_next; }
	vsEntity *		GetPrev() { return m_prev; }
	void			Append( vsEntity *sprite );
	void			Prepend( vsEntity *sprite );
	void			AddChild( vsEntity *sprite );
	void			RemoveChild( vsEntity *sprite );
	
	void			MoveToTop();
	void			MoveToBottom();
	
	void			Extract();
};

#endif // VS_ENTITY_H
