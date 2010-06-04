//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFMcBase								//
//                                                                      //
// Monte Carlo truth persistent class in PAF(Pico Analysis Framework)	//
//									//
// Author List:								//
// Rolf Dubitzky - Thu Oct 14 1999					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

// ROOT stuff
#include "TDatabasePDG.h"
#include "PAFSchema/PAFMcBase.h"

ClassImp(PAFMcBase)

#include <iostream>
#include <iomanip>
using namespace std;

PAFMcBase::PAFMcBase() :
fStatus(0),              // status code 
fPdtID(0),               // The particle id 
fMother(-1),             // The position (index) of the mother in list 
fFirstDaughter(-2),      // Position (index) of the first daughter... in list 
fLastDaughter(-2),       // Position (index) of the last daughter... in list 
fPx(0),                  // 4-Momentum
fPy(0),                  // 4-Momentum
fPz(0),                  // 4-Momentum
fEnergy(0),              // 4-Momentum
fMass(0),
fStartVx(0),             // Vertex information 
fStartVy(0),             // Vertex information 
fStartVz(0),             // Vertex information 
fLifeTime(0),            // actual lifetime
fAodMatchIndex(-1),       // Pointer to the best matching reco object
fAodMatchConsistency(0)  // Consistency of the best matching reco object
{}


PAFMcBase::PAFMcBase(const Int_t status, const Int_t pdtid,
                     const Int_t mother, const Int_t firstd, const Int_t lastd, 
                     const TLorentzVector& momentum, const TVector3& startvertex,
                     const Double_t lifetime, const Int_t aodindex, const Double_t aodcons) :
//  in this constructor no default values are privided! 
//  Think about every value every time :-)
fStatus(status),
fPdtID(pdtid),                   
fMother(mother),                  
fFirstDaughter(firstd),           
fLastDaughter(lastd),        
fMass( momentum.Mag() ),
fLifeTime(lifetime),               
fAodMatchIndex(aodindex),             
fAodMatchConsistency(aodcons)             
{
    SetMomentum(momentum);
    SetStartVertex(startvertex);
}

PAFMcBase::PAFMcBase( const PAFMcBase& mcb ) :
fStatus( mcb.GetStatus() ),              // status code 
fPdtID( mcb.GetPdtID() ),               // The particle id 
fMother( mcb.GetMother() ),              // The position (index) of the mother in list 
fFirstDaughter( mcb.GetFirstDaughter() ),       // Position (index) of the first daughter... in list 
fLastDaughter( mcb.GetLastDaughter() ),        // Position (index) of the last daughter... in list 
fPx( mcb.GetPx() ),                  // 4-Momentum
fPy( mcb.GetPy() ),                  // 4-Momentum
fPz( mcb.GetPz() ),                  // 4-Momentum
fEnergy( mcb.GetEnergy() ),              // 4-Momentum
fMass( mcb.GetMass() ),  
fStartVx( mcb.GetStartVx() ),             // Vertex information 
fStartVy( mcb.GetStartVy() ),             // Vertex information 
fStartVz( mcb.GetStartVz() ),             // Vertex information 
fLifeTime( mcb.GetLifeTime() ),           // actual lifetime
fAodMatchIndex( mcb.GetAodMatchIndex() ),  // Pointer to the best matching reco object
fAodMatchConsistency( mcb.GetAodMatchConsistency() )
{ }


void PAFMcBase::PrintOn(std::ostream& o) const 
{ 
    //    const char* pdtname(0);
    //    const Pdt *pdt = Pdt::Instance();
    //    PdtEntry* pdtentry = pdt->lookup( static_cast<const PdtLund::LundType>( fPdtID ) ); // RD!! a pitty for OSF, the const is nessecary (a Sun Bug)
    //    if (pdtentry) pdtname = pdtentry->name();
    
    //    ios::fmtflags f = cout.flags() );  // CC is not capable of ios::fmtflags
    o.setf( ios::scientific );
    o.precision( 3 );
    o  
        << " PID: "    << setw( 5 )<< fPdtID << " "
	//	<< setiosflags( ios::left ) << setw(13 ) << pdtname  << resetiosflags( ios::left )
        << " Stat: "   << setw( 2 )<< fStatus
        << " Mom: "    << setw( 3 )<< fMother
        << " fd: "     << setw( 3 )<< fFirstDaughter
        << " ld: "     << setw( 3 )<< fLastDaughter
        << " P: "      << setw(10 )<<  fPx <<" ; "
	<< setw(10 )<<  fPy <<" ; "
	<< setw(10 )<<  fPz <<" ; "
	<< setw(10 )<<  fEnergy
        << " m: "      << setw(10 )<<  fMass
        << " Vtx: "    << setw(10 )<<  fStartVx << " ; "
	<< setw(10 )<<  fStartVy << " ; "
	<< setw(10 )<<  fStartVz 
        << " tof: "    << setw(10 )<<  fLifeTime
        << " AOD: "    << setw( 3 )<< fAodMatchIndex << " (" << fAodMatchConsistency << ")";
    //    o.flags( f );
}        

std::ostream&  operator << (std::ostream& o, const PAFMcBase& a) { a.PrintOn(o); return o; }
