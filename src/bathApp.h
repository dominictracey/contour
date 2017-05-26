/*
** bathApp.h - Abstract base class for bathyscope applications.
**	As of 12.12.98 only GNOME/GTK+ is supported.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.12.98 DPT
*/

#ifndef _BATHAPP_H_
#define _BATHAPP_H_

class ContourCanvas;
class ContourPreferences;
class ContourEngine;
class ContourExtractor;
class LatLongLine;
class Position;
class ContourNavTool;

class BathApp
{
protected:
	BathApp() {};
	virtual ~BathApp() {};

public:
	virtual bool Initialize(int argc, char **argv) = 0;

	// These are the components of the application. Our subclasses instantiate
	// concrete subclasses of these pure abstract application interfaces.
	virtual ContourCanvas *GetCanvas() = 0;
	virtual ContourPreferences *GetPreferences() = 0;
	virtual ContourEngine *GetEngine() = 0;
	virtual ContourExtractor *GetExtractor() = 0;
	virtual	LatLongLine *GetLatLongLine(Position &pos, bool lat) = 0;
	
	// and some simple signals to handle
	virtual bool Run() = 0;
	virtual void Stop() = 0;

	//GPS interface
	virtual void AttachGPS(int handle) = 0;
	virtual void DetachGPS(int handle) = 0;
	virtual void UpdateGPSDisplay() = 0;
};

//helper debug util
#ifdef _DEBUG 
#define ASSERT(test) \
	if (!( test ) ) cout << "ASSERTION ( " << #test << " ) failed in " <<  __FILE__ << " at line " <<  __LINE__ << endl;
#else 
#define ASSERT(test) ; 
#endif

#endif
	
