/*
 *  FS_File.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "FS_File.h"
#include "FS_Record.h"
#include "Core.h"

#if defined __APPLE_CC__
#include <dirent.h>
#endif // __APPLE_CC__

#include <errno.h>


fsFile::fsFile( const vsString &filename_in, fsFile::Mode mode )
{
	m_mode = mode;
	
	vsString filename = GetFullFilename( filename_in.c_str() );
	
	vsString modeStr[fsFile::MODE_MAX] =
	{
		"r",	// MODE_Read
		"w"		// MODE_Write
	};
	
	m_file = fopen(filename.c_str(), modeStr[m_mode].c_str());
	
// TODO:  Make this work under Win32!
#if defined __APPLE_CC__
	if ( !m_file )
	{
		vsString dirName = filename;
		int slashIndex = dirName.rfind('/');
		if ( slashIndex != dirName.npos )
		{
			dirName.erase(slashIndex,dirName.npos);
		}
		
		DIR * dir = opendir(dirName.c_str());
		if ( !dir )
			vsLog("No such directory: %s", dirName.c_str());
		else
		{
			dirent *dent = readdir(dir);
			
			while( dent )
			{
				vsLog("File: %s", dent->d_name);
				
				dent = readdir(dir);
			}
			
			closedir(dir);
		}
			
	}
#endif // __APPLE_CC__
	
	vsAssert( m_file != NULL, STR("fopen() returned %d, opening file '%s'", errno, filename.c_str()) );
}

fsFile::~fsFile()
{
	if ( m_file )
		fclose(m_file);
}

bool
fsFile::Exists( const vsString &filename_in )		// static member!
{
	bool exists = false;
	vsString filename = GetFullFilename( filename_in.c_str() );
	FILE *file = fopen(filename.c_str(), "r");
	if ( file )
	{
		fclose(file);
		exists = true;
	}
	
	return exists;
}

bool
fsFile::Record( fsRecord *r )
{
	vsAssert(r != NULL, "Called fsFile::Record with a NULL fsRecord!");
	
	if ( m_mode == MODE_Write )
	{
		vsString recordString = r->ToString();
		
		fprintf( m_file, "%s\n", recordString.c_str() );
		
		return true;
	}
	else
	{
		// we want to read the next line into this fsRecord class.
		
		vsString line;
		
		do
		{
			ReadLine(&line);
		}while(line[0]=='#');
		
		return r->FromString(line);
	}
	
	return false;
}

bool
fsFile::ReadLine( vsString *line )
{
	char buf[1024];
	
	if ( !AtEnd() && fgets(buf, 1023, m_file) )
	{
		buf[1023] = 0;	// force a terminator on the end, just in case we receive a line of >= 1024 characters.
		
		*line = buf;
		return true;
	}
	return false;
}
/*
vsString
fsFile::ReadLabel()
{
	//int lineLength = m_lineLength;
	//getline( &m_currentLine, &lineLength, m_file );
	
	int matched = fscanf( m_file, "%s", m_label );
	
	vsAssert( matched > 0, "Unable to read label from file" );
	
	return m_label;
}

vsVector2D
fsFile::ReadVector()
{
	vsVector2D result;
	
	int matched = fscanf( m_file, "%f %f", &result.x, &result.y );
	vsAssert( matched == 2, "Unable to read vector from file" );
	
	return result;
}

float
fsFile::ReadFloat()
{
	float result;
	
	int matched = fscanf( m_file, "%f", &result );
	vsAssert( matched == 1, "Unable to read float from file" );
	
	return result;
}

vsColor
fsFile::ReadColor()
{
	vsColor result;
	
	int matched = fscanf( m_file, "%f %f %f %f", &result.r, &result.g, &result.b, &result.a );
	vsAssert( matched == 4, "Unable to read color from file" );
	
	return result;
}
*/


vsString
fsFile::GetFullFilename(const vsString &filename_in)
{
	vsString dirName = core::GetGameName();
	vsString prefsSuffix(".prefs");
	// check if filename_in is a preferences file, and if so, then look in the preferences directory, instead of in the usual directory.
	size_t prefsIndex = filename_in.find(prefsSuffix);
	if ( prefsIndex == filename_in.length() - prefsSuffix.length() )
	{
		dirName = "Preferences";
	}
	
#if defined(__APPLE_CC__) && !defined(CMAKE_BUILD)
	vsString result = vsFormatString("Damsel.app/Contents/Resources/Data/%s/%s", dirName.c_str(), filename_in.c_str());
#else // !__APPLE_CC__
	vsString result = vsFormatString("Data/%s/%s", dirName.c_str(), filename_in.c_str());
#endif // __APPLE_CC__
	
	return result;
}


bool
fsFile::AtEnd()
{
	return !m_file || feof( m_file );
}
