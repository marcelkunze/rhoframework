//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaClonedCandR_000.h,v 1.1.1.1 2001-05-15 07:02:07 marcel Exp $
//
// Description:
//      see below
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Simon Patton		Originator
//
// Copyright Information:
//	Copyright (C) 1998	LBNL
//
//------------------------------------------------------------------------

#if !defined( BtaClonedCandR_000_hh )
#define BtaClonedCandR_000_hh

//---------------
// C++ Headers --
//---------------

//-----------------
// BaBar Headers --
//-----------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "KangaSchema/BtaFitParamsR_002.h"
#include "KangaSchema/RooRef.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class RooEvtObjLocReg;
class BtaCandBase;


//----------------------
// Base Class Headers --
//----------------------
#include "KangaSchema/BtaCandidateR.h"

//		---------------------
// 		-- Class Interface --
//		---------------------

/** 
*
* This is the persisent version of a MicroCand that has been cloned.  it
* might have different properties wrt. pdg, momentum etc. but has the same
* uid as it represents the same measured object in the detector. (right
* combinatorics)
* 
*/

class BtaClonedCandR_000 : public BtaCandidateR 
{
    
    //--------------------
    // Instance Members --
    //--------------------
    
public:
    
    BtaClonedCandR_000( );
    virtual ~BtaClonedCandR_000( ) ;
    virtual TCandidate* Transient() const;
    virtual Int_t NDaughters() const { return 0; } 
    virtual Int_t Daughter(Int_t n) const { return 0; }
    virtual BtaAbsVertexR *DecayVtx() const { return 0; }
    virtual Int_t PdgId() const { return _pdtEntryPdgId; }
    virtual Bool_t IsAResonance() const { return kFALSE; }
protected:
    
private:
    
    RooRef _original ;
    BtaFitParamsR_002 _fitParams;  // this is the packed track parameter version
    Int_t _pdtEntryPdgId;
    
    // ROOT interface
    ClassDef( BtaClonedCandR_000, 1 )
};
#endif
