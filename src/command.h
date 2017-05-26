/*
** command.h - Interface for toolbar, menu and other GUI elements.
**
**	Command design pattern allows us to encapsulate menu selections, toolbar 
**	button pushes, navtool actions, etc. into objects that can be tied to these
**	GUI elements at startup and processed polymorphically through the Execute()
**	over-rides in the subclasses.
**
**	For instance, in the GTK toolkit, we define a single generic GTK_SIGNAL_FUNC
**	for all GTK GUI elements and use the data * parameter of the callbacks to hold 
**	a subclass of Command which the generic handler (GtkCommandHandler::Handle) simply
**	calls Execute() on. Note that the modules that set up the gui elements will need
**	to know how to configure the Command subclasses.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jan 27 13:06:10 EST 2001
*/

#ifndef _COMMAND_H_
#define _COMMAND_H_

class Command
{
public:
	Command();
	~Command();
	virtual void Execute() = 0;
	enum CommandClass {PAN, ZOOM, APPLICATION, DRAW, GRID, AUTOCENTER, CURSOR, TRACK};
	static Command * Create(CommandClass subclass);
};

#endif

