/*
** gtkPreferences.cpp - The dialog for collecting preference information.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
*/

#include "gtkPreferences.h"
#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "persistence.h"
#include "position.h"
#include "appFactory.h"
#include "engine.h"

//statics
GtkWidget * GtkPreferences::mDialog = NULL;
GtkWidget * GtkPreferences::mNotebook = NULL;
bool GtkPreferences::mOK = FALSE;
GtkPreferences *GtkPreferences::mInstance = NULL;

GTKPREF_DEFINE(MaxDepth,float)
GTKPREF_DEFINE(MinDepth,float)
GTKPREF_DEFINE(AutoColor,bool)
GTKPREF_DEFINE(ShowGrid,bool)
GTKPREF_DEFINE(Draw,bool)
GTKPREF_DEFINE(AutoCenter,bool)
GTKPREF_DEFINE(Track,bool)

GTKPREF_DEFINE(XScale,float)
GTKPREF_DEFINE(YScale,float)
SamplePoint * GtkPreferences::mUpperLeft = NULL;

//database
GTKPREF_DEFINE(pgHost,char *)
GTKPREF_DEFINE(pgPort,char *)
GTKPREF_DEFINE(pgOptions,char *)
GTKPREF_DEFINE(pgDBName,char *)
GTKPREF_DEFINE(pgUser,char *)
GTKPREF_DEFINE(pgPassword,char *)

//gps
GTKPREF_DEFINE(gpsHost,char *)
GTKPREF_DEFINE(gpsPort,char *)
GTKPREF_DEFINE(gpsSampleRate,int)

GtkPreferences::GtkPreferences()
{
	mUpperLeft = NULL;
	mpgHost = NULL;
	mpgPort = NULL;
	mpgOptions = NULL;
	mpgDBName = NULL;
	mpgUser = NULL;
	mpgPassword = NULL;

	mgpsHost = NULL;
	mgpsPort = NULL;
}

GtkPreferences::~GtkPreferences()
{
	if (mUpperLeft)
		delete mUpperLeft;

	if (mpgHost) delete mpgHost;
	if (mpgPort) delete mpgPort;
	if (mpgOptions) delete mpgOptions;
	if (mpgDBName) delete mpgDBName;
	if (mpgUser) delete mpgUser;
	if (mpgPassword) delete mpgPassword;

	if (mgpsHost) delete mgpsHost;
	if (mgpsPort) delete mgpsPort;
}

GtkPreferences * GtkPreferences::GetInstance()
{
	if (!mInstance)
		mInstance = new GtkPreferences;
	return mInstance;
}
bool GtkPreferences::Edit() 
{
	if (!mPersistor)
		throw ("GtkPreferences::Edit : Attempt to edit preferences\n \
with no way to save changes through persistence object.");

	mDialog = gtk_window_new (GTK_WINDOW_TOPLEVEL);
 	gtk_signal_connect (GTK_OBJECT (mDialog), "destroy",
  	GTK_SIGNAL_FUNC(OnCancel), &mDialog);
 	gtk_signal_connect (GTK_OBJECT (mDialog), "delete_event",
  	GTK_SIGNAL_FUNC(OnCancel), &mDialog);
 	gtk_window_set_title (GTK_WINDOW (mDialog), "Settings");
 	gtk_container_set_border_width (GTK_CONTAINER (mDialog), 2);
 	//gtk_widget_set_uposition (mDialog, (x_pos+10), (y_pos+10));
 	gtk_widget_set_usize (mDialog, 400, 500);
	
 	GtkWidget *vbox2 = gtk_vbox_new (FALSE, 0);
 	gtk_container_add (GTK_CONTAINER (mDialog), vbox2);
 	gtk_container_set_border_width (GTK_CONTAINER (vbox2), 5);
 	gtk_widget_show (vbox2);

 	mNotebook = gtk_notebook_new ();
 	gtk_notebook_set_tab_pos (GTK_NOTEBOOK (mNotebook), GTK_POS_TOP);
 	gtk_box_pack_start (GTK_BOX (vbox2), mNotebook, TRUE, TRUE, 0);
 	gtk_widget_show (mNotebook);

	AddDepthPage();
	AddPostgresPage();
	AddGPSPage();
	
 	GtkWidget *sep = gtk_hseparator_new ();
 	gtk_box_pack_start (GTK_BOX (vbox2), sep, FALSE, TRUE, 0);
 	gtk_widget_show (sep);

 	GtkWidget *hbox = gtk_hbutton_box_new();  
 	gtk_button_box_set_layout(GTK_BUTTON_BOX(hbox), GTK_BUTTONBOX_END);
 	gtk_button_box_set_spacing(GTK_BUTTON_BOX(hbox), 5);
 	gtk_box_pack_start (GTK_BOX (vbox2), hbox, FALSE, FALSE, 0);
 	gtk_widget_show (hbox);

 	GtkWidget *button = gtk_button_new_with_label ("OK");
 	gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
  		GTK_SIGNAL_FUNC(this->OnOK), GTK_OBJECT (mDialog));
 	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);
 	gtk_widget_show (button);

 	button = gtk_button_new_with_label ("Cancel");
 	gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
  		GTK_SIGNAL_FUNC(GtkPreferences::OnCancel), GTK_OBJECT (mDialog));
 	gtk_box_pack_start (GTK_BOX (hbox), button, TRUE, TRUE, 0);
 	gtk_widget_show (button);

	mOK = FALSE;  //gets set to TRUE in OnOK()

	gtk_widget_show(mDialog);
	gtk_grab_add(mDialog); 
	return mOK; 
}

void GtkPreferences::OnCancel(GtkWidget *widget, GtkFileSelection *fs)
{
	gtk_grab_remove(mDialog);
 	gtk_widget_destroy(mDialog);
 	mDialog = NULL;
}
/*
** OnOK - If the user clicks ok, save everything with the persisting object passed to Edit()
*/
void GtkPreferences::OnOK(GtkWidget *widget, GtkWidget *entry)
{
	mOK = TRUE;

	//@REX should check the return values on these and bail if they're false
	GetInstance()->ReadDepthPage();
	GetInstance()->ReadPostgresPage();
	GetInstance()->ReadGPSPage();
	
	//mPersistor->Write(*GetInstance());
	GetInstance()->Save();

	gtk_grab_remove(mDialog);
 	gtk_widget_destroy(mDialog);
 	mDialog = NULL;

 	//force a refresh 
 	//@REX Maybe check if any values have actually changed?
 	AppFactory::GetInstance()->GetApp()->GetEngine()->Erase();
}

/*
** Depth page contains an edit control for the maximum and minimum depth.
**	FIXME - Needs a drop-down list of available color algorithms and 
**	a preview color bar.
*/
void GtkPreferences::AddDepthPage()
{
	GtkWidget *frame = gtk_frame_new ("Depth options");
	gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
	gtk_widget_show (frame);
 
	GtkWidget *label = gtk_label_new ("Display");
	gtk_notebook_append_page (GTK_NOTEBOOK (mNotebook), frame, label);

	GtkWidget *vbox = gtk_vbox_new (TRUE, 0);
	gtk_container_add (GTK_CONTAINER (frame), vbox);
	//gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
	gtk_widget_show (vbox);

	GTKPREF_ADD_ENTRY_WIDGET(MaxDepth,float,2,vbox,Maximum Depth:,%.2f)
	GTKPREF_ADD_ENTRY_WIDGET(MinDepth,float,3,vbox,Minimum Depth:,%.2f)
	GTKPREF_ADD_CHECK_WIDGET(AutoColor,bool,1,vbox,Auto Color,GtkPreferences::OnToggleAutoColor)

	//little help
	label = gtk_label_new ("\tThe maximum depth is the depth that receives the color\n at the far left of the color bar at the bottom of the\n main window.  Depths greater than the maximum depth are \ndisplayed as black. The minimum depth is the depth\n that receives the color at the far right of the color bar.\n Depths less than the minimum depth are shown as red.\n\tChoosing autocolor will select optimum values.");
	//gtk_widget_set_usize (label, 400, 200);
	gtk_box_pack_start (GTK_BOX (vbox), label, TRUE, TRUE, 0);
	gtk_widget_show (label);

}

bool GtkPreferences::ReadDepthPage()
{
	//@REX check that depths are positive numeric
	char *max = gtk_entry_get_text (GTK_ENTRY (mMaxDepthWidget));
	mMaxDepth = atof(max);
	char *min = gtk_entry_get_text (GTK_ENTRY (mMinDepthWidget));
	mMinDepth = atof(min);

	return true;
}

/*
** OnToggleAutoColor - The user is turning the auto-color on or off.
** 	AutoColor is where the maximum depth is automatically set to be the max depth
**  currently visible and the min is the minimum currently visible.
*/
gint GtkPreferences::OnToggleAutoColor(GtkWidget *widget, GdkEvent *event, gpointer data)
{

	if (GTK_TOGGLE_BUTTON (widget)->active)
	{
		mAutoColor=true;
		gtk_widget_set_sensitive(mMaxDepthHbox,FALSE);
		gtk_widget_set_sensitive(mMinDepthHbox,FALSE);
	} else
	{
		mAutoColor=false;
		gtk_widget_set_sensitive(mMaxDepthHbox,TRUE);
		gtk_widget_set_sensitive(mMinDepthHbox,TRUE);
	}

	return (gint) 1;
}

SamplePoint & GtkPreferences::GetUpperLeft() const
{
	if (mUpperLeft)
		return *mUpperLeft;
	else
		throw "Upper Left Value not set!";
		//return new SamplePoint;  //@REX memleak
}

void GtkPreferences::SetUpperLeft(const SamplePoint &sp)
{
	if (mUpperLeft)
		delete mUpperLeft;
	mUpperLeft = new SamplePoint(sp);
}

/*
** AddAdvancedPage - Advanced settings -- more debugging stuff than anything.
*/
void GtkPreferences::AddAdvancedPage()
{
	GtkWidget *frame = gtk_frame_new ("Advanced options");
	gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
	gtk_widget_show (frame);
 
	GtkWidget *label = gtk_label_new ("Advanced");
	gtk_notebook_append_page (GTK_NOTEBOOK (mNotebook), frame, label);

	GtkWidget *vbox = gtk_vbox_new (TRUE, 0);
	gtk_container_add (GTK_CONTAINER (frame), vbox);
	//gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
	gtk_widget_show (vbox);

	//GTKPREF_ADD_ENTRY_WIDGET(SmoothingFactor,int,1,vbox,Smoothing Factor:,%d)
	//GTKPREF_ADD_ENTRY_WIDGET(NumberSines,int,2,vbox,Number Sine Curves:,%d)
	//GTKPREF_ADD_ENTRY_WIDGET(SineAmplitude,int,3,vbox,Sine Amplitude:,%d)
	//GTKPREF_ADD_ENTRY_WIDGET(SineFrequency,int,4,vbox,Sine Frequency:,%d)

	//little help
	label = gtk_label_new ("\tIncreasing the smoothing factor will increase rendering time and give the surface a smoother feel.");
	//gtk_widget_set_usize (label, 400, 200);
	gtk_box_pack_start (GTK_BOX (vbox), label, TRUE, TRUE, 0);
	gtk_widget_show (label);

}

/*
** AddPostgresPage - Database settings.
*/
void GtkPreferences::AddPostgresPage()
{
	GtkWidget *frame = gtk_frame_new ("Database options");
	gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
	gtk_widget_show (frame);
 
	GtkWidget *label = gtk_label_new ("Database");
	gtk_notebook_append_page (GTK_NOTEBOOK (mNotebook), frame, label);

	GtkWidget *vbox = gtk_vbox_new (TRUE, 0);
	gtk_container_add (GTK_CONTAINER (frame), vbox);
	//gtk_container_set_border_width (GTK_CONTAINER (vbox), 5);
	gtk_widget_show (vbox);

	GTKPREF_ADD_ENTRY_WIDGET(pgHost,char *,1,vbox,Host:,%s)
	GTKPREF_ADD_ENTRY_WIDGET(pgPort,char *,2,vbox,Port:,%s)
	GTKPREF_ADD_ENTRY_WIDGET(pgOptions,char *,3,vbox,Options:,%s)
	GTKPREF_ADD_ENTRY_WIDGET(pgDBName,char *,4,vbox,Database:,%s)
	GTKPREF_ADD_ENTRY_WIDGET(pgUser,char *,5,vbox,Userid:,%s)
	GTKPREF_ADD_ENTRY_WIDGET(pgPassword,char *,6,vbox,Password:,%s)

	//little help
	label = gtk_label_new ("\tConfigure your PostgreSQL database according\n to the installation instructions.");
	//gtk_widget_set_usize (label, 400, 200);
	gtk_box_pack_start (GTK_BOX (vbox), label, TRUE, TRUE, 0);
	gtk_widget_show (label);

}

bool GtkPreferences::ReadPostgresPage()
{
	//@REX do some error checking and return some falses
	if (mpgHost) delete mpgHost;
	if (mpgPort) delete mpgPort;
	if (mpgOptions) delete mpgOptions;
	if (mpgDBName) delete mpgDBName;
	if (mpgUser) delete mpgUser;
	if (mpgPassword) delete mpgPassword;
	
	mpgHost = new char[::strlen(gtk_entry_get_text (GTK_ENTRY(mpgHostWidget)))];
	::strcpy(mpgHost,gtk_entry_get_text (GTK_ENTRY(mpgHostWidget)));

	mpgPort = new char[::strlen(gtk_entry_get_text (GTK_ENTRY(mpgPortWidget)))];
	::strcpy(mpgPort, gtk_entry_get_text (GTK_ENTRY(mpgPortWidget)));

	mpgOptions = new char[::strlen(gtk_entry_get_text (GTK_ENTRY(mpgOptionsWidget)))];
	::strcpy(mpgOptions, gtk_entry_get_text (GTK_ENTRY(mpgOptionsWidget)));

	mpgDBName = new char[::strlen(gtk_entry_get_text (GTK_ENTRY(mpgDBNameWidget)))];
	::strcpy(mpgDBName, gtk_entry_get_text (GTK_ENTRY(mpgDBNameWidget)));

	mpgUser = new char[::strlen(gtk_entry_get_text (GTK_ENTRY(mpgUserWidget)))];
	::strcpy(mpgUser, gtk_entry_get_text (GTK_ENTRY(mpgUserWidget)));

	mpgPassword = new char[::strlen(gtk_entry_get_text (GTK_ENTRY(mpgPasswordWidget)))];
	::strcpy(mpgPassword, gtk_entry_get_text (GTK_ENTRY(mpgPasswordWidget)));

	return true;
}

/*
** AddGPSPage - GPS settings.
*/
void GtkPreferences::AddGPSPage()
{
	GtkWidget *frame = gtk_frame_new ("GPS options");
	gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
	gtk_widget_show (frame);
 
	GtkWidget *label = gtk_label_new ("GPS");
	gtk_notebook_append_page (GTK_NOTEBOOK (mNotebook), frame, label);

	GtkWidget *vbox = gtk_vbox_new (TRUE, 0);
	gtk_container_add (GTK_CONTAINER (frame), vbox);
	gtk_widget_show (vbox);

	GTKPREF_ADD_ENTRY_WIDGET(gpsHost,char *,1,vbox,Host:,%s)
	GTKPREF_ADD_ENTRY_WIDGET(gpsPort,char *,2,vbox,Port:,%s)
	GTKPREF_ADD_ENTRY_WIDGET(gpsSampleRate,int,3,vbox,Sample Rate:,%d)

	//little help
	label = gtk_label_new ("\tFor GPS host and port see gpsd installation instructions. Sample Rate\nis the number of seconds we wait before recording the\ncurrent position in the database.");
	//gtk_widget_set_usize (label, 400, 200);
	gtk_box_pack_start (GTK_BOX (vbox), label, TRUE, TRUE, 0);
	gtk_widget_show (label);

}

bool GtkPreferences::ReadGPSPage()
{
	//@REX do some error checking and return some falses
	if (mgpsHost) delete mgpsHost;
	if (mgpsPort) delete mgpsPort;
	
	mgpsHost = new char[::strlen(gtk_entry_get_text (GTK_ENTRY(mgpsHostWidget)))];
	::strcpy(mgpsHost,gtk_entry_get_text (GTK_ENTRY(mgpsHostWidget)));

	mgpsPort = new char[::strlen(gtk_entry_get_text (GTK_ENTRY(mgpsPortWidget)))];
	::strcpy(mgpsPort, gtk_entry_get_text (GTK_ENTRY(mgpsPortWidget)));

	char *samprate = gtk_entry_get_text (GTK_ENTRY (mgpsSampleRateWidget));
	mgpsSampleRate = atoi(samprate);


	return true;
}