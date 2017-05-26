/*
** spline_forwarddifference.h 
**	Uses forward differencing to increase evaluation efficiency. The brute-force
**	evaluation uses 10 additions and 9 multiplies to evaluate a 3D point. By using
**	forward diffing, it only uses 9 additions and no multiplies!
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Dec 17 20:59:57 EST 2000
*/

#ifndef _SPLINE_FORWARDDIFFERENCE_H_
#define _SPLINE_FORWARDDIFFERENCE_H_

#include "spline.h"

class SplineForwardDifference : public spline
{
public:
	SplineForwardDifference()  { mInitialized = false;}
	virtual ~SplineForwardDifference() {};

	// don't jerk around with this stuff for right now.
	virtual void compute_xz(void) { compute_all(); }
    virtual void compute_yz(void) { compute_all(); }

	virtual void Initialize(int numSteps);
	virtual spline_point & Step(spline_point & sp);
	
protected:
	void DumpD(); //debugging
	bool mInitialized;
	float mStepSize;
	float 	mD[3][4];
};

#endif

