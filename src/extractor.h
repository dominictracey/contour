/*
** extractor.h - abstract base class for data format-specific subclasses
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.20.98 DPT
*/

#ifndef _EXTRACTOR_H_
#define _EXTRACTOR_H_

#include "position.h"

class ContourExtractor
{
public:
	virtual ~ContourExtractor() {};
	virtual SamplePointVector * GetSamplePoints(SamplePoint * ul,
					    SamplePoint * lr) =0;
	virtual void Initialize(char *filename, SamplePoint & prototype) =0;
	virtual void Release() = 0;
};
	

#endif

