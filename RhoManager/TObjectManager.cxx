//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TObjectManager							//
//                                                                      //
// Object manager					    		//
//                                                                      //
// Author: Marcel Kunze, Bochum University, Jan. 2k			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "RhoManager/TObjectManager.h"

ClassImp(TObjectManager)

#include <iostream>
using namespace std;

//----------------
// Constructors --
//----------------
TObjectManager::TObjectManager() :
fList(0)
{
    fList = new THashList();
}

//--------------
// Destructor --
//--------------

TObjectManager::~TObjectManager() 
{
    fList->Delete();
    delete fList;
}

//----------------------
//-- public Functions --
//----------------------

// Add an object
TObject* TObjectManager::Add(TObject *obj) 
{
    fList->Add(obj);
    return obj;
}


// Retrieve an object
TObject* TObjectManager::Get(const char *name) 
{
    return fList->FindObject(name);
}


// Remove an object
void TObjectManager::Remove(const char *name) 
{
    Remove(Get(name));
}


// Remove an object
void TObjectManager::Remove(TObject *obj) 
{
    if (obj!=0) {
	fList->Remove(obj);
	delete obj;
    }
}


void TObjectManager::PrintOn(std::ostream & o) const {
    
    int i;

    o << endl;
    o << "TObjectManager Directory: " << fList->GetSize() << endl;
    o << "---------------------------------" << endl;
    for (i=0;i<fList->GetSize();i++) {
	TObject *l = fList->At(i);
	if (l==0)
	    o << "Reference to non existent object" << endl;
	else
	    o << l->GetName() << "\t(instanceOf " << l->ClassName() << ")" << endl;
    }

    o << endl;

    return;

}

// non-member functions
// --------------------

std::ostream&  operator << (std::ostream& o, const TObjectManager& a) {a.PrintOn(o); return o;}



