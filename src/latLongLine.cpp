/*
** latLongLine.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Mon Jan  1 13:40:40 EST 2001
*/

#include "latLongLine.h"
#include "stdio.h"
#include <math.h>


LatLongLine::LatLongLine(const Position &p, bool lat) 
{
	mPos = p;
	mLat = lat;
	sprintf(mStr,"%d° %d\' %4.2f\"\0",abs(p.GetDegrees()),abs(p.GetMinutes()),fabs(p.GetSeconds()));
}

LatLongLine::~LatLongLine()
{

}
