/*
** commandGrid.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Jan 28 01:43:37 EST 2001
*/

#include "commandGrid.h"
#include "appFactory.h"
#include "preferences.h"
#include "engine.h"
#include "latLongGrid.h"
#include "canvas.h"

GridCommand * GridCommand::mInstance = NULL;

GridCommand::GridCommand()
{

}

GridCommand::~GridCommand()
{

}

void GridCommand::Execute()
{
	//block re-entrancy
	if (!mEventSource.IsBroadcasting())
	{
		//set the preferences and regrid the screen
		BathApp *app = AppFactory::GetInstance()->GetApp();
		bool active = app->GetPreferences()->GetShowGrid();
		app->GetCanvas()->BeginDrawing();
		//flop state -- better code? ~active doesn't work...
		if (active == true) 
		{
			active = false; 
			app->GetCanvas()->RemoveDecoration(ContourCanvas::LATLONG_GRID);
			//LatLongGrid::GetInstance()->Clear();
		} else 
		{
			active = true;
			app->GetCanvas()->AddDecoration(ContourCanvas::LATLONG_GRID);
		}
		app->GetCanvas()->DoneDrawing();
		app->GetPreferences()->SetShowGrid(active);
		//app->GetEngine()->Render();

		//tell other interested parties about this
		mEventSource.Broadcast(this);
	}
}

GridCommand * GridCommand::GetInstance()
{
	if (!mInstance)
		mInstance = new GridCommand;
	return mInstance;
}
