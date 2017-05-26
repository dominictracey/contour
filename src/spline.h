/*
** spline.h - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Dec 17 16:09:51 EST 2000
*/

#ifndef SPLINE_H
#define SPLINE_H

#include <iostream.h>

class spline_point {
//	friend class spline;

public:
	spline_point();
	spline_point(int X, int Y, float Z);
	spline_point(const spline_point &old);
	const spline_point & operator =(const spline_point &rhs);
	void Dump() {cout << "(" << x << "," << y << "," << z << ")";}
//protected:
        int x;
        int y;
        float z;
};

#define NUM_OF_PTS 4
#define LAST_PTS_IDX (NUM_OF_PTS - 1)

class spline {
public:
	spline (void);
	virtual ~spline ();
	virtual void bump_along(spline_point *new_pt);   // delete position 0 and
                                                  // add to postion LAST_PTS_IDX
	virtual void set_value(spline_point *parameter, int position); // set new
                                                  // sample point at position
	virtual spline_point * get_value(int position) {return mData[position];} 
	virtual void compute_xz(void);
	virtual void compute_yz(void);
	virtual void compute_all(void);

	virtual spline_point evaluate_xz(float t);
	virtual spline_point evaluate_yz(float t);
	virtual spline_point evaluate_all(float t);
protected:
	// the control points of the spline
	spline_point* mData[NUM_OF_PTS];
	// the results of M . G
	float mX[NUM_OF_PTS], mY[NUM_OF_PTS], mZ[NUM_OF_PTS];
	// the constant basis matrix to create our blending functions from the
	// control points.
	float coef_mat[NUM_OF_PTS][NUM_OF_PTS];

	int round (float val);
	void DumpMG(); //debugging
}; 

#endif

