/*
** gtkGPSDisplay.h - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Mon Jul  2 00:52:45 EDT 2001
*/
#ifndef _GTKGPSDISPLAY_H_
#define _GTKGPSDISPLAY_H_

#include "contourGraphic.h"
#include <gtk/gtk.h>

class GtkGPSDisplay : public ContourGraphic
{
public:
	GtkGPSDisplay();
	~GtkGPSDisplay();
protected:
	void SetViewPortLocal();
	void DrawPartial(int x1, int x2, int y1, int y2);
	void DrawText();
	void DrawTextPartial(int x1, int x2, int y1, int y2);
	void Draw();
	void Clear();

	GdkGC * mGC;
	GdkFont *mFont;
	char mLatString[25];
	char mLongString[25];
	char mSpeedString[25];
	bool mDrawnGPSInfo;

	char * mLatLabel;
	char * mLongLabel;
	char * mSpeedLabel;

	int mLatPosX;
	int mLonPosX;
	int mSpeedPosX;
	int mLatPosY;
	int mLonPosY;
	int mSpeedPosY;

	bool mClear;
};

#endif

