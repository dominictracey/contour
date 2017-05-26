/*
** commandAutoCenter.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Jul  1 11:25:07 EDT 2001
*/
#include "commandAutoCenter.h"
#include "appFactory.h"
#include "preferences.h"
#include "engine.h"

AutoCenterCommand * AutoCenterCommand::mInstance = NULL;

AutoCenterCommand::AutoCenterCommand()
{
}

AutoCenterCommand::~AutoCenterCommand()
{

}

void AutoCenterCommand::Execute()
{
	//block re-entrancy
	if (!mEventSource.IsBroadcasting())
	{
		cout << "AutoCenterCommand::Execute" << endl;
		//set the preferences and redraw the screen
		BathApp *app = AppFactory::GetInstance()->GetApp();
		bool active = app->GetPreferences()->GetAutoCenter();
		//flop state -- better code? ~active doesn't work...
		if (active == true) active = false; else active = true;
		app->GetPreferences()->SetAutoCenter(active);
		app->GetEngine()->Erase();

		//tell other interested parties about this
		mEventSource.Broadcast(this);
	}
}

AutoCenterCommand * AutoCenterCommand::GetInstance()
{
	if (!mInstance)
		mInstance = new AutoCenterCommand;
	return mInstance;
}
