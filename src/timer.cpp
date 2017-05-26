/*
** timer.cpp - Implementation of performance monitor.
**
**	Log file is timer.out and should look like:
**
**	Elapsed		Interval	Comment
**	0			0			Start
**	1000		1000		Did something
**	1500		500			Something else
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Fri Dec 15 18:20:08 EST 2000
*/

#include "timer.h"
#include <sys/time.h>
#include <unistd.h>

Timer * Timer::mInstance = 0;
TimerDeleter td;

Timer::Timer()
{
	mElapsed = 0;
	mStart = 0;
	mFile = new ofstream(defaultFileName);
}

Timer::Timer(char * fileName)
{
	mElapsed = 0;
	mStart = 0;
	mFile = new ofstream(fileName);
}

Timer::~Timer()
{
	delete mFile;
}

Timer * Timer::getInstance()
{
	if (!mInstance)
	{
		mInstance = new Timer;
		mInstance->start();
	}

	return mInstance;
}

void Timer::start()
{
	timeval tv;
	if (gettimeofday(&tv,NULL) != 0)
		throw ("gettimeofday failure in Timer::start()");
	mStart = tv.tv_sec;
	mElapsed = tv.tv_usec;
	*mFile << "Elapsed (ms)\tInterval (ms)\tComment" << endl;
	mark("Start");
}

void Timer::reset()
{
	timeval tv;
	if (gettimeofday(&tv,NULL) != 0)
		throw ("gettimeofday failure in Timer::reset()");
	mStart = tv.tv_sec;
	mElapsed = tv.tv_usec;
	mark("Reset");
}

void Timer::mark(const char *msg)
{
	timeval tv;
	if (gettimeofday(&tv,NULL) != 0)
		throw ("gettimeofday failure in Timer::mark()");
	long secs = tv.tv_sec - mStart;
	long tmp = mElapsed;
	mElapsed = tv.tv_usec + ((long)1000000 * secs);
	*mFile << mElapsed << "\t\t" << mElapsed - tmp << "\t" << msg  << endl;
}

