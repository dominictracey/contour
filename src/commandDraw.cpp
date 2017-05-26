/*
** commandDraw.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Jan 28 01:43:37 EST 2001
*/

#include "commandDraw.h"
#include "appFactory.h"
#include "preferences.h"
#include "engine.h"

DrawCommand * DrawCommand::mInstance = NULL;

DrawCommand::DrawCommand()
{
}

DrawCommand::~DrawCommand()
{

}

void DrawCommand::Execute()
{
	//block re-entrancy
	if (!mEventSource.IsBroadcasting())
	{
		//set the preferences and redraw the screen
		BathApp *app = AppFactory::GetInstance()->GetApp();
		bool active = app->GetPreferences()->GetDraw();
		//flop state -- better code? ~active doesn't work...
		if (active == true) active = false; else active = true;
		app->GetPreferences()->SetDraw(active);
		app->GetEngine()->Erase();

		//tell other interested parties about this
		mEventSource.Broadcast(this);
	}
}

DrawCommand * DrawCommand::GetInstance()
{
	if (!mInstance)
		mInstance = new DrawCommand;
	return mInstance;
}
