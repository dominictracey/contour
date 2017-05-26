/*
** panner.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2001, Aquilon Technology Systems, Inc.
**
*/

#include "panner.h"
#include "appFactory.h"
#include <stdio.h>

Panner::Panner(Direction dir) : mDirection(dir)
{
#ifdef _DEBUG
	init = false;
#endif
	mNSCurrentStepSize = 0;
	mEWCurrentStepSize = 0;
	mNSTotalOffset = 0;
	mEWTotalOffset = 0;
}

Panner::~Panner()
{

}

void Panner::BeginPan()
{
#ifdef _DEBUG
	if (init)
		printf("Pan Error: Invalid reuse of Panner. Please call \
		BeginPan once on each instance of the Panner class.\n");
#endif

	//Note that for offsets, North and West are considered positive, since they
	// are the directions that our coordinate systems increase in (East longitude
	// and South latitude are represented by negative numbers to facilitate this
	// scheme globally). This contradicts however the normal X coordinate sytem
	// completed since down and right are increasing there. Hmmm...
	if (mDirection == NORTH || mDirection == NORTHEAST || mDirection == NORTHWEST)
		mNSCurrentStepSize = STEPSIZE0;
	if (mDirection == SOUTH || mDirection == SOUTHEAST || mDirection == SOUTHWEST)
		mNSCurrentStepSize = -STEPSIZE0;
	if (mDirection == EAST || mDirection == NORTHEAST || mDirection == SOUTHEAST)
		mEWCurrentStepSize = -STEPSIZE0;
	if (mDirection == WEST || mDirection == NORTHWEST || mDirection == SOUTHWEST)
		mEWCurrentStepSize = STEPSIZE0;
	
	AppFactory::GetInstance()->GetApp()->GetEngine()->BeginPan(*this);
}

void Panner::Pan()
{
	//Tell the canvas to shift
	AppFactory::GetInstance()->GetApp()->GetCanvas()->
		Shift(mNSCurrentStepSize, mEWCurrentStepSize);
	mNSTotalOffset += mNSCurrentStepSize;
	mEWTotalOffset += mEWCurrentStepSize;
	++mNumSteps;
	//AppFactory::GetInstance()->GetApp()->GetEngine()->Pan(*this);
}

void Panner::EndPan()
{
	//Tell the engine we are done moving
	AppFactory::GetInstance()->GetApp()->GetEngine()->EndPan(*this);
}



