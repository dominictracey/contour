/*
** gtkToolbar.h - Implements GTK+ toolbar 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.15.98 DPT
*/

#ifndef _GTKTOOLBAR_H_
#define _GTKTOOLBAR_H_

#include <gtk/gtk.h>
#include "eventSubscriber.h"
class EventSourceCommand;

class BGtkToolbar : public EventSubscriber
{
public:
	BGtkToolbar(GtkWidget *window);
	virtual ~BGtkToolbar();
	virtual bool Initialize();
	virtual GtkWidget * GetControl() { return mToolbar; }
	virtual void Notify(EventSourceCommand *es);
protected:
	BGtkToolbar();  //use the other constructor please
	void AddButton(GtkWidget *toolbar, char *text, char *tooltip,
                                char **xpm, GtkSignalFunc callback,
                                gpointer data, GtkStyle *style, GtkWidget *mf);
    void AddToggleButton(GtkWidget *toolbar, char *text, char *tooltip,
				char **xpm, GtkSignalFunc callback, 
				gpointer data, GtkStyle *style, GtkWidget *mf, GtkWidget **memberVar);
	GtkWidget *mToolbar;	
	GtkWidget *mDummyWindow;
	GtkWidget *mGridButton;
	GtkWidget *mDrawButton;
	GtkWidget *mTrackButton;
	
};

#endif

