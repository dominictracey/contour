/*
** colorDefaultAlgorithm.h -Defines interface for default algorithm. 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.22.98 DPT
*/
#ifndef COLORDEFAULTALGORITHM_H_
#define COLORDEFAULTALGORITHM_H_

#include "colorAlgorithm.h"

class ContourCanvas;

class ColorDefaultAlgorithm : public ColorAlgorithm 
{
public:
        virtual ~ColorDefaultAlgorithm();
        virtual bool Initialize(ContourCanvas &canvas);
};

#endif

