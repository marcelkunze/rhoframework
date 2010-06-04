//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaClonedCandR_000.cxx,v 1.1.1.1 2001-05-15 07:02:11 marcel Exp $
//
// Description:
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
#include "RhoBase/TRho.h"
#include "RhoBase/TFactory.h"
#include "RhoBase/TCandidate.h"
#include "RhoBase/VAbsVertex.h"
#include "TDatabasePDG.h"
#include "KangaSchema/BtaClonedCandR_000.h"
#include "KangaSchema/RooValArray.h"

ClassImp( BtaClonedCandR_000 );


BtaClonedCandR_000::BtaClonedCandR_000()
: BtaCandidateR(),
_original( 0 ),
_fitParams(),
_pdtEntryPdgId( 0 )
{
}

BtaClonedCandR_000::~BtaClonedCandR_000()
{
}

TCandidate*
BtaClonedCandR_000::Transient() const
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
    cout << "BtaClonedCandR_000 RefToMe: "<<refToMe().id() << endl;
*/    
    return transient;
    
}
