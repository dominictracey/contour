/*
** gtkApp.cpp - GTK+ app object
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
** DPT 12.12.98
*/

#include "gtkApp.h"
#include <gtk/gtk.h>
#include "gtkMenu.h"
#include "gtkToolbar.h"
#include "splineEngine.h"
#include "parametricSurface.h"
#include "gdkCanvas.h"
#include "colorDefaultAlgorithm.h"
#include "rcPersistence.h"
#include "gtkPreferences.h"
#include "mgd77Extractor.h"
#include "gtkLatLongLine.h"
#include "sal55NavTool.h"
#include "eventSource.h"
#include "command.h"
#include "commandEventSource.h"
#include "gtkCurrentPositionIndicator.h"
#include "gps.h"
#include "gtkSplash.h"
#include "contourDBManager.h"
#include <sys/time.h>
#include <unistd.h>
#include "cursorCrosshair.h"
#include <string.h>
#include <stdio.h>

GtkApp::GtkApp()
{
	mPreferences = NULL;
	mPersistence = NULL;
	GetPreferences();
	mEngine = NULL;
	mExtractor = NULL;
	mTable = NULL;
	mNavTool = NULL;

	//our GPS timer
	timeval tv;
	if (gettimeofday(&tv,NULL) != 0)
		throw ("gettimeofday failure in GtkApp::UpdateGPSDisplay()");
	mLastGPSSample = tv.tv_sec;
}

GtkApp::~GtkApp()
{
	if (mMenu)
		delete mMenu;
	if (mToolbar)
		delete mToolbar;
	if (GtkPreferences::mInstance)
		delete GtkPreferences::mInstance;
	if (mPersistence)
		delete mPersistence;
	if (mExtractor)
		delete mExtractor;
	if (mNavTool)
		delete mNavTool;
}

/*
** Initialize - Set up main window, etc.
*/
bool GtkApp::Initialize(int argc, char **argv)
{
	gtk_init (&argc, &argv);
	// put up a splash screen -- if we don't, we get gdk_warnings when we 
	// try to pull in the xpm's for the toolbar.
	GTKSplash *splash = GTKSplash::GetInstance();
	splash->Show(false);
		
	mTable = gtk_table_new(4,1,FALSE); //rows,cols,homogenous
	gtk_widget_show(mTable);

	mMainFrame = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize( GTK_WIDGET (mMainFrame), 800, 600);         
 	gtk_window_set_title(GTK_WINDOW (mMainFrame), "Contour");
	gtk_signal_connect (GTK_OBJECT (mMainFrame), "delete_event",
                             GTK_SIGNAL_FUNC (GtkApp::OnDeleteEvent), NULL);

	gtk_signal_connect (GTK_OBJECT (mMainFrame), "destroy",
                             GTK_SIGNAL_FUNC (GtkApp::OnDestroy), NULL);

	gtk_container_add(GTK_CONTAINER(mMainFrame), mTable);

	mCanvas = (ContourCanvas *) new GDKCanvas;
	//set up the default color scheme
	ColorDefaultAlgorithm algo;
	algo.Initialize(*mCanvas);
	//now that the colors are set we can do the actual canvas & colorbar
	mCanvas->Initialize();
	
	mMenu = new BGtkMenu;
	mMenu->Initialize();

	mToolbar = new BGtkToolbar(splash->GetWidget());
	mToolbar->Initialize();

	//initialize the database
	ContourDBManager::GetInstance()->Initialize();
	
	//Add the menubar & toolbar to the top packing box
	gtk_table_attach(GTK_TABLE(mTable),GTK_WIDGET(mMenu->GetControl()),0,1,0,1,(GtkAttachOptions)(GTK_FILL | GTK_EXPAND),(GtkAttachOptions)(GTK_SHRINK),0,0);
	gtk_table_attach(GTK_TABLE(mTable),GTK_WIDGET(mToolbar->GetControl()),0,1,1,2,(GtkAttachOptions)(GTK_FILL | GTK_EXPAND),(GtkAttachOptions)(GTK_SHRINK),0,0);
	gtk_table_attach_defaults(GTK_TABLE(mTable),GTK_WIDGET(((GDKCanvas *)mCanvas)->GetCanvasControl()),0,1,2,3);//,GTK_FILL | GTK_EXPAND,GTK_FILL | GTK_EXPAND,0,0);
	gtk_table_attach(GTK_TABLE(mTable),GTK_WIDGET(((GDKCanvas *)mCanvas)->GetColorbarControl()),0,1,3,4,(GtkAttachOptions)(GTK_FILL | GTK_EXPAND),(GtkAttachOptions)(GTK_SHRINK),0,0);

	gtk_widget_show(mMainFrame);
	
	//@REX not sure why we need to show the mainframe before this works...
	mNavTool = new Sal55NavTool();
	((Sal55NavTool *)mNavTool)->Initialize(mTable,splash->GetWidget());
	((Sal55NavTool *)mNavTool)->MoveToCorner(Sal55NavTool::TOP_LEFT);
	((Sal55NavTool *)mNavTool)->Show();

	// need to have the window for initializing some cursor stuff
	CursorCrosshair::Initialize();

	splash->Hide();
	
	//Set any preferences that are manifested in the GUI
	EventSourceCommand *gc = (EventSourceCommand *)Command::Create(Command::GRID);
	EventSourceCommand *dc = (EventSourceCommand *)Command::Create(Command::DRAW);
	EventSourceCommand *tc = (EventSourceCommand *)Command::Create(Command::TRACK);
	// added in this broadcast wrapper as a "bootstrap" way to synch up the subscribers
	// without actually Execute()ing the commands.
	gc->Broadcast();
	dc->Broadcast();
	tc->Broadcast();

	mCurrInd = new GtkCurrentPositionIndicator;
	GPS::GetInstance()->Initialize(GetPreferences()->GetgpsHost(),GetPreferences()->GetgpsPort());


	return true; 
}
         
/*
** OnDeleteEvent - Check dirty flag in here...
*/        
gint GtkApp::OnDeleteEvent(GtkWidget *widget, GdkEvent *event, gpointer data)
{
//	g_return_val_if_fail (data != NULL, 0);
	g_return_val_if_fail (widget != NULL, 0);
	g_return_val_if_fail (event != NULL, 0);
	return false;
}

/*
**
*/
void GtkApp::OnDestroy(GtkWidget *widget, gpointer data)
{
//	g_return_if_fail (data != NULL);
	g_return_if_fail (widget != NULL);
	gtk_main_quit ();
}

/*
** Run - Do yer thang
*/
bool GtkApp::Run()
{
	gtk_widget_show(mMainFrame);
 	gtk_main();
	return 0;	
}

/*
** Stop - cease and desist
*/
void GtkApp::Stop()
{
	DetachGPS(0);
	gtk_main_quit();
}

/*
** AttachGPS - Tie the GPS feed into a handler
*/
void GtkApp::AttachGPS(int handle)
{
	mGPSHandle = gdk_input_add(handle, GDK_INPUT_READ, GtkApp::GPSCallback,NULL);
}

/*
** GPSCallback - This is the callback from the GPS monitor, just pass it on
**	to the GPS class in a toolkit-independent manner.
*/
void GtkApp::GPSCallback(gpointer data, gint fd, GdkInputCondition condition)
{
	GPS::GetInstance()->HandleInput(&fd);
}

/*
** UpdateGPSDisplay - The GPS class calls this when new GPS data is available.
**	@REX Do we want to do this in "passive" or "active" manner?
*/
void GtkApp::UpdateGPSDisplay()
{
	GetCanvas()->BeginDrawing();
	GetCanvas()->DoneDrawing();

	//check if enouh time has elapsed to store a sample
	timeval tv;
	if (gettimeofday(&tv,NULL) != 0)
		throw ("gettimeofday failure in GtkApp::UpdateGPSDisplay()");
	if ((tv.tv_sec - mLastGPSSample) >= GetPreferences()->GetgpsSampleRate())
	{
		mLastGPSSample = tv.tv_sec;
		//convert from fractional degrees (45.9876) to total seconds (234456.23)
		// and put the sample in the database.
		Position lat(GPS::GetInstance()->GetLatitude());
		Position lon(GPS::GetInstance()->GetLongitude());
		//ContourDBManager::GetInstance()->AddDataPoint(lat.TotalSeconds(),lon.TotalSeconds(),77);
	}
}

/*
** AttachGPS - Tie the GPS feed into a handler
*/
void GtkApp::DetachGPS(int handle)
{
	GPS::GetInstance()->CloseInput();
	gdk_input_remove(mGPSHandle);		
}

/*
** GetPreferences - Provide the user preferences. We store these in an
**	rc file that is managed by the RCPersistence class. If you want to
**	persist in some other manner use another ContourPersistence subclass.
*/
ContourPreferences * GtkApp::GetPreferences()
{
	if (!mPreferences)
	{
		if (!mPersistence)
			mPersistence = new RCPersistence;
		mPreferences = GtkPreferences::GetInstance();
		mPreferences->SetPersistence(mPersistence);
		mPersistence->SetPrefs(mPreferences);
		mPersistence->Read();
	}

	return mPreferences;
}

ContourEngine * GtkApp::GetEngine()
{
	if (!mEngine)
		//mEngine = new SplineEngine;
		mEngine = new ParametricSurfaceEngine();
	return mEngine;
}

ContourExtractor * GtkApp::GetExtractor()
{
	if (!mExtractor)
		mExtractor = new MGD77Extractor;
	return mExtractor;
}

LatLongLine * GtkApp::GetLatLongLine(Position &pos, bool lat)
{
	return new GtkLatLongLine(pos, lat);
}

void GtkApp::ProcessEvents()
{
    while(gtk_events_pending())
        gtk_main_iteration();
}
