/* ** canvas.h - abstract base class for toolkit-specific subclasses
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.18.98 DPT
*/

#ifndef _CANVAS_H_
#define _CANVAS_H_

#include <iostream.h>

class SamplePoint;

class ContourCanvas
{
public:
	virtual ~ContourCanvas() {};
	virtual bool Initialize() = 0;
	virtual void Draw(int x, int y, float z) = 0;
	virtual void BeginDrawing() = 0;
	virtual void DoneDrawing() = 0;
	virtual void Shift(int NSOffset, int EWOffset) = 0;

	//these methods are for the external ColorAlgorithm derivatives to
	//use to set up our color array.
	virtual bool SpecifyColor(short R, short G, short B, int index) = 0;
	virtual bool AllocColors() = 0;

	//accessors
	virtual int GetNumColors() {return mNumColors;};
	virtual int GetDepth() {return mDepth;};
	virtual int GetHeight() {return mHeight;};
	virtual int GetWidth()	{return mWidth;};
	//mutators
	virtual void SetMaxDepth(float d) {mMaxDepth = d; mRange = mMaxDepth-mMinDepth;}
	virtual void SetMinDepth(float d) {mMinDepth = d; mRange = mMaxDepth-mMinDepth;}

	enum decorations {ALL, LATLONG_GRID, GPS_DISPLAY, POSITION_INDICATOR, CROSSHAIR_CURSOR, POLAR_CURSOR, DRAWTRACK_CURSOR, NULL_CURSOR, DRAW_EFFORTS};
	virtual void AddDecoration(decorations d) = 0;
	virtual void RemoveDecoration(decorations d) = 0;

protected:
	int mNumColors;
	int mDepth;  //color depth (in bits) prolly 8 or 16 (or 24)
	int mWidth;
	int mHeight; 
	float mMaxDepth;
	float mMinDepth;
	float mRange;
};

#endif

