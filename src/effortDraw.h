/*
** effortDraw.h - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Mon Nov 26 18:24:02 EST 2001
*/

#ifndef _EFFORTDRAW_H_
#define _EFFORTDRAW_H_

#include "effort.h"

class EffortDecoration;

class DrawEffort : public Effort
{
public:
	DrawEffort();
	~DrawEffort();
	virtual bool AddDecoration();
	virtual bool Start();
	virtual bool End();	
	virtual bool Load(int effortid);
protected:
	//Effort::Type mType;
	EffortDecoration *mED;
};

#endif

