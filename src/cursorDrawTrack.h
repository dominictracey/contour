/*
** cursorDrawTrack.h - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Nov 25 15:47:38 EST 2001
*/

#ifndef _CURSORDRAWTRACK_H_
#define _CURSORDRAWTRACK_H_

#include "cursor.h"
#include <gtk/gtk.h>

class GDKCanvas;
class EffortDecoration;
class Effort;
class ContourPreferences;

class CursorDrawTrack : public ContourCursor
{
public:
	CursorDrawTrack(); 
	~CursorDrawTrack();

	// Decoration methods
	virtual void Draw();
	virtual void Clear();
	virtual void DrawPartial(int x1, int x2, int y1, int y2);
	virtual void DrawText();
	virtual void DrawTextPartial(int x1, int x2, int y1, int y2);


protected:
	// ContourCursor interface
	virtual gint ButtonPressLocal(GtkWidget *widget, GdkEventButton *event);
	virtual gint ButtonReleaseLocal(GtkWidget *widget, GdkEventButton *event);
	virtual gint MouseMotionLocal(GtkWidget *widget, GdkEventMotion *event);
	
	bool mClear;
	GDKCanvas *mCanvas;
	Effort *mEffort;
	bool mDrawing;
	ContourPreferences * mPrefs;
};

#endif

