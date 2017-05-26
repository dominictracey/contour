/*
** gps.cpp - 
**
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sat Jun 16 22:32:46 EDT 2001
*/

#include "gps.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>

//linux has sys_termios, AFAICT
#if defined (HAVE_SYS_TERMIOS_H)
#include <sys/termios.h>
#else
#if defined (HAVE_TERMIOS_H)
#include <termios.h>
#endif
#endif

#include <sys/ioctl.h>

//linux doesn't have(?)
#ifdef HAVE_SYS_FILIO_H
#include <sys/filio.h>
#endif

#ifndef _NO_GPS

extern "C"{
#include <gpsd.h>
#include <nmea.h>
}

#endif

#include "appFactory.h"
 
//static Atom delw;
GPS * GPS::mInstance = NULL;
 
#define BUFSIZE 4096

//we define this for the gpsd daemon
// it contains the machine and port where the GPS
// data is being broadcast.
//@REX should be a pref
char *device_name = NULL; //"localhost:2947";
int debug = 0;
int device_type;
int device_speed = 0;//B4800;
char *latitude = 0;
char *longitude = 0;
char latd = 'N';
char lond = 'W';

GPS::GPS()
{
	mInitialized = false;
}

double GPS::GetLatitude() 
{ 
#ifndef _NO_GPS
	return gNMEAdata.latitude; 
#else
	return (double)0;
#endif
}

double GPS::GetLongitude() 
{ 
#ifndef _NO_GPS
	return gNMEAdata.longitude; 
#else
	return (double)0;
#endif
}

double GPS::GetSpeed() { 
#ifndef _NO_GPS
	return gNMEAdata.speed; 
#else
	return (double)0;
#endif
}

GPS * GPS::GetInstance()
{
	if (!mInstance)
		mInstance = new GPS;

	return mInstance;
}

void GPS::Initialize(char * host, char * port)
{
	GetInstance()->mHost = new char[::strlen(host)];
	::strcpy(GetInstance()->mHost,host);
	GetInstance()->mPort = new char[::strlen(port)];
	::strcpy(GetInstance()->mPort,port);
	
	//build host:port string
	device_name = new char[::strlen(host) + ::strlen(port) + 1];
	sprintf(device_name,"%s:%s\0",host,port);
    OpenInput();
}

void GPS::HandleInput(int *source)
{
    static char buf[BUFSIZE];	/* that is more than a sentence */
    static int offset = 0;
    int count;
    int flags;
#ifndef _NO_GPS

    ioctl(*source, FIONREAD, &count);

   /* Make the port NON-BLOCKING so reads will not wait if no data */
   if ((flags = fcntl(*source, F_GETFL)) < 0) return;
   if (fcntl(*source, F_SETFL, flags | O_NDELAY) < 0) return;

    while (offset < BUFSIZE && count--) {
	if (read(*source, buf + offset, 1) != 1)
	    return;

	if (buf[offset] == '\n') {
	    if (buf[offset - 1] == '\r')
		buf[offset - 1] = '\0';
	    handle_message(buf);
	    AppFactory::GetInstance()->GetApp()->UpdateGPSDisplay();
	    offset = 0;
	    return;
	}
	offset++;
    }

#endif

}


void GPS::OpenInput() //XtAppContext app)
{
    int input = 0;
    //XtInputId input_id;

#ifndef _NO_GPS

    if (CheckPort())
    {
    //serial_open is a lib call that looks for the
    //device_name variable for the port info.
   	 input = serial_open();
	 gNMEAdata.fdin = input;
	 gNMEAdata.fdout = input;

     AppFactory::GetInstance()->GetApp()->AttachGPS(input);
	 cout << "Found GPS at " << device_name << endl;
    }
#endif
}

void GPS::CloseInput()
{
#ifndef _NO_GPS
	serial_close();
#endif
}

int errexit(char *s)
{
    perror(s);
#ifndef _NO_GPS
    serial_close();
    //exit(1);
#endif
	return 0;
}

bool GPS::CheckPort()
{
	//char hostname[] = "localhost";
	int     sd;
	struct sockaddr_in sin;
	struct sockaddr_in pin;
	struct hostent *hp;

	bool ok = true;

        /* go find out about the desired host machine */
        if ((hp = gethostbyname(GetInstance()->mHost)) == 0) {
                perror("gethostbyname");
               	ok = false; 
        }

	if (ok)
	{
        /* fill in the socket structure with host information */
        memset(&pin, 0, sizeof(pin));
        pin.sin_family = AF_INET;
        pin.sin_addr.s_addr = ((struct in_addr *)(hp->h_addr))->s_addr;
        pin.sin_port = htons(atoi(GetInstance()->mPort));

        /* grab an Internet domain socket */
        if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
                perror("socket");
                ok = false;
        }
	}

	if (ok)
	{
        /* connect to PORT on HOST */
        if (connect(sd,(struct sockaddr *)  &pin, sizeof(pin)) == -1) {
                perror("connect");
                ok = false;
        }
	close(sd);
	}
	
	GetInstance()->mInitialized = ok;
	return ok;
}
