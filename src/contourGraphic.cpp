/*
** contourGraphic.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Mon Dec 17 21:22:04 EST 2001
*/

#include "contourGraphic.h"
#include "position.h"
#include "engine.h"
#include "preferences.h"
#include "appFactory.h"
#include "gdkCanvas.h"
#include "contourGraphicContainer.h"

SamplePoint ContourGraphic::mUL;
float ContourGraphic::mXScale = -1;
float ContourGraphic::mYScale = -1;
SamplePoint ContourGraphic::mSize;
int ContourGraphic::mWidth = -1;
int ContourGraphic::mHeight = -1;
bool ContourGraphic::mReentrantSetViewPort = false;
//update static members
void ContourGraphic::SetViewPort() 
{
	// since we give all the subclass instances notification through the
	//	SetViewPortLocal mechanism we need to check for re-entrancy.
	ASSERT(!mReentrantSetViewPort)
	mReentrantSetViewPort = true;
	ContourPreferences *prefs = AppFactory::GetInstance()->GetApp()->GetPreferences();
	mUL = prefs->GetUpperLeft();
	mXScale = prefs->GetXScale();
	mYScale = prefs->GetYScale();
	mWidth = AppFactory::GetInstance()->GetApp()->GetCanvas()->GetWidth();
	mHeight = AppFactory::GetInstance()->GetApp()->GetCanvas()->GetHeight();
	SamplePoint lowerRight(mUL);
	lowerRight.Scale(mWidth, mHeight, mXScale, mYScale);
	mSize = mUL - lowerRight;

	// now give all the decorations a chance to react
	((GDKCanvas *)AppFactory::GetInstance()->GetApp()->GetCanvas())->GetDecorations()->SetViewPortLocal();
	
	mReentrantSetViewPort = false;
}