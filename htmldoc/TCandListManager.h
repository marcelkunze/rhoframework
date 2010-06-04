#ifndef TCANDLISTMANAGER_H
#define TCANDLISTMANAGER_H
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

#include <iostream>

#include "TObject.h"
#include "THashList.h"

class TCandList;

class TCandListManager : public TObject {
    
public:
    TCandListManager();
    virtual ~TCandListManager();               
    TCandList* Add(TCandList *list);
    TCandList* Put(TCandList *list) { return Add(list); }
    TCandList* Get(const char *name);
    void Remove(const char *name);
    void Remove(TCandList *list);
    void Cleanup();
    void PrintOn( std::ostream& o=std::cout ) const;

private:    
    THashList *fList; //! List of TCandLists
public:
    ClassDef(TCandListManager,1) // Candidate list manager
};      

// standalone print
std::ostream&  operator << (std::ostream& o, const TCandListManager&);

#endif

