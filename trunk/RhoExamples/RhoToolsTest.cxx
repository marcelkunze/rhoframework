//////////////////////////////////////////////////////////////////////////
//                                                                      //
// Description:								//
//	RhoToolsTest - a small test program for the basic tools		//
//      functionality							//
//									//
// C++ Macro to run in CINT						//
// ROOT Version by Marcel Kunze, RUB					//
//////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#include "TSystem.h"
#include "TStopwatch.h"
#include "TParticlePDG.h"
#include "RhoBase/TRho.h"
#include "RhoBase/TCandidate.h"
#include "RhoTools/TOpAdd4.h"
#include "RhoTools/TTreeNavigator.h"
#include "RhoTools/TBooster.h"
#include "RhoTools/TOpMakeTree.h"
#include "RhoTools/TDummyFitter.h"

#include <iostream>
using namespace std;

void RhoToolsTest(Int_t);
int main(int argc,char* argv[]) { RhoToolsTest(1); }

#endif

class TCandidate;
class TVector3;

class Sputnik {
    
public:
    Sputnik() { Launch(); }
    virtual ~Sputnik() {}
    int NRef(TCandidate const & c);
    void PC(const char* n, TCandidate& c);
    TVector3 PStar( double M, double m1, double m2, double th, double ph );
    void PrintAncestors( const TCandidate& cand, std::ostream& o=std::cout );
    void Launch();
};

int Sputnik::NRef(TCandidate const & c) {
    return c.NDaughters();
}

void Sputnik::PC(const char* n, TCandidate& c) {
    cout <<"   "<<n<<" uid:"<<c.Uid()<<" contains "<<c<<" r: "<<NRef(c)<<endl;
}

TVector3 Sputnik::PStar( double M, double m1, double m2, double th, double ph )
{
    double P = 0.5*sqrt((pow(M,2)-pow(m1-m2,2))*(pow(M,2)-pow(m1+m2,2)))/M;
    return TVector3( sin(th)*cos(ph)*P, sin(th)*sin(ph)*P, cos(th)*P );
}



void Sputnik::PrintAncestors( const TCandidate& cand, std::ostream& o) 
{
    const TCandidate* c = &cand;
    const TParticlePDG* pdtC(0);
    const TParticlePDG* pdtM(0);
    if( (pdtC=c->PdtEntry()) )
	o << "The Candidate " << pdtC->GetName();
    else
	o << "The Candidate of mass " << c->Mass();
    const TCandidate* m(0);
    if( (m=c->TheMother()) )
    {
	o << "'s mother ";
	if( (pdtM=m->PdtEntry()) )
	    o << " is a " << pdtM->GetName();
	else
	    o << " has mass " << m->Mass();
	o << "." << endl;
	PrintAncestors(*m,o); 
    }
    else
    {
	o << " has no mother." << endl;
    }
} 

void Sputnik::Launch()
{
    cout << "starting RhoToolsTest" << endl;
    
    // the tests are organized as blocks to let variables
    // go out of scope.  In the output, each block is 
    // separated by a lines of ----
    
    
    // start testing OpAdd4
    
    std::ostream& theStream = cerr; 
    theStream  <<  "Before Testing OpAdd4 " << endl;
    theStream << endl;
    
    
    TOpAdd4 b4;
    {
	cout << "create a pair of objects:" << endl;
	TCandidate c1(TLorentzVector(1,0,0,0),0);
	TCandidate c2(TLorentzVector(0,2,0,0),0);
	TCandidate c3(TLorentzVector(0,0,3,0),0);
	PC("c1",c1);
	PC("c2",c2);
	PC("c3",c3);
	
	cout << "after OpAdd4.Fill(c3, c1, c2); "<<endl;
	b4.Fill(c3,c1,c2);
	PC("c1",c1);
	PC("c2",c2);
	PC("c3",c3);
	
	TCandidate c4(TLorentzVector(0,0,3,0),0);
	cout <<endl<< "c4(); c4 = OpAdd4().combine( c1, c2); "<<endl;
	c4 = TOpAdd4().Combine(c1,c2);
	PC("c1",c1);
	PC("c2",c2);
	PC("c3",c3);
	PC("c4",c4);
	
	cout << "---------------------------------------------------"<<endl;
    }
    
    {
	cout << "create objects:" << endl;
	TCandidate c1(TLorentzVector(1,0,0,0),0);
	TCandidate c2(TLorentzVector(0,2,0,0),0);
	TCandidate c3(TLorentzVector(0,0,3,0),0);
	TCandidate c4(TLorentzVector(0,0,0,4),0);
	PC("c1",c1);
	PC("c2",c2);
	PC("c3",c3);
	PC("c4",c4);
	
	cout <<endl<< "OpAdd4.Fill(c4,c1,c2,c3); "<<endl;
	b4.Fill(c4,c3,c2,c1);
	PC("c1",c1);
	PC("c2",c2);
	PC("c3",c3);
	PC("c4",c4);
	
	cout << "---------------------------------------------------"<<endl;
    }
    
    {
	cout << "create objects:" << endl;
	TCandidate c1(TLorentzVector(1,0,0,0),1);
	TCandidate c2(TLorentzVector(0,2,0,0),-1);
	TCandidate c3(TLorentzVector(0,0,3,0),0);
	TCandidate c4(TLorentzVector(0,0,0,4),0);
	TCandidate c5(TLorentzVector(0,0,0,0),5);
	TCandidate c6(TLorentzVector(0,0,0,0),6);
	TCandidate c7(TLorentzVector(0,0,0,0),7);
	PC("c1",c1);
	PC("c2",c2);
	PC("c3",c3);
	PC("c4",c4);
	PC("c5",c5);
	PC("c6",c6);
	PC("c7",c7);
	
	cout <<endl<< "after Add4::Fill(c5,c1,c2); Add4::Fill(c6,c3,c4); "<<endl;
	b4.Fill(c5,c1,c2);
	b4.Fill(c6,c3,c4);
	PC("c1",c1);
	PC("c2",c2);
	PC("c3",c3);
	PC("c4",c4);
	PC("c5",c5);
	PC("c6",c6);
	PC("c7",c7);
	
	cout <<endl<< "after Add4::Fill(c7,c5,c6); "<<endl;
	b4.Fill(c7,c5,c6);
	PC("c1",c1);
	PC("c2",c2);
	PC("c3",c3);
	PC("c4",c4);
	PC("c5",c5);
	PC("c6",c6);
	PC("c7",c7);
	
	// do some additional checks of copying, etc
	cout <<endl<< "after c8(c7);"<<endl;
	TCandidate c8(c7);
	PC("c1",c1);
	PC("c2",c2);
	PC("c3",c3);
	PC("c4",c4);
	PC("c5",c5);
	PC("c6",c6);
	PC("c7",c7);
	PC("c8",c8);
	
	cout <<endl<< "create c9(c1); then c9 = c7;"<<endl;
	TCandidate c9(c1); c9 = c7;
	PC("c1",c1);
	PC("c2",c2);
	PC("c3",c3);
	PC("c4",c4);
	PC("c5",c5);
	PC("c6",c6);
	PC("c7",c7);
	PC("c8",c8);
	PC("c9",c9);
	
	cout <<endl<< "c9 = c5;"<<endl;
	c9 = c5;
	PC("c1",c1);
	PC("c2",c2);
	PC("c3",c3);
	PC("c4",c4);
	PC("c5",c5);
	PC("c6",c6);
	PC("c7",c7);
	PC("c8",c8);
	PC("c9",c9);
	
	cout << endl;
	cout << "testing Overlaps:"<<endl;
	cout << "c1 and c2: "<<(c1.Overlaps(c2)?"t":"f")<<endl;
	cout << "c9 and c5: "<<(c9.Overlaps(c5)?"t":"f")<<endl;
	cout << "c8 and c5: "<<(c8.Overlaps(c5)?"t":"f")<<endl;
	cout << "c7 and c5: "<<(c7.Overlaps(c5)?"t":"f")<<endl;
	cout << "c6 and c5: "<<(c6.Overlaps(c5)?"t":"f")<<endl;
	cout << "c5 and c5: "<<(c5.Overlaps(c5)?"t":"f")<<endl;
	cout << "c5 and c9: "<<(c5.Overlaps(c9)?"t":"f")<<endl;
	cout << "c5 and c8: "<<(c5.Overlaps(c8)?"t":"f")<<endl;
	cout << "c5 and c7: "<<(c5.Overlaps(c7)?"t":"f")<<endl;
	cout << "c5 and c6: "<<(c5.Overlaps(c6)?"t":"f")<<endl;
	cout << "c5 and c5: "<<(c5.Overlaps(c5)?"t":"f")<<endl;
	cout << "---------------------------------------------------"<<endl;
    }
    
    //
    // start testing OpMakeTree
    
    theStream  <<  "Before Making Tree " << endl;
    theStream << endl;
    
    {
	
	cout << "Initialization of Pdt " << endl;
	//Pdt::readMCppTable("PARENT/PDT/pdt.table");
	TDatabasePDG *Pdt = TRho::Instance()->GetPDG();
	cout << "done." << endl;
	
	// now let's consider the Y(4S)
	double beamAngle  = 0.0204;
	double beamDeltaP = 9.-3.1; 
	
	TVector3 pY4S( -beamDeltaP*sin(beamAngle),0, 
	    beamDeltaP*cos(beamAngle) );
	TCandidate Y4S( pY4S, Pdt->GetParticle("Upsilon(4S)") );
	TTreeNavigator::PrintTree( Y4S );
	cout << "theta " << Y4S.P3().Theta() << endl;
	cout << "phi "   << Y4S.P3().Phi() << endl;
	
	// instanciate a Booster
	TBooster cmsBooster( &Y4S , kTRUE);
	
	// Let's imagine a photon with a certain angle in the CMS frame
	
	// a photon 
	double ePhot = 1.;
	double photAngle = 30*3.1416/180.;
	double photPhi   = 45*3.1416/180.;
	TCandidate 
	    photon( TVector3( ePhot*sin(photAngle)*cos(photPhi), ePhot*sin(photAngle)*sin(photPhi), ePhot*cos(photAngle) ), 
	    Pdt->GetParticle("gamma") );
	
	cout << endl << "The original photon in the CMS frame " << endl;
	TTreeNavigator::PrintTree( photon );
	cout << "theta " << photon.P3().Theta() << endl;
	cout << "phi "   << photon.P3().Phi() << endl;
	
	// the same photon in the LAB frame :
	TCandidate boostedPhoton = cmsBooster.BoostFrom( photon );
	TLorentzVector theLabP4 = cmsBooster.BoostedP4( photon, TBooster::From );
	cout << "the lab Four-Vector (" << 
	    theLabP4.X() << "," << theLabP4.Y() << "," << theLabP4.Z() << ";" << theLabP4.E() << ")" << endl;
	cout << endl << "The lab boosted photon " << endl;
	TTreeNavigator::PrintTree( boostedPhoton );
	cout << "theta " << boostedPhoton.P3().Theta() << endl;
	cout << "phi "   << boostedPhoton.P3().Phi() << endl;
	
	// now boost back in the CMS frame
	TLorentzVector theP4 = cmsBooster.BoostedP4( boostedPhoton, TBooster::To );
	cout << "the Four-Vector in CMS frame (" << 
	    theP4.X() << "," << theP4.Y() << "," << theP4.Z() << ";" << theP4.E() << ")" << endl;
	cout << " from cand :    (" << 
	    photon.P4().X() << "," << photon.P4().Y() << "," << photon.P4().Z() << ";" <<photon.P4().E() << ")" << endl;
	
	
	double mPsi = Pdt->GetParticle("J/psi")->Mass();
	double mMu  = Pdt->GetParticle("mu+")->Mass();
	double mPi  = Pdt->GetParticle("pi+")->Mass();
	double mK   = Pdt->GetParticle("K+")->Mass();
	double mB   = Pdt->GetParticle("B+")->Mass();
	double mDst = Pdt->GetParticle("D*+")->Mass();
	double mD0  = Pdt->GetParticle("D0")->Mass();
	double mRho = Pdt->GetParticle("rho+")->Mass();
	
	//
	// Let's start with a simple example   :  B+ -> J/Psi K+
	//
	
	// muons in the J/Psi frame
	double E(0.), P(0.), th(0.), ph(0.);
	TVector3 p3;
	
	th = 0.3;
	ph = 1.3;
	P  = 0.5*sqrt( pow(mPsi,2) - 4*pow(mMu,2) );
	E  = sqrt( pow(mMu,2) + pow(P,2) );
	p3 = TVector3( sin(th)*cos(ph)*P, sin(th)*sin(ph)*P, cos(th)*P );
	
	TLorentzVector mu1P4(  p3, E );
	TLorentzVector mu2P4( -p3, E );
	
	// J/psi in the B frame
	th = 1.1;
	ph = 0.5;
	P = 0.5*sqrt((pow(mB,2)-pow(mPsi-mK,2))*(pow(mB,2)-pow(mPsi+mK,2)))/mB;
	p3 = TVector3( sin(th)*cos(ph)*P, sin(th)*sin(ph)*P, cos(th)*P );
	E = sqrt( pow(mPsi,2) + pow(P,2) ); 
	TVector3 boostVector( p3.X()/E, p3.Y()/E,p3.Z()/E );
	mu1P4.Boost( boostVector );
	mu2P4.Boost( boostVector );
	E = sqrt( pow(mK,2) + pow(P,2) ); 
	TLorentzVector KP4( -p3, E );
	
	// create the TCandidates
	TCandidate* mu1 = new TCandidate( mu1P4.Vect(), Pdt->GetParticle("mu+") );
	TCandidate* mu2 = new TCandidate( mu2P4.Vect(), Pdt->GetParticle("mu-") );
	TCandidate* K   = new TCandidate( KP4.Vect(), Pdt->GetParticle("K+") );
	
	// now create the Make Tree operator
	TOpMakeTree op;
	
	// now create the J/Psi
	TCandidate psi = op.Combine( *mu1, *mu2 );
	psi.SetType( "J/psi" );
	psi.SetMassConstraint();
	
	cout << endl << "The original psi " << endl;
	TTreeNavigator::PrintTree( psi );
	
	// now create the B+
	TCandidate B = op.Combine( psi, *K );
	B.SetType( "B+" );
	B.SetMassConstraint();
	
	cout << endl << "The B " << endl;
	TTreeNavigator::PrintTree( B );
	
	// create a TTreeNavigator
	TTreeNavigator treeNavigator( B );
	TTreeNavigator::PrintTree( psi );
	
	theStream << "After tree making" << endl;
	theStream << endl;
	
	//instanciate the fitter with the B Candidate
	VAbsFitter* fitter = new TDummyFitter( psi );
	
	theStream << "After fitter construction " << endl;
	theStream << endl;
	
	// get the "fitted" TCandidates
	TCandidate fittedPsi = fitter->GetFitted( psi );
	TTreeNavigator::PrintTree( fittedPsi );
	
	TCandListIterator iterDau =  psi.DaughterIterator();
	TCandidate* dau=0;
	while( (dau=iterDau.Next()) )
	{
	    TCandidate fittedDau = fitter->GetFitted( *dau );
	    TTreeNavigator::PrintTree( fittedDau );
	}
	
	delete fitter;
	
	fitter = new TDummyFitter( B );
	
	// get the "fitted" TCandidates
	TCandidate fittedB = fitter->GetFitted( B );
	TTreeNavigator::PrintTree( fittedB );
	
	iterDau.Rewind();
	dau=0;
	while( (dau=iterDau.Next()) )
	{
	    TCandidate fittedDau = fitter->GetFitted( *dau );
	    TTreeNavigator::PrintTree( fittedDau );
	}
	
	TCandidate hello = fitter->GetFitted( psi );
	TTreeNavigator::PrintTree( hello );
	
	delete fitter;
	
	theStream << "After fitter deletion " << endl;
	theStream << endl;
	
	// first let's boost the kaon
	TCandidate boostedKaon = cmsBooster.BoostTo( *K );
	cout << endl << "The boosted Kaon " << endl;
	TTreeNavigator::PrintTree( *K );
	
	// let's boost the psi
	TCandidate boostedPsi = cmsBooster.BoostTo( psi );
	cout << endl << "The boosted Psi " << endl;
	TTreeNavigator::PrintTree( boostedPsi );
	
	
	theStream << "before tree boosting " << endl;
	theStream << endl;
	
	// now let's boost the B in the Y4S frame
	TCandidate boostedB = cmsBooster.BoostTo( B );
	
	// let's see the tree
	cout << endl << "The boosted B " << endl;
	TTreeNavigator::PrintTree( boostedB );
	
	theStream << "after tree boosting " << endl;
	theStream << endl;
	
	// now let's boost a list
	TCandList theList;
	theList.Add( psi );
	theList.Add( *K );
	theList.Add( B );
	TCandList theBoostedList;
	cmsBooster.BoostTo( theList, theBoostedList );
	TCandListIterator iter(theBoostedList);
	TCandidate* c(0);
	while( (c=iter.Next()) )
	{
	    cout << "boosted cand " << c->PdtEntry()->GetName() << endl;
	    TTreeNavigator::PrintTree( *c );
	}
	
	
	theBoostedList.Cleanup();
	
	PrintAncestors( *mu1 );
	
	delete mu1;
	delete mu2;
	delete K;
	
	cout << "---------------------------------------------------"<<endl;
     }
     
     theStream  <<  "At the end of the test program " << endl;
     theStream << endl;
}

void RhoToolsTest(Int_t nTimes=1)
{
#ifdef __CINT__
    gROOT.Macro("$RHO/RhoMacros/LoadLibs.C");
#endif
    
    TRho Rho("RhoTools test");
    cout << Rho;
    
    TStopwatch timer;					    // Measure the execution time
    timer.Start();
    for (int i=0;i<nTimes;i++) Sputnik revival;
    timer.Stop();
    
    cout<<" ----- Realtime:   "<<timer.RealTime()<<"sec"<<endl;
    cout<<" ----- Cputime:    "<<timer.CpuTime()<<"sec"<<endl;
}

