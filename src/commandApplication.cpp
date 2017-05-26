/*
** commandApplication.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jan 27 20:23:58 EST 2001
*/

#include "commandApplication.h"
#include "exception.h"
#include "appFactory.h"
#include "preferences.h"
#include "gtkSplash.h"
#include "gecko.h"
#include <stdio.h>
#include <iostream.h>


ApplicationCommand::ApplicationCommand()
{
	mAction = (Action)-1;
}

ApplicationCommand::~ApplicationCommand()
{

}

void ApplicationCommand::Execute()
{
	if (mAction == -1)
		throw new ContourException("You must call SetAction() before executing this command");

	if (mAction == EXIT)
		AppFactory::GetInstance()->GetApp()->Stop();
	else if (mAction == PREFERENCES)
		AppFactory::GetInstance()->GetApp()->GetPreferences()->Edit();
	else if (mAction == ABOUT)
		GTKSplash::GetInstance()->Show(true);
	else if (mAction == ADD_NET)
	{
		Gecko *gecko = Gecko::GetInstance();
		char URL[256];
		sprintf(URL,"%s%s",CONTOUR_DOMAIN_AND_PORT,"/nets/net.php?action=add");
		gecko->Show(URL);
	}
	else if (mAction == SHOW_NETS)
	{
		Gecko *gecko = Gecko::GetInstance();
		char URL[256];
		sprintf(URL,"%s%s",CONTOUR_DOMAIN_AND_PORT,"/nets/net.php?action=show_all");
		gecko->Show(URL);
	}
	else if (mAction == ADD_CREW)
	{
		Gecko *gecko = Gecko::GetInstance();
		char URL[256];
		sprintf(URL,"%s%s",CONTOUR_DOMAIN_AND_PORT,"/crew/crew.php?action=add");
		gecko->Show(URL);
		//gecko->Show("http://sourceforge.net");
	}
	else if (mAction == SHOW_CREW)
	{
		Gecko *gecko = Gecko::GetInstance();
		char URL[256];
		sprintf(URL,"%s%s",CONTOUR_DOMAIN_AND_PORT,"/crew/crew.php?action=show_all");
		gecko->Show(URL);
	}
	else
		throw new ContourException("Unrecognized Application Command");
}

/*
** SetAction - We have this return the this pointer so it can be used in the
**	gtkMenuFactory array thingy (see gtkMenu.cpp).
*/
ApplicationCommand * ApplicationCommand::SetAction(Action action)
{
	mAction = action;
	return this;
}

