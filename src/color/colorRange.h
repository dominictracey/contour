/*
** colorRange.h - Defines abstract interface for platform independent
**		color management
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
** DPT 12.13.98
*/

#ifndef _COLORRANGE_H_
#define _COLORRANGE_H_

#include<Xm/Xm.h>

class ColorRange
{
public:
	ColorRange() : mNumColors(0), mMin(0), mMax(0) {};
	virtual ~ColorRange() {};
	virtual BOOL Initialize() = 0;
	
	virtual Pixel GetColor(int depth) = 0;

	virtual BOOL SetMin(int d) {mMin = d; return TRUE;}
	virtual BOOL SetMax(int d) {mMax = d; return TRUE;}

	virtual int GetMin() {return mMin;}
	virtual int GetMax() {return mMax;}
protected:
	int mNumColors;
	int mMin;
	int mMax;
};

#endif 
