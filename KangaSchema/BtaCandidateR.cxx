#include "KangaSchema/BtaCandidateR.h"
#include "KangaSchema/BtaAbsVertexR.h"
#include "RhoBase/VAbsVertex.h"

ClassImp( BtaCandidateR )

#include <iostream>
using namespace std;

//
//   move on, there is nothing to see here
//


BtaCandidateR::BtaCandidateR()
  : RooEvtObj<BtaCandidate>() 
{
}


BtaCandidateR::~BtaCandidateR()
{
}

void BtaCandidateR::PrintOn(ostream &o) const
{
    //Vertex
    if (DecayVtx() != 0) {
       VAbsVertex *vtx = DecayVtx()->Transient();
       o << "Vertex " << DecayVtx()->refToMe().id() << ": " << *vtx;
    }	

    // Id
    UInt_t myId = refToMe().id();
    o << "Candidate "<< myId << ": ";

    // Daughters
    for (int n=0;n<NDaughters();++n)
    {
	o << " "<< Daughter(n);
    }

    o << endl;
}
