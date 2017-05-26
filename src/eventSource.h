/*
** eventSource.h - A mixin interface for commands that are tied to GUI elements that
**	need to be kept in synch with other GUI elements. For instance, the Draw menu item
**	has a check next to it that is on when the corresponding toolbar is pressed in.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jan 27 22:30:06 EST 2001
*/

#ifndef _EVENTSOURCE_H_
#define _EVENTSOURCE_H_

#include "eventSubscriber.h"
class EventSourceCommand;
#include <list>

class EventSource
{
public:
	EventSource();
	~EventSource();
	bool Register(EventSubscriber *es);
	void Broadcast(EventSourceCommand *esc);
	bool IsBroadcasting() {return mBroadcasting;}
protected:
	typedef list<EventSubscriber *> EventSubscriberList;
	bool mBroadcasting;
	EventSubscriberList *mList;
};


#endif
