/*
** gecko.h - Displays a web-page UI.
**
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Wed Sep 19 21:35:36 EDT 2001
*/

#ifndef _GECKO_H_
#define _GECKO_H_

#include <gtk/gtk.h>
#include "gtkmozembed.h"
//#include <gtkmozembed_internal.h>

//@REX hard code domain
#define CONTOUR_DOMAIN_AND_PORT "http://contour.aquilon.no-ip.org"

class Gecko
{
public:
	Gecko();   // use singleton accessor (GetInstance)
	~Gecko();
	static void Close(GtkWidget *widget, GtkFileSelection *fs);
	void Show(const char *url);
	static void OnNewWindow(GtkMozEmbed *moz, GtkMozEmbed **retval, guint chromemask, gpointer data);
	static void OnDestroyBrowser(GtkWidget *moz, gpointer data);
	static gint OnOpenURI(GtkMozEmbed *moz, const char *uri, gpointer data);
	GtkMozEmbed * GetMoz() { return mMoz; }
	GtkWidget * GetDialog() { return mDialog; }
	static Gecko * GetInstance();
	bool Initialize();
protected:
	char * mURL;
	GtkMozEmbed *mMoz;
	GtkWidget *mDialog;
	static Gecko *mInstance;
	GtkWidget *mVbox2;

};

#endif

