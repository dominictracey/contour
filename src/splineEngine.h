/*
** SplineEngine.h - Use Overhauser (Catmull-Rom) B-splines for surface
**	generation 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.18.98 DPT
*/

#ifndef _SPLINEENGINE_H_
#define _SPLINEENGINE_H_

#include "engine.h"
#include "spline.h"
#include <vector.h>

// Actually allocate a bit larger array than what is requested and displayed
// on-screen. We need this "edge buffer" around the outside of the picture so
// that our splines will have some initial data to interpolate right at the 
// beginning of the screen. Also could be handy for some kind of buffered
// display during panning.

class SplineEngine : public ContourEngine
{
public:
	SplineEngine();
	~SplineEngine();
	virtual void SetPoints(SamplePointVector &v, int x1, int x2, int y1, int y2);
protected:
	typedef vector<spline_point *> SplineVector;
	virtual bool TrackSpline(SplineVector &sv);
	virtual bool VerticalFrameworkIsBetter();
	virtual bool CreateFramework(bool vib, int numRibs,
					int x1, int x2, int y1, int y2);
	virtual bool CreateSineFramework(bool vib, int numRibs,
					int x1, int x2, int y1, int y2);
	virtual float Smooth(int x, int y, float z);
};

#endif

