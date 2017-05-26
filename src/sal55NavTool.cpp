/*
** sal55NavTool.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Wed Jan 10 21:44:45 EST 2001
*/

#include "sal55NavTool.h"
#include "gtkMenu.h"
#include "appFactory.h"
#include "canvas.h"
#include "gtkCommandHandler.h"
#include "commandPan.h"
#include "commandZoom.h"

#include "c11.xpm"
#include "c12.xpm"
#include "c121.xpm"
#include "c13.xpm"
#include "c21.xpm"
#include "c22.xpm"
#include "c23.xpm"
#include "c24.xpm"
#include "c25.xpm"
#include "c31.xpm"
#include "c32.xpm"
#include "c33.xpm"
#include "c34.xpm"
#include "c35.xpm"
#include "c41.xpm"
#include "c42.xpm"
#include "c43.xpm"
#include "c44.xpm"
#include "c45.xpm"
#include "c51.xpm"
#include "c521.xpm"
#include "c522.xpm"
#include "c53.xpm"
#include "all.xpm"

Sal55NavTool::Sal55NavTool()
{
	mXOffset = 0;
	mYOffset = 0;
	mSplash = NULL;
}

Sal55NavTool::~Sal55NavTool()
{

}

void Sal55NavTool::Initialize(GtkWidget *parent, GtkWidget *splash, Corner corner /*= TOP_RIGHT*/)
{
	GdkPixmap *pixmap;
	GtkStyle *style;
	GdkGC *gc;

	//mWindow = gtk_window_new( GTK_WINDOW_POPUP );
	mWindow = gtk_fixed_new();

	//we need a window that exists and has been shown from which to get our colormaps and stuff .
	mSplash = splash;
	// the parent parameter is the packing table of the main frame. We put our control
	// into the same box the canvas is in.
	gtk_table_attach(GTK_TABLE(parent),GTK_WIDGET(mWindow),0,1,2,3,(GtkAttachOptions)(GTK_EXPAND | GTK_FILL),(GtkAttachOptions)(GTK_EXPAND | GTK_FILL),0,0);

	// get ready to add the elements into the packing table
	style = gtk_widget_get_default_style();
	gc = style->black_gc;
	mTable = gtk_table_new(7,3,0);

	AddElement(0, c11_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::NORTHWEST), 0,2,0,2,52,52);
	AddElement(1, c12_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle), ((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::NORTH), 2,3,0,1,44,44);
	AddElement(2, c121_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((ZoomCommand *)Command::Create(Command::ZOOM))->SetStepSize(.75), 2,3,1,2,44,8);
	AddElement(3, c13_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::NORTHEAST), 3,5,0,2,54,52);

	AddElement(4, c21_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::WEST), 0,1,2,3,42,15);
	AddElement(5, c22_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::WEST), 1,2,2,3,10,15);
	AddElement(6, c23_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((ZoomCommand *)Command::Create(Command::ZOOM))->SetStepSize(.75), 2,3,2,3,44,15);
	AddElement(7, c24_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::EAST), 3,4,2,3,12,15);
	AddElement(8, c25_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::EAST), 4,5,2,3,42,15);

	AddElement(9,  c31_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::WEST), 0,1,3,4,42,15);
	AddElement(10, c32_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::WEST), 1,2,3,4,10,15);
	AddElement(11, c33_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),NULL, 2,3,3,4,44,15);
	AddElement(12, c34_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::EAST), 3,4,3,4,12,15);
	AddElement(13, c35_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::EAST), 4,5,3,4,42,15);
			
	AddElement(14, c41_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::WEST), 0,1,4,5,42,13);
	AddElement(15, c42_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::WEST), 1,2,4,5,10,13);
	AddElement(16, c43_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((ZoomCommand *)Command::Create(Command::ZOOM))->SetStepSize(1.25), 2,3,4,5,44,13);
	AddElement(17, c44_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::EAST), 3,4,4,5,12,13);
	AddElement(18, c45_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::EAST), 4,5,4,5,42,13);

	AddElement(19, c51_xpm,  style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::SOUTHWEST), 0,2,5,7,52,57);
	AddElement(20, c521_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((ZoomCommand *)Command::Create(Command::ZOOM))->SetStepSize(1.25), 2,3,5,6,44,12);
	AddElement(21, c522_xpm, style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::SOUTH), 2,3,6,7,44,45);
	AddElement(22, c53_xpm,  style, gc, GTK_SIGNAL_FUNC(GtkCommandHandler::Handle),((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::SOUTHEAST), 3,5,5,7,54,57);

	gtk_widget_show(mTable);
	
}

/*
** AddElement - Create a pixmap in the specified location in the packing table.
*/
void Sal55NavTool::AddElement(int index, char *xpm[], GtkStyle *style, GdkGC *gc, GtkSignalFunc cb, void *data,
				int left, int right, int top, int bottom, int width, int height)
{
	GdkBitmap *mask;
	//create the pixmap widget
	GdkPixmap *gdk_pixmap = gdk_pixmap_create_from_xpm_d( mSplash->window, &mask,
		&style->bg[GTK_STATE_NORMAL], xpm );
	mElements[index] = gtk_pixmap_new( gdk_pixmap, mask );
	gdk_pixmap_unref(gdk_pixmap);
	gtk_widget_show(mElements[index]);

	//put it in a fixed element and pack it into the table
	//GtkWidget *fixed = gtk_fixed_new();
	GtkWidget *fixed = gtk_button_new();
	gtk_widget_set_usize(fixed, width, height);
	//gtk_fixed_put( GTK_FIXED(fixed), mElements[index], 0, 0 );
	gtk_container_add (GTK_CONTAINER (fixed), mElements[index]);
	gtk_widget_show( fixed );
	gtk_table_attach_defaults(GTK_TABLE(mTable),fixed,left,right,top,bottom);

	//and attach the callback for button pushes
	gtk_signal_connect (GTK_OBJECT (fixed), "clicked",  GTK_SIGNAL_FUNC (cb), data);
	//gtk_widget_set_events( mElements[index], 
	//					gtk_widget_get_events(mElements[index]) | GDK_BUTTON_PRESS_MASK );
	//gtk_signal_connect( GTK_OBJECT(mElements[index]), "button_press_event",
	//					GTK_SIGNAL_FUNC(cb), NULL );	
}

void Sal55NavTool::Show()
{
	gtk_widget_show( mWindow );
}

void Sal55NavTool::Hide()
{
	gtk_widget_hide( mWindow );
}

/*
** MoveToCorner - Implement me please
*/
void Sal55NavTool::MoveToCorner(Corner corner)
{
	GtkStyle *style;
	GdkBitmap *mask;
	style = gtk_widget_get_default_style();
	//@REX splashscreen must be up?
	GdkPixmap *gdk_pixmap = gdk_pixmap_create_from_xpm_d( mSplash->window, &mask,
		&style->bg[GTK_STATE_NORMAL], all_xpm );
	// @REX strange that we can't get our size from mWindow at this point. Have to use canvas size
	// which is hopefully the same. (Maybe mWindow is not really getting sized and it just isn't
	// clipping mTable?)
	// took off an extra 5 pixels because it was forcing a resize on the table element (by expanding it 2 pixels?)
	mYOffset = 5; //AppFactory::GetInstance()->GetApp()->GetCanvas()->GetHeight() - SAL55NAVTOOL_VERT_PIXELS -5;
	gtk_fixed_put(GTK_FIXED(mWindow),GTK_WIDGET(mTable),mXOffset,mYOffset);
	/* This masks out everything except for the image itself */
	gtk_widget_shape_combine_mask( mWindow, mask, mXOffset, mYOffset );
}

