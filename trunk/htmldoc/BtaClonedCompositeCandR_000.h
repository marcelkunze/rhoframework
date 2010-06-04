//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtaClonedCompositeCandR_000.h,v 1.1.1.1 2002-12-20 15:05:29 marcel Exp $
//
// Description:
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//    Rolf Dubitzky
//
// Copyright Information:
//
//------------------------------------------------------------------------

#if !defined( BtaClonedCompositeCandR_000_hh )
#define BtaClonedCompositeCandR_000_hh

class TCandidate;

#include "KangaSchema/BtaCompositeCandR_000.h"
#include "KangaSchema/RooRef.h"

/**
 *
 * This class represents a composite candiate where the base has been cloned
 * and the original (or the clone) has already been persisted. We need to
 * make sure they get the same uid when making the transients, so we save a
 * ref to the original. Unlike the ClonedCand this one inherits the
 * non-cloned class (CompositeCand) because there are no memeber data that
 * are known to be unmodified in the clone. So we need to save the whole set.
 *
 */

class BtaClonedCompositeCandR_000 : public BtaCompositeCandR_000
{
  public:
    BtaClonedCompositeCandR_000();
    virtual ~BtaClonedCompositeCandR_000( ) ;
    virtual TCandidate* Transient() const;

  private:

  protected: // just in case we want to extend this by inheritance...
    RooRef _original ; // _original _must_ have a BtaCandidate as transient

    ClassDef( BtaClonedCompositeCandR_000, 1)
};

#endif
