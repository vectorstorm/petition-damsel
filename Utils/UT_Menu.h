/*
 *  UT_Menu.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 29/12/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef UT_MENU_H
#define UT_MENU_H

#include "VS_Sprite.h"

	/** utMenu
	 * Utility class for rendering and interacting with menus.
	 * Instantiate and register as normal for a vsEntity-type class.
	 * 
	 */

class utMenuAction
{
public:
	
	enum Type
	{
		Type_None,
		Type_Select,
		Type_Cancel,
		Type_Left,
		Type_Right
	};
	
	Type	type;
	int		menuItemId;

	utMenuAction() { Clear(); }
	
	void Clear() { type = Type_None; menuItemId = -1; }
	void Select(int id) { type = Type_Select; menuItemId = id; }
	void Cancel() { type = Type_Cancel; }
	void Left(int id) { type = Type_Left; menuItemId = id; }
	void Right(int id) { type = Type_Right; menuItemId = id; }
};

class utMenu : public vsSprite
{
	typedef vsSprite Parent;
	
	vsSprite **				m_itemLabel;
	vsSprite **				m_itemValue;
	int						m_itemCount;
	
	int						m_hilitedId;	// currently hilited item
	utMenuAction			m_action;
	
	float					m_pulseTimer;	// the currently hilited item pulses, to let us know which one is selected right now.
	
	float					m_letterSize;
	float					m_capSize;
	float					m_lineSpacing;

	
	void					ArrangeItems();
	
public:
	
							utMenu(int count, float letterSize = 35.0f, float capSize = 45.0f, float lineSpacing = 20.0f);	// how many menu options in this menu?
	virtual					~utMenu();
	
	void					Enter() { m_hilitedId = 0; }
	
	void					SetItemCount(int count);
	
	virtual void			Update(float timeStep);
	virtual void			Draw( vsDisplayList *list );
	
	int						GetHilitedItem() { return m_hilitedId; }
	
	bool					WasActionTaken() { return (GetAction().type != utMenuAction::Type_None); }
	const utMenuAction &	GetAction() { return m_action; }
	
	void					SetItemLabel( int itemId, const vsString & label );
	void					SetItemValue( int itemId, const vsString & value );
};

#endif // UT_MENU_H

