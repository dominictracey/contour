/*
** colorFactory.cpp
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
*/

#include "bath.h"
#include "colorFactory.h"
#include "colorRange.h"
#include "colorRange16Default.h"
#include "colorRange8Default.h"
#include <stdio.h>

//statics
ColorFactory *ColorFactory::mInstance = NULL;
ColorRange *ColorFactory::mColorRange = NULL;

/*
** default constructor - protected. Use GetInstance instead.
*/
ColorFactory::ColorFactory()
{
	
}

/*
** destructor - Called from ColorFactoryDeleter:: ~ColorFactoryDeleter
*/
ColorFactory::~ColorFactory()
{
	if (mColorRange)
		delete mColorRange;
}

/*
** GetInstance - Singleton implementation
*/
ColorFactory * ColorFactory::GetInstance()
{
	if (!mInstance)
		mInstance = new ColorFactory;
	return mInstance;
}

/*
** GetColorRange - Return the appropriate color range.
*/
const ColorRange & ColorFactory::GetColorRange(int &argc, char **argv)
{
        Display *dpy;         // display of colorWidget
        int scr;              // screen of dpy
        Colormap cmap;        // colormap for program
        XColor color;         // color used for setting colormap (the palette)

	//parse command line for --color-range
	char namebuf[200];
	char *np = namebuf;
	for (int i = 0; i < argc; i++)
	{
		for (char *ch = argv[i]; *ch != 0; ch++)
			*np++ = *ch;
		*np = 0;
		if (!::strcmp(namebuf,"--color-range\0"))
			printf("FIXME please\n (c:\n");
		np = namebuf;
	}
			
	if (!mColorRange)
	{
		//probe for some display parameters
        	int depth = DefaultDepth( dpy, scr);
        	printf("Default Depth is %d\n", depth);

        	int cells = DisplayCells(dpy,scr);
        	printf("Number of cells in default colormap is %d\n", cells);

        	int planes = DisplayPlanes(dpy,scr);
        	printf("Depth of root window is %d\n",planes);

        	//try to get a random color individually
        	color.red = 10; color.blue = 100; color.green = 200;
        	color.flags = DoRed | DoBlue | DoGreen;

        	if (XAllocColor(dpy,cmap,&color))
                	printf("Successfully allocated test color\n");
          	else
                	printf("Couldn't allocate test color\n");

		if (depth == 16)
			mColorRange = new ColorRange16Default;
		else if (depth == 8)
			mColorRange = new ColorRange8Default;

		else throw ("Couldn't allocate colors");
	}

	return *mColorRange;
}	





