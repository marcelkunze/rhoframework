//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaEventInfo								//
//                                                                      //
// Contains and provides access to summary information in the event	//
// useful for analysis							//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Dec. 2k						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "RhoBase/BtaEventInfo.hh"
#include <assert.h>

ClassImp(BtaEventInfo)

#include <iostream>
using namespace std;

BtaEventInfo::BtaEventInfo():
fBeamSpot(TVector3(0,0,0)),fBeamSpotBFlight(TVector3(0,0,0)),fPrimaryVertex(0), fEventShape(0),fMCIP(0)
{}

//--------------
// Destructor --
//--------------

BtaEventInfo::~BtaEventInfo( )
{
    delete fPrimaryVertex;
    delete fMCIP;
}

void 
BtaEventInfo::setPrimaryVertex(VAbsVertex* inVtx) {
    delete fPrimaryVertex;
    fPrimaryVertex = (inVtx == 0) ? (VAbsVertex*) 0 : (VAbsVertex*) inVtx->Clone();
}

const TPointErr
BtaEventInfo::primaryVertex() const {
    return (fPrimaryVertex == 0) ?  TPointErr(TVector3(0,0,0)):
    TPointErr(fPrimaryVertex->Point(),fPrimaryVertex->XXCov());
}
void 
BtaEventInfo::setIPTruth(const TLorentzVector& inVtx) {
    fMCIP=new TVector3(inVtx.X(),inVtx.Y(),inVtx.Z());
}

void 
BtaEventInfo::setIPTruth(const TVector3& inVtx) {
    fMCIP=new TVector3(inVtx);
}

void 
BtaEventInfo::setEventShape(BtaEventShape* eventShape)
{
    fEventShape = eventShape;
}
//--------------
// Operations --
//--------------

void BtaEventInfo::printOn(std::ostream& o) const {
    o << "e+, e- beams: "<<positronBeam()<<electronBeam();
    o << " spots: "<<beamSpot()<<beamSpotBFlight()<<primaryVertex();
}

std::ostream&  operator << (std::ostream& o, const BtaEventInfo& a) {a.printOn(o); return o;}


