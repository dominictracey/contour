/*
** rcPersistence.h - .rc file implementation of ContourPersistence interface.
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
*/

#ifndef _RCPERSISTENCE_H_
#define _RCPERSISTENCE_H_

#include "persistence.h"
#include <map>
#include <string.h>


#define FILENAME "/.contourrc"

#define RCPERSIST_READ(name,vartype,rfunc,default)			\
	if (token_data.find(#name) != token_data.end())			\
	{								\
		const char *s = token_data[#name];			\
		vartype v = rfunc(s);					\
		mPrefs->Set##name##(v);					\
	}								\
	else								\
		mPrefs->Set##name##(default); 

#define RCPERSIST_READ_STRING(name,defaultval)					\
	if (token_data.find(#name) != token_data.end())				\
	{															\
		char *s = (char *)token_data[#name];		\
		mPrefs->Set##name##(s);								\
	}								\
	  else								\
	  {								\
		mPrefs->Set##name##(defaultval);\
	  }
	
#define RCPERSIST_WRITE(name,writemask)					\
        char name##Line[512];						\
        sprintf(name##Line,writemask,#name,mPrefs->Get##name## ());\
        fs << name##Line;

#define RCPERSIST_WRITE_STRING(name,writemask)					\
		if (::strlen(mPrefs->Get##name## ()) > 0)		\
		{												\
	        char name##Line[512];						\
	        sprintf(name##Line,writemask,#name,mPrefs->Get##name## ());\
	        fs << name##Line;							\
		}

//comparison object for char *- keyed map
struct ltstr
{
  bool operator()(const char* s1, const char* s2) const
  {
    return strcmp(s1, s2) < 0;
  }
};



class RCPersistence : public ContourPersistence
{
public:
	RCPersistence();
	virtual ~RCPersistence();

	//read the data into the given Preferences dialog
	virtual void Read();

	//store the data collected by the given dialog
	virtual void Write();

protected:
	void ReadFile();
	map< const char *, const char *, ltstr > token_data;
};

#endif
