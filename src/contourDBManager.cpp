/*
** contourDBManager.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Wed Aug 15 22:24:38 EDT 2001
*/

#include "contourDBManager.h"
#include "preferences.h"
#include "appFactory.h" 


#ifdef POSTGRES
#include "postgres/pgDBManager.h"

ContourDBManager * ContourDBManager::mInstance = 0;
ContourDBManager * ContourDBManager::GetInstance()
{
	if (!mInstance)
		mInstance = new PGDatabaseManager;
	return (ContourDBManager *)mInstance;
}

bool ContourDBManager::Initialize()
{
	ContourPreferences *prefs = AppFactory::GetInstance()->GetApp()->GetPreferences();
	((PGDatabaseManager *)this)->Initialize(
				prefs->GetpgHost(),
				prefs->GetpgPort(),
				prefs->GetpgOptions(),
				prefs->GetpgDBName(),
				prefs->GetpgUser(),
				prefs->GetpgPassword());
	return true;
}

#endif

