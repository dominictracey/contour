/*
** commandTrack.h - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Jul  1 11:24:27 EDT 2001
*/
#ifndef	_COMMANDTRACK_H_
#define _COMMANDTRACK_H_

#include "commandEventSource.h"

class TrackCommand : public EventSourceCommand
{
public:
	TrackCommand();
	~TrackCommand();

	virtual void Execute();
	static TrackCommand * GetInstance();
protected:
	static TrackCommand *mInstance;
};

#endif

