//--------------------------------------------------------------------------
// File and Version Information:
//      $Id: BtaClonedCompositeCandR_000.cxx,v 1.1.1.1 2001-05-15 07:01:02 marcel Exp $
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
#include "RhoBase/TRho.h"
#include "RhoBase/TFactory.h"
#include "RhoBase/TCandidate.h"
#include "RhoBase/VAbsVertex.h"
#include "TDatabasePDG.h"

#include "KangaSchema/BtaClonedCompositeCandR_000.h"

ClassImp( BtaClonedCompositeCandR_000 );


BtaClonedCompositeCandR_000::BtaClonedCompositeCandR_000()
: BtaCompositeCandR_000( ) , 
_original()
{
}

BtaClonedCompositeCandR_000::~BtaClonedCompositeCandR_000( ) 
{
}


TCandidate* 
BtaClonedCompositeCandR_000::Transient() const
{
    const TFitParams *fitParams = _fitParams.Transient();
    TCandidate* transient = TFactory::NewCandidate(*fitParams,refToMe().id());
    delete fitParams;
    if ( _pdtEntryPdgId != 0) {
	static TDatabasePDG *pdt = TRho::Instance()->GetPDG();
	transient->SetType(pdt->GetParticle(_pdtEntryPdgId));
    }
/*
    // Id
    cout << "BtaClonedCompositeCandR_000 RefToMe: "<<refToMe().id() << " Original:" << _original.id() << endl;
    
    // Daughters
    int i;
    for (i=0;i<_daughters.length();++i)
    {
	cout << " daughter["<<i<<"] = "<< _daughters[i]<<endl;
    }
*/    
    return transient;
    
}
