/*
** effort.h - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Mon Nov 26 18:32:32 EST 2001
*/

#ifndef _EFFORT_H_
#define _EFFORT_H_

#include "parametricSurface.h"


class Effort
{
public:
	Effort();
	~Effort();
	
	virtual bool Start();
	virtual bool End();
	bool AddSamplePoint(GridPoint *sp);
	virtual bool AddDecoration() = 0;
	static void RemoveDecorations();
	virtual int Save();
	virtual bool Load(int effortid);
	GridPointVector * GetSamplePoints() { return mSamplePoints; }
	bool Recalculate(SamplePoint *ul, SamplePoint *size, int width, int height);
	enum Type {UNDEFINED,TOW,STEAM,DRAW,LAY};
protected:
	int mID;
	GridPointVector *mSamplePoints;
	
};


#endif

