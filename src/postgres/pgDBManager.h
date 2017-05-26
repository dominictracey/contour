/*
** pgDBManager.h - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Wed Aug 15 21:00:55 EDT 2001
*/

#ifndef _PGDBMANAGER_H_
#define _PGDBMANAGER_H_

#include "../contourDBManager.h"
//#include "../contourDBEffort.h"
#include "pgDBEffort.h"

#include <pgsql/libpq-fe.h>

//class ContourDBEffort;
//class PGDBEffort;

class PGDatabaseManager : public ContourDBManager
{
public:
	PGDatabaseManager();
	~PGDatabaseManager();
	bool Initialize(char *host, char *port, char *options, char *dbName, char *user, char *pw);
	PGresult * Execute(char *sql, char *errorMess);
	ContourDBEffort * GetEffortPersistor();

protected:
	void ExitNicely();
	bool mInitialized;
	void *mConn;
	PGDBEffort *mEffort;
};

#endif
