/*
** gnomeApp.h - Interface for GNOME/GTK+ app object
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
** DPT 12.12.98
*/

#include "gnomeApp.h"
#include "bath.h"
//#include <config.h>
#include <gnome.h>

BGnomeApp::BGnomeApp()
{

}

BGnomeApp::~BGnomeApp()
{

}

BOOL BGnomeApp::Initialize(int argc, char **argv)
{
	gnome_init ("909090", NULL, argc, argv, NULL, NULL);

        GtkWidget *button;

        mApp = gnome_app_new ("hello", "Hello World Gnomified");
        gtk_widget_realize (mApp);
        gtk_signal_connect (GTK_OBJECT (mApp), "delete_event",
                               GTK_SIGNAL_FUNC (quit_cb), NULL);

        button = gtk_button_new_with_label ("Hello GNOME");
        gtk_signal_connect (GTK_OBJECT (button), "clicked",
                              GTK_SIGNAL_FUNC (hello_cb), NULL);
        gtk_container_border_width (GTK_CONTAINER (button), 60);
        gnome_app_set_contents ( GNOME_APP (mApp), button);

        gtk_widget_show (button);
        gtk_widget_show (mApp);

}
        
BOOL BGnomeApp::Run()
{
	gtk_main();
}
 
        
/*
**
*/
void BGnomeApp::hello_cb(GtkWidget *widget, void *data)
{
	g_print ("Hello GNOME\n");
        gtk_main_quit ();
        return;
}
 
/*
**
*/
void BGnomeApp::quit_cb(GtkWidget *widget, void *data)
{
        gtk_main_quit ();
        return;
}
