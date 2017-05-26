/*
** position.cpp - The new math. A position has three components: degrees,
**	minutes, and seconds. A sample_point has two positions (latitude
**	and longitude. The position class has arithmetic operators so that
**	they can be manipulated. Note that positive values for a position
**	signify "north of the equator" for latitude and "east of Greenwich"
**	for longitude. 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.20.98 DPT
*/

#include "position.h"
#include "bathApp.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

Position::Position()
{
	mDegrees = 0;
	mMinutes = 0;
	mSeconds = 0;
}

Position::~Position()
{

}

Position::Position(const Position &old)
{
	mDegrees = old.mDegrees;
	mMinutes = old.mMinutes;
	mSeconds = old.mSeconds;
}

Position::Position(float totalSeconds)
{
	mDegrees = (int)(totalSeconds / 3600);

	float remainder = totalSeconds - mDegrees * 3600;

	mMinutes = (int)(remainder / 60);
	remainder = remainder - mMinutes * 60;

	mSeconds = remainder;
}

/*
** This constructor is for like 45.098763 degrees.
*/
Position::Position(double fractionalDegrees)
{
	if (fractionalDegrees > 0)
	{
		mDegrees = (int)floor(fractionalDegrees); 
		double remainder = fractionalDegrees - mDegrees;
		mMinutes = (int)floor(remainder * 60);
		mSeconds = ((remainder * 60) - mMinutes) * 60;
		//cout << mDegrees << " " << mMinutes << " " << mSeconds << endl;
	} else
	{
		mDegrees = (int)ceil(fractionalDegrees); 
		double remainder = fractionalDegrees - mDegrees;
		mMinutes = (int)ceil(remainder * 60);
		mSeconds = ((remainder * 60) - mMinutes) * 60;
	}
}
 
const Position & Position::operator=(const Position &rhs)
{
	mDegrees = rhs.mDegrees;
	mMinutes = rhs.mMinutes;
	mSeconds = rhs.mSeconds;

	return *this;
}

int Position::operator <(const Position &rhs)
{
	if (mDegrees < rhs.mDegrees)
		return true ;
	  else if (mDegrees > rhs.mDegrees)
		return false ;
  	  else if (mMinutes < rhs.mMinutes)
		return true ;
	  else if (mMinutes > rhs.mMinutes)
		return false ;
	  else if (mSeconds < rhs.mSeconds)
		return true ;
	  else  
		return false ;
}

int Position::operator >(const Position &rhs)
{
	if (mDegrees > rhs.mDegrees)
		return true ;
	  else if (mDegrees < rhs.mDegrees)
		return false ;
  	  else if (mMinutes > rhs.mMinutes)
		return true ;
	  else if (mMinutes < rhs.mMinutes)
		return false ;
	  else if (mSeconds > rhs.mSeconds)
		return true ;
	  else  
		return false ;
}

int Position::operator ==(const Position &rhs)
{
	return ((mDegrees == rhs.mDegrees) &&
		(mMinutes == rhs.mMinutes) &&
		(mSeconds == rhs.mSeconds));
}



Position Position::operator+(const Position &rhs)
{
	Position retval;

	retval.mDegrees = mDegrees + rhs.mDegrees;
	retval.mMinutes = mMinutes + rhs.mMinutes;
	retval.mSeconds = mSeconds + rhs.mSeconds;

	return retval;
}

Position Position::operator-(const Position &rhs)
{
	Position retval;

	retval.mDegrees = mDegrees - rhs.mDegrees;
	retval.mMinutes = mMinutes - rhs.mMinutes;
	retval.mSeconds = mSeconds - rhs.mSeconds;

	return retval;
}

float Position::operator/(Position &rhs)
{
	return TotalSeconds()/rhs.TotalSeconds();
}

float Position::TotalSeconds() const
{
	return (mDegrees*3600)+(mMinutes*60)+mSeconds;
}

/*
** operator * - Take a position and multiple its components by the provided
**	scalar value. Used by the funky SamplePosition constructor that takes
**	800 parameters.
**
**	Seems like a prime spot for a numerical methods analysis - I'm sure
**	we must be accumulating error here.
**	
**	NB: YOU HAVE TO "NORMALIZE" THE RETURNED POSITION - IT MAY CONTAIN
**	INVALID VALUES IN SOME OF ITS FIELDS (I.E. MINUTES > 59, ETC.)
*/ 
Position Position::operator*(float scalar)
{
	float f_degrees = mDegrees * scalar;
	int i_degrees = (int)(mDegrees * scalar);  //truncates
	
	float remainder_deg = f_degrees-i_degrees; //gets the truncated decimal

	//carry the truncated bit over to the minutes.
	float f_minutes = (remainder_deg + (float)mMinutes) * scalar;
	int i_minutes = (int)((remainder_deg + (float)mMinutes) * scalar);
	
	float remainder_min = f_minutes-i_minutes; //gets the truncated decimal
	
	//and cascade the truncated minutes' decimal to the seconds (where it
	// is cool to be a float and proud of it.
	float f_seconds = (remainder_min + (float)mSeconds) * scalar;

	//return it	
	Position pos(i_degrees,i_minutes,f_seconds);

	return pos;
}

char * Position::Stringify(char *buf, int sz)
{
	//FIXME - no length check
	sprintf(buf,"%d %d %.2f", mDegrees, mMinutes, mSeconds);

	return buf;
}

/*
** class SamplePoint
*/

SamplePoint::SamplePoint()
{
	mDepth = 0;
}

SamplePoint::~SamplePoint()
{

}

SamplePoint::SamplePoint(const SamplePoint &old)
{
	mLat = old.mLat;
	mLong = old.mLong;
	mDepth = old.mDepth;
}

/*
** Constructor from a stringified version.
*/
SamplePoint::SamplePoint(char *str)
{
	//parse the output of a Stringify() call
	//latitude
	char *token;
	if (token = strtok(str," "))   {
		mLat.mDegrees = atoi(token);
	if (token = strtok(NULL," ")) {
		mLat.mMinutes = atoi(token);
	if (token = strtok(NULL," ")) {
		mLat.mSeconds = atof(token);

	//longitude
	if (token = strtok(NULL," ")) {
		mLong.mDegrees = atoi(token);
	if (token = strtok(NULL," ")) {
		mLong.mMinutes = atoi(token);
	if (token = strtok(NULL," ")) {
		mLong.mSeconds = atof(token);
		mDepth = 0;
		return;  //all went well
	}}}}}}

	// if we make it here something went wrong, just make a
	// blank one I guess.
	ASSERT( false )
	SamplePoint dummy;
	*this = dummy;
}

SamplePoint & SamplePoint::operator=(const SamplePoint &rhs)
{
	mLat = rhs.mLat;
	mLong = rhs.mLong;
	mDepth = rhs.mDepth;
	return *this;
}

/*
** == Note we don't compare depth here. (Though we could, I suppose)
*/
bool SamplePoint::operator==(const SamplePoint &oth)
{
	return (mLat == oth.mLat && mLong == oth.mLong);
}

/*
** operator+ - We have to implement this here because of weird things
**	that happen around the poles. For instance if you go up 10 degrees
**	from 85 degrees north lat. / 90 degrees west long. you actually end
**	up at 85 degrees north lat. / 90 degrees EAST long. (you go over the
**	north pole). Since adding two latitudes effects a change in longitude
**	we have to implement the offsetting operations at the SamplePoint level
**	(where both lat & long are available), rather than at the position 
**	level.
*/
SamplePoint SamplePoint::operator +(const SamplePoint &rhs)
{
	SamplePoint retval;

	//Latitude
	retval.mLat = mLat + rhs.mLat;

	//determine hemisphere - negative nos. are southern
	bool northern_hemisphere = NorthernOrEasternHemisphere(retval.mLat);

	if (northern_hemisphere)
		Normalize_N_or_E_position(retval.mLat);
	  else   // southern hemisphere - everything must be < 0.
		Normalize_S_or_W_position(retval.mLat);

	//Longitude
	retval.mLong = mLong + rhs.mLong;

	//determine hemisphere - negative nos. are Western
	bool eastern_hemisphere = NorthernOrEasternHemisphere(retval.mLong);
	if (eastern_hemisphere)
		Normalize_N_or_E_position(retval.mLong);
	  else   // eastern hemisphere - everything must be < 0.
		Normalize_S_or_W_position(retval.mLong);

	//OK, now the degrees
	//first, did we go across a pole? If so, add 180 to the longitude
	//FIXME - Watch the poles and the date line!!

	return retval;   
}

SamplePoint SamplePoint::operator -(const SamplePoint &rhs)
{
	SamplePoint retval;

	//Latitude
	retval.mLat = mLat - rhs.mLat;

	//determine hemisphere - negative nos. are southern
	bool northern_hemisphere = NorthernOrEasternHemisphere(retval.mLat);

	if (northern_hemisphere)
		Normalize_N_or_E_position(retval.mLat);
	  else   // southern hemisphere - everything must be < 0.
		Normalize_S_or_W_position(retval.mLat);

	//Longitude
	retval.mLong = mLong - rhs.mLong;

	//determine hemisphere - negative nos. are Eastern
	bool western_hemisphere = NorthernOrEasternHemisphere(retval.mLong);
	if (western_hemisphere)
		Normalize_N_or_E_position(retval.mLong);
	  else   // eastern hemisphere - everything must be < 0.
		Normalize_S_or_W_position(retval.mLong);

	//FIXME - Watch the poles and the date line!!


#ifdef _DEBUG
	//debug verify that both Positions are either all pos or all neg...
	if (!(((retval.mLat.mSeconds >=0)&&
		(retval.mLat.mMinutes >= 0)&&
		(retval.mLat.mDegrees >= 0))
	   || ((retval.mLat.mSeconds <=0)&&
		(retval.mLat.mMinutes <= 0)&&
		(retval.mLat.mDegrees <= 0)))
	   &&(((retval.mLong.mSeconds>=0)&&
		(retval.mLong.mMinutes>=0)&&
		(retval.mLong.mDegrees>=0))
	   || ((retval.mLong.mSeconds<=0)&&
		(retval.mLong.mMinutes<=0)&&
		(retval.mLong.mDegrees<=0))))
	printf("Position offset error: mismatch. %d %d %f / %d %d %f\n",
		retval.mLat.mDegrees,retval.mLat.mMinutes,
		retval.mLat.mSeconds, retval.mLong.mDegrees,
		retval.mLong.mMinutes,retval.mLong.mSeconds);
#endif

	return retval;   
}



bool SamplePoint::NorthernOrEasternHemisphere(const Position &pos)
{
	//determine hemisphere - negative nos. are Eastern & southern
	bool pos_hemisphere;
	if (pos.mDegrees > 0)
		pos_hemisphere = true;
	else if (pos.mDegrees < 0)
		pos_hemisphere = false;
	else if (pos.mMinutes > 0)
		pos_hemisphere = true;
	else if (pos.mMinutes < 0)
		pos_hemisphere = false;
	else if (pos.mSeconds >= 0)
		pos_hemisphere = true;
	else //must be 0D 0' -x" or the equator or Greenwich line thingy 
		pos_hemisphere = false;

	return pos_hemisphere;
}

void SamplePoint::Normalize_N_or_E_position(Position &pos)
{
	//make sure seconds are between 0 & 60
	if (pos.mSeconds > 59)
	{
		pos.mSeconds -= 60;
		pos.mMinutes++;
	}
	else if (pos.mSeconds < 0)
	{
		pos.mSeconds += 60;
		pos.mMinutes--;
	}

	//make sure minutes are between 0 & 60
	if (pos.mMinutes > 59)
	{
		pos.mMinutes -= 60;
		pos.mDegrees++;
	}
	else if (pos.mMinutes < 0)
	{
		pos.mMinutes += 60;
		pos.mDegrees--;
	}
}


void SamplePoint::Normalize_S_or_W_position(Position &pos)
{
	//make sure seconds are between -60 & 0
	if (pos.mSeconds < -59)
	{
		pos.mSeconds += 60;
		pos.mMinutes--;
	}
	else if (pos.mSeconds > 0)
	{
		pos.mSeconds -= 60;
		pos.mMinutes++;
	}

	//make sure minutes are between -60 & 0
	if (pos.mMinutes < -59)
	{
		pos.mMinutes += 60;
		pos.mDegrees--;
	}
	if (pos.mMinutes > 0)
	{
		pos.mMinutes -= 60;
		pos.mDegrees++;
	}
}

void SamplePoint::Normalize_position(Position &pos)
{
	if (pos.TotalSeconds() > 0)
		Normalize_N_or_E_position(pos);
	else
		Normalize_S_or_W_position(pos);
}

/*
** GetX/Y - Used to convert world coordinates to screen coordinates.
**	Convert the location of the sample point WRT the total screen
**	size to a percentage and use this percentage to calculate the
**	spot in the given width range.
**
**	With regards to spanning hemispheres and the like, we should assert
**	that the size parameter has the same "fingerprint" as the upperleft
**	parameter. IOW, if the upperleft has a positive lat and a negative lon,
**	the size should as well, even if the bottom right corner has a negative
**	lat and positive lon.
*/
int SamplePoint::GetX(SamplePoint *upperleft, 
			SamplePoint *size, int width)
{
	SamplePoint location = *upperleft - *this;
	int retval =  (int)((location.mLong / size->mLong) * width);
	return retval;
}
	
int SamplePoint::GetY(SamplePoint *upperleft, 
			SamplePoint *size, int height)
{
	ASSERT( (size->mLat.TotalSeconds() > 0 && upperleft->mLat.TotalSeconds() > 0) \
		||  (size->mLat.TotalSeconds() < 0 && upperleft->mLat.TotalSeconds() < 0) )
	SamplePoint location = *upperleft - *this;
	return (int)((location.mLat / size->mLat) * height);
}

//@REX We should provide a GetXY that allows the caller to pass in refs to two ints.

void SamplePoint::Dump()
{
	printf("LatD %d LatM %d LatS %4.2f  -- \n\t\tLongD %d LongM %d LongS %4.2f Depth %f\n",
		mLat.mDegrees, mLat.mMinutes, mLat.mSeconds, 
		mLong.mDegrees, mLong.mMinutes, mLong.mSeconds, mDepth);
}

/*
** Stringify - Put contents into char array. Used by persistent store of user
**	preferences.
*/
char * SamplePoint::Stringify(char *buf, int sz)
{
	//FIXME - no length check
	sprintf(buf,"%d %d %.2f %d %d %.2f",
		mLat.mDegrees, mLat.mMinutes, mLat.mSeconds,
		mLong.mDegrees, mLong.mMinutes, mLong.mSeconds);

	return buf;
}

/*
** Scale - Given a SamplePoint, compute a new one based on the pixel offset and
**	the scale (in seconds per pixel). The best way to use this is get the upperLeft,
**	xscale and yscale from the preferences and the x and y positions of where you want
**	on the screen. Clone the upperleft with the = operateor and call this method on the
**	new sample point!
*/
void SamplePoint::Scale(int xOffset, int yOffset, float xScale, float yScale)
{
	ASSERT ( xScale > 0 )
	ASSERT ( xScale > 0 )
	
	float newLatSecs = mLat.TotalSeconds() - ((float)yOffset * yScale);
	Position newLat(newLatSecs);
 	float newLongSecs;
	mLat = newLat;

	//normalize
	if (mLat.mMinutes > 0)  //northern hemisphere
	{
		Normalize_N_or_E_position(mLat);
		//newLongSecs = mLong.TotalSeconds() - ((float)xOffset * xScale);
	}
	else
	{
		Normalize_S_or_W_position(mLat);
		//newLongSecs = mLong.TotalSeconds() + ((float)xOffset * xScale);
	}

	newLongSecs = mLong.TotalSeconds() + ((float)xOffset * xScale);
	//cout << "old long secs " << mLong.TotalSeconds() << " xoffset " << xOffset << " xScale " << xScale << endl;
	Position newLong(newLongSecs);
 
	mLong = newLong;

	//normalize
	if (mLong.mMinutes > 0)  //eastern hemisphere
		Normalize_N_or_E_position(mLong);
	  else
		Normalize_S_or_W_position(mLong);
}

