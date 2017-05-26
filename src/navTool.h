/*
** navTool.h - Defines abstract interface for a navigation interface (panning
**	controls, zoom in/out). This can be hardware (wired into the front of an
**	embedded system) or software (a GUI element). Assumes buttons for panning
**	and zooming.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Wed Jan 10 21:10:06 EST 2001
*/

#ifndef _NAVTOOL_H_
#define _NAVTOOL_H_

#include "position.h"

class ContourNavTool
{
public:
	ContourNavTool();
	~ContourNavTool();

	virtual void BeginPan(int direction);
	virtual void Pan();
	virtual void EndPan();

	virtual void ZoomIn();
	virtual void ZoomOut();
};

#endif

