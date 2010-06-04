//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFMcTruth								//
//                                                                      //
// Monte Carlo truth class in PAF(Pico Analysis Framework)		//
//									//
// Author List:								//
// Marcel Kunze, Bochum University, Aug. 99				//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TDatabasePDG.h"
#include "RhoBase/TRho.h"
#include "PAFSchema/PAFMcTruth.h"

ClassImp(PAFMcTruth)

#include <iostream>
using namespace std;

PAFMcTruth::PAFMcTruth() :
  fMyBase(0),
  fMother(0),
  fDaughters(0)
{
    fPDG = TRho::Instance()->GetPDG();	// Access particle DB
}

PAFMcTruth::PAFMcTruth(PAFMcBase& b) :
  fMyBase(&b),
  fMother(0),
  fDaughters( new TObjArray() )    
{
    fPDG = TRho::Instance()->GetPDG();	// Access particle DB
}

PAFMcTruth::~PAFMcTruth() 
{
    if (fDaughters) delete fDaughters;
}


void PAFMcTruth::PrintOn(std::ostream& o) const {
    o << *fMyBase <<  endl;
}        

TParticlePDG* PAFMcTruth::GetPdtEntry()   const 
{
    // Particle Id
    if (fMyBase == 0) return 0;
    return fPDG->GetParticle(fMyBase->GetPdtID()); 
}

Double_t PAFMcTruth::GetCharge()      const
{
    // Charge
    if (fMyBase == 0) return 0;
    return fPDG->GetParticle(fMyBase->GetPdtID())->Charge(); 
}

std::ostream&  operator << (std::ostream& o, const PAFMcTruth& a) { a.PrintOn(o); return o; }
