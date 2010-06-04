#if !defined( BtaCandidateR_hh )
#define BtaCandidateR_hh
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaCandidateR.h,v 1.1.1.1 2002-12-20 15:05:29 marcel Exp $
//
// Description:
//	Class BtaCandidateRSupplier:
//
//      This is the abstract base class for all types of persistent versions
//      of BtaCandidate, with the exeption of BtaMicroCandR_xxx. This are
//      mainly clones of MicroCandidates, Composite Candidates, clones of
//      composites, and references to already persisted Microcands and
//      composite cands. These classes can be supplied by a
//      BtaCandidateRSupplier.  BtaMicroCandR is supplied by the
//      BtaDefRSupplier.
//
//     NOTE:
//        This class has been implemented with microDST (AOD) and
//        composite candidates in scope. Not all existing subclasses
//        of BtaCandBase are handled, not all subcleasses of BtaAbsVeretx
//        are handled.
//          The most relevant method for trans->peristent is
//        BtaCandidateRSupplier::supplyPersistent(). That is where
//        it is checked what candidate (micro or not, clone or deep clone or not, etc) 
//        it is, and where it is decided how to create (supply) the right
//        persistent representation.
//          For pers->trans check the numerous ::transient() methods of
//        BtaCandidateR, BtaCandBaseRefR, BtaCompositeCandR, BtaClonedCompositeCandR,
//        and BtaMicroCandR
//
//           - RD Sep 2000
//

//    ToDo:    (central todo list for all the (composite) candidate persistence in Kanga)
//    ======

//     -  UIDs  (check Gerhard's stuff) with the ClonedCand
//        - I think it works now -> check with many events/cands
//        - check the ClonedCompCand that handles compression stuff.
//     -  handle references to fundamental (i.e. micro-) cands
//        -  need a MicroCandSupplier
//           A unique  uid-registry should be used by all suppliers                                            
//           (for the micro too) and must be reset (or deleted/created) once per                                             
//           "stream destination".. suppliers reset() is called once per stream 
//           check if this is just the right thing(tm) or not.
//             right now its called by BtaRooLoad::event .. that will not 
//             work for multiple destinations.
//
//     -  prevent writing of multiple vertices 
//        -  Better -> make ROOT do the Vertices not RooScribes.
//                     this would need a registry to remember all persisted
//                     vertices with their pPartners
//           +   5 Nov : done, but needs checking. I need to extend RooRef and
//                       register the Vertices
//                   -- why only persiste non-mother, nonresonace vertices ???
//
//     -  clean up the cout mess
//     -  clean up #includes
//     -  check for new dependencies
//     -  protect against double filling of refs and pointers in ALL classes
//     -  check constructors for baseclass constructor call
//     -  VtxVertex covariance matrixz fix. (Gerhard)
//        - I think he introduced a bug in the spatial coordinates > need to check.
//     -  revers order in flattener.. incorporate from Gerhard.. will this work?

// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Rolf Dubitzky    
//
// Copyright Information:
//      Copyright (C) 2000      TU-Dresden
//
//------------------------------------------------------------------------

//----------------------
// Base Class Headers --
//----------------------

#include "KangaSchema/RooEvtObj.h"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------
#include <iosfwd>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

class TCandidate;
class BtaCandidate;
class BtaAbsVertexR;
class RooValArray;

//		---------------------
// 		-- Class Interface --
//		---------------------

/**
  * This class implements the RooAbsSupplier class for any
  * transient-persistent pair where the persistent class is a subclass
  * of the  interface.
  */

class BtaCandidateR : public  RooEvtObj<BtaCandidate>
{
  public:

    BtaCandidateR();

    virtual ~BtaCandidateR();

    virtual TCandidate* Transient() const = 0;
    virtual Int_t NDaughters() const = 0; 
    virtual Int_t Daughter(Int_t n) const = 0;
    virtual BtaAbsVertexR *DecayVtx() const = 0;
    virtual Bool_t IsAResonance() const = 0;
    virtual Int_t PdgId() const = 0;
    virtual void PrintOn( ostream & ) const;

  private:

    //BtaCandidateR( const BtaCandidateR & );
    //const BtaCandidateR & operator = ( const BtaCandidateR & );

    ClassDef( BtaCandidateR, 1 )

};

#endif // BtaCandidateR_hh
