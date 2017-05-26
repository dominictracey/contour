/*
** gps.h - Class to deal with various GPS units using the gpsd daemon.
**	Most of the code in here is modeled on the model gpsd client ('gps')
**	included with that package.
**
**	The GPL'ed gpsd package is maintained by Remco Treffcorn and friends.
**
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jun 16 22:21:17 EDT 2001
*/

#ifndef _GPS_H_
#define _GPS_H_

class GPS
{
public:
	GPS();
	~GPS() {};
	static GPS * GetInstance(); //singleton
	static void Initialize(char *host, char *port);
	static void HandleInput(int *source);
	static void OpenInput();
	static void CloseInput();

	double GetLatitude();
	double GetLongitude(); 
	double GetSpeed();

protected:
	static bool CheckPort();
	static GPS * mInstance;
	bool mInitialized;
	char * mHost;
	char * mPort;
};

#endif
	
