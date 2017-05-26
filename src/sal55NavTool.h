/*
** sal55NavTool.h - An on-screen GTK GUI element that allows the user to pan and zoom.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Wed Jan 10 21:34:12 EST 2001
*/

#ifndef _SAL55NAVTOOL_H_
#define _SAL55NAVTOOL_H_

#include "softwareNavTool.h"
#include <gtk/gtk.h>

#define SAL55NAVTOOL_VERT_PIXELS 150
#define SAL55NAVTOOL_HORZ_PIXELS 152

class Sal55NavTool : public SoftwareNavTool
{
public:
	Sal55NavTool();
	~Sal55NavTool();
	enum Corner {TOP_LEFT, TOP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT};
	void Initialize(GtkWidget *parent, GtkWidget *splash, Corner = TOP_RIGHT);
	virtual void Show();
	virtual void Hide();
	virtual void MoveToCorner(Corner corner);
	
protected:
	//typedef void *callback(GtkWidget *widget, gpointer data);
	void AddElement(int index, char *xpm[], GtkStyle *style, GdkGC *gc, GtkSignalFunc cb, void *data,
				int left, int right, int top, int bottom, int width, int height);
	GtkWidget *mWindow,*mTable;
	GtkWidget *mElements[23];  //our pixmap is divided into 23 regions
	int mXOffset,mYOffset;  //distance from upper right in pixels
	GtkWidget *mSplash;
};

#endif

