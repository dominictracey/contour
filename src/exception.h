/*  
**  exception.h
**  
**  Some interesting things to think about here...
**  
**  	Subclasses of this class have information specific
**  	about a certain class or classes of errors?
**  	
**  	Have a class that displays these exceptions in an
**  	application appropriate manner (e.g. popping up a 
**  	app toolkit dialog box, making an entry into an
**  	error table in the db or what-have-you). How do these
**  	two class hierarchies interact
**
** This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
-* (c) 2001, Aquilon Technology Systems, Inc.
**
*/

#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

class ContourException
{
public:
	ContourException()
	{
		mString = new char[::strlen("Unspecified error")];
		::strcpy(mString,"Unspecified error");
	}					
	ContourException(char *str)
	{
		mString = new char[::strlen(str)+1];
		::strcpy(mString,str);
	}			
	~ContourException()
	{
		delete [] mString;
	}
	char * GetString()
	{
		return mString;
	}
protected:
	char * mString;
};

#endif

