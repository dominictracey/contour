/*
** commandDraw.h - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Jan 28 01:39:34 EST 2001
*/

#ifndef	_COMMANDDRAW_H_
#define _COMMANDDRAW_H_

#include "commandEventSource.h"

class DrawCommand : public EventSourceCommand
{
public:
	DrawCommand();
	~DrawCommand();

	virtual void Execute();
	static DrawCommand * GetInstance();
protected:
	static DrawCommand *mInstance;
};

#endif

