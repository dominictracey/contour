/*
** panner.h - Object that can handle panning of the current on-screen
**	image. It interacts with the engine and canvas interfaces to:
**		a) Allow the engine to update its internal representation.
**		b) Have the canvas move the current image appropriately until
**		the engine has a chance to update the image.
**
**	Since the panning controls are buttons that may be pushed and held,
** and we don't want to be redrawing and erasing, we adopt a transaction (as
** in database) style interact where on the mouse down a BeginPan() is called,
** while the button is held, Pan() is called and when the mouse up comes,
** EndPan() is invoked.
**	The engine should wait to modify its internal model until it recieves
** the EndPan() message while the canvas should move its currently generated
** image to match each Pan() call.
**
**	This class is pretty much a Facade (GOF - 185) that menus, toolbars,
** panning controls, and panning engines (for screensavers and root windows)
** can use without having to worry about keeping the specific canvas and engine
** in step.
**
**	These classes are light-weight throw-aways. Only use them once. IOW,
** call BeginPan and EndPan only once on an instance of this class. Menus and
** toolbars will only call Pan once as well.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
*/

#ifndef _PANNER_H_
#define _PANNER_H_

#include "engine.h"
#include "canvas.h"


#define STEPSIZE0 100
#define STEPSIZE1 12
#define STEPSIZE2 15 
#define STEPSIZE3 20
#define STEPSIZE4 22

class Panner
{
public:
	enum Direction {NORTH,NORTHEAST,EAST,SOUTHEAST,SOUTH,SOUTHWEST,WEST,NORTHWEST};
	Panner(Direction dir);
	virtual ~Panner();
	virtual void BeginPan();
	virtual void Pan();
	virtual void EndPan();
	int GetNSOffset() {return mNSTotalOffset;}
	int GetEWOffset() {return mEWTotalOffset;}
	int GetNSCurrentStepSize() {return mNSCurrentStepSize;}
	int GetEWCurrentStepSize() {return mEWCurrentStepSize;}
protected:
	int mNumSteps;
	int mNSCurrentStepSize;
	int mEWCurrentStepSize;
	int mNSTotalOffset;
	int mEWTotalOffset;
	Direction mDirection;

#ifdef _DEBUG
	bool init;
#endif

private:
	Panner() {}; //call the public constructor with the parameters
};

#endif

