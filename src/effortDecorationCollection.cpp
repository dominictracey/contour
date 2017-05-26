/*
** effortDecorationCollection.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Mon Dec 17 23:10:36 EST 2001 (actually created in Nov. over Atlantic)xxxxxxx
*/

#include "effortDecorationCollection.h"
#include "effortDecoration.h"
#include <vector>
#include <gtk/gtk.h>
#include "bathApp.h"
#include "preferences.h"
#include "appFactory.h"
#include "contourDBManager.h"

//type-specific static instances
EffortDecorationCollection * EffortDecorationCollection::mDrawInstance = NULL;
EffortDecorationCollection * EffortDecorationCollection::mTowInstance = NULL;
EffortDecorationCollection * EffortDecorationCollection::mSteamInstance = NULL;
EffortDecorationCollection * EffortDecorationCollection::mLayInstance = NULL;


EffortDecorationCollection::EffortDecorationCollection()
{
	mType = Effort::UNDEFINED;
	mDecorations = new EffortDecorationVector;
}

EffortDecorationCollection::~EffortDecorationCollection()
{
	delete mDecorations;
}


void EffortDecorationCollection::Draw()
{
	EffortDecorationVector::iterator it = mDecorations->begin();

	while (it != mDecorations->end())
	{
		(*it)->Draw();
		it++;
	}
}

void EffortDecorationCollection::Clear()
{
	EffortDecorationVector::iterator it = mDecorations->begin();

	while (it != mDecorations->end())
	{
		(*it)->Clear();
		it++;
	}
}

void EffortDecorationCollection::DrawPartial(int x1, int x2, int y1, int y2)
{

}

void EffortDecorationCollection::DrawText()
{

}

void EffortDecorationCollection::DrawTextPartial(int x1, int x2, int y1, int y2)
{

}

void EffortDecorationCollection::AddEffort(EffortDecoration *ed)
{
	mDecorations->push_back(ed);
}

EffortDecorationCollection * EffortDecorationCollection::GetEffortDecorationCollectionByType(Effort::Type t)
{
	if (t == Effort::DRAW)
	{
		if (!mDrawInstance)
				mDrawInstance = new EffortDecorationCollection;
		return mDrawInstance;
	} else if (t == Effort::TOW)
	{
		if (!mTowInstance)
				mTowInstance = new EffortDecorationCollection;
		return mTowInstance;
	} else if (t == Effort::STEAM)
	{
		if (!mSteamInstance)
				mSteamInstance = new EffortDecorationCollection;
		return mSteamInstance;
	}  else if (t == Effort::LAY)
	{
		if (!mLayInstance)
				mLayInstance = new EffortDecorationCollection;
		return mLayInstance;
	}

	// don't know what you're talking about
	return NULL;
}

void EffortDecorationCollection::SetViewPortLocal()
{
	//blow them all away and recreate them
	EffortDecoration *ed;
	
	while (!mDecorations->empty())
	{
		ed = mDecorations->back();
		mDecorations->pop_back();
		delete ed;
	}

	ASSERT (mDecorations->empty())
	
	// now get the dimensions of the current canvas and see if efforts exist in the DB for this area
	ContourPreferences *prefs = AppFactory::GetInstance()->GetApp()->GetPreferences();
	GridPoint * ul = new GridPoint(prefs->GetUpperLeft());
	GridPoint * lr = new GridPoint(*ul);
	ContourCanvas *canvas = AppFactory::GetInstance()->GetApp()->GetCanvas();
	lr->Scale(canvas->GetWidth(),canvas->GetHeight(),prefs->GetXScale(),prefs->GetYScale());
	ContourDBManager::GetInstance()->GetEffortPersistor()->GetDrawsInArea(
								ul->GetLatitude().TotalSeconds(),
								lr->GetLongitude().TotalSeconds(),
								lr->GetLatitude().TotalSeconds(),
								ul->GetLongitude().TotalSeconds());

}