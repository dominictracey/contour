/*
** gtkCurrentPositionIndicator.h - This baby is the little dot that shows where the
**	GPS- unit is currently situmerated.
**
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jun 30 21:43:31 EDT 2001
*/

#ifndef _GTKCURRENTPOSITIONINDICATOR_H_
#define _GTKCURRENTPOSITIONINDICATOR_H_

#include "contourGraphic.h"
#include <gtk/gtk.h>

class GtkCurrentPositionIndicator : public ContourGraphic
{
public:
	GtkCurrentPositionIndicator();
	~GtkCurrentPositionIndicator();
protected:
	void SetViewPortLocal();
	void DrawPartial(int x1, int x2, int y1, int y2);
	void DrawText();
	void DrawTextPartial(int x1, int x2, int y1, int y2);
	void Draw();
	void Clear();
	void CreateGC();

	int mx,my;
	bool mClear;
	static GdkGC *mGC;
};

#endif

