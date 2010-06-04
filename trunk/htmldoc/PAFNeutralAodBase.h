#ifndef PAFNeutralAODBASE_H
#define PAFNeutralAODBASE_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFNeutralAodBase							//
//                                                                      //
// Definition of the Analysis Object Data database (neutral part)	//
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


//  =================================================================
//  ===== PAFNeutralAodBase - Class for storage of Neutral cands ====
//  =================================================================

class TTree;
class TBranch;

class PAFNeutralAodBase : public PAFAodBase , public PAFAodCluster
{
public:
    
    // empty constructor
    PAFNeutralAodBase();
    // copy constructor
    PAFNeutralAodBase(PAFNeutralAodBase &other);

    virtual ~PAFNeutralAodBase() { if (dummy) { delete dummy; } }
    
    // use default destruktor
    // use default copy constructor
    
    // virtuals 
    
    virtual Float_t GetSignificanceLevel(Int_t hypo);
    virtual Float_t GetLikelihood(Int_t hypo);
    
    virtual void SetSignificanceLevel(Int_t hypo, Float_t val);
    virtual void SetLikelihood(Int_t hypo, Float_t val);
    virtual void PrintOn(ostream &o) const; 
    virtual void SetTree(TTree *theTree);
    virtual void SetTree(PAFNeutralAodBase *other);

private :
    
    Float_t _PIDconsistency[5];
    Float_t _PIDlikelihood[5];
    
    TBranch *theSLbranch; //! not stored in ROOT File
    TBranch *theLHOODbranch; //! not stored in ROOT File
    TBranch *dummy; //! not stored in ROOT File
    
public :
    ClassDef(PAFNeutralAodBase,2) // Neutral candidate base class
	
};

std::ostream&  operator << (std::ostream& o, const PAFNeutralAodBase&);

#endif                                           


