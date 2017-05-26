/*
** command.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jan 27 13:11:52 EST 2001
*/

#include "command.h"
#include "commandPan.h"
#include "commandZoom.h"
#include "commandApplication.h"
#include "commandDraw.h"
#include "commandGrid.h"
#include "commandAutoCenter.h"
#include "commandCursor.h"
#include "commandTrack.h"
#include "exception.h"

Command::Command()
{

}

Command::~Command()
{

}

Command * Command::Create(CommandClass subclass)
{
	if (subclass == PAN)
		return new PanCommand;
	else if (subclass == ZOOM)
		return new ZoomCommand;
	else if (subclass == APPLICATION)
		return new ApplicationCommand;
	else if (subclass == DRAW)
		return DrawCommand::GetInstance();  //event synching commands must be singletons
	else if (subclass == GRID)
		return GridCommand::GetInstance();  //event synching commands must be singletons
	else if (subclass == AUTOCENTER)
		return AutoCenterCommand::GetInstance();  //event synching commands must be singletons
	else if (subclass == TRACK)
		return TrackCommand::GetInstance();  //event synching commands must be singletons
	else if (subclass == CURSOR)
		return new CursorCommand();
	else
		throw new ContourException("Unknown Command Subclass");
}


