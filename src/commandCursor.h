/*
** commandCursor.h - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Nov 24 10:32:19 EST 2001
*/

#ifndef _COMMANDCURSOR_H_
#define _COMMANDCURSOR_H_

#include "command.h"

class CursorCommand : public Command
{
public:
	CursorCommand();
	~CursorCommand();

	void Execute();

	enum Cursor_Type {NONE,CROSSHAIR,POLAR,DRAWTRACK};
	CursorCommand *SetType (CursorCommand::Cursor_Type t);
protected:
	Cursor_Type mType;
};

#endif

