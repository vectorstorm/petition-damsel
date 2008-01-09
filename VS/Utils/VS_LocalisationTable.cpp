/*
 *  VS_LocalisationTable.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/05/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_LocalisationTable.h"

#include "FS_File.h"

vsLocalisationTable::vsLocalisationTable()
{
	m_string = NULL;
	m_stringCount = 0;
}

vsLocalisationTable::~vsLocalisationTable()
{
}

void
vsLocalisationTable::Init()
{
	fsFile table("string.table");
	
	vsString str;
	
	while( table.ReadLine(&str) )
	{
		
	}
}

void
vsLocalisationTable::Deinit()
{
	vsDeleteArray( m_string );
	
	m_stringCount = 0;
}


