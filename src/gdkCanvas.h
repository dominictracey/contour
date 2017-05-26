/*
** gdkCanvas.h - Color and drawing surface implemented with GDK drawing tools. 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.18.98 DPT
*/

#ifndef _GDKCANVAS_H_
#define _GDKCANVAS_H_

#include "canvas.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>

class ContourGraphicContainer;
class LatLongGrid;
class GtkGPSDisplay;
class GtkCurrentPositionIndicator;
class CursorCrosshair;
class CursorDrawTrack;
class CursorNull;
class EffortDecorationCollection;

#define GDKCANVAS_NUM_COLORS 65536 
#define GDKCANVAS_NUM_COLORS_LESS_ONE GDKCANVAS_NUM_COLORS-1

class GDKCanvas : public ContourCanvas
{
public:
	GDKCanvas();
	virtual ~GDKCanvas();
	virtual bool Initialize();
	virtual void Draw(int x, int y, float z);
	virtual void BeginDrawing();
	virtual void DoneDrawing();
	virtual void Shift(int NSOffset, int EWOffset);

	virtual bool SpecifyColor(short R, short G, short B, int index);
	virtual bool AllocColors();
	virtual GtkWidget * GetCanvasControl() {return mDrawingSurface;}
	virtual GtkWidget * GetColorbarControl() {return mColorbar;}
	GdkPixmap * GetDrawingSurfacePixmap() {return mDrawingSurfacePixmap;}
	GdkColormap * GetColorMap() {return mColormap;}
	
	//void WriteGPSInfo();
	//void EraseGPS();
	GdkGC * GetXORGC();
	GdkFont * GetFont() {GetXORGC(); return mFont;}
	virtual void AddDecoration(decorations d);
	virtual void RemoveDecoration(decorations d);
	ContourGraphicContainer *GetDecorations() { return mDecorations; }
protected:
	bool SetHeight(int h) {mHeight = h;}
	bool SetWidth(int w) {mWidth = w;}
	static gint ColorbarExposeCB(GtkWidget *widget, 
					GdkEventExpose *event); 
	static gint ColorbarConfigureCB(GtkWidget *widget,
					GdkEventExpose *event); 
	static gint DrawingSurfaceExposeCB(GtkWidget *widget, 
					GdkEventExpose *event); 
	static gint DrawingSurfaceConfigureCB(GtkWidget *widget,
					GdkEventExpose *event); 

	//allow static handlers to access some of the non-static vars
	static GDKCanvas * GetCanvas() {return mInstance;}

	static GdkColor * mColors[GDKCANVAS_NUM_COLORS];	
	GdkColormap* mColormap;	
	GtkWidget* mVOuterbox;
	GtkWidget* mDrawingSurface;
	GtkWidget* mVBarBox;  //put a ruler wiget in with the colorbar
	GtkWidget* mColorbar;
	static GdkPixmap* mColorbarPixmap;
	static GdkPixmap* mDrawingSurfacePixmap;
	static GdkGC* mGC;
	static GdkGC* mXORGC;
	static GdkFont *mFont;
	static GDKCanvas *mInstance;
	int mIndex;

	//the lat long grid, cursors and whatnot
	ContourGraphicContainer * mDecorations;
	LatLongGrid *mLatLongGrid;
	GtkGPSDisplay * mGPSDisplay;
	GtkCurrentPositionIndicator * mGtkCurrentPositionIndicator;
	CursorCrosshair * mCrosshairCursor;
	CursorDrawTrack * mDrawTrackCursor;
	CursorNull * mNullCursor;
	EffortDecorationCollection * mDrawEfforts;
};

#endif

