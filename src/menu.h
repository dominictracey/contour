/*
** menu.h - abstract base class for toolkit-specific subclasses
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.15.98 DPT
*/

#ifndef _MENU_H_
#define _MENU_H_

class ContourMenu
{
public:
	ContourMenu();
	virtual ~ContourMenu();
	virtual bool Initialize() = 0;
	virtual void SetGrid(bool state)=0;  //display a check mark next to it
	static void OnGrid(bool state);
	virtual void SetDraw(bool state)=0;  //display a check mark next to it
	static void OnDraw(bool state);

protected:
	static void OnFileSave();
	static void OnFileQuit();
	static void OnFileRender(bool active);
	static void OnPanNorth();
	static void OnPanSouth();
	static void OnPanEast();
	static void OnPanWest();
	static void OnEditPreferences();
	static void OnZoomIn();
	static void OnZoomOut();
};

#endif


