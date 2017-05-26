/*
** cursorNull.h -  When there is no cursor, there is this.
**
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Fri Nov 23 10:58:51 EST 2001
*/

#ifndef _CURSORNULL_H_
#define _CURSORNULL_H_

#include "cursor.h"
#include <gtk/gtk.h>

//#include "contourGraphic.h"

class CursorNull : public ContourCursor
{
public:
	CursorNull(); 
	~CursorNull();

	// Cursor methods
	//virtual void ButtonPressLocal();
	//virtual void MouseMotionLocal();

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

