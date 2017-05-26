/*
** menu.cpp - Provides all the call backs for the various subclass
**	implementations. In other words. The meat is here in the base
**	class and the tool-kit specific subclasses just do the message
**	handling.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.15.98
*/

#include "menu.h"
#include "appFactory.h"
#include "bathApp.h"
#include "engine.h"
#include "panner.h"
#include "preferences.h"
#include "toolbar.h"
#include <stdio.h>

ContourMenu::ContourMenu()
{

}

ContourMenu::~ContourMenu()
{

}

void ContourMenu::OnFileSave()
{
	printf("save\n");
}

void ContourMenu::OnFileQuit()
{
	AppFactory::GetInstance()->GetApp()->Stop();
} 

void ContourMenu::OnFileRender(bool active)
{
	BathApp *app = AppFactory::GetInstance()->GetApp();
	app->GetPreferences()->SetDraw(active);
	AppFactory::GetInstance()->GetApp()->GetEngine()->Erase();
//	app->GetEngine()->Render();
	app->GetToolbar()->SetDraw(active);
	
}

void ContourMenu::OnPanNorth()
{
	Panner panner(NORTH);
	panner.BeginPan();
	panner.Pan();
	panner.EndPan();
}

void ContourMenu::OnPanSouth()
{
	Panner panner(SOUTH);
	panner.BeginPan();
	panner.Pan();
	panner.EndPan();
}

void ContourMenu::OnPanEast()
{
	Panner panner(EAST);
	panner.BeginPan();
	panner.Pan();
	panner.EndPan();
}

void ContourMenu::OnPanWest()
{
	Panner panner(WEST);
	panner.BeginPan();
	panner.Pan();
	panner.EndPan();
}

/*
** OnEditPreferences - Pop up our preferences dialog. If the user clicks ok on it the
**	dialog will call OnEditPreferencesOK. It seems strange that you cannot make
**	a truly modal dialog box that you can call DoModal() on and wait for it to return
**	so you can check its return value. GTK seems to fork a separate thread to handle
**	other windows? Thus we need to send in the object capable of saving the data in
**	case the user clicks "OK"
*/
void ContourMenu::OnEditPreferences()
{
	AppFactory::GetInstance()->GetApp()->GetPreferences()->Edit();
}

void ContourMenu::OnZoomIn()
{
	AppFactory::GetInstance()->GetApp()->GetEngine()->Zoom(.75);
}

void ContourMenu::OnZoomOut()
{
	AppFactory::GetInstance()->GetApp()->GetEngine()->Zoom(1.25);
}

void ContourMenu::OnGrid(bool active)
{
	BathApp *app = AppFactory::GetInstance()->GetApp();
	app->GetPreferences()->SetShowGrid(active);
	app->GetEngine()->Render();
	app->GetToolbar()->SetGrid(active);
}

void ContourMenu::OnDraw(bool active)
{
	BathApp *app = AppFactory::GetInstance()->GetApp();
	app->GetPreferences()->SetDraw(active);
	app->GetEngine()->Erase();
	app->GetToolbar()->SetDraw(active);
}