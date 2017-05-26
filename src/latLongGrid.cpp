/*
** LatLongGrid.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Dec 31 19:59:22 EST 2000
*/
#include "appFactory.h"
#include "preferences.h"
#include "latLongGrid.h"
#include "preferences.h"
#include "engine.h"
#include "canvas.h"
#include <math.h>

//debug only
//#include "gtkLatLongLine.h"

LatLongGrid::LatLongGrid()
{
	mLats = NULL;
	mLongs = NULL;
	mClear = true;
}

LatLongGrid::~LatLongGrid()
{
	if (mLats)
		for (int i=0; i<LATLONGGRID_NUM_LAT_DIVISIONS; ++i)
			delete mLats[i];

	if (mLongs)
		for (int i=0; i<LATLONGGRID_NUM_LAT_DIVISIONS; ++i)
			delete mLongs[i];		
}

void LatLongGrid::Draw()
{
	ASSERT(mClear);
	if (!mClear)
		return;

	//@REX Don't we only need to do this if we zoom or resize?
	//SetUpGrid();
	
	for (int i=0; i < LATLONGGRID_NUM_LAT_DIVISIONS; ++i)
		mLats[i]->Draw();

	for (int j=0; j < LATLONGGRID_NUM_LAT_DIVISIONS; ++j)
		mLongs[j]->Draw();	

	mClear = false;
}

void LatLongGrid::DrawPartial(int x1, int x2, int y1, int y2)
{
	SetUpGrid();
	
	for (int i=0; i < LATLONGGRID_NUM_LAT_DIVISIONS; ++i)
		mLats[i]->DrawPartial(x1,x2,y1,y2);

	for (int j=0; j < LATLONGGRID_NUM_LAT_DIVISIONS; ++j)
		mLongs[j]->DrawPartial(x1,x2,y1,y2);
}

/*
**
*/
void LatLongGrid::SetUpGrid()
{
	BathApp *app = AppFactory::GetInstance()->GetApp();
	ContourPreferences *prefs = app->GetPreferences();

	if (mLats)
		for (int i=0; i<LATLONGGRID_NUM_LAT_DIVISIONS; ++i)
			delete mLats[i];

	if (mLongs)
		for (int i=0; i<LATLONGGRID_NUM_LAT_DIVISIONS; ++i)
			delete mLongs[i];		
		
	//figure out how many we need - first we need the number of seconds of lat and
	// long we have on screen.
	SamplePoint ul(prefs->GetUpperLeft());
	float xscale = prefs->GetXScale();
	float yscale = prefs->GetYScale();

	ContourEngine *engine = app->GetEngine();
	ContourCanvas *canvas = app->GetCanvas();
	int width = canvas->GetWidth();
	int height = canvas->GetHeight();

	// scale is in seconds/pixel
	float latsecs = ((float)height) * yscale;
	float longsecs = ((float)width) * xscale;

	int lat_incr = GetDefaultIncrement(latsecs/(float)LATLONGGRID_NUM_LAT_DIVISIONS);
	int long_incr = GetDefaultIncrement(abs((int)longsecs)/(float)LATLONGGRID_NUM_LAT_DIVISIONS);

	Position lat_incr_pos((float)lat_incr);
	Position long_incr_pos((float)long_incr);

	// here are the first lat and long lines
	//  we use the mod op here to get the first line to "align" with the increments
	int mod_lat = (int)rint(ul.GetLatitude().TotalSeconds()) % lat_incr;
	int mod_lon = (int)rint(ul.GetLongitude().TotalSeconds()) % long_incr;
	Position latitude((float)((int)rint(ul.GetLatitude().TotalSeconds()) - lat_incr - mod_lat));
	Position longitude((float)((int)rint(ul.GetLongitude().TotalSeconds()) + long_incr - mod_lon));

	// we store them in our member variables
	mLats = new LatLongLine *[LATLONGGRID_NUM_LAT_DIVISIONS];
	mLongs = new LatLongLine *[LATLONGGRID_NUM_LAT_DIVISIONS];

	for (int i=0; i < LATLONGGRID_NUM_LAT_DIVISIONS; ++i)
	{
		SamplePoint::Normalize_position(latitude);
		mLats[i] = app->GetLatLongLine(latitude,true);
		//cout << "Lat line minutes " << latitude.
		latitude = latitude - lat_incr_pos;
	}

	for (int j=0; j < LATLONGGRID_NUM_LAT_DIVISIONS; ++j)
	{
		SamplePoint::Normalize_position(longitude);
		mLongs[j] = app->GetLatLongLine(longitude,false);
		longitude = longitude + long_incr_pos;
	}
}

void LatLongGrid::Clear()
{
	// first time its ok 
	ASSERT(!mClear);
	if (mClear)
		return;
		
	for (int i=0; i < LATLONGGRID_NUM_LAT_DIVISIONS; ++i)
		mLats[i]->Draw();

	for (int j=0; j < LATLONGGRID_NUM_LAT_DIVISIONS; ++j)
		mLongs[j]->Draw();	
		
	mClear = true;
}

void LatLongGrid::DrawText()
{
	SetUpGrid();
	
	if (!mLats || !mLongs)
		throw ("Lat Long Grid: Draw() or DrawPartial() before calling DrawText().");

	for (int i=0; i < LATLONGGRID_NUM_LAT_DIVISIONS; ++i)
	{
		mLats[i]->DrawText();
		//((GtkLatLongLine *)mLats[i])->Dump();
	}

	for (int j=0; j < LATLONGGRID_NUM_LAT_DIVISIONS; ++j)
	{
		mLongs[j]->DrawText();
	}
}

void LatLongGrid::DrawTextPartial(int x1, int x2, int y1, int y2)
{
	SetUpGrid();
	
	for (int i=0; i < LATLONGGRID_NUM_LAT_DIVISIONS; ++i)
		mLats[i]->DrawTextPartial(x1,x2,y1,y2);

	for (int j=0; j < LATLONGGRID_NUM_LAT_DIVISIONS; ++j)
		mLongs[j]->DrawTextPartial(x1,x2,y1,y2);
}

/****************************************************************************
*****************************************************************************
 Function: LatLongDrawer::GetDefaultIncrement
 Purpose:  To determine the default increment given the calculated increment.
 Params:   calculated_increment- this is the value passed from DrawLatLongGrid
	   which represents the acual increments derived from SPREAD/#SECTIONS
	   If the lat and long lines COULD fall anywhere, then calculated
	   increment would be use to place the lat/long lines on the screen in
	   perfectly equal sections.  However, since we want lat/long lines to
	   fall on "whole" positions, this function is needed.  The function
	   chooses the best increment for the job given the calculated
	   increment.
 Returns:  the suggested default increment.

 Programmer: Joey Gabbard
 Date:       Oct 18, 1994
 Altered by:
 Date:
****************************************************************************/
int LatLongGrid::GetDefaultIncrement(float calculated_increment)
{
	#define NUM_DEFAULT_INCS 30

	// The following are the possible default increments in seconds.  As of
	// now they range from an interval of 1 sec, to 50 degs.  Increments can
	// be added in between the ones given here to force the number of lines
	// used to be more consistent.  If more increments are added, be sure to
	// reflect the changes in the above #define (declares the number of
	// elements in the array)

	static int default_increment[NUM_DEFAULT_INCS] = {
		1, 2, 5, 10, 15, 20, 30, 45, 60, 90, 120, 150, 180, 240, 300,
		450, 600, 900, 1200, 1800, 2700, 3600, 5400, 7200, 10800,
		18000, 36000, 72000, 180000, 216000 };

	int i=0, midpoint;


	// Find the daufault increments that the calculated_incremnt falls 
        // between

	while( (i < NUM_DEFAULT_INCS)&&
	       (calculated_increment > default_increment[i])) {
	   i++ ;
	}

	// check for out of bounds error
	if((i==0) || (i==NUM_DEFAULT_INCS)) {
		// call error box
		return(0);
	}
	else {

		// While loop normally falls out at
		// default_increment[i-1] < calculated_increment <=
		// default_increment[i]

		// Determine midpoint from surrounding default increments

		// Use an alternate midpoint - one that is 1/4 of the way from
		//  the lower default increment.
		// Makes the number of lines more consistent across all input

		midpoint = default_increment[i-1] +
			   ((default_increment[i]-default_increment[i-1])/4);

		// See if calculated_increment falls below or above the midpoint
		// This dictates which if the default increments is used

		if( calculated_increment < midpoint)
			return(default_increment[i-1]);
		else
			return(default_increment[i]);
	}
}

void LatLongGrid::SetViewPortLocal()
{
	SetUpGrid();
}
