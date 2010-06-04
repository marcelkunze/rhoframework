#ifndef TOBJECTMANAGER_H
#define TOBJECTMANAGER_H
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

#include <iostream>

#include "TObject.h"
#include "THashList.h"

class TObjectManager : public TObject {
    
public:
    TObjectManager();
    virtual ~TObjectManager();               
    TObject* Add(TObject *obj);
    TObject* Put(TObject *obj) { return Add(obj); }
    TObject* Get(const char *name);
    void Remove(const char *name);
    void Remove(TObject *obj);
    void PrintOn( std::ostream& o=std::cout ) const;
    THashList *GetListOfObjects() { return fList; }
    
private:    
    THashList *fList; //! List of transient objects
public:
    ClassDef(TObjectManager,1) // Object manager
};      

// standalone print
std::ostream&  operator << (std::ostream& o, const TObjectManager&);

//--------------------------------------------------------------------
// Get a handle to objects (instantiate if necessary)
// The objects are registered in the object manager
#include "RhoBase/TRho.h"

template <class T>
Bool_t GetTmpAnObject(T* &object,const char *name)
{
    TObjectManager *objmgr = TRho::Instance()->GetObjectManager();
    if (objmgr==0) return kFALSE;

    TObject *o = objmgr->Get(name); // Check if the name is in use
    if (o!=0) {
	cerr << "GetTmpAnObject: Instantiation of " << name << " failed. ";
	cerr << name << " already exists as a " << o->ClassName() << endl;
	return kFALSE;
    }

    object = (T *) objmgr->Put(new T(name)); // Instantiate the object

    return kTRUE;
}

#endif

