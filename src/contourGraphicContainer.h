/*
** contourGraphicContainer.h - Facade class for things like lat long lines, cursors
**	and other things drawn with an XOR brush over the canvas. Instantiated in the canvas.
**
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Jul  1 22:28:41 EDT 2001
*/

#ifndef _CONTOURGRAPHICCONTAINER_H_
#define _CONTOURGRAPHICCONTAINER_H_

#include <vector.h>
#include "contourGraphic.h"
#include "bathApp.h"

typedef vector<ContourGraphic *> ContourGraphicVector;

class ContourGraphicContainer : ContourGraphic
{
public:
	ContourGraphicContainer();
	~ContourGraphicContainer() {}
	//routines for management of sub-class instances
	bool DrawAll();
	bool ClearAll();
	void Register(ContourGraphic *subclassInstance);
	void Unregister(ContourGraphic *subclassInstance);
	void SetViewPortLocal();
protected:

	//stub 'em just to get access to other sub-classes protected members
	virtual void DrawPartial(int x1, int x2, int y1, int y2) {ASSERT(false);}
	virtual void DrawText() {ASSERT(false);}
	virtual void DrawTextPartial(int x1, int x2, int y1, int y2) {ASSERT(false);}
	virtual void Draw() {ASSERT(false);}
	virtual void Clear() {ASSERT(false);}

private:
	ContourGraphicVector *mSubClassInstances;
	bool mClear;
	
};


#endif

