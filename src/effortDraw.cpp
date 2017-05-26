/*
** effortDraw.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Mon Nov 26 18:24:02 EST 2001
*/

#include "effortDraw.h"
#include "effortDecoration.h"
#include "effortDecorationCollection.h"
#include "contourDBManager.h"
#include "contourDBEffort.h"
#include "preferences.h"
#include "canvas.h"
#include "appFactory.h"
#include "gecko.h"
#include <stdio.h>

DrawEffort::DrawEffort()
{
	mED = NULL;
}

DrawEffort::~DrawEffort()
{
	if (mED)
		delete mED;
}

bool DrawEffort::AddDecoration()
{
	if (!mED)
		mED = new EffortDecoration(this);
		
	EffortDecorationCollection::GetEffortDecorationCollectionByType(DRAW)->AddEffort(mED);
	return true;
}

bool DrawEffort::Start()
{
	Effort::Start();
	// create a new entry in the database
	mID = ContourDBManager::GetInstance()->GetEffortPersistor()->StartDraw();
	return true;
}

bool DrawEffort::End()
{
	Effort::End();
	// put an end to it in the database
	ContourDBManager::GetInstance()->GetEffortPersistor()->EndDraw(mID); 

	// pop up the data collection dialog
	Gecko *gecko = Gecko::GetInstance();
	char URL[256];
	sprintf(URL,"%s%s%d",CONTOUR_DOMAIN_AND_PORT,"/effort/tow.php?action=add&id=",mID);
	gecko->Show(URL);

	return true;
}

bool DrawEffort::Load(int effortid)
{
	Effort::Load(effortid);
	AddDecoration();
}
