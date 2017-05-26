/*
** commandZoom.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jan 27 19:23:58 EST 2001
*/

#include "commandZoom.h"
#include "appFactory.h"
#include "engine.h"
#include "exception.h"

ZoomCommand::ZoomCommand()
{
	mStepSize = -1;
}

ZoomCommand::~ZoomCommand()
{

}

void ZoomCommand::Execute()
{
	if (mStepSize == -1)
		throw new ContourException("You must call SetStepSize() before executing this command");

	AppFactory::GetInstance()->GetApp()->GetEngine()->Zoom(mStepSize);
}

/*
** SetStepSize - We have this return the this pointer so it can be used in the
**	gtkMenuFactory array thingy (see gtkMenu.cpp).
*/
ZoomCommand * ZoomCommand::SetStepSize(float stepSize)
{
	mStepSize = stepSize;
	return this;
}

