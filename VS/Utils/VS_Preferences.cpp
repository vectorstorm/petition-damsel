/*
 *  VS_Preferences.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 23/12/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "VS_Preferences.h"

#include "FS_File.h"
#include "FS_Record.h"

vsPreferences::vsPreferences(const vsString &filename_in)
{
	vsString filename = vsFormatString("%s.prefs",filename_in.c_str());
	m_filename = filename;
	
	m_preferenceList = new vsPreferenceObject;
	
	if ( fsFile::Exists(filename) )	// if the file exists, then read the current pref values out of it.
	{
		fsRecord record;
		fsFile prefsFile(filename);
		
		while( prefsFile.Record(&record) )
		{
			vsAssert( record.GetArgCount() == 3, vsFormatString("%s preference has wrong number of arguments!", record.GetLabel().c_str() ));
			
			AddPreference( record.GetLabel(), record.GetArgAsInt(0), record.GetArgAsInt(1), record.GetArgAsInt(2));
		}
	}
}

vsPreferences::~vsPreferences()
{
	vsPreferenceObject *s = m_preferenceList->m_next;
	vsPreferenceObject *next;
	while ( s != m_preferenceList )
	{
		next = s->m_next;
		s->Extract();
		delete s;
		s = next;
	}
	
	delete m_preferenceList;
}

vsPreferenceObject *
vsPreferences::FindPreference(const vsString &label)
{
	vsPreferenceObject *s = m_preferenceList->m_next;
	
	while ( s != m_preferenceList )
	{
		if ( s->m_label == label )
			return s;
		s = s->m_next;
	}
	
	return NULL;
}

vsPreferenceObject *
vsPreferences::AddPreference(const vsString &label, int value, int minValue, int maxValue)
{
	vsPreferenceObject *newObj = new vsPreferenceObject;
	newObj->m_label = label;
	newObj->m_value = value;
	newObj->m_min = minValue;
	newObj->m_max = maxValue;
	
	newObj->Validate();
	
	m_preferenceList->Insert(newObj);
	
	return newObj;
}

void
vsPreferences::Save()
{
	fsRecord record;
	fsFile prefsFile(m_filename, fsFile::MODE_Write);
	
	vsPreferenceObject *s = m_preferenceList->m_next;
	
	while( s != m_preferenceList )
	{
		record.Init();
		
		record.SetLabel( s->m_label );
		record.SetArgCount(3);
		record.SetArgAsInt(0, s->m_value);
		record.SetArgAsInt(1, s->m_min);
		record.SetArgAsInt(2, s->m_max);
		
		prefsFile.Record(&record);
		
		s = s->m_next;
	}
}

vsPreferenceObject *	
vsPreferences::GetPreference(const vsString &label, int defaultValue, int minValue, int maxValue)
{
	// first, let's find the relevant preference.
	
	vsPreferenceObject *o = FindPreference(label);
	if ( o )
	{
		o->m_min = minValue;
		o->m_max = maxValue;
		
		if ( o->m_value < minValue || o->m_value > maxValue )
			o->m_value = defaultValue;		// we seem to have changed the limits for this value, such that the saved value is no longer legal.  Switch it to the new default value.
		
		o->Validate();
	}
	else
	{
		o = AddPreference(label, defaultValue, minValue, maxValue);
	}
	
	return o;
}


vsPreferenceObject::vsPreferenceObject():
	m_label("NULL"),
	m_value(0),
	m_min(0),
	m_max(0)
{
	m_next = this;
	m_prev = this;
}

void
vsPreferenceObject::Insert( vsPreferenceObject *next )
{
	next->m_next = m_next;
	next->m_prev = this;
	
	m_next->m_prev = next;
	m_next = next;
}

void
vsPreferenceObject::Extract()
{
	m_next->m_prev = m_prev;
	m_prev->m_next = m_next;
	
	m_next = m_prev = this;
}

void
vsPreferenceObject::Validate()
{
	vsAssert(m_value >= m_min, "Preference value too low!");
	vsAssert(m_value <= m_max, "Preference value too high!");
}

