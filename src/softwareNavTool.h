/*
** softwareNavTool.h - This is an on-screen nav-tool rather than a hardware implementation
**	in an embedded system. Pure abstract interface.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Wed Jan 10 21:29:08 EST 2001
*/

#ifndef _SOFTWARENAVTOOL_H_
#define _SOFTWARENAVTOOL_H_

#include "navTool.h"

class SoftwareNavTool : public ContourNavTool
{
public:
	~SoftwareNavTool() {}
	virtual void Show() = 0;
	virtual void Hide() = 0;
};

#endif

