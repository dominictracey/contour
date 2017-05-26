/*
** effortDecoration.h - An on-screen "trackline" representation of a tow, steam, etc.
** 
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001 Aquilon Technology Systems, Inc.
**
** Created: Thu Nov 26 17:46:43 EDT 2001
*/

#ifndef _EFFORTDECORATION_H_
#define _EFFORTDECORATION_H_

#include "effort.h"
#include "gdkCanvas.h"
#include <gtk/gtk.h>
#include "contourGraphic.h"

class EffortDecoration : public ContourGraphic
{
public:
	EffortDecoration(Effort *effort);
	~EffortDecoration();
	virtual void Draw();
	virtual void Clear();
	virtual void DrawPartial(int x1, int x2, int y1, int y2);
	virtual void DrawText();
	virtual void DrawTextPartial(int x1, int x2, int y1, int y2);
protected:
	virtual void SetViewPortLocal();
	EffortDecoration() {}  //use the other one
	Effort * mEffort;
	GDKCanvas * mCanvas;
	GdkGC *mGC;
	//GdkFont *mFont;
	
};


#endif

