/*
** colorbox.h - abstract base class for toolkit-specific subclasses
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.18.98 DPT
*/

#ifndef _COLORBOX_H_
#define _COLORBOX_H_

class ContourColorBox
{
public:
        virtual ~ContourColorBox() {};
        virtual BOOL Initialize() = 0;
};

#endif

