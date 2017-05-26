/*
** commandPan.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jan 27 13:23:58 EST 2001
*/

#include "commandPan.h"
#include "exception.h"

PanCommand::PanCommand()
{
	mDirection = (Panner::Direction)-1;
}

PanCommand::~PanCommand()
{

}

void PanCommand::Execute()
{
	if (mDirection == -1)
		throw new ContourException("You must call SetDirection() before executing this command");

	Panner panner(mDirection);
	panner.BeginPan();
	panner.Pan();
	panner.EndPan();
}

/*
** SetDirection - We have this return the this pointer so it can be used in the
**	gtkMenuFactory array thingy (see gtkMenu.cpp).
*/
PanCommand * PanCommand::SetDirection(Panner::Direction direction)
{
	mDirection = direction;
	return this;
}

