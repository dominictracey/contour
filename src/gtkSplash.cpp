/*
** gtkSplash.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jul 28 22:24:31 EDT 2001
*/

#include "pixmaps/splash.xpm"
#include "gtkSplash.h"

GTKSplash * GTKSplash::mInstance = 0;

GTKSplash::GTKSplash()
{

}

GTKSplash::~GTKSplash()
{

}

GTKSplash * GTKSplash::GetInstance()
{
	if (!mInstance)
		mInstance = new GTKSplash;
	return mInstance;
}

void GTKSplash::Show(bool OKButton)
{
	if (!OKButton)
	{
		mSplash = gtk_window_new(GTK_WINDOW_POPUP);
	} else
	{
		mSplash = gtk_window_new(GTK_WINDOW_DIALOG);
	}
	gtk_widget_set_usize( GTK_WIDGET (mSplash), 400, 256);     
	gtk_window_set_position(GTK_WINDOW(mSplash),GTK_WIN_POS_CENTER); 
	GtkWidget *vbox = gtk_vbox_new(FALSE,0);
	gtk_container_add (GTK_CONTAINER(mSplash),vbox);
	gtk_widget_show(vbox);
	gtk_widget_realize(mSplash);
	
	GdkBitmap *mask;
	GtkStyle *style;
	GdkGC *gc;
	style = gtk_widget_get_default_style();

	GdkPixmap *gdk_pixmap = gdk_pixmap_create_from_xpm_d( mSplash->window,&mask,
		&style->bg[GTK_STATE_NORMAL], splash_xpm );
	mPixie = gtk_pixmap_new( gdk_pixmap, mask );
	gtk_box_pack_start (GTK_BOX(vbox),mPixie,false,false,0);
	gdk_pixmap_unref(gdk_pixmap);
	gtk_widget_show(mPixie);
	gtk_widget_show(mSplash);
	gdk_flush();
	while (gtk_events_pending())
		gtk_main_iteration();

}

void GTKSplash::Hide()
{
	gtk_widget_hide(mSplash);
}

