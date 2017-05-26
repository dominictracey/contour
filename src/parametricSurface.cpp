/*
** parametricSurface.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Mon Dec 18 23:53:14 EST 2000
*/

#include "parametricSurface.h"
#include "canvas.h"
#include <stdio.h>
#include <math.h>
#include "extractor.h"
#include "spline_forwarddifference.h"
#include "appFactory.h"
#include "preferences.h"
#include "timer.h"
#include "exception.h"
#include "panner.h"
#include "cursor.h"
#include "gdkCanvas.h"
#ifdef _DEBUG
#include <gtk/gtk.h> 
#include "canvas.h"
#endif

// Grid granularity is the ratio between the width of the patch grid and the 
// width of the engine's mArray. Note that it is scaled by the amount we are
// zoomed in. It is expressed as size(mArray)/size(grid) for zoom=1.
// @REX should be a preference
#define PSE_GRID_GRANULARITY 27

PSEGrid::PSEGrid()
{ 
	mGrid = NULL;
	mDrawnPatches = NULL;
	//saves a few calls to keep it as a member var...
	mEngine = AppFactory::GetInstance()->GetApp()->GetEngine();
}

// Don't delete the GridPoints in mGrid as they belong to the extractor.
PSEGrid::~PSEGrid()
{
	for (int i=0; i<mX; i++)
		delete [] mGrid[i];

	for (int i=0; i<mX-1; i++)
		delete [] mDrawnPatches[i];		
}

void PSEGrid::SetSize(int x, int y, int granularity) 
{ 
	++x; ++y;  //extra points at 0,0
	mGrid = new GridPoint **[x];
	for (int p=0; p < x; ++p)
		mGrid[p] = new GridPoint *[y];
	// initialize to NULL
	for (int i=0;i<x;++i) 
		for (int j=0;j<y;++j) 
			mGrid[i][j]=NULL;

	//now the array of bools that specify whether we've drawn the patches or not.
	//this boosts performance during panning and helps G1 (geometric) continuity in
	//the surface rendering.
	mDrawnPatches = new bool *[x-1];
	for (int p=0; p < x-1; ++p)
		mDrawnPatches[p] = new bool[y-1];
	//init to false
	for (int i=0; i<x-1; ++i)
		for (int j=0;j<y-1;++j) 
			mDrawnPatches[i][j] = false;
	
	mGranularity = granularity;
	
	mX=x;
	mY=y;
}

/*
** InsertPoint - Place the given GridPoint within the grid at the best 
**	position. If there is already a GridPoint there, check to see if 
**	this one is better. If so, replace it and try to reinsert the displaced
**	one at its next closest hole.
**
**	While we are panning, we only insert points into "newly exposed" grid slots,
**	we do not do any replacing (even if the points are closer). This has an
**	unfortunate side effect of inserting the first point into these new slots,
**	rather than the best. When you Show() the grid, it becomes very distorted as 
**	you pan and the rendered image's quality degrades accordingly. This becomes
**	increasingly evident as the granularity coefficient is decreased. Hence,
**
**	@REX When panning, allow "closest fit" of newly extracted points to into
**	newly opened grid points rather than the currently implemented "first fit".
**
**	@REX Special weight should be given in the selection criteria to inflection
**	points. For instance, the shoalest point should be preferred over a point that
**	is closer to the ideal location.
*/
bool PSEGrid::InsertPoint(GridPoint *gp, int x, int y)
{
	gp->x(x);
	gp->y(y);
	// px & py are the grid indices.
	int px = (int)rint((double)x / (double)mGranularity);
	int py = (int)rint((double)y / (double)mGranularity);

	if (px == mX)
		px--;
	if (py == mY)
		py--;
	// if there is someone there, see if we're closer
	if (mGrid[px][py] && !mEngine->GetPanning())
	{
		int oldx = mGrid[px][py]->x();
		int oldy = mGrid[px][py]->y();
		
		float old_distance=sqrt( pow((double)(oldy-(py*mGranularity)),(double)2) + pow((double)(oldx-(px*mGranularity)),(double)2));
		float new_distance=sqrt( pow((double)(y-(py*mGranularity)),(double)2) + pow((double)(x-(px*mGranularity)),(double)2));
		GridPoint *old;
		if (new_distance < old_distance)
		{
			old = mGrid[px][py];
			mGrid[px][py] = gp;
		} else
			old = gp;
			
		//do something to try to recycle the loser!
/*
		//@REX inefficient!
		float direction[8];
		//float nw; 
		direction[0] = sqrt( pow((double)(oldy-((py-1)*mGranularity)),(double)2) +
						 pow((double)(oldx-((px-1)*mGranularity)),(double)2));
		//float n; 
		direction[1] = sqrt( pow((double)(oldy-((py-1)*mGranularity)),(double)2) +
						 pow((double)(oldx-((px)*mGranularity)),(double)2));
		//float ne; 
		direction[2] = sqrt( pow((double)(oldy-((py-1)*mGranularity)),(double)2) +
						 pow((double)(oldx-((px+1)*mGranularity)),(double)2));
		//float e; 
		direction[3] = sqrt( pow((double)(oldy-((py)*mGranularity)),(double)2) +
						 pow((double)(oldx-((px+1)*mGranularity)),(double)2));
		//float se; 
		direction[4] = sqrt( pow((double)(oldy-((py+1)*mGranularity)),(double)2) +
						 pow((double)(oldx-((px+1)*mGranularity)),(double)2));
		//float s; 
		direction[5] = sqrt( pow((double)(oldy-((py+1)*mGranularity)),(double)2) +
						 pow((double)(oldx-((px)*mGranularity)),(double)2));
		//float sw; 
		direction[6] = sqrt( pow((double)(oldy-((py+1)*mGranularity)),(double)2) +
						 pow((double)(oldx-((px-1)*mGranularity)),(double)2));
		//float w; 
		direction[7] = sqrt( pow((double)(oldy-((py)*mGranularity)),(double)2) +
						 pow((double)(oldx-((px-1)*mGranularity)),(double)2));
		float winner=900000;
		int winner_index;
		for (int s=0; s<8; ++s)
		{
			if (direction[s] < winner)
			{
				winner = direction[s];
				winner_index=s;
			}
		}

		switch (winner_index)
		{
			case 0:
			{
				px--;
				py--;
				break;
			}
			case 1:
			{
				px;
				py--;
				break;
			}
			case 2:
			{
				px++;
				py--;
				break;
			}
			case 3:
			{
				px++;
				py;
				break;
			}
			case 4:
			{
				px++;
				py++;
				break;
			}
			case 5:
			{
				px;
				py++;
				break;
			}
			case 6:
			{
				px--;
				py++;
				break;
			}
			case 7:
			{
				px--;
				py;
				break;
			}
		}
		if (px >= 0 && px < mX && py >= 0 && py < mY && !mGrid[px][py])
			mGrid[px][py] = old;
		*/
	} else if (!mGrid[px][py]) //its empty
	{
		mGrid[px][py] = gp;
	}

	return true;
}

/*
** Fill - After we populate the grid with existing data points, we need to fill in
**  the rest of the surface in the engine's mArray with bicubic parametric surface generation.
**	The parameters define the grid coordinates needing to be filled.
*/
void PSEGrid::Fill(int x1, int x2, int y1, int y2)
{
	Timer::getInstance()->mark("Begin PSEGrid::Fill");

	BathApp *app = AppFactory::GetInstance()->GetApp();
	ParametricSurfaceEngine *engine = (ParametricSurfaceEngine *)app->GetEngine();
	ContourCanvas *canvas = app->GetCanvas();
	
	SplineForwardDifference * spliner[4];
	int topRow=y1; //toprow keeps track of the first horizontal spline's grid index
	bool morephase2 = true;  //morephase2 describes whether there are more rows to process

	//Check to make sure all our patches are 4-sided.
	Ensure4SidedPatches();
	//Show();
	//Dump();

	while (morephase2)
	{
		//phase1GridIndices stores the last mGrid index in each spliner entry
		//initialize them to the first four grid points in the first four grid
		//rows to be filled in.
		int phase1GridIndices[4][2];
		for (int gi=0;gi<4;++gi)
		{
			phase1GridIndices[gi][0]=x1;
			phase1GridIndices[gi][1]=gi + y1;
		}
			
		//morephase1 describes whether we should continue across the patches
		bool morephase1 = true;
		spliner[0]= new SplineForwardDifference;
		spliner[1]= new SplineForwardDifference;
		spliner[2]= new SplineForwardDifference;
		spliner[3]= new SplineForwardDifference;
		
		//Get our four splines set up for the first patch
		bool ok;
		int splineIndex=0, rowIndex=topRow;
		while (splineIndex<4 && rowIndex<mY)
		{
			//note that the first three params are passed by reference so they are 
			// mutated in the call.
			ok = SetUpSpline(phase1GridIndices[splineIndex][0],rowIndex,*spliner[splineIndex]);
			phase1GridIndices[splineIndex][1] = rowIndex;
			if (ok)
				splineIndex++;
			rowIndex++;
		}
		topRow = phase1GridIndices[0][1];
		topRow++;  //save this off for when we start on the next row of patches
		if (topRow >= mY-3)
			morephase2 = false;
		if (splineIndex < 4)
		{
			cerr << "Couldn't initialize patches" << endl;
			morephase1 = false;
			return;
		}

		// draw patches across the row
		while (morephase1)
		{
			//check to see if we haven't drawn this patch yet
			if (!IsPatchDrawn(phase1GridIndices[1][0],phase1GridIndices[1][1]))
			{
				//check to make sure its visible
				if (IsPatchVisible(phase1GridIndices[1][0],phase1GridIndices[1][1]))
				{
					//now we have our four splines, interpolate across them
					//@REX use sqrtx2+y2
					//use the bigger distance of the top and bottom edges of the
					// patch being painted.
					float distance1 = 
						(float)spliner[1]->get_value(2)->x -
						(float)spliner[1]->get_value(1)->x;	
					float distance2 = 
						(float)spliner[2]->get_value(2)->x -
						(float)spliner[2]->get_value(1)->x;	

					float distance;
					//@REX what's the fancy ? : sytax for this again?
					if (distance1 > distance2)
						distance = distance1;
					else
						distance = distance2;
						
					for (int q=0;q<4;++q)
					{	
						spliner[q]->compute_all();
						spliner[q]->Initialize((int)distance);
					}
					spline_point *p0=new spline_point;
					spline_point *p1=new spline_point;
					spline_point *p2=new spline_point;
					spline_point *p3=new spline_point;
					//fill in the patch
					for (int h=0; h < (int)distance; ++h)
					{
						spliner[0]->Step(*p0);
						spliner[1]->Step(*p1);
						spliner[2]->Step(*p2);
						spliner[3]->Step(*p3);
						// Here are our four points to use inside the patch
						SplineForwardDifference patcher;
						patcher.set_value(p0,0);
						patcher.set_value(p1,1);
						patcher.set_value(p2,2);
						patcher.set_value(p3,3);
						int pdist = (int)rint(sqrt(pow((float)p2->y - (float)p1->y,(float)2) +
										 pow((float)p2->x - (float)p1->x,(float)2)));
						patcher.compute_all();
						patcher.Initialize((int)pdist);
						spline_point p;
						for (int r=0;r<pdist;r++)
						{
							patcher.Step(p);
							if (p.y < engine->mHeight && p.y >= 0 && p.x < engine->mWidth && p.x >= 0)
							{
								//engine->mArray[p.x][p.y] = p.z;
								engine->SetPoint(p.x,p.y,p.z);
								//try to fill in some stretch marks
								//engine->mArray[p.x+1][p.y] = p.z;
								//engine->mArray[p.x][p.y+1] = p.z;
								//engine->mArray[p.x+1][p.y+1] = p.z;
								engine->SetPoint(p.x+1,p.y,p.z);
								engine->SetPoint(p.x,p.y+1,p.z);
								engine->SetPoint(p.x+1,p.y+1,p.z);
							}
								
						}
						p0=new spline_point;
						p1=new spline_point;
						p2=new spline_point;
						p3=new spline_point;
					}
					
					//mark this patch as drawn
					MarkPatchDrawn(phase1GridIndices[1][0],phase1GridIndices[1][1],true);
				}
			}

			//now see if we can keep going across -- to do so, each of our four control
			// splines must have another point available
			int initialized;
			//cout << "looking for more" << endl;
			for (int a=0; a<4; ++a)
			{
				initialized = false;
				(phase1GridIndices[a][0])++;
				while (!initialized && phase1GridIndices[a][0] < x2)  //was mX
				{
					if (mGrid[phase1GridIndices[a][0]][phase1GridIndices[a][1]])
						initialized = true;
					else
						(phase1GridIndices[a][0])++;
				}
				if (!initialized)
					morephase1 = false;
				else
				{
					spline_point *newp = new spline_point(
											mGrid[phase1GridIndices[a][0]][phase1GridIndices[a][1]]->x(),
											mGrid[phase1GridIndices[a][0]][phase1GridIndices[a][1]]->y(),
											mGrid[phase1GridIndices[a][0]][phase1GridIndices[a][1]]->GetDepth());
					spliner[a]->bump_along(newp);
					//cout << "Still going (" << phase1GridIndices[a][0] << "," << phase1GridIndices[a][1] << ")" << endl;
				}
			}
		}
		for (int d=0;d<4;++d)
			delete spliner[d];
	}
	Timer::getInstance()->mark("End PSEGrid::Fill");

}

/*
** SetUpSpline - Given an x & y grid coordinates to start looking at, fill in the
**	passed spline. Grid coordinates are set to be the last value in the spline.
**	The last two parameters define how far we want to go in looking for another
**	grid point for our spline.
*/
bool PSEGrid::SetUpSpline(int &x, int &y, spline &spl)
{
	int splineIndex=0;
	bool initialized = false;
	for (int j=x; j<mX-1 && !initialized; ++j)
	{
		//get the first four points
		if (mGrid[j][y])
    	{
			spline_point *p = new spline_point(mGrid[j][y]->x(),mGrid[j][y]->y(),mGrid[j][y]->GetDepth());
			spl.set_value(p,splineIndex++); 	
			//if we have the 4 spline control points we can break
			// out of the loop
			if (splineIndex == 4)
			{
				initialized = true;
				x = j;  //note that x is passed by reference so we return it for next time!
			}
    	}
    }
    return initialized;
}

/*
** Ensure4SidedPatches - Every horizontal grid segment must extend across the entire
**	grid.
*/
void PSEGrid::Ensure4SidedPatches()
{
	// these arrays keep the number of grid points in each row and column.
	int xs[mX];
	int ys[mY];

	//initialize
	for (int j=0;j<mY;j++)
	{
		ys[j] = 0;
	}
	for (int i=0;i<mX;i++)
	{
		xs[i] = 0;
	}

	// Now visit each point in the grid. If there is a grid point there,
	//  increment the count.
	for (int j=0;j<mY;j++)
	{
		for (int i=0;i<mX;i++)
		{
			if (mGrid[i][j])
			{
				(xs[i])++;
				(ys[j])++;
			}
		}
	}

	//now we know all the rows and columns with grid points.
	// (those where *s[i] != 0)
	// we need to ensure that every row with points must have
	// a point in each column with points.

	// clear out any lame columns
	for (int i=0;i<mX;++i)
	{
		// the number of points at a given x is the number in that column
		if (xs[i] > 0 && xs[i] < mY/3)
		{
			for (int j=0;j<mY;++j)
			{
				//delete mGrid[i][j];  //don't delete - they belong to the extractor
				mGrid[i][j] = NULL;
			}
			xs[i]=0;
		}
	}

	// clear out any lame rows
	for (int i=0;i<mY;++i)
	{
		if (ys[i] > 0 && ys[i] < mX/3)
		{
			for (int j=0;j<mX;++j)
			{
				//delete mGrid[j][i];
				mGrid[j][i] = NULL;
			}
			ys[i]=0;
		}
	}
	
	//first thing, make sure the corners have points -- if they
	// don't we need to extrapolate for them. Once we have the
	// corners, everything is interpolation.
	int top=0;
	while (ys[top]==0 && top < mY)
		++top;
		
	int bottom=mY-1;
	while (ys[bottom]==0 && bottom > top)
		--bottom;

	int left=0;
	while (xs[left]==0 && left < mX)
		++left;

	int right=mX-1;
	while (xs[right]==0 && right > left)
		--right;

	cout << "Grid borders at top " << top << " bottom " << bottom << " left " << left << " right " << right << endl;
		
	ExtrapolateCorner(right,top,_NORTH | _EAST);
	ExtrapolateCorner(right,bottom,_SOUTH | _EAST);
	ExtrapolateCorner(left,top,_NORTH | _WEST);
	ExtrapolateCorner(left,bottom,_SOUTH | _WEST);

	// now we interpolate the missing points.
	// to keep it simple for now, we'll just interpolate horizontally,
	// except, of course, for the east and west edges, which we will do
	// vertically and take care of first.
	for (int j=0; j<mY; ++j)
	{
		if (!mGrid[left][j] && ys[j] != 0)
		{
			//get the point above
			int up = j-1;
			while (!mGrid[left][up])
				--up;
			//now the point below (note that there may be a couple of grid points missing
			int down = j+1;
			while (!mGrid[left][down] && down < mY )
				++down;
			InterpolateVertical(left,up,left,down,(float)1/(float)(down-up),left,j);
		}
	}
	for (int j=0; j<mY; ++j)
	{
		if (!mGrid[right][j] && ys[j] != 0)
		{
			//get the point above
			int up = j-1;
			while (!mGrid[right][up])
				--up;
			//now the point below (note that there may be a couple of grid points missing
			int down = j+1;
			while (!mGrid[right][down] && down < mY )
				++down;
			InterpolateVertical(right,up,right,down,(float)1/(float)(down-up),right,j);
		}
	}

	//finally, we can go across the rows now, interpolating as necessary
	for (int j=0;j<mY;++j)
	{
		if (ys[j])
		{
			for (int i=0; i<mX; ++i)
			{
				if (xs[i] && !mGrid[i][j])
				{
					int left = i-1;
					while (!mGrid[left][j])
						--left;
					int right = i+1;
					while (!mGrid[right][j])
						++right;
					InterpolateHorizontal(left,j,right,j,(float)1/(float)(right-left),i,j);
				}
			}
		}
	}
}

void PSEGrid::Dump()
{
	int i,j;
	for (j=0; j<mY; ++j)
	{
		//first a row of whether grid patches are visible
		cout << "|";
		for (i=0; i<mX; ++i)
		{
			if (mGrid[i][j])
				if (i>=mX-2 || j>=mY-1)
					cout << "+";
				else if (IsPatchVisible(i+2,j))
					cout << "*";
				else
				  	cout << "o";
			else
				cout << " ";
		}

		//now a row of whether they've been drawn yet
		cout << "|		|";
		for (i=0; i<mX; ++i)
		{
			if (mGrid[i][j])
				if (i>=mX-2 || j>=mY-2)
					cout << "+";
				else if (IsPatchDrawn(i+2,j))
					cout << "D";
				else
				  	cout << "x";
			else
				cout << " ";
		}
		
		cout << "|" << endl;
	}

	//Have a look at the mArray coords at the grid points
	int tot = 0;
	for (j=0; j<mY; ++j)
	{
		for (i=0; i<mX; ++i)
		{
			if (mGrid[i][j])
			{
				cout << "(" << mGrid[i][j]->x() << "," << mGrid[i][j]->y() << ")	";
				tot++;
			}
			else
				cout << "(xxx,yyy)	";
		}
		cout << endl << "*************************************" << endl;
	}
	cout << "Found " << tot << " grid points" << endl;
}

void PSEGrid::InterpolateHorizontal(int x1, int y1, int x2, int y2, float percent, int gridx, int gridy)
{
	int newx, newy;
	float newz, m;
	m = ((float)(mGrid[x2][y2]->y() - mGrid[x1][y1]->y()))/((float)(mGrid[x2][y2]->x() - mGrid[x1][y1]->x()));
	newx = mGrid[x1][y1]->x() + (int)rint(percent*(float)(mGrid[x2][y2]->x() - mGrid[x1][y1]->x()));
	newy = (int)(m*(newx - mGrid[x1][y1]->x())) + mGrid[x1][y1]->y();
	float mz = ((float)(mGrid[x2][y2]->GetDepth()-mGrid[x1][y1]->GetDepth()))/((float)(mGrid[x2][y2]->x()-mGrid[x1][y1]->x()));
	newz = (mz*(newx - mGrid[x1][y1]->x())) + mGrid[x1][y1]->GetDepth();

	// we CAN extrapolate outside our mArray
	int height = mEngine->GetHeight();
	int width = mEngine->GetWidth();
	if (newy < 0) newy=0;
	if (newy >= height) newy=height-1;
	if (newx < 0) newx=0;
	if (newx >= width) newx=width-1;
		
	GridPoint *gp = new GridPoint;
	gp->x(newx);
	gp->y(newy);
	gp->SetDepth(newz);
/*	cout << "Adding (" << newx << "," << newy << "," << newz << ") for m=" << m << " mz=" << mz << 
		" between (" <<  mGrid[x1][y1]->x() << "," <<  mGrid[x1][y1]->y() << ") and (" <<
		mGrid[x2][y2]->x() << "," <<  mGrid[x2][y2]->y() << ")" << endl; */
	mGrid[gridx][gridy] = gp; 
	((ParametricSurfaceEngine *)AppFactory::GetInstance()->GetApp()->GetEngine())->SetPoint(newx,newy,newz);
}

void PSEGrid::InterpolateVertical(int x1, int y1, int x2, int y2, float percent, int gridx, int gridy)
{
	ParametricSurfaceEngine *eng = (ParametricSurfaceEngine *)AppFactory::GetInstance()->GetApp()->GetEngine();
	int newx, newy;
	float newz, m;
	//watch out for divide by zero
	if (mGrid[x2][y2]->x() != mGrid[x1][y1]->x())
	{
		m = ((float)(mGrid[x2][y2]->y() - mGrid[x1][y1]->y()))/((float)(mGrid[x2][y2]->x() - mGrid[x1][y1]->x()));
		newx = mGrid[x1][y1]->x() + (int)rint(percent*(float)(mGrid[x2][y2]->x() - mGrid[x1][y1]->x()));
		newy = (int)(m*(newx - mGrid[x1][y1]->x())) + mGrid[x1][y1]->y();
	} else 
	{  //vertical line
		newx = mGrid[x2][y2]->x();
		newy = mGrid[x1][y1]->y() + (int)rint(percent*(mGrid[x2][y2]->y() - mGrid[x1][y1]->y()));
	}
	float mz = (mGrid[x2][y2]->GetDepth()-mGrid[x1][y1]->GetDepth())/(mGrid[x2][y2]->x()-mGrid[x1][y1]->x());
	newz = (mz*(newx - mGrid[x1][y1]->x())) + mGrid[x1][y1]->GetDepth();
/*
	if (newx < 0) newx=0;
	if (newx > eng->GetWidth()-1) newx = eng->GetWidth()-1;
	if (newy < 0) newy=0;
	if (newy > eng->GetHeight()-1) newx = eng->GetHeight()-1;
*/
	GridPoint *gp = new GridPoint;
	gp->x(newx);
	gp->y(newy);
	gp->SetDepth(newz);
/*	cout << "Adding vertically interpolated (" << newx << "," << newy << "," << newz << ") for m=" << m << " mz=" << mz << 
		" between (" <<  mGrid[x1][y1]->x() << "," <<  mGrid[x1][y1]->y() << ") and (" <<
		mGrid[x2][y2]->x() << "," <<  mGrid[x2][y2]->y() << ")" << endl;
*/
	mGrid[gridx][gridy] = gp; 
	eng->SetPoint(newx,newy,newz);
}
	
void PSEGrid::ExtrapolateCorner(int x, int y, int corner)
{
	if (mGrid[x][y])
	{
//		cout << "Corner " << corner << " ok" << endl;
		return;
	}
		
	// find the two distances for possible extrapolation and use the smaller
	int e1,e2;
	float d1,d2; // d1 is N or S; d2 is E or W
	if (corner & _NORTH)
	{
		//coming up from the bottom
		int j=y+1;
		e1 = e2 = -1;
		bool found = false;
		while (!found && j < mY)
		{
			if (mGrid[x][j])
			{
				found = true;
				e1 = j;
			}
			++j;
		}
		if (found)
		{
			found = false;
			while (!found && j < mY)
			{
				if (mGrid[x][j])
				{
					found = true;
					e2 = j;
				}
				++j;
			}
		}
		if (!found)
		{
			ContourException x("Couldn't find enough north edge points to generate corners of grid.");
			throw x;
		}
	//	d1 = sqrt(pow(mGrid[x][e1]->y() - mGrid[x][e2]->y(),(double)2) +
	//				pow(mGrid[x][e1]->x() - mGrid[x][e2]->x(),(double)2));
	} else { //_SOUTH
		int j=y-1;
		e1 = e2 = -1;
		bool found = false;
		while (!found && j >= 0)
		{
			if (mGrid[x][j])
			{
				found = true;
				e1 = j;
			}
			--j;
		}
		if (found)
		{
			found = false;
			while (!found && j < mY)
			{
				if (mGrid[x][j])
				{
					found = true;
					e2 = j;
				}
				--j;
			}
		}

		if (!found)
		{
			ContourException x("Couldn't find enough south edge points to generate corners of grid.");
			throw x;
		}

	//	d1 = sqrt(pow(mGrid[x][e2]->y() - mGrid[x][e1]->y(),(double)2) +
	//				pow(mGrid[x][e2]->x() - mGrid[x][e1]->x(),(double)2));
	}	

	int f1,f2;
	if (corner & _EAST)
	{
		int i=x-1;
		f1 = f2 = -1;
		bool found = false;
		while (!found && i >= 0)
		{
			if (mGrid[i][y])
			{
				found = true;
				f1 = i;
			}
			--i;
		}
		if (found)
		{
			found = false;
			while (!found && i >= 0 )
			{
				if (mGrid[i][y])
				{
					found = true;
					f2 = i;
				}
				--i;
			}
		}
		if (!found)
		{
			ContourException  x("Couldn't find enough east edge points to generate corners of grid.");
			throw x;
		}

	//	d2 = sqrt(pow(mGrid[f1][y]->y() - mGrid[f2][y]->y(),(double)2) +
	//				pow(mGrid[f1][y]->x() - mGrid[f2][y]->x(),(double)2));
	} else { //_WEST
		int i=0;
		f1 = f2 = -1;
		bool found = false;
		while (!found && i < mX)
		{
			if (mGrid[i][y])
			{
				found = true;
				f1 = i;
			}
			++i;
		}
		if (found)
		{
			found = false;
			while (!found && i < mX)
			{
				if (mGrid[i][y])
				{
					found = true;
					f2 = i;
				}
				++i;
			}
		}

		if (!found)
		{
			ContourException x("Couldn't find enough west edge points to generate corners of grid.");
			throw x;
		}
	//	d2 = sqrt(pow(mGrid[f2][y]->y() - mGrid[f1][y]->y(),(double)2) +
	//			pow((double)mGrid[f2][y]->x() - mGrid[f1][y]->x(),(double)2));
	}

	//phweew! what a fucking pain in the ass this is. And now I realize we need to...
	// @REX the distances being checked above are between the two sets of extrapolation
	// candidates. We would rather check the distance between the closest to the corner
	// and the eventually generated point?!?

	//anyhoo, come up with a new GridPoint for the corner. It will obviously be
	// colinear with the two winning points from above, but the placement along
	// this line will be the same as that of the closest point along the losing axis.
	int newx, newy;
	float newz;
/*	if (d1 <= d2)
	{
		//vertical extrapolation line
		float m = ((float)(mGrid[x][e2]->y()-mGrid[x][e1]->y()))/((float)(mGrid[x][e2]->x()-mGrid[x][e1]->x()));
		newx = mGrid[f1][y]->x();
		newy = (int)rint((m*(float)(newx - mGrid[x][e1]->x())) + mGrid[x][e1]->y());
		float mz = ((float)(mGrid[x][e2]->GetDepth()-mGrid[x][e1]->GetDepth()))/((float)(mGrid[x][e2]->x()-mGrid[x][e1]->x()));
		newz = (int)rint((mz*(float)(newx - mGrid[x][e1]->x())) + mGrid[x][e1]->GetDepth());
	cout << "Adding corner (" << newx << "," << newy << "," << newz << ") for m=" << m << " mz=" << mz << 
		" along line thru (" <<  mGrid[x][e1]->x() << "," <<  mGrid[x][e1]->y() << ") and (" <<
		mGrid[x][e2]->x() << "," <<  mGrid[x][e2]->y() << ")" << "with our<< endl;

	} else {*/
		//horizontal extrapolation
		float m = ((float)(mGrid[f2][y]->y()-mGrid[f1][y]->y()))/((float)(mGrid[f2][y]->x()-mGrid[f1][y]->x()));
		newx = mGrid[x][e1]->x();
		newy = (int)rint((m*(float)(newx - mGrid[f1][y]->x())) + mGrid[f1][y]->y());
		float mz = ((float)(mGrid[f2][y]->GetDepth()-mGrid[f1][y]->GetDepth()))/((float)(mGrid[f2][y]->x()-mGrid[f1][y]->x()));
		newz = (int)rint((mz*(float)(newx - mGrid[f1][y]->x())) + mGrid[f1][y]->GetDepth());

		// we CAN extrapolate outside our mArray
		int height = mEngine->GetHeight();
		int width = mEngine->GetWidth();
		if (newy < 0) newy=0;
		if (newy >= height) newy=height-1;
		if (newx < 0) newx=0;
		if (newx >= width) newx=width-1;
		
/*	cout << "Adding corner (" << newx << "," << newy << "," << newz << ") for m=" << m << " mz=" << mz << 
		" between (" <<  mGrid[f1][y]->x() << "," <<  mGrid[f1][y]->y() << ") and (" <<
		mGrid[f2][y]->x() << "," <<  mGrid[f2][y]->y() << ")" << endl;
*/	//}

	GridPoint *gp = new GridPoint();
	gp->x(newx);
	gp->y(newy);
	((SamplePoint *)gp)->SetDepth(newz);
	mGrid[x][y] = gp;
	((ParametricSurfaceEngine *)AppFactory::GetInstance()->GetApp()->GetEngine())->SetPoint(newx,newy,newz);

}

//draw lines between vertices
void PSEGrid::Show()
{
	BathApp *app = AppFactory::GetInstance()->GetApp();
	ParametricSurfaceEngine *engine = (ParametricSurfaceEngine *)app->GetEngine();
	SamplePoint upperLeft = app->GetPreferences()->GetUpperLeft();
	float xscale = app->GetPreferences()->GetXScale();
	float yscale = app->GetPreferences()->GetYScale();
	SamplePoint lowerRight(upperLeft);
	lowerRight.Scale(engine->mWidth, engine->mHeight, xscale, yscale);

	//text stuff
	GdkGC *gc;
	GdkFont *font;
	gc = gdk_gc_new(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetCanvasControl()->window);
	gdk_gc_set_function(gc, GDK_XOR);
	GdkColor c;
	gdk_color_white(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetColorMap(),&c);
	gdk_gc_set_function(gc, GDK_XOR);
	gdk_gc_set_foreground(gc,&c);
	font = gdk_font_load("-adobe-courier-medium-r-normal-*-*-140-*-*-m-*-iso8859-1");
	gdk_gc_set_font(gc,font);

	
	SamplePoint size = upperLeft - lowerRight;
	
	for (int i=0; i<mY-1; ++i)
	{
		for (int j=0; j<mX-1; ++j)
		{
			int color=200;
			if (mGrid[j][i])
				color=(int)rint(mGrid[j][i]->GetDepth());
			((ParametricSurfaceEngine *)engine)->SetPoint(j*mGranularity,i*mGranularity,color);
			if (mGrid[j][i])
			{
				//Go until we find another occupied node
				int p=i+1;
				while (p < mY && !mGrid[j][p])
					++p;
				if (p < mY && mGrid[j][p])
					DrawLine(engine,mGrid[j][i]->x(),mGrid[j][p]->x(),mGrid[j][i]->y(),mGrid[j][p]->y());
				p=j+1;
				while (p < mX && !mGrid[p][i])
					++p;
				if (p < mX && mGrid[p][i])
					DrawLine(engine,mGrid[j][i]->x(),mGrid[p][i]->x(),mGrid[j][i]->y(),mGrid[p][i]->y());

				//show the grid coords
				char str[20];
				sprintf(str,"(%d,%d)",j,i);
				cout << str << " " << mGrid[j][i]->x() << " " << mGrid[j][i]->y() << endl;
				gdk_draw_text(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetDrawingSurfacePixmap(),font,gc,mGrid[j][i]->x()-EDGE_BUFFER,mGrid[j][i]->y()-EDGE_BUFFER,str,::strlen(str));
			}
		}
	}				
}

void PSEGrid::DrawLine(ContourEngine *engine, int x1, int x2, int y1, int y2)
{
	if (x2 != x1)
	{
		float m = (float)(y2-y1)/(float)(x2-x1);
			
		if (m <= 1 && m >= -1)
		{
			// y = m(x - x1) + y1
			if (x1 <= x2) {
				for (int i=x1+1; i<x2; ++i)
				{
					((ParametricSurfaceEngine *)engine)->SetPoint(i,(int)(((float)(i-x1)*m)+y1),(float)200);
				}
			} else {
				for (int i=x1+1; i>x2; --i)
				{
					((ParametricSurfaceEngine *)engine)->SetPoint(i,(int)(((float)(i-x1)*m)+y1),(float)200);
				}
			}	
		} else {  // slope < 1 (more vertical)
			// x = (y - y1)/m + x1
			if (y1 <= y2) {
				for (int i=y1+1; i<y2; ++i)
				{
					((ParametricSurfaceEngine *)engine)->SetPoint((int)(((float)(i-y1)/m)+x1),i,(float)200);
				}
			} else {
				for (int i=y1+1; i>y2; --i)
				{
					((ParametricSurfaceEngine *)engine)->SetPoint((int)(((float)(i-y1)/m)+x1),i,(float)200);
				}
			}	
		}
	}
	else
	{ //vertical line
		if (y1 <= y2)
			for (int i=y1+1;i<y2;++i)
				((ParametricSurfaceEngine *)engine)->SetPoint(x1,i,200);
		else
			for (int i=y2+1;i<y1;++i)
				((ParametricSurfaceEngine *)engine)->SetPoint(x1,i,200);
	}
	
}

/*
** Shift - When we pan, we need to move everything in the grid in the direction
**	opposite of the pan. Anything newly exposed is properly initialized. We do our
**	best to maintain the grid so that we don't have to redraw stuff we've already
**	drawn and that the newly exposed stuff is continuous because it shares the 
**	control points that already are in the grid.
*/
void PSEGrid::Shift(int ns, int ew)
{
	int width = mEngine->GetWidth();
	
	if (ns)
	{
	   //shift the data in mGrid by an amount scaled by the granularity of the mesh
	   int shift = (int)rint((double)ns/(double)mGranularity);
	   if (shift > 0) // NORTH
	   {
	   		int x,y;
	     	for (x = 0; x < mX; ++x)
	     	{
	       		for (y = mY - shift - 1; y > 0; y--)
	       		{
	       			//move the actual grid point
					mGrid[x][y+shift] = mGrid[x][y]; 
					//and adjust its mArray translation
					if (mGrid[x][y])
						mGrid[x][y+shift]->y(mGrid[x][y]->y()+ns);
					//adjust our array of booleans that specify whether a patch
					// has been drawn or not. Remember the dimensions of this array
					// are 1 less than the size of the mGrid in both directions.
					if (x < mX-1 && y+shift < mY-1 && y < mY-1)
						mDrawnPatches[x][y+shift] = mDrawnPatches[x][y];
				}
	     	}
	     	//now initialize the newly exposed points at the top of the screen
	     	for (x=0; x<mX; ++x)
	     	{
	     		for (y=0; y <= shift; ++y)
	     		{
	     			mGrid[x][y] = NULL;
	     			if (x<mX-1 && y<mY-1)
	     				mDrawnPatches[x][y] = false;
	     		}
	     	}
	   }
	   else  //shift < 0  -- SOUTH
	   {
	   		int x,y;
	     	for (x = 0; x < mX; ++x)
	     	{
	       		for (y = -shift; y < mY; y++)
	       		{
					mGrid[x][y+shift] = mGrid[x][y]; 
					if (mGrid[x][y])
						mGrid[x][y+shift]->y(mGrid[x][y]->y()+ns);
					if (x < mX-1 && y+shift < mY-1 && y < mY-1)
						mDrawnPatches[x][y+shift] = mDrawnPatches[x][y];
				}
	     	}
	     	//now initialize the newly exposed points at the bottom of the screen
	     	for (x=0;x<mX;++x)
	     	{
	     		for (y = mY + shift - 1; y<mY; ++y)
	     		{
	     			mGrid[x][y] = NULL;
	     			if (y < mY-1 && x < mX-1)
	     			{
	     				//to preserve G1 continuity, the ramifications of the new
	     				// set of points will be felt within the 16 control point
	     				// area needed to draw any given patch, thus we need to
	     				// redraw an extra patch.
	     				mDrawnPatches[x][y] = false;
	     				if (y-1 > 0) mDrawnPatches[x][y-1] = false;
	     			}
	     		}
	     	}
	   }
	}
	if (ew)
	{
	   //shift the data in mGrid by an amount scaled by the granularity of the mesh
	   int shift = (int)rint((double)ew/(double)mGranularity);
	   if (shift > 0)  //west, move points right
	   {
	   		int x,y;
	     	for (y = 0; y < mY; ++y)
	     	{
	       		for (x = mX - shift - 1; x > 0; x--)
	       		{
					mGrid[x+shift][y] = mGrid[x][y]; 
					if (mGrid[x][y])
						mGrid[x+shift][y]->x(mGrid[x][y]->x()+ew);
					if (x+shift < mX-1 && y < mY-1 && x < mX-1)
						mDrawnPatches[x+shift][y] = mDrawnPatches[x][y];
				}
	     	}
	     	//now initialize the newly exposed points
	     	for (y=0; y<mY; ++y)
	     	{
	     		for (x=0; x <= shift; ++x)
	     		{
	     			mGrid[x][y] = NULL;
	     			if (y < mY-1 && x < mX-1)
	     				mDrawnPatches[x][y] = false;
	     		}
	     	}
	   }
	   else  //shift < 0 -- panning east, move points left
	   {
	   		int x,y;
	     	for (y = 0; y < mY; ++y)
	     	{
	       		for (x = -shift; x < mX; x++)
	       		{
					mGrid[x+shift][y] = mGrid[x][y]; 
					if (mGrid[x][y])
						mGrid[x+shift][y]->x(mGrid[x][y]->x()+ew);
					if (x+shift < mX-1 && y < mY-1 && x < mX-1)
						mDrawnPatches[x+shift][y] = mDrawnPatches[x][y];
					//cout << "shifting (" << x << "," << y << ") to (" << x+shift << "," << y << ")" << endl;
				}
	     	}
	     	//now initialize the newly exposed points
	     	for (y = 0; y < mY; ++y)
	     	{
	       		for (x = mX + shift - 1; x < mX; x++)
	       		{
	       			mGrid[x][y] = NULL;
	     			if (y < mY-1 && x < mX-1)
	     			{
	     				//to preserve G1 continuity, the ramifications of the new
	     				// set of points will be felt within the 16 control point
	     				// area needed to draw any given patch, thus we need to
	     				// redraw an extra patch.
	       				mDrawnPatches[x][y] = false;
	       				if (x-1 >= 0) mDrawnPatches[x-1][y] = false;
	       			}
	       		}
	       	}
	     }
	}
}

/*
** IsPatchVisible - Returns true if some or all of the patch referenced by the params,
**	which are interpreted as one grid co-ordinate to the right of the top right corner
**	of a patch, is visible in the current viewport.
**	@REX This assumes the borders of the patch are straight, which they aren't!
*/
bool PSEGrid::IsPatchVisible(int x, int y)
{
	
	//find the right side of the patch (patches aren't always one grid unit per side)
	bool found;
	int right, left, bottom;
	found = FindOneLeft(x, y, right);
	if (!found)
	{
		cerr << "Couldn't find right edge of patch in visibility check! " << x << "," << y << endl;
		return false;
	}

	//now the left
	found = FindOneLeft(right, y, left);
	if (!found)
	{
		cerr << "Couldn't find left edge of patch in visibility check! " << x << "," << y << " @ " << y << endl;
		return false;
	}

	
	//and the bottom
	found = false;
	for (bottom = y+1; bottom < mY && !found; ++bottom)
		if (mGrid[x][bottom] != NULL)
			found = true;
	//take one back
	bottom--;
	
	if (!found)
	{
		cerr << "Couldn't find bottom edge of patch in visibility check!" << x << "," << y << " @ " << y << endl;
		return false;
	}

	//cout << "visibility check! (l,r,x) " << left << "," << right << "," << x << " @ " << y << endl;

				
	if 	(mEngine->IsPointVisible(mGrid[left][y]->x(),mGrid[left][y]->y()) ||
		mEngine->IsPointVisible(mGrid[right][y]->x(),mGrid[right][y]->y()) ||
		mEngine->IsPointVisible(mGrid[left][bottom]->x(),mGrid[left][bottom]->y()) ||
		mEngine->IsPointVisible(mGrid[right][bottom]->x(),mGrid[right][bottom]->y()))
	{
		return true;
	}
/*	cout << "Points NV: (" << mGrid[left][y]->x() << "," << mGrid[left][y]->y() <<
	") (" << mGrid[right][y]->x() << "," << mGrid[right][y]->y() <<
	") (" << mGrid[left][bottom]->x() << "," << mGrid[left][bottom]->y() <<
	") (" << mGrid[right][bottom]->x() << "," << mGrid[right][bottom]->y() << ") at grid "
	<< left << "," << y <<endl; */
	return false;
}

/*
** IsPatchDrawn - As above, due to the nature of the local variables kept in Fill(),
**	the x & y parameters are the grid indices of a point one to the right of the
**	top right corner of the patch in question.
*/
bool PSEGrid::IsPatchDrawn(int x, int y)
{
	//find the right side of the patch (patches aren't always one grid unit per side)
	bool found;
	int right, left, bottom;
	found = FindOneLeft(x, y, right);
	if (!found)
	{
		cerr << "Couldn't find right edge of patch while checking patch drawn! " << x << "," << y << endl;
		return false;
	}

	//now the left
	found = FindOneLeft(right, y, left);
	if (!found)
	{
		cerr << "Couldn't find left edge of patch while checking patch drawn! " << x << "," << y << endl;
		return false;
	}

	return mDrawnPatches[left][y];
}

void PSEGrid::MarkPatchDrawn(int x, int y, bool drawn)
{
	//find the right side of the patch (patches aren't always one grid unit per side)
	bool found;
	int right, left, bottom;
	found = FindOneLeft(x, y, right);
	if (!found)
	{
		cerr << "Couldn't find right edge of patch while setting patch drawn! " << x << "," << y << endl;
		return;
	}

	//now the left
	found = FindOneLeft(right, y, left);
	if (!found)
	{
		cerr << "Couldn't find left edge of patch while setting patch drawn! " << x << "," << y << endl;
		return;
	}

	mDrawnPatches[left][y] = drawn;
}

/*
** FindOneLeft - Utility function for previous three methods.
*/
bool PSEGrid::FindOneLeft(int x, int y, int &target)
{
	bool found = false;
	for (target = x-1; target >= 0 && !found; target--)
	{
		if (mGrid[target][y] != NULL)
			found = true;
	}
	target++;  //take back last decrement
	return found;
}

ParametricSurfaceEngine::ParametricSurfaceEngine()
{
	mGrid = NULL;
	mArray = NULL;
	mCanvas = AppFactory::GetInstance()->GetApp()->GetCanvas();
	GridPoint proto;
	AppFactory::GetInstance()->GetApp()->GetExtractor()->Initialize("data/42_068.nos",proto);
}

ParametricSurfaceEngine::~ParametricSurfaceEngine()
{
	if (mGrid)
		delete mGrid;
}

bool ParametricSurfaceEngine::Initialize(int height, int width)
{

	ContourEngine::Initialize(height, width);
}

/*
** SetPoints - This vector is the "hard" data that we are provided with. It is
**	in an STL vector of SamplePoints. We dump it into our array.
**	Note that we need to know the area of the real world that our array
**	represents to do this.
*/
void ParametricSurfaceEngine::SetPoints(SamplePointVector &v, int x1, int x2, int y1, int y2)
{
	ContourCursor::WaitCursor();

	Timer::getInstance()->mark("Begin ParametricSurfaceEngine::SetPoints");
	BathApp *app = AppFactory::GetInstance()->GetApp();
	ContourCanvas *canvas = app->GetCanvas();
	SamplePoint upperLeft = app->GetPreferences()->GetUpperLeft();
	float xscale = app->GetPreferences()->GetXScale();
	float yscale = app->GetPreferences()->GetYScale();
	SamplePoint lowerRight(upperLeft);
	lowerRight.Scale(mWidth, mHeight, xscale, yscale);
	float z;
	SamplePoint size = upperLeft - lowerRight;
	
	//the grid granularity is the ratio between the grid (which defines the "patches"
	// our bicubic parametric engine works over
	int grid_granularity = (int)(((float)PSE_GRID_GRANULARITY) / AppFactory::GetInstance()->GetApp()->GetPreferences()->GetXScale());


	//only do the autocolor thing if we aren't panning
	if (!GetPanning())
	{
		mMaxDepth=0;
		mMinDepth=100000;
	}
		
	if (!mGrid || !mPanning)
	{
		cout << "Granularity " << grid_granularity << endl;
		if (mGrid)
			delete mGrid;
		mGrid = new PSEGrid;
		mGrid->SetSize((x2-x1)/grid_granularity,(y2-y1)/grid_granularity,grid_granularity);
	}
	
	//traverse all the points and put them into the grid.
	SamplePointVector::iterator it;
	for (it = v.begin(); it != v.end(); it++)
	{
		//For each sample point, we ask it for its x and y
		// coordinates, given the current viewport settings
		// See SamplePoint::Get[X|Y] for more details.
		//convert to screen co-ordinates
		int x = (*it)->GetX(&upperLeft, &size, mWidth);
		int y = (*it)->GetY(&upperLeft, &size, mHeight);

		z = (*it)->GetDepth();
		//put the sample points into the pixmap
		//mArray[x][y] = z;
		//canvas->Draw(x,y,z);
		SetPoint(x,y,z);
		//generate the polygon grid
		mGrid->InsertPoint((GridPoint *)*it,x,y);

		//only do the autocolor thing if we aren't panning
		if (!GetPanning())
		{
			if (mMinDepth > z)
				mMinDepth = z;
			if (mMaxDepth < z)
				mMaxDepth = z;
		}
	}
	
	Timer::getInstance()->mark("New points incorporated");
	
	//Interpolate all the sample points
	//pass in the grid coordinates we want filled (not the mArray coords!)
	//We move "in" one from each co-ordinate because the patch is generated in the middle
	// of the 16 gridpoints. See Fill for more details.
	//mGrid->Fill((x1/grid_granularity)+1,(x2/grid_granularity)-1,(y1/grid_granularity)+1,(y2/grid_granularity)-1);
	//mGrid->Fill(0,13,0,10);
	mGrid->Fill(0,(mWidth/grid_granularity),0,(mHeight/grid_granularity));

	//CleanUp();
	Render(x1,x2,y1,y2);
	Timer::getInstance()->mark("End ParametricSurfaceEngine::SetPoints");
	ContourCursor::RemoveWaitCursor();

}

void ParametricSurfaceEngine::CleanUp()
{
	int count;
	float sum,avg;
	//Pick off any stray spots that haven't been colored
	// @REX HOW EXPENSIVE IS THIS?
	for (int i=EDGE_BUFFER;i<mWidth-EDGE_BUFFER;++i)
	{
		for (int j=EDGE_BUFFER;j<mHeight-EDGE_BUFFER;++j)
		{
			if (mArray[i][j] == NO_DEPTH)
			{
				count=0;
				sum = 0.0;
				//see how many points there are around
				for (int p=i-1;p<i+2;++p)
				{
					for (int q=j;q<j+2;++q)
					{
						if (mArray[p][q] != NO_DEPTH)
						{
							sum += mArray[p][q];
							++count;
						}
					}
				}

				if (count > 3)
				{
					mArray[i][j] = ((int)(sum/ (float)count));
					cout << "Cleaning up point (" << i << "," << j << ")" << endl;
				}
			}
		}
	}
}

void ParametricSurfaceEngine::EndPan(Panner &panner)
{
	//first we need to get our grid shifted
	if (mGrid)
	{
		mGrid->Shift(panner.GetNSOffset(),panner.GetEWOffset());
		SetPanning(true);
	}

	ContourEngine::EndPan(panner);
}
