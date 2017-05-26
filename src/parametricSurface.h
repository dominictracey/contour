/*
** parametricSurface.h - The engine generates the surface from the given control
**	points using bicubic parametric surface patches. We first generate a mesh of
**	four-sided surface patches over the area we are needing to render. We need 16
**	points to render one patch. First we generate the top and bottom edges of our
**	patch to render:
**
		*	*	*	*
		
		*	*xxx*	*
		
		*	*xxx*	*
		
		*	*	*	*
**
**	Then we fill in the patch with vertical splines between the two horizontal:
**
		*	*	*	*
		
		*	*xxx*	*
			yyyyy	
		*	*xxx*	*

		*	*	*	*

**
**	Then we move on to the next patch.
**
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Mon Dec 18 23:23:07 EST 2000
*/

#ifndef _PARAMETRICSURFACE_H_
#define _PARAMETRICSURFACE_H_

#include "engine.h"
#include "spline.h"
#include "position.h"
#include "canvas.h"
#include <vector.h>


class PSEGrid
{
public:
	PSEGrid();
	virtual ~PSEGrid();
	// mGrid is an array of pointers to GridPoints
	void SetSize(int x, int y, int granularity);
	bool InsertPoint(GridPoint *gp, int x, int y);
	void Fill(int x1, int x2, int y1, int y2);
	void Show();  //debugging
	void Shift(int ns, int ew);
	void Dump();
protected:
	void DrawLine(ContourEngine *engine, int x1, int x2, int y1, int y2);
	bool SetUpSpline(int &x, int &y, spline &spl);
	void InterpolateHorizontal(int x1, int y1, int x2, int y2, float percent, int gridx, int gridy);
	void InterpolateVertical(int x1, int y1, int x2, int y2, float percent, int gridx, int gridy);
	void Ensure4SidedPatches();
	void ExtrapolateCorner(int x, int y, int corner);
	bool IsPatchVisible(int x, int y);
	bool IsPatchDrawn(int x, int y);
	void MarkPatchDrawn(int x, int y, bool drawn);
	bool FindOneLeft(int x, int y, int &target);
	GridPoint ***mGrid;
	bool **mDrawnPatches;
	int mGranularity;  //ratio of PSE::mArray size to mX,mY
	int mX, mY;  //size
	ContourEngine *mEngine;
};

class ParametricSurfaceEngine : public ContourEngine
{
friend class PSEGrid; // for debugging Show() routine
public:
	ParametricSurfaceEngine();
	virtual ~ParametricSurfaceEngine();
	virtual void SetPoints(SamplePointVector &v, int x1, int x2, int y1, int y2);
	virtual void EndPan(Panner &panner);
	virtual bool Initialize(int height, int width);
protected:
	inline void SetPoint(int x,int y, float z)  {if (x>=0 && y>=0 && x < mWidth && y < mHeight) mArray[x][y]=z;}//{mCanvas->Draw(x-EDGE_BUFFER,y-EDGE_BUFFER,z);}
	void CleanUp();
	ContourCanvas *mCanvas;
	typedef vector<spline_point *> SplineVector;
	PSEGrid *mGrid;
};

#endif

