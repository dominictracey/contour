/*
** rcPersistence.cpp - .rc file implementation of ContourPersistence interface.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
*/

#include "rcPersistence.h"
#include <fstream.h>
#include <iostream.h>
#include <string.h>
#include <stdio.h>
#include "position.h"

#define KEY_UPPERLEFT "UpperLeft"
#define DEFAULT_UPPERLEFT "42 40 0.0 -68 -30 -0.1"

RCPersistence::RCPersistence()
{

}

RCPersistence::~RCPersistence()
{
	//FIXME - mem leak of both keys and values in token_data
	// iterate through and reclaim char mem
}

/*
** Read - read the data into the given Preferences dialog
*/
void RCPersistence::Read()
{
	ReadFile();

	RCPERSIST_READ(MaxDepth,float,atof,130)
	RCPERSIST_READ(MinDepth,float,atof,60)
	RCPERSIST_READ(AutoColor,bool,atoi,0)
	RCPERSIST_READ(ShowGrid,bool,atoi,0)
	RCPERSIST_READ(Draw,bool,atoi,0)
	RCPERSIST_READ(Track,bool,atoi,0)
	RCPERSIST_READ(XScale,float,atof,.75)
	RCPERSIST_READ(YScale,float,atof,.75)

	//postgresql stuff
	RCPERSIST_READ_STRING(pgHost,"localhost")
	RCPERSIST_READ_STRING(pgPort,"5432")
	RCPERSIST_READ_STRING(pgOptions,"")
	RCPERSIST_READ_STRING(pgDBName,"contour")
	RCPERSIST_READ_STRING(pgUser,"contour")
	RCPERSIST_READ_STRING(pgPassword,"")
	
	//Upper left
	if (token_data.find(KEY_UPPERLEFT) != token_data.end())
	{
		const char *supperleft = token_data[KEY_UPPERLEFT];
		char *temp = new char[strlen(supperleft)+1];
		strcpy(temp,supperleft);
		SamplePoint ul(temp);
		delete temp;

		//check if we initialized or not (compare to an uninitialized one)
		SamplePoint dummy;
		if (ul == dummy)
		{
			//didn't initialize for whatever reason - use default
			char *str = new char[strlen(DEFAULT_UPPERLEFT+1)];
			strcpy(str,DEFAULT_UPPERLEFT);
			SamplePoint temp(str);
			mPrefs->SetUpperLeft(temp);
		}
		  else
			mPrefs->SetUpperLeft(ul);
	}
	  else
	  {
		char *str = new char[strlen(DEFAULT_UPPERLEFT+1)];
		strcpy(str,DEFAULT_UPPERLEFT);
		SamplePoint temp(str);
		mPrefs->SetUpperLeft(temp);
	  }

	//GPS
	RCPERSIST_READ_STRING(gpsHost,"localhost")
	RCPERSIST_READ_STRING(gpsPort,"2947")
	RCPERSIST_READ(gpsSampleRate,int,atoi,60)
	
}

/*
** Write - store the data collected by the given dialog
*/
void RCPersistence::Write()
{
	fstream fs;

	char home[100]; 
	strncpy(home, getenv("HOME"), 100);
	strcat(home, FILENAME);

	//  Open the file for writing
	fs.open(home, ios::out );

	char line_arr[512];
	char *line = line_arr;

	RCPERSIST_WRITE(MaxDepth,"%s %.2f\n")
	RCPERSIST_WRITE(MinDepth,"%s %.2f\n")
	RCPERSIST_WRITE(AutoColor,"%s %d\n")	
	RCPERSIST_WRITE(ShowGrid,"%s %d\n")
	RCPERSIST_WRITE(Draw,"%s %d\n")
	RCPERSIST_WRITE(Track,"%s %d\n")
	RCPERSIST_WRITE(XScale,"%s %.2f\n")
	RCPERSIST_WRITE(YScale,"%s %.2f\n")

	//upper left
	char ulLine[512];
	char latlongdata[35];  //long enough? {-89 -59 -28.47 -168 -59 -28.47}
	sprintf(ulLine,"%s %s\n", KEY_UPPERLEFT, mPrefs->GetUpperLeft().Stringify(latlongdata, 35));
	fs << ulLine;
	
	//postgresql stuff
	RCPERSIST_WRITE_STRING(pgHost,"%s %s\n")
	RCPERSIST_WRITE_STRING(pgPort,"%s %s\n")
	RCPERSIST_WRITE_STRING(pgOptions,"%s %s\n")
	RCPERSIST_WRITE_STRING(pgDBName,"%s %s\n")
	RCPERSIST_WRITE_STRING(pgUser,"%s %s\n")
	RCPERSIST_WRITE_STRING(pgPassword,"%s %s\n")

	//gps
	RCPERSIST_WRITE_STRING(gpsHost,"%s %s\n")
	RCPERSIST_WRITE_STRING(gpsPort,"%s %s\n")
	RCPERSIST_WRITE(gpsSampleRate,"%s %d\n")
}

/*
** ReadFile - Code pinched from Phil Ottewell's STL Tutorial 
**	(http://www.yrl.co.uk/~phil/stl/stl.htmlx#Map_and_Multimap)
**
**	Actually, couldn't get the string crap working so using char *'s instead.
**	I won't say I miss CStrings.
**
**  Tokenize rc file into STL map.
*/
void RCPersistence::ReadFile()
{
	size_t ip, lnum;
	fstream fs;
	char line[512];
//	char key[512];
//	char value[512];

	char home[100]; 
	strncpy(home, getenv("HOME"), 100);
	strcat(home, FILENAME);

	//  Open the file for writing
	fs.open(home, ios::in );

	//  Read each line and parse it
   	lnum = 0;
    	while ( fs.good() ) 
	{
      		fs.getline(line,512);
      		if ( fs.good() ) 
		{
			//Parse out the keys and values
        		++lnum;
        		
			char *key = strtok(line," ");
			char *keyp = new char[strlen(key) +1];   //FIXME mem leak
			strcpy(keyp,key);

			char *value = strtok(NULL, "\n");
			char *valp = new char[strlen(value) +1];
			strcpy(valp,value);

          		token_data[keyp] = valp;
      		}
    	}

}
