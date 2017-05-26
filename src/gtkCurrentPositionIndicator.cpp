/*
** gtkCurrentPositionIndicator.cpp - 
**
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Jul  1 00:08:32 EDT 2001
*/

#include "gtkCurrentPositionIndicator.h"
#include "gps.h"
#include "position.h"
#include "gdkCanvas.h"
#include "preferences.h"
#include "appFactory.h"
#include "engine.h"

GdkGC *GtkCurrentPositionIndicator::mGC = NULL;

GtkCurrentPositionIndicator::GtkCurrentPositionIndicator()
{
	mx = -7;
	my = -7;
	mClear = true;
}

GtkCurrentPositionIndicator::~GtkCurrentPositionIndicator()
{

}

void GtkCurrentPositionIndicator::Draw()
{
	ASSERT(mClear);
	if (mClear)
	{
		CreateGC();
		GDKCanvas *canvas = ((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas());

		Position pLat = GPS::GetInstance()->GetLatitude();
		Position pLon = GPS::GetInstance()->GetLongitude();

		SamplePoint sp(pLat,pLon);
		
		ContourPreferences *prefs = AppFactory::GetInstance()->GetApp()->GetPreferences();
		SamplePoint UL = prefs->GetUpperLeft();
		
		float xScale = prefs->GetXScale();
		float yScale = prefs->GetYScale();
		int width = AppFactory::GetInstance()->GetApp()->GetEngine()->GetWidth();
		int height = AppFactory::GetInstance()->GetApp()->GetEngine()->GetHeight();
		SamplePoint lowerRight(UL);
		lowerRight.Scale(width, height, xScale, yScale);

		SamplePoint size = UL - lowerRight;

		mx = sp.GetX(&UL,&size,width);
		my = sp.GetY(&UL,&size,height);

		//if its off the screen -- don't draw it
		if (mx < 7 || mx > width - 14 || my < 7 || my > height - 14)
		{
			mClear = false; // we tried to draw it...
			return;
		}
			
		gdk_draw_arc ((GdkDrawable *)canvas->GetDrawingSurfacePixmap(), mGC,true,mx-6,my-6,12,12,0,23040); //true, mx, my,7,7);//,0,32);
		//gdk_draw_rectangle ((GdkDrawable *)canvas->GetDrawingSurfacePixmap(), mGC,true,mx,my,20,20); //true, mx, my,7,7);//,0,32);
		mClear = false;
	}
}

void GtkCurrentPositionIndicator::Clear()
{
	ASSERT(!mClear);
	if (!mClear)
	{
		CreateGC();
		GDKCanvas *canvas = ((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas());

		//@REX this check shouldn't be necessary
		int width = AppFactory::GetInstance()->GetApp()->GetEngine()->GetWidth();
		int height = AppFactory::GetInstance()->GetApp()->GetEngine()->GetHeight();
		if (mx < 7 || mx > width - 14 || my < 7 || my > height - 14)
			//gdk_draw_rectangle ((GdkDrawable *)canvas->GetCanvasControl(), mGC,true, mx, my,7,7);//,0,32);
			//gdk_draw_rectangle ((GdkDrawable *)canvas->GetDrawingSurfacePixmap(), mGC,true,mx,my,20,20); //true, mx, my,7,7);//,0,32);
			gdk_draw_arc ((GdkDrawable *)canvas->GetDrawingSurfacePixmap(), mGC,true,mx-6,my-6,12,12,0,23040); //true, mx, my,7,7);//,0,32);

		mClear = true;
	}
}

void GtkCurrentPositionIndicator::DrawPartial(int x1, int x2, int y1, int y2)
{

}

void GtkCurrentPositionIndicator::DrawText()
{

}

void GtkCurrentPositionIndicator::DrawTextPartial(int x1, int x2, int y1, int y2)
{

}

void GtkCurrentPositionIndicator::CreateGC()
{
	if (!mGC)
	{
		mGC = gdk_gc_new(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetCanvasControl()->window);
		//gdk_gc_set_function(mGC, GDK_XOR);
		GdkColor c;
		gdk_color_white(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetColorMap(),&c);
		//gdk_gc_set_function(mGC, GDK_XOR);
		gdk_gc_set_foreground(mGC,&c);
		gdk_gc_set_background(mGC,&c);
		//mFont = gdk_font_load("-adobe-courier-medium-r-normal-*-*-140-*-*-m-*-iso8859-1");
		//gdk_gc_set_font(mGC,mFont);
	}

}

void GtkCurrentPositionIndicator::SetViewPortLocal()
{

}
