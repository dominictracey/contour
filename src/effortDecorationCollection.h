/*
** effortDecorationCollection.h - Allows the canvas to control a set of efforts by 
** 	their type.
** 	
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001 Aquilon Technology Systems, Inc.
**
** Created: Thu Nov 26 17:46:43 EDT 2001
*/

#ifndef _EFFORTDECORATIONCOLLECTION_H_
#define _EFFORTDECORATIONCOLLECTION_H_

#include "effort.h"
#include "contourGraphic.h"
class EffortDecoration;

/*
** EffortDecorationCollection is a grouping of said effort collections, to be used 
** 	by the GDKCanvas::[Add|Remove]Decoration overlay mechanism.
** 	@REX Mightn't we do this with that design pattern?
*/
class EffortDecorationCollection : public ContourGraphic
{
public:
	EffortDecorationCollection();
	~EffortDecorationCollection();

	typedef vector<EffortDecoration *> EffortDecorationVector;

	void AddEffort(EffortDecoration *ed);

	static EffortDecorationCollection * GetEffortDecorationCollectionByType(Effort::Type t);
protected:
	virtual void SetViewPortLocal();
	virtual void Draw();
	virtual void Clear();
	virtual void DrawPartial(int x1, int x2, int y1, int y2);
	virtual void DrawText();
	virtual void DrawTextPartial(int x1, int x2, int y1, int y2);
	
	Effort::Type mType;
	EffortDecorationVector *mDecorations;
	
	//type-specific static instances
	static EffortDecorationCollection * mDrawInstance;
	static EffortDecorationCollection * mTowInstance;
	static EffortDecorationCollection * mSteamInstance;
	static EffortDecorationCollection * mLayInstance;

};


#endif

