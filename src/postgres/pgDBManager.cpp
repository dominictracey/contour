/*
** pgDBManager.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Wed Aug 15 21:22:30 EDT 2001
*/

#include <string.h>
#include "pgDBManager.h"
//#include "pgDBEffort.h"
#include <iostream.h>

PGDatabaseManager::PGDatabaseManager()
{
	mConn = NULL;
	mInitialized = false;	
	mEffort = NULL;
}

PGDatabaseManager::~PGDatabaseManager()
{
	PQfinish((PGconn *)mConn);
	if (mEffort)
		delete mEffort;
}

bool PGDatabaseManager::Initialize(char *host, char *port, char *options, char *dbName, char *user, char *pw)
{
	mConn = (PGconn *)PQsetdbLogin(host,port,options,NULL,dbName,user,pw);

	//check status
	if (PQstatus((PGconn *)mConn) == CONNECTION_BAD)
	{
		cerr << "Connection to database " << dbName << " failed" << endl;
		cerr << PQerrorMessage((PGconn *)mConn) << endl;
		ExitNicely();
	} else
	{
		mInitialized = true;
	}
	return mInitialized;
}

void PGDatabaseManager::ExitNicely()
{
	PQfinish((PGconn *)mConn);
	//@REX should throw a contour error? Or at least more info...
	// throw("Database error");
}

/*
** Execute - pass in SQL to be run on back-end -- don't forget to PQClear the
**	returned res pointer.
*/
PGresult * PGDatabaseManager::Execute(char *sql, char *errorMess)
{
	if (mInitialized)
	{
		PGresult *res;

		res = PQexec((PGconn *)mConn, sql);

		if (!res || PQresultStatus(res) != PGRES_TUPLES_OK)
		{
			cerr << "*** PostgreSQL error: " << errorMess << endl;
			PQclear(res);
			return NULL;
		}

		return res;
	} else
	{
		cerr << "Attempt to write data sample to uninitialized database" << endl;
		return 0;
	}
}

ContourDBEffort * PGDatabaseManager::GetEffortPersistor()
{
	if (!mEffort)
		mEffort = new PGDBEffort;
	return mEffort;
}
