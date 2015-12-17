///////////////////////////////////////////////////////////////////////////////
///
///	\file    MemoryTools.h
///	\author  Paul Ullrich
///	\version April 23, 2014
///
///	<summary>
///		This header file provides access to various mathematical functions
///		that are not normally exposed in the standard C++ libraries.
///	</summary>
///	<remarks>
///		Copyright 2000-2010 Paul Ullrich
///
///		This file is distributed as part of the Tempest source code package.
///		Permission is granted to use, copy, modify and distribute this
///		source code and its documentation under the terms of the GNU General
///		Public License.  This software is provided "as is" without express
///		or implied warranty.
///	</remarks>

#ifndef _MEMORYTOOLS_H_
#define _MEMORYTOOLS_H_

#include "sys/resource.h"
#include <cstdlib>

///////////////////////////////////////////////////////////////////////////////

void PrintMemoryLine(const char * szString = NULL);

///////////////////////////////////////////////////////////////////////////////

#endif

