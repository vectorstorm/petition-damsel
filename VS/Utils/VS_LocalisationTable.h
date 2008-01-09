/*
 *  VS_LocalisationTable.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/05/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef VS_LOCALISATION_TABLE_H
#define VS_LOCALISATION_TABLE_H

#include "MIX_Singleton.h"

enum
{
	Lang_English,
	Lang_French,
	Lang_Spanish,
	Lang_German,
	Lang_MAX
};

class vsLocalisationTable : public mixSingleton<vsLocalisationTable>
{
	vsString	*m_string;
	
	int			m_stringCount;
	
public:
	
		vsLocalisationTable();
		~vsLocalisationTable();
		
	void Init();
	void Deinit();
};


#endif // VS_LOCALISATION_TABLE_H

