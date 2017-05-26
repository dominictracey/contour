/*
** cursorNull.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Fri Nov 23 17:18:00 EST 2001
*/

#include "cursorNull.h"
#include "gdkCanvas.h"
#include "bathApp.h"
#include "appFactory.h"

CursorNull::CursorNull()
{
	Reference(this); //the static callback tedium
	mClear = true;
}

CursorNull::~CursorNull()
{
	 //dealing with the static callback tedium
	Dereference();
}


void CursorNull::Draw()
{
	mClear = false;

}

void CursorNull::Clear()
{
	mClear = true;
}

void CursorNull::DrawPartial(int x1, int x2, int y1, int y2)
{

}

void CursorNull::DrawText()
{

}

void CursorNull::DrawTextPartial(int x1, int x2, int y1, int y2)
{

}

gint CursorNull::ButtonPressLocal(GtkWidget *widget, GdkEventButton *event)
{
	return true;
}

gint CursorNull::ButtonReleaseLocal(GtkWidget *widget, GdkEventButton *event)
{
	return true;
}

gint CursorNull::MouseMotionLocal(GtkWidget *widget, GdkEventMotion *event)
{
	return true;
}
