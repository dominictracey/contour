/*
** cursor.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Fri Nov 23 18:41:13 EST 2001
*/

#include "cursor.h"
#include "stdio.h"
#include "engine.h"
#include "appFactory.h"
#include "bathApp.h"
#include "gdkCanvas.h"

int ContourCursor::mX = 0;
int ContourCursor::mY = 0;
bool ContourCursor::mShowText = false;
bool ContourCursor::mShowCursor = true;
GdkGC *ContourCursor::mGC = NULL;
GdkFont *ContourCursor::mFont = NULL;
ContourCursor * ContourCursor::mInstance = NULL;
bool ContourCursor::mRegisteredForEvents = false;
GdkCursor * ContourCursor::currentCursor = NULL;
GdkCursor * ContourCursor::lastCursor = NULL;

ContourCursor::ContourCursor()
{
	//only do this once
	if (!mRegisteredForEvents)
	{
		//register for notifications from GTK of mouse events
		GtkWidget * canvas = ((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetCanvasControl();
		
		gtk_signal_connect (GTK_OBJECT (canvas), "motion_notify_event",
			      (GtkSignalFunc) MouseMotion, NULL);
		gtk_signal_connect (GTK_OBJECT (canvas), "button_press_event",
			      (GtkSignalFunc) ButtonPress, NULL);
		gtk_signal_connect (GTK_OBJECT (canvas), "button_release_event",
			      (GtkSignalFunc) ButtonRelease, NULL);

		gtk_widget_set_events (canvas, GDK_EXPOSURE_MASK
				 | GDK_LEAVE_NOTIFY_MASK
				 | GDK_BUTTON_PRESS_MASK
				 | GDK_BUTTON_RELEASE_MASK
				 | GDK_POINTER_MOTION_MASK
				 | GDK_POINTER_MOTION_HINT_MASK);

		mRegisteredForEvents = true;
	}
	
}

ContourCursor::~ContourCursor()
{

}

ContourCursor * ContourCursor::GetInstance() 
{
	return mInstance;
}

ContourCursor * ContourCursor::Reference(ContourCursor * c)
{
	ASSERT (!mInstance)
	mInstance = c;
	return c;
}

void  ContourCursor::Dereference()
{
	mInstance = NULL;
}

// ContourCursor methods
gint ContourCursor::ButtonPress(GtkWidget *widget, GdkEventButton *event)
{
	ASSERT(mInstance)
    mInstance->ButtonPressLocal(widget,event);
	return true;
}

gint ContourCursor::ButtonRelease(GtkWidget *widget, GdkEventButton *event)
{
	ASSERT(mInstance)
    mInstance->ButtonReleaseLocal(widget,event);
	return true;
}

gint ContourCursor::MouseMotion(GtkWidget *widget, GdkEventMotion *event)
{
	if (!mInstance)
		return 0;
		
	ASSERT(mInstance)
	GDKCanvas *canvas = ((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas());
	canvas->BeginDrawing();
  	GdkModifierType state;

  	if (event->is_hint) 
  	{
   		gdk_window_get_pointer (event->window, &mX, &mY, &state);
   	}
	else
    {
		mX = (int)event->x;
		mY = (int)event->y;
		state = (GdkModifierType)event->state;
    }

    mInstance->MouseMotionLocal(widget,event);
//	if (mShowCursor & state & GDK_BUTTON1_MASK & (GetInstance() != NULL))
	canvas->DoneDrawing();
  
	return true;

}

void ContourCursor::Initialize()
{
	SetViewPort();
	CreateGC();
}

void ContourCursor::SetViewPortLocal() 
{

}

// @REX -- use the canvas'
void ContourCursor::CreateGC()
{
	ASSERT (false)
	if (!mGC)
	{
		mGC = gdk_gc_new(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetCanvasControl()->window);
		gdk_gc_set_function(mGC, GDK_XOR);
		GdkColor c;
		gdk_color_white(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetColorMap(),&c);
		gdk_gc_set_function(mGC, GDK_XOR);
		gdk_gc_set_foreground(mGC,&c);
		mFont = gdk_font_load("-adobe-courier-medium-r-normal-*-*-140-*-*-m-*-iso8859-1");
		gdk_gc_set_font(mGC,mFont);
	}

}

void ContourCursor::SetCursorIcon(GdkCursorType cursor_type)
{
	if (lastCursor)
		gdk_cursor_destroy (lastCursor);

	//save off the current
	lastCursor = currentCursor;

	currentCursor = gdk_cursor_new(cursor_type);

	gdk_window_set_cursor(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetCanvasControl()->window, currentCursor);
}

void ContourCursor::WaitCursor()
{
	SetCursorIcon(GDK_CLOCK);
}

void ContourCursor::RemoveWaitCursor()
{
	if (currentCursor)
		gdk_cursor_destroy (currentCursor);
		
	//restore the old
	currentCursor = lastCursor;

	gdk_window_set_cursor(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetCanvasControl()->window, currentCursor);
}

