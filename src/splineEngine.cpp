/*
** SplineEngine.cpp - Use Overhauser (Catmull-Rom) B-splines for surface
**	generation 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.21.98 DPT
*/

#include "splineEngine.h"
#include "canvas.h"
#include <stdio.h>
#include <math.h>
#include "extractor.h"
//#include "mgd77Extractor.h"
#include "spline_forwarddifference.h"
#include "appFactory.h"
#include "preferences.h"
#include "timer.h"

#define PI 3.14159

SplineEngine::SplineEngine()
{
	SamplePoint proto;
	AppFactory::GetInstance()->GetApp()->GetExtractor()->Initialize("data/42_068.nos",proto);
}

SplineEngine::~SplineEngine()
{
		
}

/*
** SetPoints - This is the "hard" data that we are provided with. It is
**	in an STL vector of SamplePoints. We dump it into our array.
**	Note that we need to know the area of the real world that our array
**	represents to do this.
*/
void SplineEngine::SetPoints(SamplePointVector &v, int x1, int x2, int y1, int y2)
{

	Timer::getInstance()->mark("Begin SplineEngine::SetPoints");
	BathApp *app = AppFactory::GetInstance()->GetApp();
	SamplePoint upperLeft = app->GetPreferences()->GetUpperLeft();
	float xscale = app->GetPreferences()->GetXScale();
	float yscale = app->GetPreferences()->GetYScale();
	SamplePoint lowerRight(upperLeft);
	lowerRight.Scale(mWidth, mHeight, xscale, yscale);

	SamplePoint size = upperLeft - lowerRight;

	SamplePointVector::iterator it = v.begin();
	//a vector to keep spline data in.
	SplineVector splineV;
	SplineVector::iterator svit;
	spline_point *sp;

	for (it = v.begin(); it != v.end(); it++)
	{
		//For each sample point, we ask it for its x and y
		// coordinates, given the current viewport settings
		// See SamplePoint::Get[X|Y] for more details.
		int x = (*it)->GetX(&upperLeft, &size, mWidth);
		int y = (*it)->GetY(&upperLeft, &size, mHeight);
		mArray[x][y] = (*it)->GetDepth();
		sp = new spline_point(x, y, mArray[x][y]);
		splineV.push_back(sp);
	}
	Render(x1,x2,y1,y2);
	Timer::getInstance()->mark("New points incorporated");
	//Interpolate all the sample points
	TrackSpline(splineV);
	Render(x1,x2,y1,y2);
	Timer::getInstance()->mark("TrackSpline Complete");	
	//FIXME - mem leak - delete final contents of splineV
	
	//See whether its better to go with a vertical framework
	// filled in horizontally or vice versa...
	bool vib = VerticalFrameworkIsBetter();

	//Now create a few edge-to-edge splines that serve as the 
	// skeleton upon which we fill the rest of the array in (using
	// interpolating lines that run perpindicular to the framework).
	// These are done as parallel sine waves to give better coverage.
	CreateSineFramework(vib,77,x1+EDGE_BUFFER,x2-EDGE_BUFFER,y1+EDGE_BUFFER,y2-EDGE_BUFFER);
	Render(x1,x2,y1,y2);
	Timer::getInstance()->mark("SineFrame complete");
	//Finally, fill in the surface
	int numRibs;
	if (vib)
		numRibs = y2-y1;
	  else
		numRibs = x2-x1;
	CreateFramework(~vib,numRibs,x1,x2,y1,y2);
	Timer::getInstance()->mark("End SplineEngine::SetPoints");

}

/*
** TrackSpline - Fill in all the sampling tracks with Overhauser splines
*/
bool SplineEngine::TrackSpline(SplineVector &splineV)
{
	//fill in along the tracks with splines.
	SplineVector::iterator svit;
	SplineForwardDifference brentSpliner;
	svit = splineV.begin();
	//initialize the four positions needed by the spline.
	for (int i = 0; i < 4; ++i)
	{
		if (svit)
			brentSpliner.set_value(*svit++,i);
		  else
		  {
			printf("No data points available\n");
			return false;   //FIXME - no points?
		  }
	}

	//Since we need four control points to evaluate between
	// the second and third, we cannot evaluate between the
	// last and second to last point.
	SplineVector::iterator stopAt = splineV.end();
	stopAt--; stopAt--;
	//now use the spline to fill in the values
	float distance;
	spline_point *second, *third; //used for determining distance
	for (;svit != stopAt;svit++)
	{
		//FIXME - we miss the first chance here I think.
		brentSpliner.bump_along(*svit);
		//use distance to determine the number of evaluations
		//to do between the 2nd and 3rd control points.
		second = brentSpliner.get_value(1);
		third = brentSpliner.get_value(2);
		// D = sqrt(x^2 + y ^2)
		distance = sqrt(pow(second->x-third->x,2) + 
				pow(second->y-third->y,2));
		distance += 7;
		if (distance < 200)
		{
			brentSpliner.compute_all();
			brentSpliner.Initialize((int)distance);

			//now vary the spline parameter from t=1/distance to
			// distance-1/distance.
			spline_point p;
			for(int i=0; i<(int)distance; ++i)
			{
				brentSpliner.Step(p);
				//p.Dump();
				if (p.x<mWidth && p.x>0 && p.y<mHeight && p.y>0)
					mArray[p.x][p.y] = p.z;
			}
		}
	}
}

/*
** VerticalFrameworkIsBetter - Returns true if we feel that most of the 
**	track lines run east-west. Returns false if they are more northish-
**	southish. It is important for the framework to be constructed perpin-
**	dicular to the general pattern of the tracklines since this will give
**	use more sample points to interpolate with our splines and thus give
**	us better surface fidelity.
*/
bool SplineEngine::VerticalFrameworkIsBetter()
{
	int i,j,numVertHits = 0, numHorzHits = 0;

	//check the vertical hits in 7 places
	for (i = 0; i <= mWidth; i+=mWidth/7)
		for (j=0; j < mHeight;j++)
			if (mArray[i][j] != NO_DEPTH)
				numVertHits++;

	//do the same for the horizontal 
	for (i = 0; i <= mHeight; i+=mHeight/7)
		for (j=0; j < mWidth;j++)
			if (mArray[j][i] != NO_DEPTH)
				numHorzHits++;

	//compare them, taking into account the aspect ratio of the canvas
	if ((float)numVertHits/(float)numHorzHits>(float)mHeight/(float)mWidth)
		return true;
	  else
		return false;
}

/*
** CreateFramework - Fill in some "ribs" into the array, either vertically or
**	horizontally, depending on the parameter.
*/
bool SplineEngine::CreateFramework(bool vertical, int numRibs, 
					int x1, int x2, int y1, int y2)
{
	int i,j;
	SplineForwardDifference spliner;
	int splineIndex;
	bool initialized = false;
	float check;

	int MaxDim1, MinDim1, MaxDim2, MinDim2;
		
	if (vertical)
	{
		MinDim1 = x1;
		MaxDim1 = x2;
		MinDim2 = y1;
		MaxDim2 = y2;
	}
  	  else  //horizontal
	  {
		MinDim1 = y1;
		MaxDim1 = y2;
		MinDim2 = x1;
		MaxDim2 = x2;
	  }
		

	for (i=MinDim1;i<MaxDim1;i+=MaxDim1/numRibs)
	{
	  splineIndex = 0;
	  for(j=MinDim2;j<MaxDim2 && !initialized; j++)
	  {
	    if (vertical)
		check = mArray[i][j];
	      else
		check = mArray[j][i];
	    if (check != NO_DEPTH)
	    {
		spline_point *p;
		if (vertical)
		  p = new spline_point(i,j,mArray[i][j]);
		 else
		  p = new spline_point(j,i,mArray[j][i]);
		spliner.set_value(p,splineIndex++); 	
		//if we have the 4 spline control points we can break
		// out of the loop and move on to the interpolation step
		if (splineIndex == 4)
			initialized = true;
	    }
	  }

	  //make sure we have the 4 points necessary.
	  while (initialized)
	  {
		if (vertical)
		{
		  float distance = 
		      (float)spliner.get_value(2)->y -
		      (float)spliner.get_value(1)->y;		
		  spliner.compute_all();
		  spliner.Initialize((int)distance);
		  int schlep = spliner.get_value(1)->y+1;
		  spline_point p;
		  for (int h=0; h < (int)distance; ++h)
		  {
		   	spliner.Step(p);
			if (p.y < mHeight && p.y >= 0) {
				// Experiment -- to try to smooth out the
				// "rastering" - average the new value with
				// any found around it.
				// ** THIS IS RUN **
				int a = i;
				int b = schlep+1;
				float c = p.z;
				float sz = Smooth(a,b,c);
		   		mArray[i][schlep++] = sz;
			}
		  }
		}
		else //horizontal
		{
		  float distance = 
		      (float)spliner.get_value(2)->x -
		      (float)spliner.get_value(1)->x;		
		  spliner.compute_all();
		  spliner.Initialize((int)distance);
		  spline_point p;
		  int schlep = spliner.get_value(1)->x+1;
		  for (int h=0; h < (int)distance; ++h)
		  {
		   	spliner.Step(p);
			if (p.x < mWidth && p.x >= 0)  {
				// Experiment -- to try to smooth out the
				// "rastering" - average the new value with
				// any found around it.
				int a = schlep+1;
				int b = i;
				float c = p.z;
				float sz = Smooth(a,b,c);
		   		mArray[schlep++][i] = sz;
			}
		  }
		}	

		//now find the next point to work with.
		bool found = false;
		int q;
		if (vertical)
		{
		  for (q=spliner.get_value(3)->y + 1;q < y2 && !found;++q)
		  { 
		    if (mArray[i][q] != NO_DEPTH)
			found = true;
		  }
		}
		else  //horizontal
		{
		  for (q=spliner.get_value(3)->x + 1;q < x2 && !found;++q)
		  { 
		    if (mArray[q][i] != NO_DEPTH)
			found = true;
		  }
		}
	
		//if we found another candidate, add it into the mix	
		spline_point *candidate;
		if (vertical && found)
			candidate = new spline_point(i,q-1,mArray[i][q-1]);
 		  else
			candidate = new spline_point(q-1,i,mArray[q-1][i]);
			
		if (found)
		 spliner.bump_along(candidate);
		else
		 initialized = false;	
	  }
	}

     return true;
}

/*
** CreateSineFramework - Fill in some sinusoidal "ribs" into the array, 
*/
bool SplineEngine::CreateSineFramework(bool vertical, int numRibs, 
					int x1, int x2, int y1, int y2)
{
	int i,j;
	SplineForwardDifference spliner;
	spline_point *second, *third; //used for determining distance
	int splineIndex;
	bool initialized = false;
	float check,distance;
	char ch;

	int MaxDim1, MinDim1, MaxDim2, MinDim2;
	// Dim1 is the dimension the sin waves vary in.
	// Dim2 is the direction that the sin waves propogate along.
	if (vertical)
	{
		MinDim1 = x1;
		MaxDim1 = x2;
		MinDim2 = y1;
		MaxDim2 = y2;
	}
  	else  //horizontal
	{
		MinDim1 = y1;
		MaxDim1 = y2;
		MinDim2 = x1;
		MaxDim2 = x2;
	}

	//printf("Sining on with vert %d (%d,%d) (%d,%d)\n",vertical,x1,y1,x2,y2);
	
	// amp is the amplitude of the sine func.
	// stretch is the period of the sine func.	
	BathApp *app = AppFactory::GetInstance()->GetApp();
	int amp = app->GetPreferences()->GetSineAmplitude();
	numRibs = app->GetPreferences()->GetNumberSines(); 
	int max_distance = 200; //@REX should also be a pref 

	// The user preference "sine frequency" 
	int stretch = app->GetPreferences()->GetSineFrequency(); 
	
	// make sure that we aren't trying to do more passes than we have
	// area to cover: can't do 700 sine waves in 100 pixels.
	if (numRibs > (MaxDim1-MinDim1-(2*amp)))
		numRibs=(MaxDim1-MinDim1-2*amp);

 	// i represents the number of sine waves --
 	// y for horizontal, x for vertical.
	for (i=MinDim1+amp;i<MaxDim1-amp;i+=MaxDim1/numRibs)
	{
	  // Phase one is initialize the B-spline 4 sample points
	  splineIndex = 0;
	  // j represents the constant axis of the sine wave --
	  // x for horizontal, y for vertical.
	  for(j=MinDim2;j<MaxDim2 && !initialized; j++)
	  {
		// The user pref "sine frequency" is expressed in cycles that fit
		// within the viewport. We therefore need to figure out the size of the viewport
		// and determine what we need to multiply our radians parameter to the sin
		// math function so we get the right number of cycles. Since 2 * PI is equal
		// to one cycle and the viewport size is equal to MaxDim2-MinDim2, we vary the
		// parameter being passed to sin from 0 -> (2 * PI) for the user defined sine freq
		// as j moves from MinDim2 to MaxDim2.
	    int k = int(sin((double)((double)(j - MinDim2)/(double)(MaxDim2 - MinDim2)) * (double)(2* PI) * (double)stretch) * (double)amp);
//	    cout  << j << " " << sin((double)((double)(j - MinDim2)/(double)(MaxDim2 - MinDim2)) * (double)(2* PI) * (double)stretch) * (double)amp << " ";
	    if (vertical)
			check = mArray[i+k][j];
//			mArray[i+k][j] = 200;
		else
			check = mArray[j][i+k];
//			mArray[j][i+k] = 200;

	    if (check != NO_DEPTH)
	    {
			spline_point *p;
			if (vertical)
				p = new spline_point(i+k,j,mArray[i+k][j]);
			else
				p = new spline_point(j,i+k,mArray[j][i+k]);
			spliner.set_value(p,splineIndex++); 	
			//if we have the 4 spline control points we can break
			// out of the loop and move on to the interpolation step
			if (splineIndex == 4)
				initialized = true;
	    }
	  }

	  // Phase two is to use B-splines to interpolate between samples 2 & 3.
	  //make sure we have the 4 points necessary.
	  while (initialized)
	  {
		//set up matrix
		spliner.compute_all();
		//use distance to determine the number of evaluations
		//to do between the 2nd and 3rd control points.
		second = spliner.get_value(1);
		third = spliner.get_value(2);
		// D = sqrt(x^2 + y ^2)
		distance = sqrt(pow(second->x-third->x,2) + 
				pow(second->y-third->y,2));
		distance += 7;
		spliner.Initialize((int)distance);
		//if the distance is larger than this number,
		// it might be too wacky to interpolate.
		//@REX This ceiling should be a preference (albeit advanced)
		//if (distance < (float)max_distance)
		//{
		  //now vary the spline parameter from t=1/distance to
		  // distance-1/distance.
		//cout << "distance from (" << second->x << "," << second->y << ") to (" << third->x << "," << third->y << ") is " << distance << endl;
		spline_point p;
		for(int h=0; h< (int)distance; ++h)
		{
			spliner.Step(p);
			if (p.x<x2 && p.x>x1 && p.y<y2 && p.y>y1) {
				//printf("Adding point x %d y %d as %4.2f\n",p.x,p.y,p.z);
				mArray[p.x][p.y] = p.z;
			}
		  }
		//}

		//now find the next point to work with.
		int start;
		if (vertical)
			start = spliner.get_value(3)->y + 1;
		else
			start = spliner.get_value(3)->x + 1;
		
		bool found = false;
		int q,k;
		for (q=start; q < MaxDim2 && !found; q++)
		{ 
	    	k = int(sin((double)((double)(q - MinDim2)/(double)(MaxDim2 - MinDim2)) * (double)(2* PI) * (double)stretch) * (double)amp);
			if (vertical)
			{
				//printf("Checking point x %d y %d as %4.2f\n",k+i,q,mArray[k+i][q]);
		    	if (mArray[k+i][q] != NO_DEPTH)
					found = true;
			}
			else  //horizontal
			{
				//printf("Checking point x %d y %d as %4.2f\n",q,k+i,mArray[q][k+i]);
				if (mArray[q][k+i] != NO_DEPTH)
					found = true;	
		    }
		}
		//if we found another candidate, add it into the mix	
		if (found) {
			q--; //?
			spline_point *candidate;
			if (vertical)
				candidate = new spline_point(k+i,q,mArray[k+i][q]);
 		  	else
				candidate = new spline_point(q,k+i,mArray[q][k+i]);
			//cout << "Found (" << candidate->x << "," << candidate->y << "," << candidate->z << ")\n";
		 	spliner.bump_along(candidate);
		}
		else
			initialized = false;	
	  }
	}

	//cout << "Done sining" << endl;
	return true;
}


/*
** Smooth - Average a value with the values of points found around it.
**
**	There is a lower weight to values
**	that are not directly adjacent and a user configureable "smoothing 
**	factor" that goes from 0 (no smoothing) to x where the value returned
**	= weighted average where points adjacent to the target are weighted at
**	x and the values at points y away are weighted at x-y.
*/
float SplineEngine::Smooth(int x, int y, float z)
{
	//smooth is a user pref
	BathApp *app = AppFactory::GetInstance()->GetApp();
	int smooth = app->GetPreferences()->GetSmoothingFactor();
	float total = z*(smooth+1);
	int vals = smooth+1;
	int width=GetWidth();
	int height=GetHeight();
	int i,j,k;
//	printf("For point x %d y %d we used: ",x,y);
	i=1;
	for (i=1;i<smooth+1;++i) {
		//top
		j=y-i;
		for (k=x-i;k<x+i+1;++k) {
			if (k>0 && j>0 && k<width && j<height && mArray[k][j]!=NO_DEPTH) 
			{
				total += mArray[k][j]*(smooth-i+1);
//				printf("->top x %d y %d i %d<-",k,j,i);
				vals += smooth-i+1;	
			}
		}
		//bottom
		j=y+i;
		for (k=x-i;k<x+i+1;++k) {
			if (k>0 && j>0 && k<width && j<height && mArray[k][j]!=NO_DEPTH) 
			{
				total += mArray[k][j]*(smooth-i+1);
	//			printf("->bottom x %d y %d <-",k,j);
				vals += smooth-i+1;	
			}
		}	
		//right
		j=x+i;
		for (k=y-i+1;k<y+i;++k) {
			if (k>0 && j>0 && k<height && j<width && mArray[j][k]!=NO_DEPTH) 
			{
				total += mArray[j][k]*(smooth-i+1);
	//			printf("->right  x %d y %d <-",j,k);
				vals += smooth-i+1;	
			}
		}	
		//left
		j=x-i;
		for (k=y-i+1;k<y+i;++k) {
			if (k>0 && j>0 && k<height && j<width && mArray[j][k]!=NO_DEPTH) 
			{
				total += mArray[j][k]*(smooth-i+1);
	//			printf("->left x %d y %d <-",j,k);
				vals += smooth-i+1;	
			}
		}	
	}
//	printf("In %4.2f, out %4.2f total %f vals %d\n",z,total/vals,total,vals);
	return float(total/vals);
}
