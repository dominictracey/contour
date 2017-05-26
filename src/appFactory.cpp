/*
** appFactory.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
** DPT 12.12.98
*/

#include "appFactory.h"
#include "gtkApp.h"
#include <stdio.h>

//statics
AppFactory * AppFactory::mInstance = NULL;

AppFactory::AppFactory()
{
	mInstance = NULL;
	mCurrentApp = NULL;
}

AppFactory::~AppFactory()
{

}

AppFactory * AppFactory::GetInstance()
{
	if (!mInstance)
		mInstance = new AppFactory();

	return mInstance;
}

/*
** GetApp - As we get more application frameworks, we may
**	wish to return different ones here.
*/
BathApp * AppFactory::GetApp(int argc, char **argv)
{
	//all we do is gtk...
	mCurrentApp = new GtkApp;

	mCurrentApp->Initialize(argc,argv);
	return mCurrentApp;	
}

/*
** GetApp - Call this without params if you need to get ahold
**	of the App object.
*/
BathApp * AppFactory::GetApp()
{
	return(mCurrentApp);
}
