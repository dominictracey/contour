/*
** commandCursor.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Nov 24 10:39:03 EST 2001
*/

#include "commandCursor.h"
#include "cursor.h"
#include "cursorCrosshair.h"
#include "bathApp.h"
#include "appFactory.h"
#include "canvas.h"
#include "cursor.h"

CursorCommand::CursorCommand()
{
	mType = NONE;
}

CursorCommand::~CursorCommand()
{

}

void CursorCommand::Execute()
{
	BathApp *app = AppFactory::GetInstance()->GetApp();
	if (mType == NONE)
	{
		// add in a NULL placeholder cursor to ignore events		
		app->GetCanvas()->AddDecoration(ContourCanvas::NULL_CURSOR);
		ContourCursor::SetCursorIcon(GDK_ARROW);
	} else if (mType == CROSSHAIR)
	{
		app->GetCanvas()->AddDecoration(ContourCanvas::CROSSHAIR_CURSOR);
		ContourCursor::SetCursorIcon(GDK_DOTBOX);
	} else if (mType == POLAR)
	{

	} else if (mType == DRAWTRACK)
	{
		app->GetCanvas()->AddDecoration(ContourCanvas::DRAWTRACK_CURSOR);
		ContourCursor::SetCursorIcon(GDK_PENCIL);
	}

}

CursorCommand * CursorCommand::SetType (CursorCommand::Cursor_Type t)
{
	mType = t;
	return this;
}
