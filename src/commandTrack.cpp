/*
** commandTrack.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Mar 10 18:04:11 EST 2002
*/
#include "commandTrack.h"
#include "appFactory.h"
#include "preferences.h"
#include "engine.h"
#include "canvas.h"

TrackCommand * TrackCommand::mInstance = NULL;

TrackCommand::TrackCommand()
{
}

TrackCommand::~TrackCommand()
{

}

void TrackCommand::Execute()
{
	//block re-entrancy
	if (!mEventSource.IsBroadcasting())
	{
		cout << "TrackCommand::Execute" << endl;
		//set the preferences and redraw the screen
		BathApp *app = AppFactory::GetInstance()->GetApp();
		bool active = app->GetPreferences()->GetTrack();
		//flop state -- better code? ~active doesn't work...
		if (active == true)
		{
			active = false; 
			app->GetCanvas()->RemoveDecoration(ContourCanvas::DRAW_EFFORTS);
		}
		else
		{
			active = true;
			app->GetCanvas()->AddDecoration(ContourCanvas::DRAW_EFFORTS);
		}
		
		app->GetPreferences()->SetTrack(active);
		app->GetEngine()->Erase();

		//tell other interested parties about this
		mEventSource.Broadcast(this);
	}
}

TrackCommand * TrackCommand::GetInstance()
{
	if (!mInstance)
		mInstance = new TrackCommand;
	return mInstance;
}
