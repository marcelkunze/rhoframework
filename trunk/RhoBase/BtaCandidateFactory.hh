#ifndef TFACTORY_H
#define TFACTORY_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaCandidateFactory								//
//                                                                      //
// Object factory							//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Feb. 01						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "TClonesArray.h"

class BtaCandidate;
class BtaFitParams;
class BtaSimpleVertex;
class TLorentzVector;
class TError;
class BtaCandListIterator;
class BtaAbsVertex;
class PdtEntry;

class BtaCandidateFactory : public TObject {
    
public:
    static BtaCandidateFactory* instance();
    virtual ~BtaCandidateFactory();
    static void reset();
    static BtaCandidate* newCandidate();
    static BtaCandidate* newCandidate(const BtaCandidate &c);
    static BtaCandidate* newCandidate(const BtaFitParams &f, Int_t uid=0);
    static BtaCandidate* newCandidate( TLorentzVector p4, 
	TError& p4Err,
	BtaCandListIterator& iterDau,
	BtaAbsVertex& theVertex,
	const PdtEntry* hypo = 0);

    static BtaCandidate* getCandidate(Int_t);
    static Int_t getCandidateWatermark();
    static BtaAbsVertex* newVertex();
    static BtaAbsVertex* newVertex(const BtaSimpleVertex &c);
    static BtaAbsVertex* getVertex(Int_t);
    static Int_t getVertexWatermark();

private:
    BtaCandidateFactory() {}
    static BtaCandidateFactory*	fgBcfInstance;  //! Do not stream
    static TClonesArray*   fgBcfCandBuffer; //! Candidate buffer
    static Int_t	   fgBcfCandPointer; //! Actual position
    static Int_t	   fgBcfCandWatermark; //! Max. position
    static TClonesArray*   fgBcfVertexBuffer; //! Candidate buffer
    static Int_t	   fgBcfVertexPointer; //! Actual position
    static Int_t	   fgBcfVertexWatermark; //! Max. position
    
    ClassDef(BtaCandidateFactory,1) //Factory for candidates and vertices

};      
#endif
