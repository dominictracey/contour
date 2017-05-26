/*
** effortDecoration.cpp - An on-screen "trackline" representation of a tow, steam, etc.
** 
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001 Aquilon Technology Systems, Inc.
**
** Created: Thu Nov 26 17:46:43 EDT 2001
*/

#include "effortDecoration.h"
#include "gdkCanvas.h"
#include "position.h"
#include "appFactory.h"

EffortDecoration::EffortDecoration(Effort * effort)
{
	mEffort = effort;
	mCanvas = ((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas());
	mGC = mCanvas->GetXORGC();
}

EffortDecoration::~EffortDecoration()
{

}

void EffortDecoration::Draw()
{

	// iterate through the sample points of the effort, drawing a line from the
	// last one to the current one.
	GridPointVector *v = mEffort->GetSamplePoints();
	GridPointVector::iterator it = v->begin();

	if (!it)   //empty -- no points
		return;
		
	GridPoint *two = *it;
	GridPoint *one;
	it++;
	while (it != v->end())
	{
		one = two;
		two = *it;

		gdk_draw_line(mCanvas->GetDrawingSurfacePixmap(),mGC,one->x(),one->y(),two->x(),two->y());
		it++;
	}	
}

void EffortDecoration::Clear()
{
	Draw();
}

void EffortDecoration::DrawPartial(int x1, int x2, int y1, int y2)
{

}

void EffortDecoration::DrawText()
{

}

void EffortDecoration::DrawTextPartial(int x1, int x2, int y1, int y2)
{

}

void EffortDecoration::SetViewPortLocal()
{

}

