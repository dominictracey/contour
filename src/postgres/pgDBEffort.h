/*
** pgDBEffort.h - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Fri Dec 14 22:25:59 EST 2001
*/

#ifndef _PGDBEFFORT_H_
#define _PGDBEFFORT_H_

#include "../contourDBEffort.h"

class PGDBEffort : public ContourDBEffort
{
public:
	PGDBEffort();
	~PGDBEffort();

	// Generic routines 
	virtual int SetBoundaries(int effortid, float startLat, float startLong,
						float endLat, float endLong,
						float maxLat, float maxLong,
						float minLat, float minLong);
	virtual int AddDataPoint(int effortid, float lat, float lon, float depth);
	virtual GridPointVector * GetGridPoints(int effortID);
	// Drawing a trackline
	virtual int StartDraw();
	virtual int EndDraw(int effortID);

	// get the drawings in an area
	virtual bool GetDrawsInArea(float maxLat, float maxLong,
										float minLat, float minLong);

};

#endif

