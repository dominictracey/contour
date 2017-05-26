/*
** gtkSplash.h - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jul 28 22:22:27 EDT 2001
*/

#ifndef _GTKSPLASH_H_
#define _GTKSPLASH_H_

#include <gtk/gtk.h>

class GTKSplash
{
public:
	~GTKSplash();
	void Show(bool OKButton);
	void Hide();
	GtkWidget * GetWidget() { return mSplash; }
	static GTKSplash * GetInstance();
protected:
	GTKSplash();
	GtkWidget *mSplash;
	GtkWidget *mPixie;
	static GTKSplash *mInstance;
};

#endif

