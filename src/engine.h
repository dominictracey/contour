/*
** engine.h - abstract base class for toolkit-specific subclasses
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001 Aquilon Technology Systems, Inc.
**
** 12.18.98 DPT
*/

#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "position.h"
//class ContourEngineDeleter;
class ContourCanvas;
class Panner;

// Actually allocate a bit larger array than what is requested and displayed
// on-screen. We need this "edge buffer" around the outside of the picture so
// that our patches will have some initial data to interpolate right at the 
// beginning of the screen. Also could be handy for some kind of buffered
// display during panning.
#define EDGE_BUFFER 250

//we initialize all of our depth array to some weird number so that we
//know if a point needs to be interpolated.
#define NO_DEPTH -99.0

class ContourEngine
{
friend class ContourEngineDeleter;
public:
	virtual bool Initialize(int height, int width);
	virtual bool Resize(int height, int width);
	virtual void BeginPan(Panner &panner);
	virtual void Pan(Panner &panner);
	virtual void EndPan(Panner &panner);
	virtual void Erase();
	virtual void Render(int x1=-1, int x2=-1, int y1=-1, int y2=-1);
	int GetWidth() {return mWidth;}
	int GetHeight() {return mHeight;}
	float GetMaxDepth() { return mMaxDepth; }
	float GetMinDepth() { return mMinDepth; }
	virtual void Zoom(float amount);
	virtual void ExposeArea(int x1, int x2, int y1, int y2);
	bool IsPointVisible(int x, int y);
	bool GetPanning() { return mPanning; }
	void SetPanning(bool b) { mPanning=b; }
protected:
	ContourEngine();
	~ContourEngine();
	virtual void SetPoints(SamplePointVector &v,int x1, int x2, int y1, int y2)=0;	//pure virtual
	int mHeight;
	int mWidth;
	float mMaxDepth, mMinDepth;
	float **mArray;  //array for all our data points
	bool mPanning;  //this variable is set to true if we have started panning.
					// it gets reset to false when we do a full-screen redraw.
					// When it is true we may not:
					//		* add grid points to rows that do not contain points.
					//		* replace existing grid points.
	bool mResizing;
};

#endif

