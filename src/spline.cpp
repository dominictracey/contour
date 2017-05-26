/*
** spline.cpp - 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Dec 17 01:24:04 EST 2000
*/

#include <stdlib.h>
#include <math.h>
#include "spline.h"

spline_point::spline_point()
{

}

spline_point::spline_point(int X, int Y, float Z)
: x(X), y(Y), z(Z)
{

}

spline_point::spline_point(const spline_point &old)
{
	x = old.x;
	y = old.y;
	z = old.z;
}

const spline_point & spline_point::operator =(const spline_point &rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;

	return *this;
}


/*****************************************************************************
FUNCTION spline::round
PROGRAMMER - Keivan Khalichi
DATE PROGRAMMED - 10/31/94
DESCRIPTION - Rounds the float argument to the nearest integer.  All real values
    with decimal point values above and including 0.5 round up and all below
    0.5 round down.
PARAMETERS
    val: the float value to be rounded
    Returns: rounded integer
CALLS: no other
*****************************************************************************/
int spline::round (float val)
{
  int tmp;
  double roundpoint;

  tmp = (int) val;
  roundpoint = fmod ((double) val, 0.5);
  tmp += (roundpoint >= 0.5) ? 1 : 0;
  return tmp;
} // spline::round

/*****************************************************************************
FUNCTION spline::spline
PROGRAMMER - Keivan Khalichi
DATE PROGRAMMED - 10/16/94
DESCRIPTION - Constructor.  Sets up coefficient matrix coef_mat and
    sets each index of mData to NULL.
PARAMETERS none
CALLS: no other
*****************************************************************************/
spline::spline (void)
{
  int i, j;
  float coef[][NUM_OF_PTS] = { {-0.5, 1.5, -1.5, 0.5},
                               {1.0, -2.5, 2.0, -0.5},
                               {-0.5, 0.0, 0.5, 0.0 },
                               {0.0, 1.0, 0.0, 0.0  }
                             };

  // Initializing the coef_mat private member mData with coef
  // Also initializing the mData array to all NULLs

  for (i = 0; i < NUM_OF_PTS; i++) {
    for (j = 0; j < NUM_OF_PTS; j++)
      coef_mat[i][j] = coef[i][j];
    mData[i] = NULL;
  } 
   
} 

/*****************************************************************************
FUNCTION spline::~spline
PROGRAMMER - Keivan Khalichi
DATE PROGRAMMED - 10/16/94
DESCRIPTION - Destructor.  Destroys the memory for each of mData's indecies.
PARAMETERS none
CALLS: no other
*****************************************************************************/
spline::~spline ()
{
  int i;

  for (i = 0; i < NUM_OF_PTS; i++)
    if (mData[i] != NULL)
      delete mData[i];

}

/*****************************************************************************
FUNCTION spline::bump_along
PROGRAMMER - Keivan Khalichi
DATE PROGRAMMED - 10/16/94
DESCRIPTION - Adds a new sample point to the array of sample points.  All
    sample points are moved up one index in the array of sample points.
    The new sample point is set into the last index of the array.
PARAMETERS
    new_pt: The new point to be inserted into the array
CALLS: no other
*****************************************************************************/
void spline::bump_along(spline_point *new_pt)
{
  int i;

  if (mData[0] != NULL) {
    delete mData[0];  // Deallocating memory for the first item of mData
    mData[0] = NULL;
  } // if

  // Moving each data member to the previous index.
  for (i = 1; i < NUM_OF_PTS; i++)
    mData[i - 1] = mData[i];

  mData[LAST_PTS_IDX] = new_pt; // Setting last index's memory location to
                               // new_pt
}

/*****************************************************************************
FUNCTION spline::set_value
PROGRAMMER - Keivan Khalichi
DATE PROGRAMMED - 10/16/94
DESCRIPTION - Sets a sample point in the array of sample points at a
    certain position.
PARAMETERS
    parameter: The new sample point to be placed into the array
    position: The position in the array where the new sample point will be
              set
CALLS: no other
*****************************************************************************/
void spline::set_value(spline_point *parameter, int position)
{
  // deallocate memory for the array index's memory location that is to be
  // reassigned
  if (mData[position] != NULL) {
    delete mData[position];
    mData[position] = NULL;
  } // if

  mData[position] = parameter;  // set parameter equal at position
} 

/*****************************************************************************
FUNCTION spline::compute_xz
PROGRAMMER - Keivan Khalichi
DATE PROGRAMMED - 10/16/94
DESCRIPTION - Uses the four current sample points to create the polynomial
    coefficients necessary to evaluate only the mX and mZ values at certain
    t-parameter values.
PARAMETERS - none
(Bhaba-11/18/94) No longer CALLS: sample_point::get_x, sample_point::get_z
*****************************************************************************/
void spline::compute_xz(void)
{
  int i, j;

  // For the number of points, compute x_i and z_i.
  for (i = 0; i < NUM_OF_PTS; i++) {

    // Reseting values of x_i and z_i for use in the next code segment.
    mX[i] = 0.0;
    mZ[i] = 0.0;

    // Multiply the mX and mY sample points with corresponding non-zero entries
    // of the coef_mat, and add to the coefficients of x(t) and z(t)
    // equations.
    for (j = 0; j < NUM_OF_PTS; j++)
      if (coef_mat[i][j] != 0.0) {
        mX[i] += coef_mat[i][j] * (float) mData[j]->x;
        mZ[i] += coef_mat[i][j] * (float) mData[j]->z;
      }
  }
}

/*****************************************************************************
FUNCTION spline::compute_yz
PROGRAMMER - Keivan Khalichi
DATE PROGRAMMED - 10/16/94
DESCRIPTION - Uses the four current sample points to create the polynomial
    coefficients necessary to evaluate only the y and z values at certain
    t-parameter values.
PARAMETERS - none
(Bhaba-11/18/94) No longer CALLS: sample_point::get_y, sample_point::get_z
*****************************************************************************/
void spline::compute_yz(void)
{
	int i, j;

	for (i = 0; i < NUM_OF_PTS; i++) {

		mY[i] = 0.0;
   		 mZ[i] = 0.0;

	    // Multiply the y and z sample points with corresponding non-zero entries
	    // of the coef_mat, and add to the coefficinets of y(t) and z(t)
	    // equations.
	    for (j = 0; j < NUM_OF_PTS; j++)
	    {
			if (coef_mat[i][j] != 0.0) {
				mY[i] += coef_mat[i][j] * (float) mData[j]->y;
				mZ[i] += coef_mat[i][j] * (float) mData[j]->z;
	      	}

		}
	}
}

/*****************************************************************************
FUNCTION spline::compute_all
PROGRAMMER - Keivan Khalichi
DATE PROGRAMMED - 10/16/94
DESCRIPTION - same as compute_xz and compute_yz, except that all the three
    coordinate, x, y and z, are evaluted.
PARAMETERS - none
(Bhaba-11/18/94) No longer CALLS: sample_point::get_[x|y|z]
*****************************************************************************/
void spline::compute_all(void)
{
  int i, j;

  // For the number of points, compute x_i, y_i and z_i.
  for (i = 0; i < NUM_OF_PTS; i++) {

    // Reseting values of x_i, y_i and z_i for use in the next code segment.
    mX[i] = 0.0;
    mY[i] = 0.0;
    mZ[i] = 0.0;
    // Compute coefficients for parametric blending ploynomials.
    for (j = 0; j < NUM_OF_PTS; j++)
      if (coef_mat[i][j] != 0.0) {
        mX[i] += coef_mat[i][j] * (float) mData[j]->x;
        mY[i] += coef_mat[i][j] * (float) mData[j]->y;
        mZ[i] += coef_mat[i][j] * (float) mData[j]->z;
      }

  }
}

/*****************************************************************************
FUNCTION spline::evaluate_xz
PROGRAMMER - Keivan Khalichi
DATE PROGRAMMED - 10/16/94
DESCRIPTION - Returns an x and z value for a given value of the t-parameter.
PARAMETERS
    t: The float t-parameter to evaluate x and z upon
    Returns spline_point in which x and z are set
CALLS: no other
*****************************************************************************/
spline_point spline::evaluate_xz(float t)
{
  int i;
  spline_point result;

  // Set the x and z values to the last coefficient.
  result.x = round (mX[LAST_PTS_IDX]);
  result.z = (mZ[LAST_PTS_IDX]);

  for (i = 0; i < (NUM_OF_PTS - 1); i++) {
    result.x += round ((float) pow ((double) t, (double) (NUM_OF_PTS - i - 1))
                       * mX[i]);
    result.z += ((float) pow ((double) t, (double) (NUM_OF_PTS - i - 1))
                       * mZ[i]);
  }

  return result;
} 

/*****************************************************************************
FUNCTION spline::evaluate_yz
PROGRAMMER - Keivan Khalichi
DATE PROGRAMMED - 10/16/94
DESCRIPTION - Returns a y and z value for a given value of the t-parameter.
PARAMETERS
    t: The float t-parameter to evaluate y and z upon
    Returns spline_point in which y and z are set
CALLS: no other
*****************************************************************************/
spline_point spline::evaluate_yz(float t)
{
  int i;
  spline_point result;

  // Set the y and z values to the last coefficient.
  result.y = round (mY[LAST_PTS_IDX]);
  result.z =  (mZ[LAST_PTS_IDX]);

  // Multiply each ceofficient of y and z (i.e. y_i and z_i) with
  // t^i.  Adding each result to the value of result.y and result.z.
  // Not multiplying the t^0 for optimization.
  for (i = 0; i < (NUM_OF_PTS - 1); i++) {
    result.y += round ((float) pow ((double) t, (double) (NUM_OF_PTS - i - 1))
                       * mY[i]);
    result.z +=  ((float) pow ((double) t, (double) (NUM_OF_PTS - i - 1))
                       * mZ[i]);
  }

  return result;
}

/*****************************************************************************
FUNCTION spline::evaluate_all
PROGRAMMER - Keivan Khalichi
DATE PROGRAMMED - 10/16/94
DESCRIPTION - Returns the x, y, and z values for the t-parameter specified.
PARAMETERS
    t: The float t-parameter to evaluate x, y and z upon
    Returns spline_point in which x, y and z are set
CALLS: no other
*****************************************************************************/
spline_point spline::evaluate_all(float t)
{
  int i;
  spline_point result;

  // Set the x, y and z values to the last coefficient.
  result.x = round (mX[LAST_PTS_IDX]);
  result.y = round (mY[LAST_PTS_IDX]);
  result.z = (mZ[LAST_PTS_IDX]);

  // Multiply each ceofficient of mX, y and z (i.e. x_i, y_i and z_i) with
  // t^i.  Adding each result to the value of result.x, result.y and result.z.
  // Not multiplying the t^0 for optimization.
  for (i = 0; i < (NUM_OF_PTS - 1); i++) {
    result.x += round ((float) pow ((double) t, (double) (NUM_OF_PTS - i - 1))
                       * mX[i]);
    result.y += round ((float) pow ((double) t, (double) (NUM_OF_PTS - i - 1))
                       * mY[i]);
    result.z += ((float) pow ((double) t, (double) (NUM_OF_PTS - i - 1))
                       * mZ[i]);
  }

  return result;
}

void spline::DumpMG()
{
	cout << "Results of computing M . G:" << endl;
	for (int i=0; i<4; ++i)
		cout << mX[i] << " " << mY[i] << " " << mZ[0] << endl;
}		
