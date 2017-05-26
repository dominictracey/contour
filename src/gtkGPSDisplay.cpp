/*
** gtkGPSDisplay.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Mon Jul  2 00:52:15 EDT 2001
*/

#include "gtkGPSDisplay.h"
#include "gps.h"
#include <gtk/gtk.h>
#include "position.h"
#include "gdkCanvas.h"
#include "preferences.h"
#include "appFactory.h"
#include "engine.h"
#include <stdio.h>
#include <math.h>

GtkGPSDisplay::GtkGPSDisplay()
{
	GDKCanvas *canvas = ((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas());
	// location of GPS readout
	int hoff = 200;
	mLatPosX = canvas->GetWidth() - hoff;
	mLonPosX = canvas->GetWidth() - hoff;
	mSpeedPosX = canvas->GetWidth() - hoff;

	mLatPosY = canvas->GetHeight() - 45;
	mLonPosY =canvas->GetHeight() - 30;
	mSpeedPosY = canvas->GetHeight() - 15;

	mLatLabel = "Lat: ";
	mLongLabel = "Lon: ";
	mSpeedLabel = "Speed: ";

	mFont = canvas->GetFont();
	mGC = canvas->GetXORGC();

	mClear = false;
}

GtkGPSDisplay::~GtkGPSDisplay()
{

}

void GtkGPSDisplay::Draw()
{
	ASSERT(mClear)
	GDKCanvas *canvas = ((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas());

	//draw new values
	gdk_draw_text(canvas->GetDrawingSurfacePixmap(),mFont,mGC,mLatPosX,mLatPosY,mLatString,::strlen(mLatString));
	gdk_draw_text(canvas->GetDrawingSurfacePixmap(),mFont,mGC,mLonPosX,mLonPosY,mLongString,::strlen(mLongString));
	gdk_draw_text(canvas->GetDrawingSurfacePixmap(),mFont,mGC,mSpeedPosX,mSpeedPosY,mSpeedString,::strlen(mSpeedString));
	mClear = false;
}

void GtkGPSDisplay::Clear()
{
	ASSERT(!mClear)
	GDKCanvas *canvas = ((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas());

	//draw new values
	gdk_draw_text(canvas->GetDrawingSurfacePixmap(),mFont,mGC,mLatPosX,mLatPosY,mLatString,::strlen(mLatString));
	gdk_draw_text(canvas->GetDrawingSurfacePixmap(),mFont,mGC,mLonPosX,mLonPosY,mLongString,::strlen(mLongString));
	gdk_draw_text(canvas->GetDrawingSurfacePixmap(),mFont,mGC,mSpeedPosX,mSpeedPosY,mSpeedString,::strlen(mSpeedString));
	mClear = true;
}

void GtkGPSDisplay::DrawPartial(int x1, int x2, int y1, int y2)
{

}

void GtkGPSDisplay::DrawText()
{

}

void GtkGPSDisplay::DrawTextPartial(int x1, int x2, int y1, int y2)
{

}

void GtkGPSDisplay::SetViewPortLocal()
{
	Position pLat(GPS::GetInstance()->GetLatitude());
	Position pLon(GPS::GetInstance()->GetLongitude());

	int latHemi,lonHemi;
	if (pLat.GetDegrees() > 0) latHemi=0x4e; else latHemi=0x53;
	if (pLon.GetDegrees() > 0) lonHemi=0x45; else lonHemi=0x57;
	sprintf(mLatString,"%s %d° %d\' %4.2f\" %c\0",mLatLabel,abs(pLat.GetDegrees()),abs(pLat.GetMinutes()),fabs(pLat.GetSeconds()),latHemi);	
	sprintf(mLongString,"%s %d° %d\' %4.2f\" %c\0",mLongLabel,abs(pLon.GetDegrees()),abs(pLon.GetMinutes()),fabs(pLon.GetSeconds()),lonHemi);	
	sprintf(mSpeedString,"%s %.2f kts",mSpeedLabel,GPS::GetInstance()->GetSpeed());

}
