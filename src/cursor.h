/*
** cursor.h - A pure virtual base class for cursors. Defines the interface for
**	things like mouse clicks and movement.
**
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Fri Nov 23 10:06:23 EST 2001
*/

#ifndef _CURSOR_H_
#define _CURSOR_H_

#include "contourGraphic.h"
#include "gtk/gtk.h"
#include "position.h"

class ContourCursor : public ContourGraphic
{
public:
	ContourCursor();
	~ContourCursor();
	static ContourCursor * GetInstance();
	static void Dereference(); // call when you are done with the current cursor
	static gint ButtonPress(GtkWidget *widget, GdkEventButton *event);
	static gint ButtonRelease(GtkWidget *widget, GdkEventButton *event);
	static gint MouseMotion(GtkWidget *widget, GdkEventMotion *event);
	static void Initialize();

	//these control the actual icon being displayed
	static void SetCursorIcon(GdkCursorType cursor_type);
	static void WaitCursor();
	static void RemoveWaitCursor();
	
protected:
	// give the subclasses a chance to handle mouse events
	virtual gint ButtonPressLocal(GtkWidget *widget, GdkEventButton *event) = 0;
	virtual gint ButtonReleaseLocal(GtkWidget *widget, GdkEventButton *event) = 0;
	virtual gint MouseMotionLocal(GtkWidget *widget, GdkEventMotion *event) = 0;

	//initialization
	static void CreateGC();
	void SetViewPortLocal();
	
	// call when you create a cursor (from the subclass constructor
	//	This is to handle this whole sticky thing leading from:
	//		1) GTK event handlers must be static
	//		2) We want to work with the current cursor in the event handler.
	//		3) Maybe we even want multiple or composite cursors
	static ContourCursor * Reference(ContourCursor *cursor);
	
	static int mX;
	static int mY;
	static bool mShowText;
	static bool mShowCursor;
	static GdkGC *mGC;
	static GdkFont *mFont;

	static bool mRegisteredForEvents;
	
	// I guess we'll make this a singleton class for the time-being. The major
	// implication is that we only get one cursor (radio buttons on the toolbar?).
	static ContourCursor * mInstance;

	// cursor icons
	static GdkCursor *currentCursor;
	static GdkCursor *lastCursor;
};

#endif

