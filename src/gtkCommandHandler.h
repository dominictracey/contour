/*
** gtkCommandHandler.h - Provides a generic callback handler for gtk GUI elements.
**	Implements a single GTK_SIGNAL_HANDLER.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jan 27 13:41:36 EST 2001
*/

#ifndef _GTKCOMMANDHANDLER_H_
#define _GTKCOMMANDHANDLER_H_

#include "command.h"
#include <gtk/gtk.h>

class GtkCommandHandler
{
public:
	~GtkCommandHandler() {}

	static void Handle(GtkWidget *widget, gpointer data);
};

#endif

