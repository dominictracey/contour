/*
** Main.cpp - Main deal.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** DPT 12.12.98 
*/

#include "appFactory.h"
#include "bathApp.h"

int main(int argc, char **argv)
{
	BathApp * theApp = AppFactory::GetInstance()->GetApp(argc, argv);
	theApp->Run();

	return 0;
}
	
