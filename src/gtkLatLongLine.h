/*
** gtkLatLongLine.h - Toolkit specific implementation of the latitude and longitude lines.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYIN for the full GPL license.
**
** Created: Mon Jan  1 10:56:00 EST 2001
*/

#ifndef _GTKLATLONGLINE_H_
#define _GTKLATLONGLINE_H_

#include "latLongLine.h"
#include <gtk/gtk.h>

class GtkLatLongLine : public LatLongLine
{
public:
	GtkLatLongLine(const Position &p, bool lat) : LatLongLine(p,lat) {}
protected:
	void Draw();
	virtual void DrawPartial(int x1, int x2, int y1, int y2);
	virtual void DrawText();
	virtual void DrawTextPartial(int x1, int x2, int y1, int y2);
	void Clear();
	void Dump();
	void SetViewPortLocal();
//	GtkLatLongLine() {};  //use the other one
	void CreateGC();
	static GdkGC *mGC;
	static GdkFont *mFont;
};

#endif

