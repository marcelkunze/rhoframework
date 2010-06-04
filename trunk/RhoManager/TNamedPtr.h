#ifndef TNAMEDPTR_H
#define TNAMEDPTR_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TNamedPtr								//
//                                                                      //
// Smart named pointers							//
//                                                                      //
// Author: Marcel Kunze, Bochum University, Sept. 2k			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TNamed.h"
#include "RhoBase/TRho.h"
#include "RhoManager/TEventManager.h"

class TNamedPtr : public TNamed {
public:
    TNamedPtr(const char *name,const char *title,void *value,Bool_t owner=kTRUE) :
      TNamed(name,title), fPtr(value), fOwner(owner), fEvt(-1)
    {
      NewEvent();
    }

    virtual ~TNamedPtr() { if (fOwner) delete (void*)fPtr; }

    void* GetValue() { return fPtr; }
    void SetValue(void* ptr) { fPtr=ptr; }

    Bool_t NewEvent()
    {
	// Is it a new event ?
	Int_t event = TRho::Instance()->GetEventManager()->GetEventNumber();
	Bool_t newEvent = kFALSE;
	if (fEvt != event) {
	    fEvt = event;
	    newEvent = kTRUE;
	}
	return newEvent;
    }

private:
    Bool_t fOwner;
    Int_t fEvt;
    void *fPtr;
};


#endif
