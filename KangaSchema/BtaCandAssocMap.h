//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtaCandAssocMap.h,v 1.1.1.1 2001-05-15 07:01:17 marcel Exp $
//
// Description:
//      This class holds a triplet if Index1,Index2,Consistency, where
//      Index1/2 are meant to be indices in lists of BetaCandidates and
//      consistency is the quality of the association. A list of these
//      objects, together with two known lists of beta candidates
//      can represent the functionality of a AstTwoWayMatch.
//       This class is used for an easy (and easy to persist) representation
//      of the Reco-MCTruth-Matching in the MicroDST.
//
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Rolf Dubitzky
//
// History (add to end):
//
// Copyright Information:
//
//------------------------------------------------------------------------


#ifndef BTACANDASSOCMAP_hh
#define BTACANDASSOCMAP_hh

class BtaCandAssocMap
{
  private:
    int   _microIndex;
    int   _mcTruIndex;
    float _consistency;

  public:

    BtaCandAssocMap() {}
    BtaCandAssocMap(int microI, int mcTruI, float consistency ) : 
      _microIndex(microI) , _mcTruIndex(mcTruI), _consistency(consistency) {}

    ~BtaCandAssocMap() {}

    void setMicro( int micro )     { _microIndex = micro; }
    void setMcTru( int mcTru )     { _mcTruIndex = mcTru; }
    void setConsistency( float c ) { _consistency = c;    }

    int getMicro() const { return _microIndex; }
    int getMcTru() const { return _mcTruIndex; }
    float getConsistency() const { return _consistency; }

};

#endif
