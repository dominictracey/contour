/*
** gtkMenu.cpp
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** DPT 12.15.98
*/

#ifndef _GTKMENU_H_
#define _GTKMENU_H_

#include <gtk/gtk.h>
#include <strings.h>
#include "eventSubscriber.h"
class EventSourceCommand;

class BGtkMenu : public EventSubscriber
{
public:
	BGtkMenu();
	~BGtkMenu();
	bool Initialize();
	GtkWidget * GetControl() { return mMenubar; }
	void AddItem(GtkWidget *menu, char *label, 
		     GtkSignalFunc handler, char *tooltip = "");
	GtkItemFactory * GetItemFactory() {return mItemFactory;}
	virtual void Notify(EventSourceCommand *esc);
protected:
	GtkWidget *mMenubar;
	GtkItemFactory *mItemFactory;

};

#endif
