/*
** appFactory.h - Chooses the right application model to run.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** DPT 12.12.98
*/

#ifndef _APPFACTORY_H_
#define _APPFACTORY_H_

#include "bathApp.h"

class AppFactory
{
friend class AppFactoryDeleter;

protected:
	AppFactory();
	~AppFactory();

public:
	static AppFactory * GetInstance();
	BathApp * GetApp(int argc, char **argv);
	BathApp * GetApp();

private:
	static AppFactory *mInstance;
	BathApp *mCurrentApp;
};

class AppFactoryDeleter
{
public:
	~AppFactoryDeleter() { delete AppFactory::GetInstance(); }

};
#endif	
