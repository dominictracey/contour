/*
** gdkCanvas.cpp -Color and drawing surface implemented with GDK drawing tools. 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.18.98 DPT
*/


#include "gdkCanvas.h"
#include "position.h"
#include <stdio.h>
//#include <gdk/gdkx.h>
#include "gtkApp.h"
//note these are all abstract classes - please don't add
// dependencies to concrete classes.
#include "bathApp.h"
#include "appFactory.h"
#include "engine.h"
#include "timer.h"
#include "gps.h"
#include "contourGraphicContainer.h"
#include "latLongGrid.h"
#include "gtkGPSDisplay.h"
#include "gtkCurrentPositionIndicator.h"
#include "preferences.h"
#include "cursorCrosshair.h"
#include "cursorDrawTrack.h"
#include "cursorNull.h"
#include "effortDecorationCollection.h"
//#include "effort.h"

#define COLORBARHEIGHT 30
//statics
GdkPixmap *GDKCanvas::mColorbarPixmap = NULL;
GdkPixmap *GDKCanvas::mDrawingSurfacePixmap = NULL;
GdkGC 	  *GDKCanvas::mGC = NULL;
GdkColor * GDKCanvas::mColors[GDKCANVAS_NUM_COLORS];
GDKCanvas * GDKCanvas::mInstance = NULL;

GdkGC *GDKCanvas::mXORGC = NULL;
GdkFont *GDKCanvas::mFont = NULL;
/*
int GDKCanvas::mLatPosX = 0;
int GDKCanvas::mLonPosX = 0;
int GDKCanvas::mSpeedPosX = 0;
int GDKCanvas::mLatPosY = 0;
int GDKCanvas::mLonPosY = 0;
int GDKCanvas::mSpeedPosY = 0;
*/
/*
** GDKCanvas constructor - Flush out the array (mColors) that contains the color
**	"pallette". This is an array of GDKcolors that we use to paint 
**	the depths we are handed. Our minimum depth maps to mColors[0] and the
**	maximum depth maps to mColors[GDKCANVAS_NUM_COLORS], with a linear 
**	gradient in between. Any depth greater than maxdepth maps to black and
**	any less that mindepth goes to white.
*/
GDKCanvas::GDKCanvas()
{
	for (int i=0; i < GDKCANVAS_NUM_COLORS; i++)
		mColors[i] = NULL;

	mColormap = gdk_colormap_get_system();
	if (!mColormap)
		printf("Canvas error: Couldn't snag a default colormap.\n");

	mNumColors = GDKCANVAS_NUM_COLORS;

	//figure out screen depth
	mDepth = gdk_visual_get_best_depth();

	g_return_if_fail (mInstance == NULL);
	mInstance = this;

	mIndex = -1;
	
	mDecorations = new ContourGraphicContainer;
	mGPSDisplay = NULL;
	mLatLongGrid = NULL;
	mGtkCurrentPositionIndicator = NULL;
	mCrosshairCursor = NULL;
	mDrawTrackCursor = NULL;
	mNullCursor = NULL;
	mDrawEfforts = NULL;
}

GDKCanvas::~GDKCanvas()
{
	for (int i=0; i < GDKCANVAS_NUM_COLORS; i++)
		if (mColors[i])
			delete mColors[i];

	gdk_pixmap_unref(mDrawingSurfacePixmap);
	gdk_pixmap_unref(mColorbarPixmap);
	gdk_gc_unref(mGC);
	gdk_gc_unref(mXORGC);

	if (mLatLongGrid)
		delete mLatLongGrid;
		
	if (mGPSDisplay)
	{
		mDecorations->Unregister(mGPSDisplay);
		delete mGPSDisplay;
	}

	if (mGtkCurrentPositionIndicator)
	{
		mDecorations->Unregister(mGtkCurrentPositionIndicator);
		delete mGtkCurrentPositionIndicator;
	}
	
	if (mCrosshairCursor)
	{
		mDecorations->Unregister(mCrosshairCursor);
		delete mCrosshairCursor;
	}
	
	if (mDrawTrackCursor)
	{
		mDecorations->Unregister(mDrawTrackCursor);
		delete mDrawTrackCursor;
	}

	if (mNullCursor)
	{
		mDecorations->Unregister(mNullCursor);
		delete mNullCursor;
	}

	if (mDrawEfforts)
	{
		mDecorations->Unregister(mDrawEfforts);
		delete mDrawEfforts;
	}


	delete mDecorations;
}

/*
** Initialize - Create the managed widgets that we maintain. We have a vertical
**	packing box (mVOuterbox) that contains the actual map surface
**	(mDrawingSurface) and another vertical packing box (mVBarBox). This
**	inner vertical box contains the color bar (mColorbar) at the bottom
**	of the main window.
**	FIXME - Need to add a horizontal ruler in with the colorbar to show
**	the maximum and minimum depths represented by the color scheme (and
**	some of the intermediate depths along the way). 
*/
bool GDKCanvas::Initialize()
{
	//drawing canvas proper
	mDrawingSurface = gtk_drawing_area_new();


	//I believe the dimensions are ignored because of the packing scheme
	gtk_drawing_area_size(GTK_DRAWING_AREA(mDrawingSurface),500,500);

	// we have to make a dummy one of these to get the event pipes hooked up
	CursorCrosshair c;
	
	gtk_widget_show(mDrawingSurface);
	
	gtk_signal_connect (GTK_OBJECT (mDrawingSurface), "expose_event",
                      (GtkSignalFunc) DrawingSurfaceExposeCB, NULL);

	gtk_signal_connect (GTK_OBJECT (mDrawingSurface), "configure_event",
                      (GtkSignalFunc) DrawingSurfaceConfigureCB, NULL);


	//colorbar
	mColorbar = gtk_drawing_area_new();
	//I believe the dimensions are ignored because of the packing scheme
	gtk_drawing_area_size(GTK_DRAWING_AREA(mColorbar),
			99,COLORBARHEIGHT);
	gtk_widget_show(mColorbar);

	gtk_signal_connect (GTK_OBJECT (mColorbar), "expose_event",
                      (GtkSignalFunc) ColorbarExposeCB, NULL);

	gtk_signal_connect (GTK_OBJECT (mColorbar), "configure_event",
                      (GtkSignalFunc) ColorbarConfigureCB, NULL);

}


/*
** Draw - Draw a point on the screen. Called by SplineEngine::Render().
**	Note that we draw to the backing pixmap and let the Expose events
**	actually draw to the DrawingArea widget.
**	FIXME - should be a data structure (array of XPoints?) that we
**	add these points to and when DoneDrawing() is called we call
**	gdk_draw_points.
*/
void GDKCanvas::Draw(int x, int y, float z)
{
	float adj = z - mMinDepth;
	if (adj > mRange)
		adj = mRange;
	if (adj < 0)
		adj = 0;
		
	int index = (int)((GDKCANVAS_NUM_COLORS_LESS_ONE) * (1- adj / mRange));

	if (index != mIndex)
	{
		gdk_gc_set_foreground(mGC,mColors[index]);
		mIndex = index;
	}
	gdk_draw_point(mDrawingSurfacePixmap,mGC,x,y);
}

void GDKCanvas::BeginDrawing()
{
	//this is ok here
	mDecorations->ClearAll();
}

/*
** DoneDrawing() - After repeated calls to Draw() this method is called to
**	empty the draw queue to the screen. See FIXME for Draw() - this should
**	include a call to gdk_draw_points. 
*/
void GDKCanvas::DoneDrawing()
{
	ASSERT(mDecorations)
	mDecorations->DrawAll();
	
	GdkRectangle update_rect;
	update_rect.x = 0;
	update_rect.y = 0;
	update_rect.width = mDrawingSurface->allocation.width;
	update_rect.height = mDrawingSurface->allocation.height;

	((GtkApp *)(AppFactory::GetInstance()->GetApp()))->ProcessEvents();

	gtk_widget_draw (mDrawingSurface, &update_rect);
}

/*
** SpecifyColor - Called by the Color algorithms (inherit from 
**	ColorAlgorithm - as of 12.98 DefaultColorAlgorithm is
**	only implementation). 
*/
bool GDKCanvas::SpecifyColor(short R, short G, short B, int index)
{
	GdkColor c;
	c.red = R;
	c.green = G;
	c.blue = B;

	//allocate color
	if (gdk_colormap_alloc_color (mColormap, &c, false, true))
	{
		mColors[index] = gdk_color_copy(&c);
		return true;
	}
	  else
	  {
		printf("GDK color error: \
			Failed to allocate %d %d %d for slot %d\n",R,G,B,index);
		return false;
	  }
}

/*
** AllocColors - FIXME
*/
bool GDKCanvas::AllocColors()
{
	//no-op - should cache things above and allocate them in one fell swoop
	return true;
}

/*
** ColorbarExposeCB - Redraw the colorbar from its backing pixmap.
*/
gint GDKCanvas::ColorbarExposeCB(GtkWidget *widget, GdkEventExpose *event)
{
  gdk_draw_pixmap(widget->window,                      
                  widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                  mColorbarPixmap,
                  event->area.x, event->area.y,
                  event->area.x, event->area.y,
                  event->area.width, event->area.height);

  return FALSE;
}

/*
** ColorbarConfigureCB - Set up colorbar's backing pixmap.
*/
gint GDKCanvas::ColorbarConfigureCB(GtkWidget *widget, GdkEventExpose *event)
{
  if (widget->allocation.width > 1)
  {
  if (mColorbarPixmap)
    gdk_pixmap_unref(mColorbarPixmap);

  mColorbarPixmap = gdk_pixmap_new(widget->window,
                          widget->allocation.width,
                          widget->allocation.height,
                          -1);
  gdk_draw_rectangle (mColorbarPixmap,
                      widget->style->white_gc,
                      TRUE,
                      0, 0,
                      widget->allocation.width,
                      widget->allocation.height);

  int factor = GDKCANVAS_NUM_COLORS/widget->allocation.width;
  mGC = gdk_gc_new(widget->window);
 // mXORGC = gdk_gc_new(widget->window);
  for (int i=0;i<widget->allocation.width;i++)
  {
	gdk_gc_set_foreground(mGC,mColors[i*factor]);
	gdk_draw_line(mColorbarPixmap,mGC,i,0,i,widget->allocation.height);
  }
  }
  return TRUE;

}

/*
** DrawingSurfaceExposeCB - Redraw canvas from its backing pixmap.
*/
gint GDKCanvas::DrawingSurfaceExposeCB(GtkWidget *widget, GdkEventExpose *event)
{
  gdk_draw_pixmap(widget->window,                      
                  widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
                  mDrawingSurfacePixmap,
                  event->area.x, event->area.y,
                  event->area.x, event->area.y,
                  event->area.width, event->area.height);

  return FALSE;
}

/*
** DrawingSurfaceConfigureCB - Set up canvas' backing pixmap. 
**	Tell the rendering engine our size with a call to ContourEngine::Resize.
**
**	This gets called at startup and also when the window is resized.
*/
gint GDKCanvas::DrawingSurfaceConfigureCB(GtkWidget *widget, 
					GdkEventExpose *event)
{
	// added in check to make sure allocation size has changed because the
	// NavTool was generating a superfluous config signal when I tried to
	// show it.
	//cout << "Configure canvas width " << widget->allocation.width << " height " << widget->allocation.height << endl;
	BathApp *app = AppFactory::GetInstance()->GetApp();
	if ((widget->allocation.width > 1) &&
		((app->GetCanvas()->GetHeight() != widget->allocation.height) ||
		(app->GetCanvas()->GetWidth() != widget->allocation.width)))
  	{
  		if (mDrawingSurfacePixmap)
    			gdk_pixmap_unref(mDrawingSurfacePixmap);
  		
		mDrawingSurfacePixmap = gdk_pixmap_new(widget->window, 
				widget->allocation.width,
				widget->allocation.height,
				-1);

 		GetCanvas()->SetHeight(widget->allocation.height);
		GetCanvas()->SetWidth(widget->allocation.width);

		//We blow the GPS display away first so it will move to its proper place
		// on a resize.
		GetCanvas()->RemoveDecoration(GPS_DISPLAY);
		GetCanvas()->AddDecoration(GPS_DISPLAY);	
		
		GetCanvas()->AddDecoration(POSITION_INDICATOR);

		// show the lat-long grid only if needed.
		if (app->GetPreferences()->GetShowGrid())
			GetCanvas()->AddDecoration(ContourCanvas::LATLONG_GRID);

		// show the tracklines only if needed.
		if (app->GetPreferences()->GetTrack())
			GetCanvas()->AddDecoration(ContourCanvas::DRAW_EFFORTS);

		// a dummy cursor object to ignore the mouse events
		GetCanvas()->AddDecoration(NULL_CURSOR);
			
		//now tell the engine about our size.
		app->GetEngine()->Resize(widget->allocation.height, widget->allocation.width);

  	}
	return TRUE;

}

/*
** Shift - We are panning so we need to offset the displayed image the 
**	requested amount.
*/
void GDKCanvas::Shift(int NSOffset, int EWOffset)
{
  //draw the pixmap from the proper part of itself	
  gdk_draw_pixmap(mDrawingSurfacePixmap,                      
        mDrawingSurface->style->fg_gc[GTK_WIDGET_STATE(mDrawingSurface)],
        mDrawingSurfacePixmap,
        0, 0,
        EWOffset, NSOffset,
        mWidth-EWOffset, mHeight-NSOffset); 
/*
  //now fill the empty bit that has just been exposed with black
  if (NSOffset < 0)
  	gdk_draw_rectangle (mDrawingSurfacePixmap,
        	mDrawingSurface->style->black_gc,
        	TRUE,
        	0, mDrawingSurface->allocation.height+NSOffset,
        	mDrawingSurface->allocation.width,
        	mDrawingSurface->allocation.height);
  else if (NSOffset > 0)  
  	gdk_draw_rectangle (mDrawingSurfacePixmap,
        	mDrawingSurface->style->black_gc,
        	TRUE,
        	0, 0,
        	mDrawingSurface->allocation.width,
        	NSOffset);

  if (EWOffset < 0)
  	gdk_draw_rectangle (mDrawingSurfacePixmap,
        	mDrawingSurface->style->black_gc,
        	TRUE,
        	mDrawingSurface->allocation.width+EWOffset,0,
        	mDrawingSurface->allocation.width,
        	mDrawingSurface->allocation.height);
  else if (EWOffset > 0)  
  	gdk_draw_rectangle (mDrawingSurfacePixmap,
        	mDrawingSurface->style->black_gc,
        	TRUE,
        	0, 0,
        	EWOffset,mDrawingSurface->allocation.height);
  */
  //force a refresh
  GdkRectangle update_rect;        
  update_rect.x = 0;
  update_rect.y = 0;
  update_rect.width = mDrawingSurface->allocation.width;
  update_rect.height = mDrawingSurface->allocation.height;

  gtk_widget_draw (mDrawingSurface, &update_rect);
}

GdkGC * GDKCanvas::GetXORGC()
{
	if (!mXORGC)
	{
		//mXORGC = gdk_gc_new(GetCanvasControl()->window);
		//mXORGC = gdk_gc_new(widget->window);
		mXORGC = gdk_gc_new(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetCanvasControl()->window);
		gdk_gc_set_function(mXORGC, GDK_XOR);
		GdkColor c;
		gdk_color_white(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetColorMap(),&c);
		gdk_gc_set_function(mXORGC, GDK_XOR);
		gdk_gc_set_foreground(mXORGC,&c);
		gdk_gc_set_background(mXORGC,&c);
		mFont = gdk_font_load("-adobe-courier-medium-r-normal-*-*-140-*-*-m-*-iso8859-1");
		gdk_gc_set_font(mXORGC,mFont);
	}

	return mXORGC;
}

/*
** Add/RemoveDecoration - These functions allow overlays to be added and removed
** 	to the canvas without having to worry about redrawing and everything. The
** 	"decorations" must implement the ContourGraphic interface, which allows the
** 	canvas to request the decoration draw or clear itself.
*/
void GDKCanvas::AddDecoration(decorations d)
{
	if (d == LATLONG_GRID)
	{
		if (!mLatLongGrid )
		{
			mLatLongGrid = new LatLongGrid();
			mDecorations->Register(mLatLongGrid);
		}
	} else if (d == GPS_DISPLAY)
	{
		if (!mGPSDisplay)
		{
			mGPSDisplay = new GtkGPSDisplay;
			mDecorations->Register(mGPSDisplay);
		}
	} else if (d == POSITION_INDICATOR)
	{
		if (!mGtkCurrentPositionIndicator)
		{
			mGtkCurrentPositionIndicator = new GtkCurrentPositionIndicator;
			mDecorations->Register(mGtkCurrentPositionIndicator);
		}
	} else if (d == NULL_CURSOR)
	{
		RemoveDecoration(CROSSHAIR_CURSOR);
		RemoveDecoration(DRAWTRACK_CURSOR);
		if (!mNullCursor)
		{
			mNullCursor = new CursorNull;
			cout << "added null cursor" << endl;
			mDecorations->Register(mNullCursor);
		}
	}  else if (d == CROSSHAIR_CURSOR)
	{
		RemoveDecoration(NULL_CURSOR);
		RemoveDecoration(DRAWTRACK_CURSOR);
		if (!mCrosshairCursor)
		{
			mCrosshairCursor = new CursorCrosshair;
			mDecorations->Register(mCrosshairCursor);
		}
	} else if (d == DRAWTRACK_CURSOR)
	{
		RemoveDecoration(CROSSHAIR_CURSOR);
		RemoveDecoration(NULL_CURSOR);
		if (!mDrawTrackCursor)
		{
			mDrawTrackCursor = new CursorDrawTrack;
			mDecorations->Register(mDrawTrackCursor);
		}
	} else if (d == DRAW_EFFORTS)
	{
		if (!mDrawEfforts)
		{
			mDrawEfforts = EffortDecorationCollection::GetEffortDecorationCollectionByType(Effort::DRAW);
			mDecorations->Register(mDrawEfforts);
		}
	}  else if (d == ALL)
	{
		AddDecoration(LATLONG_GRID);
		AddDecoration(GPS_DISPLAY);
		AddDecoration(POSITION_INDICATOR);
		AddDecoration(NULL_CURSOR);
		if (AppFactory::GetInstance()->GetApp()->GetPreferences()->GetTrack())
			AddDecoration(ContourCanvas::DRAW_EFFORTS);
	}
}

void GDKCanvas::RemoveDecoration(decorations d)
{
	if (d == LATLONG_GRID)
	{
		if ( mLatLongGrid )
		{
			mDecorations->Unregister(mLatLongGrid);
			delete mLatLongGrid;
			mLatLongGrid = NULL;
		}
	} else if (d == GPS_DISPLAY)
	{
		if ( mGPSDisplay )
		{
			mDecorations->Unregister(mGPSDisplay);
			delete mGPSDisplay;
			mGPSDisplay = NULL;
		}
	} else if (d == POSITION_INDICATOR)
	{
		if ( mGtkCurrentPositionIndicator )
		{
			mDecorations->Unregister(mGtkCurrentPositionIndicator);
			delete mGtkCurrentPositionIndicator;
			mGtkCurrentPositionIndicator = NULL;
		}
	}  else if (d == NULL_CURSOR)
	{
		if ( mNullCursor )
		{
			mDecorations->Unregister(mNullCursor);
			delete mNullCursor;
			mNullCursor = NULL;
		}
	}  else if (d == CROSSHAIR_CURSOR)
	{
		if ( mCrosshairCursor )
		{
			mDecorations->Unregister(mCrosshairCursor);
			delete mCrosshairCursor;
			mCrosshairCursor = NULL;
		}
	}  else if (d == DRAWTRACK_CURSOR)
	{
		if ( mDrawTrackCursor )
		{
			mDecorations->Unregister(mDrawTrackCursor);
			delete mDrawTrackCursor;
			mDrawTrackCursor = NULL;
		}
	} else if (d == DRAW_EFFORTS)
	{
		if ( mDrawEfforts )
		{
			mDecorations->Unregister(mDrawEfforts);
			//@REX do we want to delete this?
			//delete mDrawEfforts; 
			mDrawEfforts = NULL;
		}
	} 	else if (d == ALL)
	{
		RemoveDecoration(LATLONG_GRID);
		RemoveDecoration(GPS_DISPLAY);
		RemoveDecoration(POSITION_INDICATOR);
		RemoveDecoration(CROSSHAIR_CURSOR);
		RemoveDecoration(DRAWTRACK_CURSOR);
		RemoveDecoration(DRAW_EFFORTS);
		AddDecoration(NULL_CURSOR);
	}
}

