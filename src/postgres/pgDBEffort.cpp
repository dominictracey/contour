/*
** pgDBEffort.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Fri Dec 14 22:28:42 EST 2001
*/

#include "pgDBEffort.h"
#include "pgDBManager.h"
#include <stdlib.h>
#include <iostream.h>
#include "../position.h"
#include "../effortDraw.h"
#include "../contourGraphic.h"

PGDBEffort::PGDBEffort()
{

}

PGDBEffort::~PGDBEffort()
{

}

// Generic routines 
int PGDBEffort::SetBoundaries(int effortid, float startLat, float startLong,
					float endLat, float endLong,
					float maxLat, float maxLong,
					float minLat, float minLong)
{
	char q[256];
	sprintf(q,"select effort_set_boundaries(%d,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f)",
			effortid, startLat, startLong, endLat, endLong, maxLat, maxLong, minLat, minLong);
	PGresult * res = ((PGDatabaseManager *)ContourDBManager::GetInstance())->Execute(q,"Set boundaries on effort failed");

	//return the effortid
	if (res)
	{
		char * retval = PQgetvalue(res,0,0);
		PQclear(res);
		return atoi(retval);
	}
	else
		return 0;
}

int PGDBEffort::AddDataPoint(int effortid, float lat, float lon, float depth)
{
	char q[256];
	
	sprintf(q,"select trackpoint_add(%d,%.4f,%.4f,%.2f)",effortid,lat,lon,depth);
	PGresult * res = ((PGDatabaseManager *)ContourDBManager::GetInstance())->Execute(q,"Trackpoint insertion failed");

	//return the trackpointid
	if (res)
	{
		char * retval = PQgetvalue(res,0,0);
		PQclear(res);
		return atoi(retval);
	}
	else
		return 0;
}

// Drawing a trackline
int PGDBEffort::StartDraw()
{

	PGresult * res = ((PGDatabaseManager *)ContourDBManager::GetInstance())->Execute("select effort_draw_start()","Draw start effort failed");

	//return the new effortid
	if (res)
	{
		char * retval = PQgetvalue(res,0,0);
		PQclear(res);
		return atoi(retval);
	}
	else
		return 0;
}

int PGDBEffort::EndDraw(int effortID)
{
	char q[256];
	sprintf(q,"select effort_draw_end(%d)",effortID);
	PGresult * res = ((PGDatabaseManager *)ContourDBManager::GetInstance())->Execute(q,"Draw end effort failed");

	//return the effortid
	if (res)
	{
		char * retval = PQgetvalue(res,0,0);
		PQclear(res);
		return atoi(retval);
	}
	else
		return 0;
}

/*
** GetDrawsInArea - This seems really hideous, there must be a better way of seeing if
**	the bounding area of the passed in viewport intersects with the efforts without
**	16 comparisons!
**
**	Returns true if it found some, false otherwise
*/
bool PGDBEffort::GetDrawsInArea(float xt, float xn,
						float nt, float nn)
{
	char q[2560];
	sprintf(q,"select * from effort_draw,effort where effort.effortid=effort_draw.effortid and \
					(((max_lat > %.4f and min_lat < %.4f) or \
					(max_lat > %.4f and min_lat < %.4f) or \
					(max_lat < %.4f and min_lat < %.4f and max_lat > %.4f and min_lat > %.4f)) and \
					((max_long > %.4f and min_long < %.4f) or \
					(max_long > %.4f and min_long < %.4f) or \
					(max_long < %.4f and min_long < %.4f and max_long > %.4f and min_long > %.4f)))"
					,xt,xt,nt,nt,xt,xt,nt,nt,xn,xn,nn,nn,xn,xn,nn,nn);
	//cout << q << endl;
	PGresult * res = ((PGDatabaseManager *)ContourDBManager::GetInstance())->Execute(q,"Draws in area query failed");
	if (res)
	{
		char * retval;
		DrawEffort *new_draw;
		for (int i=0; i < PQntuples(res); ++i)
		{
			retval = PQgetvalue(res,i,0);
			new_draw = new DrawEffort;
			// note that as part of this load, AddDecoration gets called, which
			// grabs the DecorationCollection and adds a decoration for the effort.
			// This is how we don't lose the DrawEffort handle. Pretty fancy.
			new_draw->Load(atoi(retval));
		}
		PQclear(res);
		return true;
	}
	else
		return false;

}

GridPointVector * PGDBEffort::GetGridPoints(int effortID)
{
	GridPointVector *v = new GridPointVector;

	char q[256];
	sprintf(q,"select * from trackpoints,trackpoint_effort where trackpoints.trackpointid=trackpoint_effort.trackpointid and trackpoint_effort.effortid=%d",effortID);

	PGresult * res = ((PGDatabaseManager *)ContourDBManager::GetInstance())->Execute(q,"Draw effort get points failed");

	GridPoint *g;
	SamplePoint ul, size;
	float xscale, yscale;
	int width, height;
	// @REX why is this expensive operation done here?
	ContourGraphic::GetViewPort(ul,size,xscale,yscale,width,height);
	
	//return the effortid
	if (res)
	{
		for (int i=0; i < PQntuples(res); ++i)
		{
			char * lat = PQgetvalue(res,i,1);
			char * lon = PQgetvalue(res,i,2);
			Position pLat((float)atof(lat));
			Position pLon((float)atof(lon));
			g = new GridPoint(pLat,pLon);
			g->SetDepth(atof(PQgetvalue(res,i,3)));
			g->Recalculate(&ul,&size,width,height);
			v->push_back(g);
		}
		PQclear(res);
		return v;
	}
	else
	{
		delete v;
		return 0;
	}
	
}

