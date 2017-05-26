/*
** mgd77Extractor.cpp - Data extractor for the MGD77 text file format for
**	bathymetric data described in data subdirectory. 
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
** 12.20.98 DPT
*/

#include "mgd77Extractor.h"
#include "position.h"
#include <stdio.h>

MGD77Extractor::MGD77Extractor()
{

}

MGD77Extractor::~MGD77Extractor()
{

}

/*
** GetSamplePoints - Given the upper left and lower right coords,
**	return all the points that are within the described rectangle.
*/
SamplePointVector * MGD77Extractor::GetSamplePoints(SamplePoint *ul,
					    SamplePoint *lr)
{
	SamplePointVector *retval = new SamplePointVector();
	int count = 0;
	SamplePointVector::iterator it = mVector.begin();
	while (it != mVector.end())
	{
		// @FIXME - why doesn't SP::> work?
		if (((*it)->mLat < ul->mLat) && 
		    (lr->mLat < (*it)->mLat) && 
		    (lr->mLong > (*it)->mLong) && 
		    ((*it)->mLong > ul->mLong))
		{
			retval->push_back(*it);
		}
		*it++;
	}

	//cout << "Found " << retval->size() << " points for " << endl;
	//ul->Dump();
	//lr->Dump();
	
	return retval;
}

/*
** Initialize - Open the passed data file and initialize our internal
**	vector with the smaple points retrieved from it. 
**
**	The prototype parameter allows us to pass in an instance of a subclass 
**	of SamplePoint and have prototype.Clone() used instead of new SamplePoint.
*/
void MGD77Extractor::Initialize(char *filename, SamplePoint & prototype)
{
	FILE *fp;
	fp = fopen(filename, "r");
	//int count = 0;
	if (fp)
	{
		SamplePoint *sp;
		char buf[10];
		while (!feof(fp))
		{
			//sp = new SamplePoint;
        		char    buffer[40]; // buffer in which we read variables
        		int     i,      // field index
                		j,      // buffer index
        		// temporary point member storage
                	rnumber, latdeg, latmin, londeg, lonmin;
        		float   latsec, lonsec, depth;

        		// initialize the buffer
        		for (i = 0; i < 40; i++) {
                		buffer[i] = ' ';
        		}

        		// ok start reading the line...
        		j = 0;
        		// read in the 5 digit registry number
        		for (i = 1; i <= 5; i++) 
                		buffer[j++] = getc(fp);
        		buffer[j++] = ' ';

        		// read the spaces
        		for (i = 6; i <= 11; i++)
                		getc(fp);

        		// read in the latitude degrees
        		for (i = 12; i <= 13; i++)
                		buffer[j++] = getc(fp);
        		buffer[j++] = ' ';

        		// read in the latitude minutes
        		for (i = 14; i <= 15; i++)
                		buffer[j++] = getc(fp);
        		buffer[j++] = ' ';

        		// read in the latitude seconds
        		for (i = 16; i <= 17; i++)
                		buffer[j++] = getc(fp);
        		buffer[j++] = '.';
        		for (i = 18; i <= 19; i++)
                		buffer[j++] = getc(fp);
        		buffer[j++] = ' ';

        		// read in the longitude degrees
        		buffer[j++] = '-';
        		for (i = 21; i <= 23; i++)
                		buffer[j++] = getc(fp);
        		buffer[j++] = ' ';

        		// read in the longitude minutes
        		buffer[j++] = '-';
        		for (i = 25; i <= 26; i++)
                		buffer[j++] = getc(fp);
        		buffer[j++] = ' ';

        		// read in the longitude seconds
        		buffer[j++] = '-';
        		for (i = 28; i <= 29; i++)
                		buffer[j++] = getc(fp);
        		buffer[j++] = '.';
        		for (i = 31; i <= 32; i++)
                		buffer[j++] = getc(fp);
        		buffer[j++] = 0;

			char chardepth[9];
			chardepth[5] = 0;
			int k;
			for (k = 0; k < 5; k++)
				chardepth[k] = getc(fp);

			char cartcode[4];
			for (k = 0; k < 3; k++)
				cartcode[k] = getc(fp);

			//interpret cartcode:
			// 129  SOUNDING (WHOLE FATHOMS)
			// 130  SOUNDING (FATHOMS & TENTHS)
			// 131  SOUNDING (FATHOMS & FRACTION)
			// 
			// There is something about survey 6564 that is weird. Depths are
			// coming out with a 6 appended to them (i.e. 102 -> 1026)
			int ncc;
			sscanf(cartcode,"%d",&ncc);
			if (ncc == 129)
			{
				int intdepth;
				sscanf(chardepth,"%d",&intdepth);
				depth = (float)intdepth;
			}
			else if (ncc == 130)
			{
				//one decimal place
				chardepth[5] = chardepth[4];
				chardepth[4] = '.';
				sscanf(chardepth,"%f",&depth);
			}
			else if (ncc == 131)
			{
				//two decimal places?
				chardepth[5] = chardepth[4];
				chardepth[4] = chardepth[3];
				chardepth[3] = '.';
				sscanf(chardepth,"%f",&depth);
			}
			else  //can be weird stuff like "Mud" or "Very small rocks"
			{
				//FIXME - we should keep this stuff. There
				// should be subclasses of SamplePoint that 
				// are used for bottom composition and other
				// "marks" like wrecks or big trees or whatever.
				// Along these lines, the thing that we are 
				// calling a SamplePoint should probably not 
				// have a depth member but should rather only
				// have the lat and long and the subclass
				// DepthSamplePoint should have the depth
				// member.
#ifdef _DEBUG
	//			printf("depth -%d- cc -%d-\n",depth,ncc);
#endif
				depth = BAD_CARTCODE_DEPTH;
			}
			
			//the \n at the end of the line
                	getc(fp);
                	getc(fp);
			
			if ( depth != BAD_CARTCODE_DEPTH)
			{
				int surveyNumber;
        			sscanf(buffer, " %d %d %d %f %d %d %f", 
                			&rnumber, &latdeg, &latmin, &latsec, 
							&londeg, &lonmin, &lonsec);
				SamplePoint *tmp = new SamplePoint(latdeg, latmin, latsec,
                                        londeg, lonmin, lonsec, depth);
                //allow subclasses of SamplePoint through prototype param.
                sp = prototype.Clone();
                //@FIXME - gross inefficiency
                //Position lat(latdeg,latmin,latsec);
                //Position longitude(londeg,lonmin,lonsec);
                //sp->mLat = lat;
                //cout << "sp ";
                //cout << " local lat ";
                //sp->mLong = longitude;
                //sp->mDepth = depth;
                *sp = *tmp;
                //sp->Dump();
                delete tmp;
               // ++count;
				mVector.push_back(sp);
			}
		}
		mInitialized = true;
		fclose(fp);
	}			
	//cout << "Found " << count << " points" << endl;

}
	
void MGD77Extractor::Release()
{
	SamplePointVector::iterator it;
	for (it = mVector.begin(); it != mVector.end(); it++)
	{
		delete (*it);
	}
}

