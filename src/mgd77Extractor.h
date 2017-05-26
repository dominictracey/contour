/*
** mgd77Extractor.h - Data extractor for the MGD77 text file format for
**	bathymetric data described in data subdirectory. 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.20.98 DPT
*/

#ifndef _MGD77EXTRACTOR_H_
#define _MGD77EXTRACTOR_H_

#include "extractor.h"

#define BAD_CARTCODE_DEPTH -1

class MGD77Extractor : public ContourExtractor 
{
public:
	MGD77Extractor(); 
    virtual ~MGD77Extractor();
	SamplePointVector * GetSamplePoints(SamplePoint * ul,
					    SamplePoint * lr);					    
	virtual void Release();
//	SamplePointVector * GetSamplePoints(const SamplePoint & ul,
//					    const SamplePoint & lr);					    
protected:
	void Initialize(char *filename, SamplePoint & prototype);
	bool mInitialized;
	SamplePointVector mVector;
};
	

#endif

