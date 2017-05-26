/*
** commandAutoCenter.h - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Jul  1 11:24:27 EDT 2001
*/
#ifndef	_COMMANDAUTOCENTER_H_
#define _COMMANDAUTOCENTER_H_

#include "commandEventSource.h"

class AutoCenterCommand : public EventSourceCommand
{
public:
	AutoCenterCommand();
	~AutoCenterCommand();

	virtual void Execute();
	static AutoCenterCommand * GetInstance();
protected:
	static AutoCenterCommand *mInstance;
};

#endif

