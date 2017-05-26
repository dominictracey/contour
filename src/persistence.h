/*
** persistence.h - Interface definition for saving configurable information to disk.
**	We split this interface off as we might wish to save this information in some
**	sort of registry in certain cases (like in a winblows port).
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
*/

#ifndef _PERSISTENCE_H_
#define _PERSISTENCE_H_

#include "preferences.h"

class ContourPersistence
{
public:
	ContourPersistence() {};
	virtual ~ContourPersistence() {};

	virtual void SetPrefs(ContourPreferences *prefs) {mPrefs = prefs;}
	//read the data into the given Preferences dialog
	virtual void Read() = 0;

	//store the data collected by the given dialog
	virtual void Write() = 0;
protected:
	ContourPreferences *mPrefs;
};

#endif
