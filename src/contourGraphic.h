/*
** ContourGraphic.h - Abstract base class for things like lat long lines, cursors
**	and other things drawn with an XOR brush over the canvas.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat May 26 17:46:43 EDT 2001
*/

#ifndef _CONTOURGRAPHIC_H_
#define _CONTOURGRAPHIC_H_

#include "position.h"

class ContourGraphic
{
public:
	~ContourGraphic() {}
	
	//these are for the subclasses to implement -- 
	// called only by ContourGraphicContainer::Draw/ClearAll()
	virtual void Draw() = 0;
	virtual void Clear() = 0;
	virtual void DrawPartial(int x1, int x2, int y1, int y2) = 0;
	virtual void DrawText() = 0;
	virtual void DrawTextPartial(int x1, int x2, int y1, int y2) = 0;
	static void SetViewPort();
	virtual void SetViewPortLocal() = 0; //called when the viewport changes
	static void GetViewPort(SamplePoint &ul, SamplePoint &size, float &xScale, float &yScale, int &width, int &height)
			{
				ul = mUL;
				size = mSize;
				xScale = mXScale;
				yScale = mYScale;
				width = mWidth;
				height = mHeight;
			}
protected:
	// we keep this stuff in statics to speed things up since everything shares it
	static SamplePoint mUL;
	static float mXScale;
	static float mYScale;
	static SamplePoint mSize;
	static int mWidth;
	static int mHeight;
	static bool mReentrantSetViewPort;
};


#endif

