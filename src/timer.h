/*
** timer.h - Class to facilitate performance profiling.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Fri Dec 15 17:53:33 EST 2000
*/

#ifndef _TIMER_H_
#define _TIMER_H_

#include <fstream.h>

#define defaultFileName "timer.out"
//class TimerDeleter;

class Timer
{
friend class TimerDeleter;
public:
	static Timer * getInstance();
	void start();
	void reset();
	void mark(const char *msg);
private:
	~Timer();
	Timer();  //use singleton accessor
	Timer(char * fileName); 
	static Timer *mInstance;
	long mStart;
	long mElapsed;
	ofstream * mFile;
};

class TimerDeleter
{
public:
	TimerDeleter() {}
	~TimerDeleter() { delete Timer::getInstance(); }
};
#endif
