//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaCandidateFactory							//
//                                                                      //
// Object factory							//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Feb. 01						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <stdlib.h>

#include "RhoBase/BtaCandidateFactory.hh"
#include "RhoBase/BtaFitParams.hh"
#include "RhoBase/BtaCandidate.hh"
#include "RhoBase/BtaSimpleVertex.hh"

ClassImp(BtaCandidateFactory)

using namespace std;

BtaCandidateFactory* BtaCandidateFactory::fgBcfInstance = 0;
TClonesArray* BtaCandidateFactory::fgBcfCandBuffer = 0;
Int_t BtaCandidateFactory::fgBcfCandPointer=0;
Int_t BtaCandidateFactory::fgBcfCandWatermark=0;
TClonesArray* BtaCandidateFactory::fgBcfVertexBuffer = 0;
Int_t BtaCandidateFactory::fgBcfVertexPointer=0;
Int_t BtaCandidateFactory::fgBcfVertexWatermark=0;

void BtaCandidateFactory::reset() 
{
    fgBcfCandPointer = 0;
    fgBcfVertexPointer = 0;
}

BtaCandidateFactory* BtaCandidateFactory::instance()
{
    if (!fgBcfInstance) fgBcfInstance= new BtaCandidateFactory();
    return fgBcfInstance;
}

BtaCandidateFactory::~BtaCandidateFactory()
{
    delete fgBcfInstance;
    fgBcfInstance = 0;
}

//---------------------------------------

BtaCandidate* BtaCandidateFactory::newCandidate()
{
    BtaCandidate c;
    return newCandidate(c);
}

BtaCandidate* BtaCandidateFactory::newCandidate(const BtaCandidate &c)
{
    if (fgBcfCandBuffer==0) fgBcfCandBuffer = new TClonesArray("BtaCandidate");
    int current = fgBcfCandPointer++;
    if (current>fgBcfCandWatermark) fgBcfCandWatermark = current;
    if (current<fgBcfCandWatermark) (const_cast<BtaCandidate*>(&c))->removeAssociations();
    new ((*fgBcfCandBuffer)[current]) BtaCandidate(c);
    return getCandidate(current);
}

BtaCandidate* BtaCandidateFactory::newCandidate(const BtaFitParams &f, Int_t uid)
{
    if (fgBcfCandBuffer==0) fgBcfCandBuffer = new TClonesArray("BtaCandidate");
    int current = fgBcfCandPointer++;
    if (current>fgBcfCandWatermark) fgBcfCandWatermark = current;
    if (current<fgBcfCandWatermark) {
	BtaCandidate *c = getCandidate(current);
	c->removeAssociations();
    }
    new ((*fgBcfCandBuffer)[current]) BtaCandidate(f.p4(),f.getCharge());
    BtaCandidate *c = getCandidate(current);
    c->setPosition(f.getPosition());
    c->getFitParams().setCov7(f.cov7());
    c->setUid(uid);
    return c;
}

BtaCandidate* BtaCandidateFactory::newCandidate( TLorentzVector p4, 
			   TError &p4Err,
			   BtaCandListIterator& iterDau,
			   BtaAbsVertex& theVertex,
			   const PdtEntry* hypo )
{
    if (fgBcfCandBuffer==0) fgBcfCandBuffer = new TClonesArray("BtaCandidate");
    int current = fgBcfCandPointer++;
    if (current>fgBcfCandWatermark) fgBcfCandWatermark = current;
    if (current<fgBcfCandWatermark) {
	BtaCandidate *c = getCandidate(current);
	c->removeAssociations();
    }
    new ((*fgBcfCandBuffer)[current]) BtaCandidate(p4,p4Err,iterDau,theVertex,hypo);
    return getCandidate(current);
}

BtaCandidate* BtaCandidateFactory::getCandidate(Int_t i)
{
    if (fgBcfCandBuffer==0 || i>fgBcfCandWatermark) return 0;
    return (BtaCandidate *) (*fgBcfCandBuffer)[i];
}

Int_t BtaCandidateFactory::getCandidateWatermark() { return fgBcfCandWatermark; }

//---------------------------------------

BtaAbsVertex* BtaCandidateFactory::newVertex()
{
    BtaSimpleVertex c;
    return newVertex(c);
}

BtaAbsVertex* BtaCandidateFactory::newVertex(const BtaSimpleVertex &c)
{
    if (fgBcfVertexBuffer==0) fgBcfVertexBuffer = new TClonesArray("BtaSimpleVertex");
    int current = fgBcfVertexPointer++;
    if (current>fgBcfVertexWatermark) fgBcfVertexWatermark = current;
    new ((*fgBcfVertexBuffer)[current]) BtaSimpleVertex(c);
    return getVertex(current);
}

BtaAbsVertex* BtaCandidateFactory::getVertex(Int_t i)
{
    if (fgBcfVertexBuffer==0 || i>fgBcfVertexWatermark) return 0;
    BtaSimpleVertex *vertex = (BtaSimpleVertex *) (*fgBcfVertexBuffer)[i];
    return vertex;
}

Int_t BtaCandidateFactory::getVertexWatermark() { return fgBcfVertexWatermark; }

