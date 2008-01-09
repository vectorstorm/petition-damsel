/*
 *  CORE_Version.h
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/12/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#ifndef CORE_VERSION_H
#define CORE_VERSION_H

class coreVersion
{
	static const vsString s_versionString;
	static const vsString s_versionNameString;
public:
	
	static const vsString &		GetVersionString() { return s_versionString; }
	static const vsString &		GetVersionNameString() { return s_versionNameString; }
};


#endif // CORE_VERSION_H

