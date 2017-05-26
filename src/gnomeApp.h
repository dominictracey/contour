/*
** gnomeApp.h - Interface for GNOME/GTK+ app object
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** DPT 12.12.98
*/

#ifndef _GNOMEAPP_H_
#define _GNOMEAPP_H_

#include "gtkApp.h"

class BGnomeApp : public GtkApp
{
public:
	BGnomeApp();
	~BGnomeApp();

	virtual BOOL Initialize(int argc, char **argv);
	virtual BOOL Run();

private:
	GtkWidget *mApp;
	static void hello_cb (GtkWidget *widget, void *data);
	static void quit_cb (GtkWidget *widget, void *data);	
};

#endif
