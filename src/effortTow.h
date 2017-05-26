/*
** effortTow.h - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Mon Nov 26 18:24:02 EST 2001
*/

#ifndef _EFFORTTOW_H_
#define _EFFORTTOW_H_

#include "effort.h"

class Tow : public Effort
{
public:
	Tow();
	~Tow();
	virtual bool AddDecoration();
	

protected:
	int mNetID;
};

#endif

