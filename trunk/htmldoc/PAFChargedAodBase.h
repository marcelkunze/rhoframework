#ifndef PAFCHARGEDAODBASE_H
#define PAFCHARGEDAODBASE_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFChargedAodBase							//
//                                                                      //
// Definition of the Analysis Object Data database (charged part)	//
//                                                                      //
// Author: T. Brandt, TUD, June 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iosfwd>
#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "PAFSchema/PAFAodBase.h"
#include "PAFSchema/PAFAodCluster.h"
#include "PAFSchema/PAFAodTrack.h"


//  =================================================================
//  ===== PAFChargedAodBase - Class for storage of charged cands ====
//  =================================================================

class TTree;

class PAFChargedAodBase : public PAFAodBase, public PAFAodCluster, public PAFAodTrack 
{
public:
    
    // empty constructor
    PAFChargedAodBase();

    // copy constructor
    PAFChargedAodBase(PAFChargedAodBase &other);	
 
    virtual ~PAFChargedAodBase();
    
    
    // virtuals 
    virtual Float_t GetSignificanceLevel(Int_t hypo);
    virtual Float_t GetLikelihood(Int_t hypo);
    
    virtual void SetSignificanceLevel(Int_t hypo, Float_t val);
    virtual void SetLikelihood(Int_t hypo, Float_t val);
    
    virtual void PrintOn(ostream &o) const;

    virtual void SetTree(TTree *theTree);
    virtual void SetTree(PAFChargedAodBase *other);
private :
    
    Float_t _PIDconsistency[5];
    Float_t _PIDlikelihood[5];
    TSmartPointer *theSLbranch; //! not stored in ROOT File
    TSmartPointer *theLHOODbranch; //! not stored in ROOT File
    
public :
    ClassDef(PAFChargedAodBase,2) // Charged candidate base class
	
};

std::ostream&  operator << (std::ostream& o, const PAFChargedAodBase&);

#endif                                           


