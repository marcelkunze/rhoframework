//--------------------------------------------------------------------------
// 
// Description:
//     incBDStarPi
//     PAF Analysis Example:
//     inclusive Analysis of
//     B0 ---> DStar + Pi ---> Pi  ( D )  Pi
//    
// Environment:
//      Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Helmut Schmuecker, Bochum University      
//
// Copyright Information:
//      Copyright (C) 1999          
//
//------------------------------------------------------------------------

#include "PAFExamples/incBDstarPi.hh"
#include "RhoManager/TEventManager.h"
#include "RhoManager/TPersistenceManager.h"
#include "RhoSelector/TSimpleVertexSelector.h"

#include "assert.h"

#include "TStyle.h"
#include "TRandom.h"

#include "RhoManager/TRunDB.h"
#include "RhoManager/TEventCollection.h"

#include "RhoBase/TCandList.h"
#include "RhoBase/TCandListIterator.h"
#include "RhoBase/TCandidate.h"

#include "TApplication.h"
#include "TCanvas.h"

#include <iostream>
using namespace std;

#ifndef SHLIB

// --- some useful constants

const double xBeam=0.1;
const double yBeam=0.32;
const double zBeam=-0.5;

const double EBeam=10.58/2.0;

const TVector3 BeamPos(xBeam,yBeam,zBeam);
const TLorentzVector UpsFrame(0.11943,0,-5.8534,12.0919); 
const TVector3 UpsBoost=UpsFrame.BoostVector();

const Double_t mPi=0.13957; //Pdt::mass("pi+"); //0.13957; // ok, should be taken from PDT
const Double_t mDStar=2.01;//Pdt::mass("D*+");//2.01;
const Double_t mB=5.2792;//Pdt::mass("B0");//5.2792;
//const Double_t mKa=Pdt::mass("K+");

const Double_t gammaB=EBeam/mB; 
const Double_t PB=sqrt(EBeam*EBeam-mB*mB);
const Double_t betaB=PB / EBeam;


// --- some cut values

//#define TAGCUT "TAG_i_nTracks>2 && TAG_f_R2<0.35"   // tag preselection, Kanga format 
#define TAGCUT "_tag._nTracks>2 && _tag._R2<0.35"   // tag preselection, PAF format 

const double softPiMinP=0.05;  // alternatively 0.09 ( 0.05 )
const double softPiMaxP=0.8;   // alternatively 0.24 ( 0.8 )

const int BINS=40;


Int_t main(int argc,char** argv )
{
    // Create a named Framework and register services
    TRho PAF("B0 ---> DStar + Pi ---> Pi  ( D )  Pi");
    PAF.RegisterService(new TPersistenceManager("DStar.root")); // Saves the histograms
    TAnalysis *myAnal = new TAnalysis(argc,argv);
    //myAnal->SetParm("cluster","test");
    //myAnal->SetParm("run",11118);
    myAnal->SetParm("cluster","system");
    myAnal->SetParm("start",10000);
    myAnal->SetParm("end",10500);
    myAnal->SetParm("tagcut",TAGCUT);
    DStarAnalysis *theModule = new DStarAnalysis("PAFAnalysis","Sample Analysis Module");
    theModule->SetParm("draw",100);
    myAnal->Add(theModule);
    
    PAF.RegisterService(myAnal);			// The sample analysis
    
    cout << PAF;
    
    PAF.GetAnalysis()->Run(); // Execute the analysis
    
    return 0;
}
#endif

DStarAnalysis::DStarAnalysis(const char* const theName, const char* const theDescription) : 
TModule(theName, theDescription) 
{
    TApplication *theApp = new TApplication("App",0, 0); 
    
    c1 = new TCanvas("c1","incBDstarPi",0,0,800,800);
    c1->Divide(2,2);
    InitHistograms();
}

Bool_t
DStarAnalysis::BeginJob(TEventManager*)
{
    
    // --- set up random generator
    gRandom->SetSeed();
    
    ComputeHardPionCuts();  //initialization 
    
    chargedList = new TCandList("chargedList");
    piList	= new TCandList("piList");
    leptonList	= new TCandList("leptonList");
    
    // If you want to store selected Events in a collection, you
    // should create the Collection-object at the End of this 
    // Method :
    
    vertexer = new TSimpleVertexSelector("simplevertexer",4);
    _coll = new TEventCollection("DStar_Collection.root");
    
    return kTRUE;
}




Bool_t DStarAnalysis::Event(TEventManager *eventmgr){
    
    // This is the statement to add the current Event
    // to the Collection :
    //ev->AddEventToCollection(_coll);
    
    //AbsTag &tag = eventmgr->GetTag();       // Shortcut to access TAG
    
    //TVector3 UpsBoost=-1.0*eventmgr->GetHeader().GetBoost();
    //cerr << UpsBoost.X() << " " <<  UpsBoost.Y() << " " << UpsBoost.Z() << endl;

    static Int_t counter = 0;
    if (counter++%GetIntParm("draw") == 0) DrawHistograms();

    
    chargedList->Cleanup();
    piList->Cleanup();
    leptonList->Cleanup(); 
    
    eventmgr->Fill(chargedList);
    
    TCandListIterator iter(*chargedList);
    TCandidate *c;
    
    Int_t nLepton=0;
    Int_t nTrk=0;
    Int_t qEvent=0;
    
    
    // loop over all charged tracks and apply some cuts
    while (c = iter.Next()) {
	if (c->GetCharge()==0) continue;
	TVector3 poca=c->GetPosition();
	Double_t yy=poca.Y()-yBeam; yy*=yy;
	Double_t doca=sqrt(poca.X()*poca.X()+yy); // strange!
	if(TMath::Abs(poca.Z()-zBeam)<4.0 && doca<1.0){		// distance to beamvertex
	    nTrk++;
	    qEvent += (Int_t) c->Charge();
	    
	    const VAbsMicroCandidate &mc = c->GetMicroCandidate();
	    
	    TLorentzVector p4cm(c->P4());
	    p4cm.Boost(UpsBoost); // boost into Y(4S)-Frame
	    Double_t pcm(p4cm.Vect().Mag());
	    
	    /*
	    Double_t LAT=mc.GetEmcLateralShape();  // ???? Thorsten's LAT  == lateral shape ???
	    Double_t EoverP=mc.GetEmcRawEnergy() / c->P();
	    
	      if(EoverP>0.7 &&  // lepton selection 
	      EoverP<1.4 && 
	      LAT<0.6 && 
	      c->P3().Z()<0.15 && 
	      mc.GetEmcNumberOfCrystals()>2){
	      
		if(pcm>1.3 && pcm<2.5){ 
		// some histtogramming stuff
		leptonList->Add(*c);
		}
		}else{ // if not an e- test if it is a muon 
		if(mc.GetIfrHitLayers()>12){      // muons with E/p near 1 ??
		if(pcm>1.3 && pcm<2.5){ 
		// some histtogramming stuff
		leptonList->Add(*c);
		}
		}  
		}
	    */
	    
	    if( !(pcm>hardPiMinP && pcm<hardPiMaxP ||
		pcm>softPiMinP && pcm<softPiMaxP) ) continue;  // cut on momentum of soft and hard pion
	    
	    // --- some cuts on track quality
	    
	    if(c->Pt()<0.035) continue; 
	    if(c->Pt()>0.1 && mc.GetDchHits()<8) continue;  
	    if(mc.GetSvtHits()<6) continue;
	    if(mc.GetDchHits()<8 && mc.GetSvtHits()<8) continue;
	    if(c->P()<0.2 && mc.GetSvtMeanDEdx()<6.0-30.0*c->P()) continue;
	    if(mc.GetDchHits()<4 && mc.GetProbability()<0.01) continue;
	    
	    c->SetMass(mPi);
	    piList->Add(*c); 
	    
	} // if dist to beamvertex 
    } // while
    
    
    if(TMath::Abs(qEvent)>3 || nTrk<3) return kTRUE;  
    
    
    TCandListIterator iterPi1(*piList);
    TCandidate *pi1,*pi2;
    
    // now combine the two "pions"
    while(pi1=iterPi1.Next()){
	TCandListIterator iterPi2(iterPi1);
	while(pi2=iterPi2.Next()){
	    
	    //if(pi1->Charge()*pi2->Charge()==0) continue; // like sign and unlike sign combinations
	    
	    TVector3 KsV=0.5*(pi1->GetPosition()+pi2->GetPosition());
	    //if(vertexer->Accept(*pi1,*pi2))
	    //KsV=vertexer->GetVertex();
	    KsV-=BeamPos;
	    
	    //if(KsV.Perp2()>0.4) continue; 
	    
	    // ------------------------- select the hard pi
	    // Note: all LorentzVector are boosted into Y(4S) Frame
	    
	    TCandidate *pih,*pis;
	    TLorentzVector P4pis,P4pih;
	    TLorentzVector P4pi1(pi1->P4());
	    P4pi1.Boost(UpsBoost);
	    TLorentzVector P4pi2(pi2->P4());
	    P4pi2.Boost(UpsBoost);
	    Double_t Ppi1(P4pi1.Vect().Mag());
	    Double_t Ppi2(P4pi2.Vect().Mag());
	    Double_t Ppih,Ppis;
	    
	    if( Ppi1>hardPiMinP &&  Ppi1<hardPiMaxP &&
		Ppi2>softPiMinP &&  Ppi2<softPiMaxP){
		pih=pi1;
		pis=pi2;
		P4pih=P4pi1;
		P4pis=P4pi2;
		Ppih=Ppi1;
		Ppis=Ppi2;
	    }else
		if( Ppi2>hardPiMinP &&  Ppi2<hardPiMaxP &&
		    Ppi1>softPiMinP &&  Ppi1<softPiMaxP){
		    pih=pi2;
		    pis=pi1;
		    P4pih=P4pi2;
		    P4pis=P4pi1;
		    Ppih=Ppi2;
		    Ppis=Ppi1;
		}else continue;
		
		
		Double_t EoP=pih->GetMicroCandidate().GetEmcRawEnergy() / pih->P(); // EoverP cut on hard pi
		
		// ----------------------- get B direction wrt hard pi
		// ------1) calculate angle between hard pi and B in the Y(4S)-Frame  
		//          P4pi * P4B invariance :  (MB,0)*(Epi,Ppi) = (EB,PB)*(Epi,Ppi) in Y(4S)-Frame
		//          solve for cos(theta) : cos(theta) = gammaB*Epi(Y)-Epi(B) / betaB / gammaB / Ppi(Y)    
		
		Double_t costhB=(gammaB*P4pih.E()-hardPiECM) / betaB / gammaB / Ppih;
		
		// build B 4-vector with arbitrary phi (wrt hardPI)
		
		Double_t phiB=gRandom->Rndm()*TMath::Pi()*2.0;
		TLorentzVector P4B(1,1,1,EBeam);
		P4B.SetRho(PB);
		P4B.SetTheta(TMath::ACos(costhB));
		P4B.SetPhi(phiB);
		// rotate B 4-vector from hardpi-frame into Y(4S)-Frame
		TVector3 PiDirection=P4pih.Vect().Unit();
		P4B.RotateUz(PiDirection); // hopefully ! what does Uz mean ?
		
		//test,test,test one two three
		//if(TMath::Abs(cos(P4B.Vect().Angle(P4pih.Vect()))-costhB)>1E-8){
		//cerr << "think 3D !!! " << endl;
		//exit(1);
		//}
		
		// --- compute the missing mass
		
		TLorentzVector P4D(P4B - P4pih - P4pis);  // D four vector
		Double_t missmass=P4D.M();
		
		if (missmass<0) continue; // reject unphysical cases
		if (missmass<1.8) continue; 
		
		// --- calculate D* helicity angle 
		
		TLorentzVector P4DS(P4D+P4pis);
		TVector3 boosterDS(-P4DS.BoostVector());
		TLorentzVector refdir(P4pih);
		refdir.Boost(boosterDS);
		TLorentzVector pisInDS(P4pis);
		pisInDS.Boost(boosterDS);
		
		Double_t heli( TMath::Cos( pisInDS.Vect().Angle(refdir.Vect()) ) );
		
		// --- look for closest track to hard pi
		
		TCandListIterator iter(*chargedList);
		TCandidate *c;
		Double_t minsoftangle(999);
		Double_t minhardangle(999);
		while (c = iter.Next()) {
		    if(c->Overlaps(*pi1) || c->Overlaps(*pi2)) continue;
		    TVector3 poca=c->GetPosition(); 
		    Double_t yy=poca.Y()-yBeam; yy*=yy;
		    Double_t doca=sqrt(poca.X()*poca.X()+yy);
		    if(TMath::Abs(poca.Z()-zBeam)<4.0 && doca<1.0){   // distance to beamvertex
			TLorentzVector P4(c->P4());
			P4.Boost(UpsBoost);
			TVector3 P3(P4.Vect());
			Double_t P(P3.Mag());
			Double_t softangle=P4pis.Angle(P3);
			Double_t hardangle=P4pih.Angle(P3);
			if(P>0.150){
			    if(softangle<minsoftangle)
				minsoftangle=softangle;
			    if(hardangle<minhardangle)
				minhardangle=hardangle;
			}
		    }
		}
		
		if(minsoftangle==0 || minhardangle==0)
		    cerr << "Warning, angle==0, something wrong with OverlapFunction ?" << endl;
		
		Bool_t iskaon=DrcSimpleKaonPid(*pih); 
		
		// --- histogram stuff
		
		tuppa->Column( "smom", (Float_t)Ppis , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "hmom", (Float_t)Ppih , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "heli", (Float_t)heli , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "missm", (Float_t)missmass , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "sc", (Float_t)pis->Charge() , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "hc", (Float_t)pih->Charge() , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "sangle", (Float_t)minsoftangle , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "hangle", (Float_t)minhardangle , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "kaon", (Float_t)iskaon , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "EoP", (Float_t)EoP , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "vr", (Float_t)KsV.Perp() , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "vz", (Float_t)KsV.Z() , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "scos", (Float_t)P4pis.Vect().CosTheta() , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "hcos", (Float_t)P4pih.Vect().CosTheta() , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->Column( "run", (Float_t)eventmgr->GetRunNumber() , 0.0f, "DOUBLE"); // In a ntuple
		tuppa->DumpData();
		
		if(pi1->Charge()*pi2->Charge()<0){
		    histMissMass1->Fill(missmass);
		    if(!iskaon && minhardangle>0.4 && EoP<0.7 && Ppis>0.09 && Ppis<0.24 && KsV.Perp()<0.4)
			histMissMass2->Fill(missmass);
		}else{
		    histMissMass3->Fill(missmass);
		    if(! iskaon && minhardangle>0.4 && EoP<0.7 && Ppis>0.09 && Ppis<0.24 && KsV.Perp()<0.4)
			histMissMass4->Fill(missmass);
		}
		
    }// while
  }//while
  
  return kTRUE;
}

void DStarAnalysis::InitHistograms()
{
    TPersistenceManager *persmgr = TRho::Instance()->GetPersistenceManager();

    // Set up 1d-histograms   
    
    persmgr->SetDir("Histograms");
    gStyle->SetHistFillColor(5);
    histMissMass1 = new TH1F("MissMass1","missing mass 1",BINS,1.8,1.9);
    histMissMass2 = new TH1F("MissMass2","missing mass 2",BINS,1.8,1.9);
    histMissMass3 = new TH1F("MissMass3","missing mass 3",BINS,1.8,1.9);
    histMissMass4 = new TH1F("MissMass4","missing mass 4",BINS,1.8,1.9);

    // Set up ntuples

    persmgr->SetDir("Ntuples");
    tuppa = persmgr->Ntuple("tuppa");
    
}

void DStarAnalysis::ComputeHardPionCuts(){
    
    hardPiPCM=sqrt((mB*mB-(mPi+mDStar)*(mPi+mDStar)) * (mB*mB-(mPi-mDStar)*(mPi-mDStar))) / mB / 2.0;
    cerr << hardPiPCM << endl;
    hardPiECM=sqrt( hardPiPCM*hardPiPCM+mPi*mPi);
    
    hardPiMaxP=gammaB*hardPiPCM+betaB*gammaB*hardPiECM; 
    hardPiMinP=gammaB*hardPiPCM-betaB*gammaB*hardPiECM;
    
    cerr << "\n\n" << hardPiMaxP << " > hardPiP > " << hardPiMinP << endl << endl;
    
    return;
} 


Bool_t DStarAnalysis::DrcSimpleKaonPid(TCandidate &c){
    
    const Double_t mKa=0.49358;//Pdt::mass("K+");
    const Double_t mpi=0.13957;
    const Double_t nq=1.473;
    const Double_t minphot=10.0;
    const Double_t sigma=5.0;
    
    const VAbsMicroCandidate &mc = c.GetMicroCandidate();
    Double_t p=c.P();
    
    Double_t e=sqrt(p*p+mpi*mpi);
    Double_t betan=e/p/nq;
    if(betan>1)
	return kFALSE;
    Double_t thcpi=acos(betan);
    
    e=sqrt(p*p+mKa*mKa);
    betan=e/p/nq;
    if(betan>1)
	return kFALSE;
    Double_t thcka=acos(betan);
    
    Double_t thc=mc.GetDrcThetaC();  
    Double_t thce=mc.GetDrcThetaCError();
    Int_t nphot=mc.GetDrcNumberOfPhotons();
    
    if(thc>0 && thce>0 && nphot>minphot){
	Double_t difka=TMath::Abs(thc-thcka);
	Double_t difpi=TMath::Abs(thc-thcpi);
	Double_t pulka=difka/thce;
	Double_t pulpi=difpi/thce;
	if(difka<difpi && pulka<sigma) return kTRUE;
	if(pulka<pulpi && pulka<sigma) return kTRUE;
    }
    return kFALSE;
}

Bool_t DStarAnalysis::EndRun(TEventManager*){
    DrawHistograms();
    return kTRUE;
}

void DStarAnalysis::DrawHistograms(){
    c1->cd(1);
    histMissMass1->Draw();
    c1->cd(2);
    histMissMass2->Draw();
    c1->cd(3);
    histMissMass3->Draw();
    c1->cd(4);
    histMissMass4->Draw();
    c1->Update();
}

