/*
 *  CORE_Version.cpp
 *  VectorStorm
 *
 *  Created by Trevor Powell on 28/12/07.
 *  Copyright 2007 PanicKitten Softworks. All rights reserved.
 *
 */

#include "CORE_Version.h"


	// We mark off major revisions with a build number and name.  
	// once we finalise a particular build, that build is set up
	// with a build branch (in case any bugfixes are needed, 
	// post-release), and we renumber and rename the trunk.

#define BUILD_NUMBER "001pre"
#define BUILD_NAME "Ant"

const vsString coreVersion::s_versionString = BUILD_NUMBER;
const vsString coreVersion::s_versionNameString = BUILD_NAME;

