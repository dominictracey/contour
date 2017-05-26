/*
** gtkCommandHandler.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jan 27 20:05:16 EST 2001
*/

#include "gtkCommandHandler.h"


void GtkCommandHandler::Handle(GtkWidget *widget, gpointer data)
{
	((Command *)data)->Execute();
}

