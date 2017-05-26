/*
** effort.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Mon Nov 26 18:32:32 EST 2001
*/


#include "effort.h"
#include "contourDBManager.h"
#include "contourDBEffort.h"

Effort::Effort()
{
	mSamplePoints = new GridPointVector;
}

Effort::~Effort()
{
	if (mSamplePoints)
		delete mSamplePoints;
}

bool Effort::Start()
{
	return true;
}

bool Effort::End()
{
	// put the points in the database too
	ContourDBEffort *db = ContourDBManager::GetInstance()->GetEffortPersistor();
	GridPointVector::iterator it = mSamplePoints->begin();

	if (!it)   //empty -- no points
		return true;
		
	GridPoint *gp;
	//keep track of the boundaries of the effort as we go
	float lat, lon, maxlat=-324000, maxlon=-648000, minlat=324000, minlon=648000;
	float startlat = (*it)->GetLatitude().TotalSeconds();
	float startlon = (*it)->GetLongitude().TotalSeconds();
	it++;
	while (it != mSamplePoints->end())
	{
		gp = *it;
		lat = gp->GetLatitude().TotalSeconds();
		lon = gp->GetLongitude().TotalSeconds();
		if (lat > maxlat)
			maxlat = lat;
		else if (lat < minlat)
			minlat = lat;

		if (lon > maxlon)
			maxlon = lon;
		else if (lon < minlon)
			minlon = lon;
			
		db->AddDataPoint(mID, lat, lon, gp->GetDepth());		
		++it;
	}	

	--it;
	float endlat = (*it)->GetLatitude().TotalSeconds();
	float endlon = (*it)->GetLongitude().TotalSeconds();
	
	db->SetBoundaries(mID,startlat, startlon,
					endlat, endlon,
					maxlat, maxlon,
					minlat, minlon);
	
	return true;
}

bool Effort::AddSamplePoint(GridPoint *sp)
{
//	cout << "Adding point w x" << sp->x() << endl;
	mSamplePoints->push_back(sp);
}

void Effort::RemoveDecorations()
{

}
int Effort::Save()
{
	return true;
}
bool Effort::Load(int effortid)
{
	//@REX GridPoints mem leak -- going to New Year's party, fix later
	if (mSamplePoints)
		delete mSamplePoints;

	mID = effortid;
	// get our samplepoints out of the database
	mSamplePoints = ContourDBManager::GetInstance()->GetEffortPersistor()->GetGridPoints(mID);
	return true;
}

/*
** Recalculate - call when the viewport changes, resets GridPoint::x|y.
**	@REX - just use the GridPoint::recalculate
*/
bool Effort::Recalculate(SamplePoint *ul, SamplePoint *size, int width, int height)
{
	GridPointVector::iterator it = mSamplePoints->begin();

	if (!it)   //empty -- no points
		return true;
		
	GridPoint *gp;
	it++;
	while (it != mSamplePoints->end())
	{
		gp = *it;
		gp->x(gp->GetX(ul,size,width));
		gp->y(gp->GetY(ul,size,height));
		++it;
	}	
	return true;
}
