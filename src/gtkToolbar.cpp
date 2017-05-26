/*
** BGtkToolbar.cpp - GTK+ toolbar implementation. 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** DPT 12.15.98
*/

#include "gtkToolbar.h"
#include "gtkMenu.h"
#include "appFactory.h"
#include "gtkApp.h"
#include "preferences.h"
#include "engine.h"
#include "exception.h"
#include "commandApplication.h"
#include "gtkCommandHandler.h"
#include "eventSource.h"
#include "commandDraw.h"
#include "commandGrid.h"
#include "commandTrack.h"

//toolbar buttons
#include "pixmaps/exit.xpm"
#include "pixmaps/html-image.xpm"
#include "pixmaps/settings.xpm"
#include "pixmaps/render.xpm"
#include "pixmaps/grid.xpm"
#include "pixmaps/html-break.xpm"

BGtkToolbar::BGtkToolbar()
{
}

BGtkToolbar::BGtkToolbar(GtkWidget * window)
{
	mDummyWindow = window;
}

BGtkToolbar::~BGtkToolbar()
{

}

/*
** Initialize - Set up toolbar.
*/
bool BGtkToolbar::Initialize()
{
	GtkWidget *mainframe = mDummyWindow;
	//    ((GtkApp *)AppFactory::GetInstance()->GetApp())->GetTable();
	GtkStyle *style = gtk_widget_get_style(mainframe);
	
	mToolbar = gtk_handle_box_new();
	GtkWidget *tb = gtk_toolbar_new (GTK_ORIENTATION_HORIZONTAL, 
					GTK_TOOLBAR_ICONS);
	gtk_container_add (GTK_CONTAINER(mToolbar),tb);
	//gtk_container_border_width (GTK_CONTAINER (mToolbar), 2);


	AddButton(tb, "Quit","Exit Contour",exit_xpm,
		GTK_SIGNAL_FUNC(GtkCommandHandler::Handle), (gpointer)((ApplicationCommand *)Command::Create(Command::APPLICATION))->SetAction(ApplicationCommand::EXIT), style, mainframe);
	AddToggleButton(tb, "Render","Draw",render_xpm,
		GTK_SIGNAL_FUNC(GtkCommandHandler::Handle), (gpointer)(Command::Create(Command::DRAW)), style, mainframe,&mDrawButton);
	AddButton(tb, "Preferences","Configure user settings",settings_xpm,
		GTK_SIGNAL_FUNC(GtkCommandHandler::Handle), (gpointer)((ApplicationCommand *)Command::Create(Command::APPLICATION))->SetAction(ApplicationCommand::PREFERENCES), style, mainframe);
	AddToggleButton(tb, "Grid","Toggle lat-long grid",grid_xpm,
		GTK_SIGNAL_FUNC(GtkCommandHandler::Handle), (gpointer)(Command::Create(Command::GRID)), style, mainframe,&mGridButton);
	AddToggleButton(tb, "Tracks","Toggle tracklines",html_break_xpm,
		GTK_SIGNAL_FUNC(GtkCommandHandler::Handle), (gpointer)(Command::Create(Command::TRACK)), style, mainframe,&mTrackButton);

	gtk_widget_show(tb);
	gtk_widget_show(mToolbar);

	//set any toggles from the prefs
/*	GtkApp *app = ((GtkApp*)AppFactory::GetInstance()->GetApp());
	cout << "Grid is " << app->GetPreferences()->GetShowGrid() << endl;
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mDrawButton), app->GetPreferences()->GetDraw());
	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mGridButton), app->GetPreferences()->GetShowGrid());
*/
	return true;
}

/*
** AddButton - 
*/
void BGtkToolbar::AddButton(GtkWidget *toolbar, char *text, char *tooltip,
				char **xpm, GtkSignalFunc callback, 
				gpointer data, GtkStyle *style, GtkWidget *mf)
{
	GtkWidget *pixmapwid;
    	GdkBitmap *mask;
    	GdkPixmap *pixmap 
		= gdk_pixmap_create_from_xpm_d(mf->window, &mask, 
						 &style->bg[GTK_STATE_NORMAL],
                                                 (gchar **)xpm );
	pixmapwid = gtk_pixmap_new( pixmap, mask );
	gtk_toolbar_append_item (GTK_TOOLBAR (toolbar), text, tooltip, 
				"What's this for?",pixmapwid,
				(GtkSignalFunc)callback, data);
}

/*
** AddToggleButton - 
*/
void BGtkToolbar::AddToggleButton(GtkWidget *toolbar, char *text, char *tooltip,
				char **xpm, GtkSignalFunc callback, 
				gpointer data, GtkStyle *style, GtkWidget *mf, GtkWidget **memberVar)
{
	GtkWidget *pixmapwid;
    GdkBitmap *mask;
    GdkPixmap *pixmap = gdk_pixmap_create_from_xpm_d(mf->window, &mask, 
						 &style->bg[GTK_STATE_NORMAL], (gchar **)xpm );
	pixmapwid = gtk_pixmap_new( pixmap, mask );
	((EventSourceCommand *)data)->GetEventSource().Register((EventSubscriber *)this);
	*memberVar = 
	gtk_toolbar_append_element (GTK_TOOLBAR (toolbar), GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
				NULL, text, tooltip, 
				"What's this for?",GTK_WIDGET(pixmapwid),
				GTK_SIGNAL_FUNC(callback), data);
}

/*
** Notify - This is our event notification handler. Check which toggle button needs to
**	be synched up (with the menus or whatever) and do it.
*/
void BGtkToolbar::Notify(EventSourceCommand *es)
{
	GtkApp *app = ((GtkApp*)AppFactory::GetInstance()->GetApp());
	if ((DrawCommand *)es == (Command::Create(Command::DRAW)))
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mDrawButton), app->GetPreferences()->GetDraw());
	}
	else if ((GridCommand *)es == (Command::Create(Command::GRID)))
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mGridButton), app->GetPreferences()->GetShowGrid());
	}
	else if ((TrackCommand *)es == (Command::Create(Command::TRACK)))
	{
		gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(mTrackButton), app->GetPreferences()->GetTrack());
	}
	else
		throw new ContourException("Received notification of unknown source.");
}

