/*
** LatLongGrid.h - This is the grid of latitude and longitude lines. It is 
**	controlled by the Application subclass.
**
**	I'd imagine we'll end up refactoring this into an interface abstract class
**	that we subclass with different kinds of projections. Right now its just
**	a Mercator thing.
**
**	Made it a Composite pattern thing by inheriting from the LatLongLine abstract
**	base class. Not sure what this buys us but it makes me feel nice inside.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Dec 31 19:53:21 EST 2000
*/

#ifndef _LATLONGGRID_H_
#define _LATLONGGRID_H_

#include "latLongLine.h"

//# of lines of lat and long
#define LATLONGGRID_NUM_LAT_DIVISIONS 5 

class LatLongGrid : public ContourGraphic
{
public:
	LatLongGrid();
	~LatLongGrid();
protected:
	void SetViewPortLocal();
	void Draw(); //only accessible through ContourGraphicContainer
	void Clear();
	virtual void DrawPartial(int x1, int x2, int y1, int y2);
	virtual void DrawText();
	virtual void DrawTextPartial(int x1, int x2, int y1, int y2);

	int GetDefaultIncrement(float calculated_increment);
	void SetUpGrid();
	LatLongLine **mLats;  //our latitude lines
	LatLongLine **mLongs; //longitude lines
	bool mClear;
};

#endif

