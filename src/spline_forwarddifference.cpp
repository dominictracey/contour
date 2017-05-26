/*
** spline_forwarddifference.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Dec 17 21:56:02 EST 2000
*/

#include "spline_forwarddifference.h"
#include <math.h>


/*
**	Initialize our D= | f0 df0 d2f0 d3f0 | by matrix multiplying the C * G
** result from calling our parent's compute_all (stored in mX,mY,mZ) with
** E(d) = 	|	0		0		0		1		|
**			|	d^3		d^2		d 		0		|
**			|	6d^3	2d^2	0		0		|
**			|	6d^3	0		0		0		|
**
**	Where d is the step size (0 < d < 1)
*/
void SplineForwardDifference::Initialize(int numSteps)
{
	//@REX boundary check
	mStepSize = (float)1/(float)numSteps;
	//save typing
	float d = mStepSize;

	// i will go 0,1,2 representing x,y,z
	// @REX redo parent's to put mX, mY & mZ in one 4x3 array.
	mD[0][0] = mX[3];
	mD[0][1] = (mX[0] * pow(d,(float)3)) + (mX[1] * pow(d,(float)2)) + (mX[2] * d);
	mD[0][2] = (mX[0] * pow(d,(float)3) * (float)6) + (mX[1] * pow(d,(float)2) * (float)2);
	mD[0][3] = (mX[0] * pow(d,(float)3) * (float)6);

	mD[1][0] = mY[3];
	mD[1][1] = (mY[0] * pow(d,(float)3)) + (mY[1] * pow(d,(float)2)) + (mY[2] * d);
	mD[1][2] = (mY[0] * pow(d,(float)3) * (float)6) + (mY[1] * pow(d,(float)2) * (float)2);
	mD[1][3] = (mY[0] * pow(d,(float)3) * (float)6);

	mD[2][0] = mZ[3];
	mD[2][1] = (mZ[0] * pow(d,(float)3)) + (mZ[1] * pow(d,(float)2)) + (mZ[2] * d);
	mD[2][2] = (mZ[0] * pow(d,(float)3) * (float)6) + (mZ[1] * pow(d,(float)2) * (float)2);
	mD[2][3] = (mZ[0] * pow(d,(float)3) * (float)6);

	mInitialized = true;
}

void SplineForwardDifference::DumpD()
{
	cout << "Forward Difference D array " << endl;
	for (int i=0; i<4; ++i)
	{
		for (int j=0; j<3; ++j)
			cout << mD[j][i] << " ";
		cout << endl;
	}
}

spline_point & SplineForwardDifference::Step(spline_point & sp)
{
	// move everything along for the next step
	mD[0][0] = mD[0][0] + mD[0][1];
	mD[1][0] = mD[1][0] + mD[1][1];
	mD[2][0] = mD[2][0] + mD[2][1];

	mD[0][1] = mD[0][1] + mD[0][2];
	mD[1][1] = mD[1][1] + mD[1][2];
	mD[2][1] = mD[2][1] + mD[2][2];
	
	mD[0][2] = mD[0][2] + mD[0][3];
	mD[1][2] = mD[1][2] + mD[1][3];
	mD[2][2] = mD[2][2] + mD[2][3];

	sp.x = (int)mD[0][0];
	sp.y = (int)mD[1][0];
	sp.z = mD[2][0];

	return sp;

}

