/*
** contourGraphicContainer.cpp - 
**   This program is free software. You can use, copy and modify it under
** the terms of the GPL. See the file COPYING for the full GPL license.
**
** (c) 2000 Aquilon Technology Systems, Inc.
**
** Created: Sun Jul  1 22:28:07 EDT 2001
*/
#include "contourGraphicContainer.h"
#include "bathApp.h"


ContourGraphicContainer::ContourGraphicContainer()
{
	mSubClassInstances = new ContourGraphicVector;
	mClear = false;
}

//@REX we should delete the vector during the last destructor

bool ContourGraphicContainer::DrawAll()
{
	ASSERT(mClear);
	ContourGraphicVector::iterator it;
	for (it = mSubClassInstances->begin(); it != mSubClassInstances->end(); it++)
	{
		(*it)->Draw();
	}
	mClear = false;

	return mClear;
}

bool ContourGraphicContainer::ClearAll()
{
	ASSERT(!mClear);
	ContourGraphicVector::iterator it;
	for (it = mSubClassInstances->begin(); it != mSubClassInstances->end(); it++)
	{
		(*it)->Clear();
	}
	mClear = true;

	return mClear;
}

void ContourGraphicContainer::Register(ContourGraphic *subclassInstance)
{
	//check if we have it
	ContourGraphicVector::iterator it;
	bool found = false;
	for (it = mSubClassInstances->begin(); it != mSubClassInstances->end() && !found; it++)
	{
		if (*it == subclassInstance)
			found = true;
	}

	ASSERT(!found)

	if (found) return;  //was it lost?

	subclassInstance->SetViewPortLocal();
	mSubClassInstances->push_back(subclassInstance);
	
}

void ContourGraphicContainer::Unregister(ContourGraphic *subclassInstance)
{
	//check if we have it
	ContourGraphicVector::iterator it;
	bool found = false;
	for (it = mSubClassInstances->begin(); it != mSubClassInstances->end() && !found; it++)
	{
		if (*it == subclassInstance)
			found = true;
	}
	//back up to it
	it--;
	
	ASSERT(found)

	if (!found) return;  // how can we return it if we don't have it?

	int size = mSubClassInstances->size();
	mSubClassInstances->erase(it);
	ASSERT ( mSubClassInstances->size() == size-1);
}

/*
** When the viewport changes, we need to notify everyone. Called from the static
**	ContourGraphic::SetViewPort().
*/
void ContourGraphicContainer::SetViewPortLocal()
{
	ContourGraphicVector::iterator it;
	for (it = mSubClassInstances->begin(); it != mSubClassInstances->end(); it++)
	{
		(*it)->SetViewPortLocal();
	}
}

	
