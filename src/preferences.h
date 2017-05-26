/*
** preferences.h - abstract base class for toolkit-specific subclasses. The
**	abstract Edit function must validate the data collected for the 
**	following:
**		1) minDepth < maxDepth
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
*/

#ifndef _PREFERENCES_H_
#define _PREFERENCES_H_

class ContourPersistence;
class SamplePoint;

#define PREF_DECLARE_PROP(name,vartype) 		\
public:							\
	virtual vartype Get##name() const = 0;		\
	virtual void Set##name(vartype s) = 0;

class ContourPreferences
{
public:
	ContourPreferences() {mPersistor = 0;}
	virtual ~ContourPreferences() {};
	virtual bool Edit() = 0;  //return true if they changed. 
	virtual bool SetPersistence(ContourPersistence *persistor) 
			{mPersistor = persistor;}
	virtual void Save() = 0;
protected:
	ContourPersistence *mPersistor;

	//Display
	PREF_DECLARE_PROP(MaxDepth,float)
	PREF_DECLARE_PROP(MinDepth,float)
	PREF_DECLARE_PROP(AutoColor,bool)
	PREF_DECLARE_PROP(ShowGrid,bool)
	PREF_DECLARE_PROP(Draw,bool)
	PREF_DECLARE_PROP(AutoCenter,bool)
	PREF_DECLARE_PROP(Track,bool)

	//Screen area
	PREF_DECLARE_PROP(XScale,float)
	PREF_DECLARE_PROP(YScale,float)
	virtual SamplePoint & GetUpperLeft() const = 0;
	virtual void SetUpperLeft(const SamplePoint &sp) = 0;

	//Postgres
	PREF_DECLARE_PROP(pgHost,char *)
	PREF_DECLARE_PROP(pgPort,char *)
	PREF_DECLARE_PROP(pgOptions,char *)
	PREF_DECLARE_PROP(pgDBName,char *)
	PREF_DECLARE_PROP(pgUser,char *)
	PREF_DECLARE_PROP(pgPassword,char *)

	//GPS
	PREF_DECLARE_PROP(gpsHost,char *)
	PREF_DECLARE_PROP(gpsPort,char *)
	PREF_DECLARE_PROP(gpsSampleRate,int)
	
	
};

#endif

