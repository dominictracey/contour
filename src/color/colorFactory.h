/*
** colorFactory.h - Defines interface for installation independent
**	color management facilities. Singleton.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.7.98 DPT
*/

#ifndef _COLORFACTORY_H_
#define _COLORFACTORY_H_

class ColorRange;
class ColorDeleter;

class ColorFactory
{
friend class ColorDeleter;

protected:
	//don't use these - use the static singleton accessor
	ColorFactory();
	virtual ~ColorFactory();
	static ColorFactory *mInstance;
	static ColorRange *mColorRange;

public:
	static ColorFactory * GetInstance();
	
	const ColorRange & GetColorRange(int &argc, char **argv);
};

class ColorDeleter
{
public:
	~ColorDeleter() { delete ColorFactory::GetInstance();  }
};

#endif
