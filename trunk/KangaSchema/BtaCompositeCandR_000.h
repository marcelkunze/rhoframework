//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaCompositeCandR_000.h,v 1.1.1.1 2001-05-15 07:01:09 marcel Exp $
//
// Description:
//	Class BtaCompositeCandR_000 
//      see below
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Rolf Dubitzky
//
// Copyright Information:
//	Copyright (C) 2000	TU-Dresden
//
//------------------------------------------------------------------------

#if !defined( BtaCompositeCandR_000_hh )
#define BtaCompositeCandR_000_hh

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//-----------------
// BaBar Headers --
//-----------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------


#include "KangaSchema/BtaFitParamsR.h"
#include "KangaSchema/RooRef.h"
#include "KangaSchema/RooValArray.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class RooEvtObjLocReg;

//----------------------
// Base Class Headers --
//----------------------

#include "KangaSchema/BtaCandidateR.h"
#include "KangaSchema/BtaFitParamsR.h"


//		---------------------
// 		-- Class Interface --
//		---------------------

/**
 * 
 *  This is the peristent representaion of a composite candidate with
 *  references to its daughters and vertex
 *
 * 
 */


class BtaCompositeCandR_000 : public BtaCandidateR //  : public RooEvtObj<BtaCandBase> 
{

//------------------
// Static Members --
//------------------

  public:

    // Typedefs, consts, and enums

    // Selectors (const)

    // Modifiers

  private:

    // Data members

//--------------------
// Instance Members --
//--------------------

  public:

    // Constructors
    BtaCompositeCandR_000( );
    //BtaCompositeCandR_000( const BtaCandidate* transCand,
    //              RooEvtObjLocReg& registry );


    // Destructor
    virtual ~BtaCompositeCandR_000( ) ;

    // Operators
    
    // Selectors (const)
    virtual TCandidate* Transient() const;    
    virtual Int_t NDaughters() const { return _daughters.length(); } 
    virtual Int_t Daughter(Int_t n) const { return _daughters[n]; } 
    virtual BtaAbsVertexR *DecayVtx() const { return _decayVtx; }
    virtual Bool_t IsAResonance() const { return _isAResonance; }
    virtual Int_t PdgId() const { return _pdtEntryPdgId; }

    // Operators
    //const BtaCompositeCandR_000& operator= ( const BtaCompositeCandR_000& );

  protected:

    // Copy Constructor
    //BtaCompositeCandR_000( const BtaCompositeCandR_000& ) ; // stop default

    // Data members
    //   by value
    BtaFitParamsR       _fitParams;       // this is the unpacked 4-Vector version
    BtaAbsVertexR*      _decayVtx;        // this might be shared with other composites
    Int_t               _pdtEntryPdgId;
    Bool_t              _isAResonance;

    // references to daughters
    RooValArray _daughters;

    // ROOT Interface
    ClassDef( BtaCompositeCandR_000, 1 )
};

#endif
