//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TCacheManager							//
//                                                                      //
// Object cache manager							//
//                                                                      //
// Author: Marcel Kunze, Bochum University, Sept. 2k			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "RhoManager/TCacheManager.h"

ClassImp(TCacheManager)

#include <iostream>
using namespace std;

//----------------
// Constructors --
//----------------
TCacheManager::TCacheManager(Bool_t owner) : 
fOwnership(owner), fVerbose(kFALSE)
{
    fCache = new THashList();
}

//--------------
// Destructor --
//--------------

TCacheManager::~TCacheManager() 
{
    fCache->Delete();
    delete fCache;
}

TNamedPtr* TCacheManager::CacheObject(const char *name, const char *title, void* value)
{
    // Cache objects using named pointers

    // Do we already have it in the cache ?
    TNamedPtr *ptr = LocateObject(name,title);
    if (ptr == 0) {
	TString desc = TString(title);
	TString key = TString(name) + desc;
	ptr = new TNamedPtr(key,desc,value,fOwnership);
	if (ptr != 0) fCache->Add(ptr);
        if (fVerbose) cout << "TCacheManager: Cache " << key << endl;
    }
   
    return ptr;
}

TNamedPtr* TCacheManager::LocateObject(const char *name, const char *title)
{
    // Look up objects in the hashlist

    TString desc = TString(title);
    TString key = TString(name) + desc;
    TObject *o  = fCache->FindObject(key); // Look it up

    // It is not yet in the cache
    if (o==0) return 0;

    if (fVerbose) cout << "TCacheManager: Return cached " << key << endl;
    return (TNamedPtr*)o;
}

void TCacheManager::RemoveObject(const char *name, const char *title)
{
    // Look up objects in the hashlist

    TString desc = TString(title);
    TString key = TString(name) + desc;
    TObject *o  = fCache->FindObject(key); // Look it up

    if (o!=0) {
	fCache->Remove(o);
	TNamedPtr *ptr = (TNamedPtr*)o;
	delete ptr;
    }
}

void TCacheManager::Reset()
{
    fCache->Delete();
}
