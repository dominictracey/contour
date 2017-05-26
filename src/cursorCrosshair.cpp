/*
** cursorCrosshair.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Fri Nov 23 17:18:00 EST 2001
*/

#include "cursorCrosshair.h"
#include "gdkCanvas.h"
#include "bathApp.h"
#include "appFactory.h"

CursorCrosshair::CursorCrosshair()
{
	Reference(this); //the static callback tedium
	mClear = true;
}

CursorCrosshair::~CursorCrosshair()
{
	 //dealing with the static callback tedium
	Dereference();
}


void CursorCrosshair::Draw()
{
	ASSERT(mClear)
	GDKCanvas *canvas = ((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas());

	gdk_draw_line(canvas->GetDrawingSurfacePixmap(),mGC,0,mY,mWidth,mY);
	gdk_draw_line(canvas->GetDrawingSurfacePixmap(),mGC,mX,0,mX,mHeight);
	
	mClear = false;

}

void CursorCrosshair::Clear()
{
	ASSERT(!mClear)
	GDKCanvas *canvas = ((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas());

	gdk_draw_line(canvas->GetDrawingSurfacePixmap(),mGC,0,mY,mWidth,mY);
	gdk_draw_line(canvas->GetDrawingSurfacePixmap(),mGC,mX,0,mX,mHeight);
	
	mClear = true;


}

void CursorCrosshair::DrawPartial(int x1, int x2, int y1, int y2)
{

}

void CursorCrosshair::DrawText()
{

}

void CursorCrosshair::DrawTextPartial(int x1, int x2, int y1, int y2)
{

}

gint CursorCrosshair::ButtonPressLocal(GtkWidget *widget, GdkEventButton *event)
{
	return true;
}

gint CursorCrosshair::ButtonReleaseLocal(GtkWidget *widget, GdkEventButton *event)
{
	return true;
}

gint CursorCrosshair::MouseMotionLocal(GtkWidget *widget, GdkEventMotion *event)
{
	return true;
}
