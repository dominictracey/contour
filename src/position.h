/*
** position.h - The new math! A position has three components: degrees,
**	minutes, and seconds. A sample_point has two positions (latitude
**	and longitude. The position class has arithmetic operators so that
**	they can be manipulated. Note that positive values for a position
**	signify "north of the equator" for latitude and "east of Greenwich"
**	for longitude. 
**
**	@REX Position should be an abstract base class with LatPosition
**	and LongPosition implementing the pure virtual operators. This is 
**	because	latitude maxes out at 90 while the maximum longitude is 180.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.15.98 DPT
*/

#ifndef _POSITION_H_
#define _POSITION_H_

#include <vector>

// A bitmap thing for doing basic compass points
//  usage:
//	int northwest;
//	northwest &= _NORTH;
//	northwest &= _WEST;
//	...
//	if (northwest & _NORTH)
//	...
#define _NORTH 1
#define _EAST 2
#define _SOUTH 4
#define _WEST 8

class Position 
{
friend class SamplePoint;
public:
	Position();
        virtual ~Position();
	Position(const Position &old);
	Position(float totalSecs);
	Position(double fractionalDegrees);
	Position(int d,int m,float s) : mDegrees(d),mMinutes(m),mSeconds(s) {};
	const Position & operator=(const Position &rhs);
	virtual Position operator+(const Position &rhs);
	virtual Position operator-(const Position &rhs);
	int operator <(const Position &rhs);
	int operator >(const Position &rhs);
	int operator ==(const Position &rhs);
	float operator /(Position &rhs);
	Position operator *(float scalar);
	float TotalSeconds() const;
	int GetDegrees() const {return mDegrees;}
	int GetMinutes() const {return mMinutes;}
	float GetSeconds() const {return mSeconds;}
	char * Stringify(char *buf, int sz);
protected:
	int mDegrees;
	int mMinutes;
	float mSeconds;
};


class SamplePoint
{
friend class MGD77Extractor;

public:
	SamplePoint();
	virtual ~SamplePoint();
	SamplePoint(const SamplePoint &old);
	SamplePoint(int td,int tm,float ts,int gd,int gm,float gs,float depth) :
		mLat(td,tm,ts), mLong(gd,gm,gs), mDepth(depth) {};
	SamplePoint(const Position & lat, const Position & lon) : mLat(lat), mLong(lon) {};
	SamplePoint(char *str);

	SamplePoint & operator=(const SamplePoint &rhs);
	bool operator==(const SamplePoint &oth);

	SamplePoint operator+(const SamplePoint &rhs);
	SamplePoint operator-(const SamplePoint &rhs);
	int GetX(SamplePoint *upperleft, 
		 SamplePoint *size, int width);
	int GetY(SamplePoint *upperleft, 
		 SamplePoint *size, int height);
	void Dump();
	float GetDepth() {return mDepth;}
	void SetDepth(float z) {mDepth=z;}
	char * Stringify(char *buf, int sz);

	void Scale(int xOffset, int yOffset, float xScale, float yScale);
	virtual SamplePoint * Clone() { return new SamplePoint; }
	const Position & GetLatitude() { return mLat; }
	const Position & GetLongitude() { return mLong; }
	static void Normalize_position(Position &pos);
	
protected:
	static void Normalize_N_or_E_position(Position &pos);
	static void Normalize_S_or_W_position(Position &pos);
	Position mLat;
	Position mLong;
	float mDepth;
private:
	bool NorthernOrEasternHemisphere(const Position &pos);
};

class GridPoint : public SamplePoint
{
public:
	GridPoint() {};
	GridPoint(Position lat, Position lon) : SamplePoint(lat, lon) {};
	GridPoint(const SamplePoint & old) : SamplePoint(old) {};
	virtual ~GridPoint() {};
	void x(int X) {mx = X;}
	int x() {return mx;}
	void y(int Y) {my = Y;}
	int y() {return my;}
	virtual SamplePoint * Clone() {return new GridPoint();}
	void Recalculate(SamplePoint *ul, SamplePoint *size, int width, int height)
		{
			x(GetX(ul,size,width));
			y(GetY(ul,size,height));
		}
protected:
	int mx,my;
};


//an stl vector of sample points is...
typedef vector<SamplePoint *> SamplePointVector;
typedef vector<GridPoint *> GridPointVector;

#endif

