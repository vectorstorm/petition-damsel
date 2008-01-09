/*
 *  FS_File.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 18/03/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef FS_FILE_H
#define FS_FILE_H

class fsRecord;

class fsFile
{
public:
	enum Mode
	{
		MODE_Read,
		MODE_Write,
		
		MODE_MAX
	};
	
private:
	FILE *		m_file;
	
	Mode		m_mode;
	
public:
	
			// In general, files should be opened by creating an fsFile;  the fsFile class automatically deals with finding where the file is
			// located on the particular platform and build.  However, if a file needs to be opened by a piece of middleware (such as SDL_Mixer,
			// for example), then you can use fsFile::GetFullFilename() to convert from a filename to the correct path to reach that file on
			// our current platform.
	static vsString	GetFullFilename(const vsString &filename_in);
	
				fsFile( const vsString &filename, fsFile::Mode mode = MODE_Read );
	virtual		~fsFile();
	
	static bool	Exists( const vsString &filename );	// returns true if the requested file exists.  (Useful for prefs files, which may or may not exist)
	
	bool		Record( fsRecord *record );		// returns true if we found or successfully wrote another record
	bool		ReadLine( vsString *line );
	
	/*  These functions are probably deprecated;  use fsRecord objects instead!
	 *
	vsString	ReadLabel();	
	float		ReadFloat();
	vsVector2D	ReadVector();
	vsColor		ReadColor();
	 */
	
	bool		AtEnd();
};

#endif // FS_FILE_H
