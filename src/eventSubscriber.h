/*
** eventSubscriber.h - Mixin interface for GUI element implementors who wish to stay
**	synched up with other Command invocations. See eventSource.h and the Command
**	inheritance structure.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Jan 28 01:11:15 EST 2001
*/

#ifndef _EVENTSUBSCRIBER_H_
#define _EVENTSUBSCRIBER_H_

class EventSourceCommand;

class EventSubscriber
{
public:
	~EventSubscriber() {}
	virtual void Notify(EventSourceCommand *esc)=0;
};

#endif

