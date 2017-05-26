/*
** engine.cpp - abstract base class for algo-specific subclasses
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.21.98 DPT
*/

#include "engine.h"
#include "canvas.h"
#include "appFactory.h"
#include "preferences.h"
#include "timer.h"
#include "panner.h"
#include "extractor.h"
#include "exception.h"
#include "latLongGrid.h"
#include "gps.h"
#include <stdio.h>

ContourEngine::ContourEngine()  
{
	mArray=NULL;
	mHeight=mWidth=0;
	mPanning=false;
	mResizing = false;
}

ContourEngine::~ContourEngine()
{
	if (mArray)
		for (int i=0; i < mWidth; i++)
		for (int j=0; j < mHeight; j++)
			delete [] mArray[i];
}

/*
** Initialize - We represent the world (or at least our viewport on it) as a
**	series of floats that are the depth (or altitude) at that point. The
**	floats are kept in dynamically allocated arrays of floats. The first
**	dimension is horizontal so the arrays look like this:
**		||||||||||
**		||||||||||
**		""""""""""
*/
bool ContourEngine::Initialize(int height, int width)
{
	Timer::getInstance()->mark("Begin ContourEngine::Initialize");

	BathApp *app = AppFactory::GetInstance()->GetApp();
	app->GetCanvas()->BeginDrawing();

	int i,j,k=0;
	// if the screen is being created or resized, we need to allocate memory
	if (mResizing)
	{
		if (mArray)
			for (i=0; i < mWidth; i++)
				delete [] mArray[i];

		mHeight = height+2*EDGE_BUFFER;
		mWidth = width+2*EDGE_BUFFER;
		mArray = new float *[mWidth];
		for (i=0; i < mWidth; i++)
			mArray[i] = new float[mHeight];
	}
	//initialize each cell to default value (really deep is black)
	for (i=0; i < mWidth; i++)
		for (j=0; j < mHeight; j++)
		{
			mArray[i][j] = (float)10000;
			k++;
		}
	Timer::getInstance()->mark("End refresh vals");

	ExposeArea(0,mWidth,0,mHeight);
		
	Timer::getInstance()->mark("End ContourEngine::Initialize");

	return true;
}

/*
** Render - Pass no params to show entire viewport.
*/
void ContourEngine::Render(int x1, int x2, int y1, int y2)
{
	//handle no param default
	if (x1 == -1)
	{
		x1 = EDGE_BUFFER;
		x2 = mWidth - EDGE_BUFFER;
		y1 = EDGE_BUFFER;
		y2 = mHeight - EDGE_BUFFER;
	}
	
	//these are the cases where the requested areas can't possibly be seen
	if(	(x1 > mWidth - EDGE_BUFFER) ||
		(x2 < EDGE_BUFFER) ||
		(y1 > mHeight - EDGE_BUFFER) ||
		(y2 < EDGE_BUFFER)) {
		cerr << "Bad render request for (" << x1 << "," << y1 << ") (" << x2 << "," << y2 << ")" << endl;
		return;
	}

	//now make sure we're only drawing visible stuff (inside the edge buffers)
	// this is important because the canvas we are drawing to is only the size
	// of the mArray clipped by EDGE_BUFFER on each side.
	if (x1 < EDGE_BUFFER)
		x1 = EDGE_BUFFER;
	if (x2 > mWidth - EDGE_BUFFER || x2 == -1)
		x2 = mWidth - EDGE_BUFFER;
	if (y1 < EDGE_BUFFER)
		y1 = EDGE_BUFFER;
	if (y2 > mHeight - EDGE_BUFFER || y2 == -1)
		y2 = mHeight - EDGE_BUFFER;
		
	BathApp *app = AppFactory::GetInstance()->GetApp();
	ContourCanvas *canvas = app->GetCanvas();
	if (app->GetPreferences()->GetAutoColor())
	{
		canvas->SetMaxDepth(GetMaxDepth());
		canvas->SetMinDepth(GetMinDepth());
	} else
	{
		canvas->SetMaxDepth(app->GetPreferences()->GetMaxDepth());
		canvas->SetMinDepth(app->GetPreferences()->GetMinDepth());
	}

	for (int i = x1; i < x2; i++)
		for (int j = y1; j < y2; j++)
			canvas->Draw(i-EDGE_BUFFER,j-EDGE_BUFFER,mArray[i][j]);
/*
	// DrawPartial wants canvas coords, not mArray coords
	if (app->GetPreferences()->GetShowGrid())
	{
		LatLongGrid::GetInstance()->DrawPartial(x1-EDGE_BUFFER,x2-EDGE_BUFFER,y1-EDGE_BUFFER,y2-EDGE_BUFFER);
		//LatLongGrid::GetInstance()->DrawTextPartial(x1-EDGE_BUFFER,x2-EDGE_BUFFER,y1-EDGE_BUFFER,y2-EDGE_BUFFER);
		LatLongGrid::GetInstance()->DrawText();
	}
	*/
	ContourGraphic::SetViewPort();
	canvas->DoneDrawing();
}

/*
** Resize - When the canvas gets a new shape we call this to re-adjust our
**	array size.
**
**	FIXME - Need to set this up so that we can handle this better. To do this
**	we will need to:
**		1) Create a new array of the proper size.
**		2) Copy in the values from mArray
**		3) Initialize the values that are not in the intersection of
**		the old array and the new array to NO_DEPTH.
*/
bool ContourEngine::Resize(int height, int width)
{
	mResizing = true;
	return Initialize(height, width);
	mResizing = false;
}

void ContourEngine::Pan(Panner &panner)
{

}

/*
**
*/
void ContourEngine::BeginPan(Panner &panner)
{
	//first thing is to take off the lat long grid -- otherwise the numbers can
	// get replicated across..
	BathApp *app = AppFactory::GetInstance()->GetApp();
	app->GetCanvas()->BeginDrawing();

}

/*
** EndPan - We have received a request for a lateral movement.
*/
void ContourEngine::EndPan(Panner &panner)
{
	BathApp *app = AppFactory::GetInstance()->GetApp();

	//determine how much we moved.
	int ns = panner.GetNSOffset();	
	int ew = panner.GetEWOffset();	

	int x1,x2,y1,y2;   //store the boundaries of the exposed area here.

	//check to see if we went at an angle (NE,SE,SW or NW). If so, 
	// we want to make sure we don't redo the overlapped corner twice.
	bool overlap = false;
	if (ew && ns)
		overlap = true;

	//Do the North-South rect first
	if (ns)
	{
	   //shift the data in the mArray
	   if (ns > 0)
	   {
	     	for (int x = 0; x < mWidth; ++x)
	     	{
	       		for (int y = mHeight - ns -1; y > 0; y--)
					mArray[x][y+ns] = mArray[x][y]; 
	     	}
			y1 = 0;
			y2 = ns + EDGE_BUFFER +50;
		}
		else  //ns < 0
		{
	     	for (int x = 0; x < mWidth; ++x)
	     	{
	       		for (int y = -ns; y < mHeight; y++)
				mArray[x][y+ns] = mArray[x][y]; 
	     	}
			y1 = mHeight + ns - EDGE_BUFFER -50;
			y2 = mHeight;
	   }
	}  else 
	{
		y1 = 0;
		y2 = mHeight;
	}
	
	if (ew)
	{
	   //shift the data in the mArray
	   if (ew > 0)
	   {
	     	for (int y = 0; y < mHeight; ++y)
	     	{
	       		for (int x = mWidth - ew -1; x > 0; x--)
					mArray[x+ew][y] = mArray[x][y]; 
	     	}
			x1 = 0;
			x2 = ew + EDGE_BUFFER +50;
		}
		else  //ew < 0
		{
	     	for (int y = 0; y < mHeight; ++y)
	     	{
	       		for (int x = -ew; x < mWidth; x++)
				mArray[x+ew][y] = mArray[x][y]; 
	     	}
			x1 = mWidth + ew - EDGE_BUFFER -50;
			x2 = mWidth;
	   }
	} else
	{
		x1 = 0;
		x2 = mWidth;
	}


	//Save new upper left and lower right
	SamplePoint newUL(app->GetPreferences()->GetUpperLeft());
	float xscale = app->GetPreferences()->GetXScale();
	float yscale = app->GetPreferences()->GetYScale();
	//newUL.Dump();
	newUL.Scale(-ew, -ns, xscale, yscale);
	app->GetPreferences()->SetUpperLeft(newUL);
	app->GetPreferences()->Save();
	
	if (!overlap)
	{
		//Render(x1,x2,y1,y2);
		ExposeArea(x1, x2, y1, y2);
	} else  //moving diagonally means redo the whole shooting match
	{
		//Render(0,mWidth,0,mHeight);
		ExposeArea(0,mWidth,0,mHeight);
	}

}

/*
** ExposeArea - Either through panning or just initializing the whole thing.
*/
void ContourEngine::ExposeArea(int x1, int x2, int y1, int y2)
{
	BathApp *app = AppFactory::GetInstance()->GetApp();
	if (app->GetPreferences()->GetAutoCenter())
	{
		//the current viewport
		//SamplePoint newUL(app->GetPreferences()->GetUpperLeft());
		float xscale = app->GetPreferences()->GetXScale();
		float yscale = app->GetPreferences()->GetYScale();

		//new center
		Position lat(GPS::GetInstance()->GetLatitude());
		Position lon(GPS::GetInstance()->GetLongitude());
		SamplePoint center(lat,lon);

		//current center
		//SamplePoint center(newUL);
		//center.Scale(mWidth/2,mHeight/2,xscale,yscale);

		//shift the center up and left to find the new upperleft corner
		center.Scale(-(mWidth - (2*EDGE_BUFFER))/2,-(mHeight - (2*EDGE_BUFFER))/2,xscale,yscale);
		cout << "Setting autocenter center to lat " << endl;
		center.Dump();
		app->GetPreferences()->SetUpperLeft(center);
	}
	
	if (app->GetPreferences()->GetDraw())
	{	
		//determine the lat and long of these points so we can get some data for them
		SamplePoint ul = app->GetPreferences()->GetUpperLeft();
		float xscale = app->GetPreferences()->GetXScale();
		float yscale = app->GetPreferences()->GetYScale();
		SamplePoint n1(ul);
		n1.Scale(x1,y1,xscale,yscale);
		SamplePoint n2(ul);
		n2.Scale(x2,y2,xscale,yscale);
		
		//initialize the exposed area
		//for (int x = x1; x < x2; ++x)
		//	for (int y = y1; y < y2; ++y)
		//		mArray[x][y] = NO_DEPTH;

		cout << "Exposing area (" << x1 << "," << y1 << ") (" << x2 << "," << y2 << ")" << endl;
		SamplePointVector *v = app->GetExtractor()->GetSamplePoints(&n1,&n2);
	try
	{
		SetPoints(*v,x1,x2,y1,y2);
	}
	catch (ContourException &x)
	{
		cout << x.GetString() << endl;
	}
	} else
	{
		//skip the surface generation and just dump out the existing contents
		Render(x1,x2,y1,y2);
	}
}

/*
** Erase - Flush out the whole array and (re)fill it.
*/
void ContourEngine::Erase()
{
	Initialize(mHeight - (2 * EDGE_BUFFER),mWidth - (2 * EDGE_BUFFER));
	//ExposeArea(0,mWidth,0,mHeight);
}

void ContourEngine::Zoom(float amount)
{
	//Find a new top left & lower right
	BathApp *app = AppFactory::GetInstance()->GetApp();

	SamplePoint ul = app->GetPreferences()->GetUpperLeft();
	float xscale = app->GetPreferences()->GetXScale();
	float yscale = app->GetPreferences()->GetYScale();

//	cout << "Zoom scale " << xscale << endl;

	//Figure out where the middle of the screen is.
	// use the canvas width and height?
	int width = mWidth; //app->GetCanvas()->GetWidth();
	int height = mHeight; // app->GetCanvas()->GetHeight();
	SamplePoint middle(ul);
	middle.Scale(width/2,height/2,xscale,yscale);

	//Set our preferences
	float new_xscale = xscale * amount;
	float new_yscale = yscale * amount;
	app->GetPreferences()->SetXScale(new_xscale);
	app->GetPreferences()->SetYScale(new_yscale);
	
	//now go back to the upper left with the new lat/long co-ords 
	SamplePoint newUL(middle);
	newUL.Scale(-width/2,-height/2,new_xscale,new_yscale);

	app->GetPreferences()->SetUpperLeft(newUL);

	app->GetCanvas()->RemoveDecoration(ContourCanvas::ALL);
	SetPanning(false);
	
	try
	{
		ExposeArea(0,mWidth,0,mHeight); 

		//now redraw all the screen decorations like the graticule and GPS readout
		app->GetCanvas()->BeginDrawing();
		app->GetCanvas()->AddDecoration(ContourCanvas::ALL);
		app->GetCanvas()->DoneDrawing();
	}
	catch (ContourException &x)
	{
		cout << x.GetString() << endl;
		//if the zoom doesn't work out, reset the values
		app->GetPreferences()->SetXScale(xscale);
		app->GetPreferences()->SetYScale(yscale);
		app->GetPreferences()->SetUpperLeft(ul);
	}
	catch (...)
	{
		//if the zoom doesn't work out, reset the values
		app->GetPreferences()->SetXScale(xscale);
		app->GetPreferences()->SetYScale(yscale);
		app->GetPreferences()->SetUpperLeft(ul);
	}
}

/*
** IsPointVisible - Returns true if the referenced point is visible in the current
**	viewport.
*/
bool ContourEngine::IsPointVisible(int x, int y)
{
	if ((x > EDGE_BUFFER) &&
		(x < mWidth - EDGE_BUFFER) &&
		(y > EDGE_BUFFER) &&
		(y < mHeight - EDGE_BUFFER))
		return true;
	else
		return false;
}
