/*
** gtkMenu.cpp
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** DPT 12.15.98
*/

#include "gtkMenu.h"
#include <gtk/gtk.h>
#include <strings.h>
#include "appFactory.h"
#include "gtkApp.h"
#include "gtkToolbar.h"
#include "preferences.h"
#include "engine.h"
#include "exception.h"
#include "iostream.h"
#include "commandPan.h"
#include "commandZoom.h"
#include "commandApplication.h"
#include "commandDraw.h"
#include "commandGrid.h"
#include "commandAutoCenter.h"
#include "commandTrack.h"
#include "commandCursor.h"
#include "gtkCommandHandler.h"
#include "eventSource.h"

/* This is the GtkItemFactoryEntry structure used to generate new menus.
        Item 1: The menu path. The letter after the underscore indicates an
                accelerator key once the menu is open.
        Item 2: The accelerator key for the entry
        Item 3: The callback function.
        Item 4: The callback action.  This changes the parameters with
                which the function is called.  The default is 0.
        Item 5: The item type, used to define what kind of an item it is.
                Here are the possible values:

                NULL               -> "<Item>"
                ""                 -> "<Item>"
                "<Title>"          -> create a title item
                "<Item>"           -> create a simple item
                "<CheckItem>"      -> create a check item
                "<ToggleItem>"     -> create a toggle item
                "<RadioItem>"      -> create a radio item
                <path>             -> path of a radio item to link against
                "<Separator>"      -> create a separator
                "<Branch>"         -> create an item to hold sub items (optional)
                "<LastBranch>"     -> create a right justified branch 
     */
    
static GtkItemFactoryEntry menu_items[] = {
	{ "/_File",			(gchar *)NULL,		(GtkItemFactoryCallback) NULL, 0,					"<Branch>" },
	{ "/File/sep1",		(gchar *)  NULL,	(GtkItemFactoryCallback)NULL, 0,					"<Separator>" },
	{ "/File/Quit",		"<control>Q",		(GtkItemFactoryCallback)GtkCommandHandler::Handle, (guint)((ApplicationCommand *)Command::Create(Command::APPLICATION))->SetAction(ApplicationCommand::EXIT),	(gchar *)NULL },
	{ "/_Edit",			(gchar *)NULL,		(GtkItemFactoryCallback)NULL, 0,					"<Branch>" },
	{ "/Edit/Preferences","<control>P",		(GtkItemFactoryCallback)GtkCommandHandler::Handle, (guint)((ApplicationCommand *)Command::Create(Command::APPLICATION))->SetAction(ApplicationCommand::PREFERENCES),(gchar *)NULL },
	{ "/_View",			(gchar *)NULL,		(GtkItemFactoryCallback)NULL, 0,					"<Branch>" },
	{ "/View/_Draw",	"<control>D",		(GtkItemFactoryCallback)GtkCommandHandler::Handle,	(guint)Command::Create(Command::DRAW),	"<CheckItem>" },
	{ "/View/Zoom In",	(gchar *)NULL,		(GtkItemFactoryCallback)GtkCommandHandler::Handle,	(guint)((ZoomCommand *)Command::Create(Command::ZOOM))->SetStepSize(.75),(gchar *)NULL },
	{ "/View/Zoom Out",	(gchar *)NULL,		(GtkItemFactoryCallback)GtkCommandHandler::Handle,	(guint)((ZoomCommand *)Command::Create(Command::ZOOM))->SetStepSize(.75),		(gchar *)NULL },
	{ "/View/Pan",		(gchar *)NULL,		(GtkItemFactoryCallback)NULL, 0,					"<Branch>" },
	{ "/View/Pan/North","<control>N",		(GtkItemFactoryCallback)GtkCommandHandler::Handle,	(guint)((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::NORTH), (gchar *)NULL },
	{ "/View/Pan/South","<control>S",		(GtkItemFactoryCallback)GtkCommandHandler::Handle,	(guint)((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::SOUTH),	(gchar *)NULL },
	{ "/View/Pan/East",	"<control>E",		(GtkItemFactoryCallback)GtkCommandHandler::Handle,	(guint)((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::EAST),		(gchar *)NULL },
	{ "/View/Pan/West",	"<control>W",		(GtkItemFactoryCallback)GtkCommandHandler::Handle,	(guint)((PanCommand *)Command::Create(Command::PAN))->SetDirection(Panner::WEST),		(gchar *)NULL },
	{ "/View/Cursor",		(gchar *)NULL,		(GtkItemFactoryCallback)NULL, 0,					"<Branch>" },
	{ "/View/Cursor/None",(gchar *)NULL,		(GtkItemFactoryCallback)GtkCommandHandler::Handle,	(guint)((CursorCommand *)Command::Create(Command::CURSOR))->SetType(CursorCommand::NONE), (gchar *)NULL },
	{ "/View/Cursor/Crosshair",(gchar *)NULL,		(GtkItemFactoryCallback)GtkCommandHandler::Handle,	(guint)((CursorCommand *)Command::Create(Command::CURSOR))->SetType(CursorCommand::CROSSHAIR), (gchar *)NULL },
	{ "/View/Cursor/Draw Track",(gchar *)NULL,		(GtkItemFactoryCallback)GtkCommandHandler::Handle,	(guint)((CursorCommand *)Command::Create(Command::CURSOR))->SetType(CursorCommand::DRAWTRACK), (gchar *)NULL },
	{ "/View/sep1",		(gchar *)  NULL,	(GtkItemFactoryCallback)NULL, 0,					"<Separator>" },
	{ "/View/Grid",		(gchar *)  NULL,	(GtkItemFactoryCallback)GtkCommandHandler::Handle,	(guint)Command::Create(Command::GRID),	"<CheckItem>" },
	{ "/View/Auto Center",(gchar *)NULL,	(GtkItemFactoryCallback)GtkCommandHandler::Handle,	(guint)Command::Create(Command::AUTOCENTER),	"<CheckItem>" },
	{ "/View/Tracks",	(gchar *)NULL,		(GtkItemFactoryCallback)GtkCommandHandler::Handle,	(guint)Command::Create(Command::TRACK),	"<CheckItem>" },
	{ "/_Action",		(gchar *)NULL,		(GtkItemFactoryCallback)NULL, 0,					"<Branch>" },
	{ "/Action/Net",	(gchar *)NULL,		(GtkItemFactoryCallback)NULL, 0,					"<Branch>" },
	{ "/Action/Net/Add",(gchar *)NULL,		(GtkItemFactoryCallback)GtkCommandHandler::Handle, (guint)((ApplicationCommand *)Command::Create(Command::APPLICATION))->SetAction(ApplicationCommand::ADD_NET),(gchar *)NULL },
	{ "/Action/Net/Show All",(gchar *)NULL,	(GtkItemFactoryCallback)GtkCommandHandler::Handle, (guint)((ApplicationCommand *)Command::Create(Command::APPLICATION))->SetAction(ApplicationCommand::SHOW_NETS),(gchar *)NULL },
	{ "/Action/Crew",	(gchar *)NULL,		(GtkItemFactoryCallback)NULL, 0,					"<Branch>" },
	{ "/Action/Crew/Add",(gchar *)NULL,		(GtkItemFactoryCallback)GtkCommandHandler::Handle, (guint)((ApplicationCommand *)Command::Create(Command::APPLICATION))->SetAction(ApplicationCommand::ADD_CREW),(gchar *)NULL },
	{ "/Action/Crew/Show All",(gchar *)NULL,(GtkItemFactoryCallback)GtkCommandHandler::Handle, (guint)((ApplicationCommand *)Command::Create(Command::APPLICATION))->SetAction(ApplicationCommand::SHOW_CREW),(gchar *)NULL },
	{ "/_Help",			(gchar *)NULL,		(GtkItemFactoryCallback)NULL, 0,					"<LastBranch>" },
	{ "/_Help/About",	(gchar *)NULL,		(GtkItemFactoryCallback)GtkCommandHandler::Handle,  (guint)((ApplicationCommand *)Command::Create(Command::APPLICATION))->SetAction(ApplicationCommand::ABOUT),(gchar *)NULL },
};


BGtkMenu::BGtkMenu()
{
	mMenubar = (GtkWidget *)NULL;
}

BGtkMenu::~BGtkMenu()
{
	//do we delete widget pointers when we're done with them?
	if (mMenubar)
		delete mMenubar;
}

bool BGtkMenu::Initialize()
{

	GtkApp *app = ((GtkApp *)AppFactory::GetInstance()->GetApp());
	GtkWidget  *window = app->GetMainFrame();
	GtkAccelGroup *accel_group;
	gint nmenu_items = sizeof (menu_items) / sizeof (menu_items[0]);

	accel_group = gtk_accel_group_new ();

	/* This function initializes the item factory.
		Param 1: The type of menu - can be GTK_TYPE_MENU_BAR, GTK_TYPE_MENU,
			or GTK_TYPE_OPTION_MENU.
		Param 2: The path of the menu.
		Param 3: A pointer to a gtk_accel_group.  The item factory sets up
			the accelerator table while generating menus.
	*/

	mItemFactory = gtk_item_factory_new (GTK_TYPE_MENU_BAR, "<main>", 
                                            accel_group);
	
	/* This function generates the menu items. Pass the item factory,
	the number of items in the array, the array itself, and any
	callback data for the the menu items. */
	gtk_item_factory_create_items (mItemFactory, nmenu_items, menu_items, NULL);

	/* Attach the new accelerator group to the window. */
	gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);

	//tie our Notify function into the Event notification pumpers
	((EventSourceCommand *)(Command::Create(Command::DRAW)))->GetEventSource().Register((EventSubscriber *)this);
	((EventSourceCommand *)(Command::Create(Command::GRID)))->GetEventSource().Register((EventSubscriber *)this);
	((EventSourceCommand *)(Command::Create(Command::AUTOCENTER)))->GetEventSource().Register((EventSubscriber *)this);
	((EventSourceCommand *)(Command::Create(Command::TRACK)))->GetEventSource().Register((EventSubscriber *)this);
   /* Finally, set the actual menu bar created by the item factory. */ 
	mMenubar = gtk_item_factory_get_widget (mItemFactory, "<main>");
	gtk_widget_show(mMenubar);

}


/*
** Notify - This is our event notification handler. Check which menu item needs to
**	be synched up (with the toolbar or whatever) and do it.
*/
void BGtkMenu::Notify(EventSourceCommand *es)
{
	GtkApp *app = ((GtkApp*)AppFactory::GetInstance()->GetApp());
	if ((DrawCommand *)es == (Command::Create(Command::DRAW)))
	{
		GtkWidget *item = gtk_item_factory_get_item (mItemFactory, "/View/Draw");
		g_return_if_fail (GTK_IS_CHECK_MENU_ITEM (item));
		gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (item), app->GetPreferences()->GetDraw());
	}
	else if ((GridCommand *)es == (Command::Create(Command::GRID)))
	{
		GtkWidget *item = gtk_item_factory_get_item (mItemFactory, "/View/Grid");
		g_return_if_fail (GTK_IS_CHECK_MENU_ITEM (item));
		gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (item), app->GetPreferences()->GetShowGrid());
	}
	else if ((AutoCenterCommand *)es == (Command::Create(Command::AUTOCENTER)))
	{
		GtkWidget *item = gtk_item_factory_get_item (mItemFactory, "/View/Auto Center");
		g_return_if_fail (GTK_IS_CHECK_MENU_ITEM (item));
		gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (item), app->GetPreferences()->GetAutoCenter());
	}
	else if ((TrackCommand *)es == (Command::Create(Command::TRACK)))
	{
		GtkWidget *item = gtk_item_factory_get_item (mItemFactory, "/View/Tracks");
		g_return_if_fail (GTK_IS_CHECK_MENU_ITEM (item));
		gtk_check_menu_item_set_active (GTK_CHECK_MENU_ITEM (item), app->GetPreferences()->GetTrack());
	}
	else
		throw new ContourException("Received notification of unknown source.");
}
