#ifndef PAFBitBase_hh
#define PAFBitBase_hh
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFBitBase								//
//                                                                      //
// Definition of the Tag database (KanGA layout)			//
//                                                                      //
// Author: Marcel Kunze, Bochum University, Nov. 1999			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "RhoBase/VAbsBit.h"

class PAFBitBase : public VAbsBit {
    
public:
    //Constructor
    PAFBitBase();
    PAFBitBase(TTree *t,const unsigned &bs = 32768); // Work in a specific tree
    
    //Destructor
    virtual ~PAFBitBase();

public:        
    ClassDef(PAFBitBase,1) //Analysis bits (TAG)
	
};      

// standalone print
std::ostream&  operator <<(std::ostream& o, const PAFBitBase&);

#endif
