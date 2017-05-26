/*
** colorAlgorithm.h - Defines interface for algorithms that can fill in a 
**	pallette sequentially (i.e. an array where the low indices
**	represent deep water or low-lying ground and the high indices 
**	represent shoal water or high ground. 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.18.98 DPT
*/

#ifndef _COLORALGORITHM_H_
#define _COLORALGORITHM_H_

#include "canvas.h"

class SamplePoint;

class ColorAlgorithm 
{
public:
        virtual ~ColorAlgorithm() {};
        virtual bool Initialize(ContourCanvas &canvas)=0;
};

#endif

