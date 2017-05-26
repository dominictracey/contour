/*
** toolbar.h - abstract base class for toolkit-specific subclasses
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.15.98 DPT
*/

#ifndef _TOOLBAR_H_
#define _TOOLBAR_H_

class ContourToolbar
{
public:
        virtual ~ContourToolbar() {};
        virtual bool Initialize() = 0;
        virtual void SetGrid(bool state)=0;
        virtual void SetDraw(bool state)=0;
};

#endif

