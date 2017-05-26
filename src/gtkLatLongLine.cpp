/*
** gtkLatLongLine.cpp - 
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYIN for the full GPL license.
**
** Created: Mon Jan  1 11:08:27 EST 2001
*/

#include "appFactory.h"
#include "gtkLatLongLine.h"
#include "gdkCanvas.h"
#include "stdio.h"
#include "engine.h"

GdkGC *GtkLatLongLine::mGC = NULL;
GdkFont *GtkLatLongLine::mFont = NULL;

/*
** Draw - Assumes you've just called SetViewPort.
*/
void GtkLatLongLine::Draw()
{
	CreateGC();
	GDKCanvas *canvas = (GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas();
	if (mLat)
	{
		//horizontal line
		SamplePoint sp(mPos,mUL.GetLongitude());
		//sp.Dump();
		int y = sp.GetY(&mUL,&mSize,mHeight);
		gdk_draw_line(canvas->GetDrawingSurfacePixmap(),mGC,0,y-EDGE_BUFFER,mWidth,y-EDGE_BUFFER);
		gdk_draw_text(canvas->GetDrawingSurfacePixmap(),mFont,mGC,7,y-7-EDGE_BUFFER,mStr,::strlen(mStr));
	} else
	{
		//vertical line
		SamplePoint sp(mUL.GetLatitude(),mPos);
		int x = sp.GetX(&mUL,&mSize,mWidth);
		 
		gdk_draw_line(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetDrawingSurfacePixmap(),mGC,x-EDGE_BUFFER,0,x-EDGE_BUFFER,mHeight);	
		gdk_draw_text(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetDrawingSurfacePixmap(),mFont,mGC,x+7-EDGE_BUFFER,15,mStr,::strlen(mStr));
	}
}

void GtkLatLongLine::DrawPartial(int x1, int x2, int y1, int y2)
{
	CreateGC();
	GDKCanvas *canvas = (GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas();
	if (mLat)
	{
		//horizontal line
		SamplePoint sp(mPos,mUL.GetLongitude());
		int y = sp.GetY(&mUL,&mSize,mHeight);
		if (y >= y1 && y <= y2)
		{
			gdk_draw_line(canvas->GetDrawingSurfacePixmap(),mGC,x1,y,x2,y);
			//cout << "Drawing lat line x1 " << x1 << " x2 " << x2 << " y " << y << endl;
			//gdk_draw_text(canvas->GetDrawingSurfacePixmap(),mFont,mGC,7,y-7,mStr,::strlen(mStr));
		}
	} else
	{
		//vertical line
		SamplePoint sp(mUL.GetLatitude(),mPos);
		int x = sp.GetX(&mUL,&mSize,mWidth);
		if (x >= x1 && x <= x2)
		{		 
			gdk_draw_line(canvas->GetDrawingSurfacePixmap(),mGC,x,y1,x,y2);	
			//cout << "Drawing lon line y1 " << y1 << " y2 " << y2 << " x " << x << endl;
			//gdk_draw_text(canvas->GetDrawingSurfacePixmap(),mFont,mGC,x+7,15,mStr,::strlen(mStr));
		}
	}
}

void GtkLatLongLine::DrawText()
{
	CreateGC();
	
	GDKCanvas *canvas = (GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas();
	if (mLat)
	{
		//horizontal line
		SamplePoint sp(mPos,mUL.GetLongitude());
		int y = sp.GetY(&mUL,&mSize,mHeight);
		gdk_draw_text(canvas->GetDrawingSurfacePixmap(),mFont,mGC,7,y-7,mStr,::strlen(mStr));
	} else
	{
		//vertical line
		SamplePoint sp(mUL.GetLatitude(),mPos);
		int x = sp.GetX(&mUL,&mSize,mWidth);		 
		gdk_draw_text(canvas->GetDrawingSurfacePixmap(),mFont,mGC,x+7,15,mStr,::strlen(mStr));
	}

}

void GtkLatLongLine::DrawTextPartial(int x1, int x2, int y1, int y2)
{
	CreateGC();
	GDKCanvas *canvas = (GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas();
	if (mLat)
	{
		//horizontal line
		SamplePoint sp(mPos,mUL.GetLongitude());
		int y = sp.GetY(&mUL,&mSize,mHeight);
		if (y >= y1 && y <= y2)
		{
			gdk_draw_text(canvas->GetDrawingSurfacePixmap(),mFont,mGC,7,y-7,mStr,::strlen(mStr));
		}
	} else
	{
		//vertical line
		SamplePoint sp(mUL.GetLatitude(),mPos);
		int x = sp.GetX(&mUL,&mSize,mWidth);
		if (x >= x1 && x <= x2)
		{		 
			gdk_draw_text(canvas->GetDrawingSurfacePixmap(),mFont,mGC,x+7,15,mStr,::strlen(mStr));
		}
	}
}

void GtkLatLongLine::CreateGC()
{
	if (!mGC)
	{
		mGC = gdk_gc_new(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetCanvasControl()->window);
		gdk_gc_set_function(mGC, GDK_XOR);
		GdkColor c;
		gdk_color_white(((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetColorMap(),&c);
		gdk_gc_set_function(mGC, GDK_XOR);
		gdk_gc_set_foreground(mGC,&c);
		mFont = gdk_font_load("-adobe-courier-medium-r-normal-*-*-140-*-*-m-*-iso8859-1");
		gdk_gc_set_font(mGC,mFont);
	}

}

void GtkLatLongLine::Clear()
{

}

void GtkLatLongLine::Dump()
{
	cout << "GtkLatLongLine Dump " << endl;
	mUL.Dump();
}

void GtkLatLongLine::SetViewPortLocal()
{
	// grid takes care of this for us
}

