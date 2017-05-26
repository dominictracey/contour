/*
** controlpanel.h - abstract base class for toolkit-specific subclasses
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.18.98 DPT
*/

#ifndef _CONTROLPANEL_H_
#define _CONTROLPANEL_H_

class ContourControlPanel
{
public:
        virtual ~ContourControlPanel() {};
        virtual BOOL Initialize() = 0;
};

#endif

