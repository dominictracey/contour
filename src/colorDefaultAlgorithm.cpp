/*
** colorDefaultAlgorithm.h - Implementation for default algorithm. 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.22.98 DPT
*/

#include "colorDefaultAlgorithm.h"
#include "canvas.h"
#include <stdio.h>
#include <math.h>

ColorDefaultAlgorithm::~ColorDefaultAlgorithm()
{

}

/*
** Initialize - Just drew a graph with three curves on it (R,G,B) and
**	horse-shitted some trig to fit them.
*/
bool ColorDefaultAlgorithm::Initialize(ContourCanvas &canvas)
{
	int i, red, green, blue;
	int numColors = canvas.GetNumColors();
	float inc = 3.1416 / numColors;
        for (i=0; i<numColors; i++)
        {
                if (i <= numColors/2)
                {
                        blue = (int)(sin(i*2*inc) * 65536);
                        green = (int)(sin(i*inc) * 65536);
                        red = 0;
                }
                else
                {
			blue = 0;
			green = (int)(sin(i*inc) * 65536);
			red = (int)(-(cos(i*inc) * 65536));
                }
                canvas.SpecifyColor(red,green,blue,i);
	}

	return canvas.AllocColors();
}

