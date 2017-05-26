/*
** colorRange8Default.cpp - Implementation for color manager on systems with
**      8-bit color plane capabilities.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
** DPT - 12.12.1998
*/

#include "bath.h"
#include <stdio.h>
#include "colorRange8Default.h"

ColorRange8Default::ColorRange8Default()
{

}

ColorRange8Default::~ColorRange8Default()
{

}

/*
** Initialize - Allocate colors for the program. 
**
*/
BOOL ColorRange8Default::Initialize()
{
	int i;                // iterator
        int inc;              // color step between Pixels in colormap
	int center_offset;    // how far from middle
        Display *dpy;         // display of Widget
	Widget dummy;
	Colormap cmap;	
        int scr;              // screen of dpy
        XColor color;         // color used for setting colormap (the palette)

	dpy = XtDisplay(dummy);

        scr = DefaultScreen( dpy );
        cmap = DefaultColormap( dpy, scr );

        inc = 65535 / NUM_COLORS_8BIT;
        for (i=0; i<NUM_COLORS_8BIT; i++)
        {
		center_offset = i-NUM_COLORS_8BIT/2;
                color.flags = DoBlue | DoGreen | DoRed;
                if (i <= NUM_COLORS_8BIT/2)
                {
                	color.blue = 65535 - i*2*inc;
                	color.green = i*2*inc;	
                	color.red = 0;
                }
                  else
                  {
                  	color.blue = 0;
                        color.green = 
			      int(65535 - 6.5535*center_offset*center_offset);
			int f = NUM_COLORS_8BIT-i;
                        color.red = int(65535 - 0.065535*f*f*f);
		  }

        	if (XAllocColor(dpy,cmap,&color))
        	{
                	mColors[i] = color.pixel;	
        	}
          	  else
		  {
                	printf("Couldn't allocate map color %d\n", i);
			return FALSE;
		  }
	}
	
	return TRUE;
}

/*
** GetColor - Returns appropriate pixel for given value.
*/
Pixel ColorRange8Default::GetColor(int depth)
{
	Widget dummy;
	//normalize depth
	int loc = (depth - mMin)*NUM_COLORS_8BIT/ (mMax-mMin);  

	//check if its out of range
        if (loc >= NUM_COLORS_8BIT)
            return WhitePixel(XtDisplay(dummy), 0);
        if(loc < 0)
            return BlackPixel(XtDisplay(dummy), 0);

        return mColors[loc];
}
