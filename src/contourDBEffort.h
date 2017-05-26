/*
** contourDBEffort.h - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Fri Dec 14 21:50:25 EST 2001
*/

#ifndef _CONTOURDBEFFORT_H_
#define _CONTOURDBEFFORT_H_

#include "position.h"

class ContourDBEffort
{
public:
	//~ContourDBEffort() {}

	// Generic routines 
	virtual int SetBoundaries(int effortid, float startLat, float startLong,
						float endLat, float endLong,
						float maxLat, float maxLong,
						float minLat, float minLong) = 0;
	virtual int AddDataPoint(int effortid, float lat, float lon, float depth) = 0;
	virtual GridPointVector * GetGridPoints(int effortID) = 0;

	// Drawing a trackline
	virtual int StartDraw() = 0;
	virtual int EndDraw(int effortID) = 0;

	// Looking up drawing efforts in a certain area
	virtual bool GetDrawsInArea(float maxLat, float maxLong,
						float minLat, float minLong) = 0;

};

#endif

