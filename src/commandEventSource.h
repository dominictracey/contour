/*
** commandEventSource.h - Interface for classes that must generate event notifications
**	for EventSubscribers who wish to receive notifications of Commands being executed.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Mon Jan 29 22:49:39 EST 2001
*/

#ifndef _COMMANDEVENTSOURCE_H_
#define _COMMANDEVENTSOURCE_H_

#include "command.h"
#include "eventSource.h"

class EventSourceCommand : public Command
{
public:
	~EventSourceCommand() {}
	EventSource & GetEventSource() {return mEventSource;}
	void Broadcast() {mEventSource.Broadcast(this);}
protected:
	EventSource mEventSource;
};

#endif

