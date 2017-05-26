/*
** commandPan.h - Command subclass that handles panning
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jan 27 13:17:28 EST 2001
*/

#ifndef _COMMANDPAN_H_
#define _COMMANDPAN_H_

#include "command.h"
#include "panner.h"

class PanCommand : public Command
{
public:
	PanCommand();
	~PanCommand();

	void Execute();
	PanCommand * SetDirection(Panner::Direction direction);
protected:
	Panner::Direction mDirection;
};

#endif

