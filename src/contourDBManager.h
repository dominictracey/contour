/*
** contourDBManager.h - This is the interface for interacting directly with the
**	configured RDBMS. It is a pure virtual class, other than the singleton
**	stuff. This breaks a couple of tenets that I try to adhere to (no knowledge of 
**	subclasses in base classes, no inheritence across library boundaries, etc.)
**
**	@REX please help
**
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Wed Aug 15 21:03:05 EDT 2001
*/

#ifndef _CONTOURDBMANAGER_H_
#define _CONTOURDBMANAGER_H_

#include "config.h"
//#include "position.h"
#include "contourDBEffort.h"
//#include <vector.h>

//#define POSTGRES

class ContourDBManager
{
public:
	~ContourDBManager() {}

	// I'm not really wild about this,
#ifdef POSTGRES
	static ContourDBManager * GetInstance();
#endif
	bool Initialize();
	virtual ContourDBEffort * GetEffortPersistor() = 0;
protected:
#ifdef POSTGRES
	static ContourDBManager * mInstance;
#endif
};

#endif
