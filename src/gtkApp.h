/*
** gtkApp.h - Interface for GTK+ app object
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
** DPT 12.12.98
*/

#ifndef _GTKAPP_H_
#define _GTKAPP_H_

#include <gtk/gtk.h>
#include "bathApp.h"
class ContourPersistence;
class BGtkMenu;
class BGtkToolbar;
class GtkCurrentPositionIndicator;

class GtkApp : public BathApp
{
public:
	GtkApp();
	virtual ~GtkApp();

	virtual bool Initialize(int argc, char **argv);
	virtual bool Run();
	virtual void Stop();

	virtual void AttachGPS(int handle);
	virtual void DetachGPS(int handle);
	virtual void UpdateGPSDisplay();

	//accessors for the various sub-systems
	virtual ContourPreferences *GetPreferences();	
	virtual ContourCanvas * GetCanvas() {return mCanvas;}
	GtkWidget *GetMainFrame() {return mMainFrame;}
	GtkWidget *GetOuterBox() {return mVOuterBox;}
	virtual ContourEngine * GetEngine();
	virtual ContourExtractor * GetExtractor();
	GtkWidget * GetTable() {return mTable;}
	virtual BGtkToolbar * GetToolbar() {return mToolbar;}
	virtual BGtkMenu * GetMenu() {return mMenu;}
	LatLongLine * GetLatLongLine(Position &pos, bool lat);
	//Event handlers
	static gint OnDeleteEvent(GtkWidget *w, GdkEvent *e, gpointer d);
	static void OnDestroy(GtkWidget *widget, gpointer data);
	void ProcessEvents();
protected:

	//see diagram below for containment
	GtkWidget *mMainFrame;
	BGtkMenu *mMenu;
	BGtkToolbar *mToolbar;
	GtkWidget *mVOuterBox;
	GtkWidget *mVBarBox;
	GtkWidget *mHContentBox;
	ContourCanvas *mCanvas;
	ContourPreferences *mPreferences;
	ContourPersistence *mPersistence;
	ContourEngine *mEngine;
	ContourExtractor *mExtractor;
	ContourNavTool *mNavTool;
	GtkWidget *mTable;
	
	//file handle for GPS port
	gint mGPSHandle;
	static void GPSCallback(gpointer data, gint fd, GdkInputCondition condition);

	//GPS sample interval (last epoch sampled)
	int mLastGPSSample;
	
	GtkCurrentPositionIndicator * mCurrInd;

};

/*

+-------------------------------------------------------------------------+
|+-----------------------------------------------------------------------+|
||+---------------------------------------------------------------------+||
|||                        Menubar                                      |||
||+---------------------------------------------------------------------+||
||+---------------------------------------------------------------------+||
|||                        Toolbar                                      |||
||+---------------------------------------------------------------------+||
||                         VBarBox (contains menubar & toolbar)          ||
|+-----------------------------------------------------------------------+|
|                                                                         |
|+-----------------------------------------------------------------------+|
||+------------------------------------------------------++-------------+||
|||                                                      ||             |||
||| Canvas  actually this is a VBox with a Drawing w.    ||  Controls   |||
|||                                                      ||             |||
|||                                                      ||             |||
|||+----------------------------------------------------+||             |||
||||                        ColorBar                    |||             |||
|||+----------------------------------------------------+||             |||
||+------------------------------------------------------++-------------+||
||                        HContentBox (contains canvas & controls)       ||
|+-----------------------------------------------------------------------+|
|              VOuterBox (contains VBarBox, HContentsBox)                 |
+-------------------------------------------------------------------------+

		(VOuterBox contained by MainFrame)
*/
#endif
