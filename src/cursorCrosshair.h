/*
** cursorCrosshair.h -  Show mouse crosshairs that appear when the mouse is over the 
**	canvas. The lat, long and depth under the crosshairs may be displayed in the upper
**	right of the screen. Depending on a boolean preference defined in the rc file, \
**	CURSOR_DISPLAY, the display is shown either all the time or only when the left mouse 
**	button is held down.
**
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Fri Nov 23 10:58:51 EST 2001
*/

#ifndef _CURSORCROSSHAIR_H_
#define _CURSORCROSSHAIR_H_

#include "cursor.h"
#include <gtk/gtk.h>

//#include "contourGraphic.h"

class CursorCrosshair : public ContourCursor
{
public:
	CursorCrosshair(); 
	~CursorCrosshair();

	// Decoration methods
	virtual void Draw();
	virtual void Clear();
	virtual void DrawPartial(int x1, int x2, int y1, int y2);
	virtual void DrawText();
	virtual void DrawTextPartial(int x1, int x2, int y1, int y2);

protected:
	virtual gint ButtonPressLocal(GtkWidget *widget, GdkEventButton *event);
	virtual gint ButtonReleaseLocal(GtkWidget *widget, GdkEventButton *event);
	virtual gint MouseMotionLocal(GtkWidget *widget, GdkEventMotion *event);
	bool mClear;

};

#endif

