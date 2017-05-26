/*
** cursorDrawTrack.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Nov 25 22:46:27 EST 2001
*/

#include "cursorDrawTrack.h"
#include "gdkCanvas.h"
#include "appFactory.h"
#include "effortDecoration.h"
#include "parametricSurface.h"
#include "effortDraw.h"
#include "preferences.h"

CursorDrawTrack::CursorDrawTrack()
{
	Reference(this); //the static callback tedium
	mClear = true;
	mCanvas = ((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas());
	mDrawing = false;
	mEffort = NULL;
	mPrefs = AppFactory::GetInstance()->GetApp()->GetPreferences();

}

CursorDrawTrack::~CursorDrawTrack()
{
	 //dealing with the static callback tedium
	Dereference();
}


void CursorDrawTrack::Draw()
{
	if (mDrawing)
	{
		ASSERT(mClear)
		mClear = false;
	}

}

void CursorDrawTrack::Clear()
{
	if (mDrawing)
	{	
		ASSERT(!mClear)	
		mClear = true;
	}
}

void CursorDrawTrack::DrawPartial(int x1, int x2, int y1, int y2)
{

}

void CursorDrawTrack::DrawText()
{

}

void CursorDrawTrack::DrawTextPartial(int x1, int x2, int y1, int y2)
{

}

gint CursorDrawTrack::ButtonPressLocal(GtkWidget *widget, GdkEventButton *event)
{
	//if (mEffort)
	//	delete mEffort;
	mEffort = new DrawEffort;

	mEffort->AddDecoration();
	mEffort->Start();
	mCanvas->AddDecoration(ContourCanvas::DRAW_EFFORTS);
	mDrawing = true;
	
	return true;
}

gint CursorDrawTrack::ButtonReleaseLocal(GtkWidget *widget, GdkEventButton *event)
{
	mDrawing = false;
	mEffort->End();
	return true;
}

gint CursorDrawTrack::MouseMotionLocal(GtkWidget *widget, GdkEventMotion *event)
{
	if (mDrawing)
	{
		//create the new sample point from the on-screen X & Y
		/*GridPoint *gp = new GridPoint;
		gp->x(mX); 
		gp->y(mY);
		mEffort->AddSamplePoint(gp);*/
		GridPoint * sp = new GridPoint(mPrefs->GetUpperLeft());
		sp->Scale(mX,mY,mPrefs->GetXScale(),mPrefs->GetYScale());
		sp->x(mX);
		sp->y(mY);
		mEffort->AddSamplePoint(sp);
	}
	return true;
}

