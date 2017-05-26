/*
** colorRange8Default.h - Defines interface for default color manager on 
**	systems with 8-bit color plane capabilities.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
** DPT - 12.12.1998
*/

#ifndef _COLORRANGE8DEFAULT_H_
#define _COLORRANGE8DEFAULT_H_

#include "colorRange.h"

#define NUM_COLORS_8BIT 200

class ColorRange8Default : public ColorRange
{
public:
	ColorRange8Default();
	virtual ~ColorRange8Default();

	virtual BOOL Initialize();
	virtual Pixel GetColor(int depth);

protected:
	Pixel mColors[NUM_COLORS_8BIT];
};

#endif	
