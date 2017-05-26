/*
** commandZoom.h - Command subclass that handles Zooming
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jan 27 19:17:28 EST 2001
*/

#ifndef _COMMANDZOOM_H_
#define _COMMANDZOOM_H_

#include "command.h"

class ZoomCommand : public Command
{
public:
	ZoomCommand();
	~ZoomCommand();

	void Execute();
	ZoomCommand * SetStepSize(float stepSize);
protected:
	float mStepSize;
};

#endif

