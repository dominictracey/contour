/*
** commandApplication.h - Command subclass that handles Application related actions
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jan 27 13:17:28 EST 2001
*/

#ifndef _COMMANDAPPLICATION_H_
#define _COMMANDAPPLICATION_H_

#include "command.h"

class ApplicationCommand : public Command
{
public:
	ApplicationCommand();
	~ApplicationCommand();

	void Execute();
	enum Action { EXIT,PREFERENCES, END_TOW, ADD_NET, SHOW_NETS, ADD_CREW, SHOW_CREW, ABOUT};
	ApplicationCommand * SetAction(ApplicationCommand::Action action);
protected:
	Action mAction;
};

#endif

