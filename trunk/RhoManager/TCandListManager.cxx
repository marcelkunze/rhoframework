//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TCandListManager							//
//                                                                      //
// Candidate list manager				    		//
//                                                                      //
// Author: Marcel Kunze, Bochum University, Jan. 2k			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "RhoManager/TCandListManager.h"
#include "RhoBase/TCandList.h"

ClassImp(TCandListManager)

#include <iostream>
using namespace std;

//----------------
// Constructors --
//----------------
TCandListManager::TCandListManager() :
fList(0)
{
    fList = new THashList();
}

//--------------
// Destructor --
//--------------

TCandListManager::~TCandListManager() 
{
    fList->Delete();
    delete fList;
}

//----------------------
//-- public Functions --
//----------------------

// Add a TCandList
TCandList* TCandListManager::Add(TCandList *list) 
{
    fList->Add(list);
    return list;
}


// Retrieve a TCandList
TCandList* TCandListManager::Get(const char *name) 
{
    return (TCandList *) fList->FindObject(name);
}


// Remove a TCandList
void TCandListManager::Remove(const char *name) 
{
    Remove(Get(name));
}


// Remove a TCandList
void TCandListManager::Remove(TCandList *list) 
{
    if (list!=0) {
	fList->Remove(list);
	delete list;
    }
}


// Reset all lists
void TCandListManager::Cleanup() 
{
    for (Int_t i=0;i<fList->GetSize();i++) {
      TCandList *list = (TCandList *) fList->At(i); 
      list->Cleanup();
    }
}


void TCandListManager::PrintOn(std::ostream & o) const {
    
    int i;

    o << endl;
    o << "TCandList Inventory: " << fList->GetSize() << " instances" << endl;
    o << "---------------------------------" << endl;
    for (i=0;i<fList->GetSize();i++) {
	TCandList *l = (TCandList*) fList->At(i);
	if (l==0)
	    o << "Reference to non existent list" << endl;
	else
	    o << l->GetName() << endl;
    }

    o << endl;

    return;

}

// non-member functions
// --------------------

std::ostream&  operator << (std::ostream& o, const TCandListManager& a) {a.PrintOn(o); return o;}



