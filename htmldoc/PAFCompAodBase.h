#ifndef PAFCOMPOSITEAODBASE_H
#define PAFCOMPOSITEAODBASE_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFCompAodBase							//
//                                                                      //
// Definition of an aod-object which represents a composite particle	//
//                                                                      //
//    * V0 s								//
//    * Clusters with more than on Bump					//
//    * Particles from PID - lists					//
// Author : Thorsten Brandt , TU Dresden				//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iosfwd>
#include "TObject.h"

#define NDAUGHTERS 5

// #include "PAFSchema/PAFAodBase.h"   (for later ...)

class PAFCompAodBase : public TObject  // later : public PAFAodBase
{
    // Index of Base-Object in charged (0..ncharged-1) or neutral (ncharged ... nTracks)
    // list. Is -1 in case of V0.

    Short_t fAodRef;  

    // Indices of daughters in charged/neutral list. Only entries>0 are valid. For
    // stavle particles from PID - lists, all entries are -1. 

    Short_t fDaughters[NDAUGHTERS];

    // Particle ID
    Short_t fPdtRef;        // Pdt IDs

public :

    PAFCompAodBase();
    PAFCompAodBase(Int_t aodref, Int_t pdtref=0);
    virtual ~PAFCompAodBase() { };
    void SetAodRef(Int_t ref);
    void SetPdtRef(Int_t ref);
    void SetDaughters(Int_t n, Int_t *indexList);
    void AddDaughter(Int_t index);
    void PrintOn(ostream &o);

    Int_t GetAodRef() { return fAodRef; }
    Int_t GetPdtRef() { return fPdtRef; }
    Int_t GetDaughter(Int_t which);
    void Reset();
    ClassDef(PAFCompAodBase,1) // Composite candidate base class

};

#endif
