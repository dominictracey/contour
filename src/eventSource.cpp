/*
** eventSource.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Jan 28 01:14:45 EST 2001
*/

#include "eventSource.h"
#include "exception.h"
#include "commandEventSource.h"

#define NUM_SUBSCRIBERS 10

EventSource::EventSource()
{
	mList = new EventSubscriberList;
	mBroadcasting = false;
}

EventSource::~EventSource()
{
	delete mList;
}

bool EventSource::Register(EventSubscriber *es)
{
	mList->push_back(es);
	return true;
}

void EventSource::Broadcast(EventSourceCommand *esc)
{
	mBroadcasting = true;
	EventSubscriberList::iterator iter;
	try
	{
		for (iter = mList->begin(); iter != mList->end(); ++iter)
			(*iter)->Notify(esc);
	} catch (ContourException *e)
	{
		cout << "ERROR IN BROADCAST EVENTS: " << e->GetString() << endl;
	}
	mBroadcasting = false;
}

