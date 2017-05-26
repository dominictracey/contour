/*
** gecko.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Fri Sep 21 22:15:51 EDT 2001
*/
#include <stdlib.h>
#include "gecko.h"
#include <iostream.h>
#include "exception.h"
#include <stdio.h>
// singleton
Gecko * Gecko::mInstance = NULL;

Gecko::Gecko()
{
	mDialog = (GtkWidget *)NULL;
//	mNotebook = (GtkWidget *)NULL;
	mDialog = gtk_window_new (GTK_WINDOW_TOPLEVEL);
 	gtk_window_set_title (GTK_WINDOW (mDialog), "Settings");
 	gtk_container_set_border_width (GTK_CONTAINER (mDialog), 2);
 	//gtk_widget_set_uposition (mDialog, (x_pos+10), (y_pos+10));
 	gtk_widget_set_usize (mDialog, 500, 500);

	mVbox2 = gtk_vbox_new (FALSE, 0);
 	gtk_container_add (GTK_CONTAINER (mDialog), mVbox2);
 	gtk_container_set_border_width (GTK_CONTAINER (mVbox2), 5);
 	gtk_widget_show (mVbox2);
	mURL = new char[3];

	//@REX get from autoconf
	putenv("MOZILLA_FIVE_HOME=/usr/local/mozilla");
	gtk_moz_embed_set_comp_path("/usr/local/mozilla");
	char *home;
	home = getenv("HOME");
	if (!home)
		throw new ContourException("Couldn't get home directory");

	char profile[256];
	sprintf(profile,"%s%s",home,"/.contour");
	cout << profile << endl;
	gtk_moz_embed_set_profile_path (profile, "mozilla");

	Initialize();
	
	// Hook up the little GTK x to the Close function, which just hides the dialog
 	gtk_signal_connect (GTK_OBJECT (mDialog), "destroy",
  	GTK_SIGNAL_FUNC(Close), mDialog);
 	gtk_signal_connect (GTK_OBJECT (mDialog), "delete_event",
  	GTK_SIGNAL_FUNC(Close), mDialog);


}


Gecko::~Gecko()
{
	delete [] mURL;
 //	gtk_widget_destroy(GTK_WIDGET(mMoz));
 	gtk_widget_destroy(GTK_WIDGET(mDialog));
}

/*
** Initialize - Set up the Embedded mozilla GTK widget
*/
bool Gecko::Initialize()
{
	mMoz = GTK_MOZ_EMBED(gtk_moz_embed_new());
	
	//gtk_moz_embed_load_url (GTK_MOZ_EMBED(mMoz), mURL);
	gtk_box_pack_start (GTK_BOX (mVbox2), GTK_WIDGET(mMoz), TRUE, TRUE, 0);
 	gtk_widget_show (GTK_WIDGET(mMoz));

 	gtk_signal_connect_object (GTK_OBJECT (mMoz), "new_window",
  		GTK_SIGNAL_FUNC(this->OnNewWindow), GTK_OBJECT (mDialog));
  		
 	gtk_signal_connect_object (GTK_OBJECT (mMoz), "destroy_browser",
  		GTK_SIGNAL_FUNC(this->OnDestroyBrowser), GTK_OBJECT (this));

 	gtk_signal_connect_object (GTK_OBJECT (mMoz), "open_uri",
  		GTK_SIGNAL_FUNC(this->OnOpenURI), GTK_OBJECT (mDialog));

  	return true;
}

Gecko * Gecko::GetInstance()
{
	if (!mInstance)
		mInstance = new Gecko();
	return mInstance;
}

void Gecko::Close(GtkWidget *widget, GtkFileSelection *fs)
{
	//@REX Put up a blank page
	gtk_grab_remove(GTK_WIDGET(widget));
	gtk_widget_hide(GTK_WIDGET(widget)); //its not really going away!
	//delete (Gecko *)widget;
	//mInstance = NULL;
}

void Gecko::Show(const char *url)
{
	if (!mDialog)
		Initialize();
		
	delete [] mURL;
	mURL = new char[::strlen(url)+1];
	::strcpy(mURL,url);
	gtk_moz_embed_load_url (GTK_MOZ_EMBED(mMoz), mURL);
	gtk_widget_show(mDialog);
	gtk_grab_add(mDialog); 
	
}

/*
** OnNewWindow - @REX this is bad -- the lizard wants another window, not the same one!
*/
void Gecko::OnNewWindow(GtkMozEmbed *moz, GtkMozEmbed **retval, guint chromemask, gpointer data)
{
	cout << "OnNewWindow" << endl;
	Gecko *gecko = GetInstance();
	//Gecko *newGecko = new Gecko;
	gecko->Show("about:blank");
	*retval = gecko->GetMoz();
}

/*
** OnDestroyBrowser - Something weird is going on. The parameters seem to be opposite of
**	what I was expecting them to be...
**
**	@REX - I can't seem to get this to work so don't point it at anything that tosses out
**	a javascript Window.Close() call!
*/
void Gecko::OnDestroyBrowser(GtkWidget *gecko, gpointer moz)
{
	cout << "OnDestroyBrowser! " << ((Gecko *)gecko)->mURL <<  " data is " << moz << endl;

	//gtk_widget_destroy ((GtkWidget *)((Gecko *)moz)->mMoz);
	//((Gecko *)moz)->mMoz = NULL;
	
	gtk_grab_remove(GTK_WIDGET(((Gecko *)gecko)->GetDialog()));
	gtk_widget_hide(GTK_WIDGET(((Gecko *)gecko)->GetDialog())); //its not really going away!
	
	//gtk_widget_destroy ((GtkWidget *)((Gecko *)gecko)->mDialog);
	//((Gecko *)gecko)->mDialog = NULL;
//	delete (Gecko *)gecko;
//	mInstance = NULL;
}

gint Gecko::OnOpenURI(GtkMozEmbed *moz, const char *uri, gpointer data)
{
	// This is our chance to veto a new page. False means go for it.
	cout << "On	OpenURI: " << uri << endl;
	return false;
}

