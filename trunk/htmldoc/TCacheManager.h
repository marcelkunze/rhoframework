#ifndef TCACHEMANAGER_H
#define TCACHEMANAGER_H
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

#include "TObject.h"
#include "THashList.h"
#include "RhoManager/TNamedPtr.h"

class TCacheManager : public TObject {
    
public:
    TCacheManager(Bool_t owner=kTRUE);
    virtual ~TCacheManager();               
    TNamedPtr* CacheObject(const char *name,const char *title,void* value);
    TNamedPtr* LocateObject(const char *name,const char *title);
    void RemoveObject(const char *name,const char *title);
    void Reset();
    void SetVerbose(Bool_t yesNo=kTRUE) { fVerbose = yesNo; }
private:    
    THashList *fCache; //! List of named pointers
    Bool_t fVerbose;   //! Debug
    Bool_t fOwnership; //! True = Owner of objects
public:
    ClassDef(TCacheManager,1) // T cache manager
};      

#endif

