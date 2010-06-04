//////////////////////////////////////////////////////////////////////////
//                                                                      //
// THistogram			    					//
//                                                                      //
// Histogram class							//
//                                                                      //
// Author: Marcel Kunze, RUB, March 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TAxis.h"

#include "RhoHistogram/THistID.h"
#include "RhoHistogram/THistogram.h"
#include "RhoBase/TCandList.h"
#include "RhoBase/TCandListIterator.h"
#include "RhoBase/TCandidate.h"

ClassImp(THistogram)
ClassImp(TMassHistogram)
ClassImp(TEnergyHistogram)
ClassImp(TMomentumHistogram)
ClassImp(TEoverPHistogram)
ClassImp(TMoverPHistogram)
ClassImp(TDalitzPlot)

#include <iostream>
using namespace std;

// Constructors:
// 1-D histo:
THistogram::THistogram( const char* hname, const char* htitle, 
			       Int_t nbins, Axis_t lowX, Axis_t highX ) :
TNamed( hname,htitle )
{   
    // Create a 1-D ROOT histo:
    histp= new TH1F( hname, htitle, nbins, lowX, highX );    
}

THistogram::THistogram( const char* hname, const char* htitle, 
			       Int_t nbinsX, Axis_t lowX, Axis_t highX,
			       Int_t nbinsY, Axis_t lowY, Axis_t highY ) :
TNamed( hname,htitle )
{    
    // Create a 2-D ROOT histo:
    histp= new TH2F( hname, htitle, nbinsX, lowX, highX, nbinsY, lowY, highY );   
}

THistogram::THistogram( const char* hname, const char* htitle, 
			       Int_t nbins, Axis_t lowX, Axis_t highX, 
			       Axis_t lowY, Axis_t highY ) :
TNamed( hname,htitle )
{   
    // Create a 1-D profile ROOT histo with default error calculation:
    // error= sigma/sqrt(N)
    histp= new TProfile( (char*)hname, (char*)htitle, nbins, lowX, highX,lowY, highY );    
}

THistogram::~THistogram() 
{
    delete histp;
}

void THistogram::Accumulate( Axis_t x, Stat_t weight ) 
{    
    if( histp->GetDimension() == 2 ||
	( histp->GetDimension() == 1 && 
	histp->IsA() == TProfile::Class() ) ) {
	Axis_t y = weight;
	weight = 1.0;
	((TH2 *)histp)->Fill( x, y, weight );
    }
    else {
	histp->Fill( x, weight );
    }
    return;    
}

void THistogram::Accumulate1( Axis_t x, Stat_t weight ) 
{    
    histp->Fill( x, weight );
}

void THistogram::Accumulate( Axis_t x, Axis_t y, Stat_t weight ) 
{    
    ((TH2 *)histp)->Fill( x, y, weight );   
}

void THistogram::Accumulate2( Axis_t x, Axis_t y, Stat_t weight ) 
{   
    ((TH2 *)histp)->Fill( x, y, weight );     
}


void THistogram::Reset() 
{
    histp->Reset();
}


Float_t THistogram::GetContents( Int_t nbinsX, Int_t nbinsY ) const 
{  
    Float_t cont= 0;
    if( nbinsY == 0 ) {
	cont= (Float_t) histp->GetBinContent( nbinsX );
    }
    else {
	cont= (Float_t) histp->GetCellContent( nbinsX, nbinsY );
    }
    return cont;    
}

Float_t THistogram::GetErrors( Int_t nbinsX, Int_t nbinsY ) const 
{   
    Float_t error= 0;
    if( nbinsY == 0 ) {
	error = (Float_t) histp->GetBinError( nbinsX );
    }
    else {
	error = (Float_t) histp->GetCellError( nbinsX, nbinsY );
    }
    return error;   
}



Int_t THistogram::GetEntries() const { return (Int_t) histp->GetEntries(); }

Float_t THistogram::GetWtSum() const 
{    
    return (Float_t) histp->GetSumOfWeights();    
}



Int_t THistogram::GetNbins( Int_t theDim ) const {
    
    Int_t nbins= 0;
    if( theDim == 0 ) {
	nbins= histp->GetNbinsX();
    }
    else if( theDim == 1 ) {
	if( histp->GetDimension() == 2 ) nbins= histp->GetNbinsY();
    }
    return nbins;
    
}

Float_t THistogram::GetLow( Int_t theDim ) const {
    
    Float_t low= 0;
    if( theDim == 0 ) {
	low= histp->GetXaxis()->GetBinLowEdge( 1 );
    }
    else if( theDim == 1 ) {
	if( histp->GetDimension() == 2 ) {
	    low= histp->GetYaxis()->GetBinLowEdge( 1 );
	}
    }
    return low;
    
}

Float_t THistogram::GetHigh( Int_t theDim ) const {
    
    TAxis* axisp= 0;
    if( theDim == 0 ) {
	axisp= histp->GetXaxis();
    }
    else if( theDim == 1 ) {
	if( histp->GetDimension() == 2 ) axisp= histp->GetYaxis();
    }
    Float_t high= 0;
    if( axisp != 0 ) {
	Int_t n= axisp->GetNbins();
	high= axisp->GetBinLowEdge( n ) + axisp->GetBinWidth( n );
    }
    return high;
    
}

Float_t THistogram::GetAvg( Int_t theDim ) const {
    
    Float_t mean= 0;
    if( theDim == 0 ) {
	mean= (Float_t) histp->GetMean( 1 );
    }
    else if( theDim == 1 ) {
	if( histp->GetDimension() == 2 ) mean= (Float_t) histp->GetMean( 2 );
    }
    return mean;
    
}

Float_t THistogram::GetCovar( Int_t dim1, Int_t dim2 ) const {
    
    // Calculate weighted sums:
    TAxis* xaxisp= histp->GetXaxis();
    TAxis* yaxisp= histp->GetYaxis();
    Int_t maxxbin= histp->GetNbinsX();
    Int_t maxybin= histp->GetNbinsY();
    Double_t meanx= 0;
    Double_t meany= 0;
    Double_t meanx2= 0;
    Double_t meany2= 0;
    Double_t meanxy= 0;
    for( Int_t ybin= 1; ybin <= maxybin; ++ybin ) {
	Double_t cony= 0;
	for( Int_t xbin= 1; xbin <= maxxbin; ++xbin ) {
	    Double_t cellc= histp->GetCellContent( xbin, ybin );
	    cony+= cellc;
	    Double_t xbinc= xaxisp->GetBinCenter( xbin );
	    meanx+= cellc * xbinc;
	    meanx2+= cellc * xbinc*xbinc;
	    meanxy+= cellc * xbinc * yaxisp->GetBinCenter( ybin );
	}
	Double_t ybinc= yaxisp->GetBinCenter( ybin );
	meany+= cony * ybinc;
	meany2+= cony * ybinc*ybinc;
    }
    Double_t sumcon= histp->GetSumOfWeights();
    if( sumcon ) {
	meanx/= sumcon;
	meany/= sumcon;
	meanx2/= sumcon;
	meany2/= sumcon;
	meanxy/= sumcon;
    }
    else {
	meanx= 0;
	meany= 0;
	meanx2= 0;
	meany2= 0;
	meanxy= 0;
    }
    
    // Return covariance between dim1 and dim2:
    Float_t cov= 0;
    if( dim1 == 0 && dim2 == 0 ) {
	cov = (Float_t) (meanx2 - meanx*meanx);
    }
    else if( dim1 == 0 && dim2 == 1 || 
	dim1 == 1 && dim2 == 0 ) {
	cov = (Float_t) (meanxy - meanx*meany);
    }
    else if( dim1 == 1 && dim2 == 1 ) {
	cov = (Float_t) (meany2 - meany*meany);
    }
    return cov;
    
}

Int_t THistogram::GetType() const {
    
    Int_t ht= 0;
    if( histp->GetDimension() == 1 ) ht= 1;
    else if( histp->GetDimension() == 2 ) ht= 2;
    return ht;
    
}

THistID THistogram::GetHistID() const {
    
    return THistID( histp->GetName() );
    
}

Bool_t THistogram::PtrIsEqual( TObject* ptr ) const {
    
    Bool_t result;
    if( (TObject*) histp == ptr ) result= kTRUE; 
    else result= kFALSE;
    return result;
    
}

THistogram& THistogram::operator<<(Axis_t x)
{
    Accumulate(x); return *this;
}


//----------------Spezialization-----------------

//----------------
// Constructors --
//----------------

TMassHistogram::TMassHistogram(const Text_t *name,const Text_t *title,Int_t nbins,Axis_t xlow,Axis_t xup)  
: THistogram(name,title,nbins,xlow,xup)
{
}

TEnergyHistogram::TEnergyHistogram(const Text_t *name,const Text_t *title,Int_t nbins,Axis_t xlow,Axis_t xup)  
: THistogram(name,title,nbins,xlow,xup)
{
}

TMomentumHistogram::TMomentumHistogram(const Text_t *name,const Text_t *title,Int_t nbins,Axis_t xlow,Axis_t xup)  
: THistogram(name,title,nbins,xlow,xup)
{
}

TEoverPHistogram::TEoverPHistogram(const Text_t *name,const Text_t *title,Int_t nbinsx,Axis_t xlow,Axis_t xup
					   ,Int_t nbinsy,Axis_t ylow,Axis_t yup)
					   : THistogram(name,title,nbinsx,xlow,xup,nbinsy,ylow,yup)
{
}

TMoverPHistogram::TMoverPHistogram(const Text_t *name,const Text_t *title,Int_t nbinsx,Axis_t xlow,Axis_t xup
					   ,Int_t nbinsy,Axis_t ylow,Axis_t yup)
					   : THistogram(name,title,nbinsx,xlow,xup,nbinsy,ylow,yup)
{
}

//--------------
// Destructor --
//--------------

TMassHistogram::~TMassHistogram( ) 
{
}

TEnergyHistogram::~TEnergyHistogram( ) 
{
}

TMomentumHistogram::~TMomentumHistogram( ) 
{
}

TEoverPHistogram::~TEoverPHistogram( ) 
{
}

TMoverPHistogram::~TMoverPHistogram( ) 
{
}

TDalitzPlot::~TDalitzPlot( ) 
{
}

//----------------------
//-- public Functions --
//----------------------

void TMassHistogram::Accumulate( Axis_t x, Stat_t weight ) 
{    
    Accumulate1( x, weight );
}

void TMassHistogram::Accumulate(TCandList &l) 
{
    for (Int_t i=0;i<l.GetNumberOfTracks();++i) {
        Accumulate1((Axis_t)l[i].GetMass());
    }
}        

TMassHistogram& TMassHistogram::operator<<(TCandList &l)
{
    Accumulate(l); return *this;
}

void TMassHistogram::Accumulate(TCandidate &p) 
{
    Accumulate1((Axis_t)p.GetMass());
}        

TMassHistogram& TMassHistogram::operator<<(TCandidate &p)
{
    Accumulate(p); return *this;
}

void TEnergyHistogram::Accumulate( Axis_t x, Stat_t weight ) 
{    
    Accumulate1( x, weight );
}

void TEnergyHistogram::Accumulate(TCandList &l) 
{
    for (Int_t i=0;i<l.GetNumberOfTracks();++i) {
        Accumulate1((Axis_t)l[i].E());
    }
}

TEnergyHistogram& TEnergyHistogram::operator<<(TCandList &l)
{
    Accumulate(l); return *this;
}

void TEnergyHistogram::Accumulate(TCandidate &p) 
{
    Accumulate1((Axis_t)p.GetMass());
}        

TEnergyHistogram& TEnergyHistogram::operator<<(TCandidate &p)
{
    Accumulate(p); return *this;
}

void TMomentumHistogram::Accumulate( Axis_t x, Stat_t weight ) 
{    
    Accumulate1( x, weight );
}

void TMomentumHistogram::Accumulate(TCandList &l) 
{
    for (Int_t i=0;i<l.GetNumberOfTracks();++i) {
        Accumulate1((Axis_t)l[i].P());
    }
}

TMomentumHistogram& TMomentumHistogram::operator<<(TCandList &l)
{
    Accumulate(l); return *this;
}

void TMomentumHistogram::Accumulate(TCandidate &p) 
{
    Accumulate1((Axis_t)p.GetMass());
}        

TMomentumHistogram& TMomentumHistogram::operator<<(TCandidate &p)
{
    Accumulate(p); return *this;
}

void TMomentumHistogram::Accumulate(TCandList &l, Int_t component) 
{
    for (Int_t i=0;i<l.GetNumberOfTracks();++i) {
	switch (component) {
	case(1): Accumulate1((Axis_t)l[i].Px()); break;
	case(2): Accumulate1((Axis_t)l[i].Py()); break;
	case(3): Accumulate1((Axis_t)l[i].Pz()); break;
	default: Accumulate1((Axis_t)l[i].E()); break;
	}      
    }
}

void TEoverPHistogram::Accumulate( Axis_t x, Axis_t y, Stat_t weight ) 
{    
    Accumulate2( x, y, weight );
}

void TEoverPHistogram::Accumulate(TCandList &l) 
{
    Stat_t weight = 1.0;
    for (Int_t i=0;i<l.GetNumberOfTracks();++i) {
	Axis_t p=(Axis_t)l[i].P();
	Axis_t e=(Axis_t)l[i].E();
	Accumulate2(p,e,weight);
    }
}

TEoverPHistogram& TEoverPHistogram::operator<<(TCandList &l)
{
    Accumulate(l); return *this;
}

void TEoverPHistogram::Accumulate(TCandidate &c) 
{
    Stat_t weight = 1.0;
    Axis_t p=(Axis_t)c.P();
    Axis_t e=(Axis_t)c.E();
    Accumulate2(p,e,weight);
}        

TEoverPHistogram& TEoverPHistogram::operator<<(TCandidate &p)
{
    Accumulate(p); return *this;
}

void TMoverPHistogram::Accumulate( Axis_t x, Axis_t y, Stat_t weight ) 
{    
    Accumulate2( x, y, weight );
}

void TMoverPHistogram::Accumulate(TCandList &l)
{
    Stat_t weight = 1.0;
    for (Int_t i=0;i<l.GetNumberOfTracks();++i) {
	Axis_t p=(Axis_t)l[i].P();
	Axis_t m=(Axis_t)l[i].GetMass();
        Accumulate2(p,m,weight);
    }
}

TMoverPHistogram& TMoverPHistogram::operator<<(TCandList &l)
{
    Accumulate(l); return *this;
}

void TMoverPHistogram::Accumulate(TCandidate &c) 
{
    Stat_t weight = 1.0;
    Axis_t p=(Axis_t)c.P();
    Axis_t m=(Axis_t)c.GetMass();
    Accumulate2(p,m,weight);
}        

TMoverPHistogram& TMoverPHistogram::operator<<(TCandidate &p)
{
    Accumulate(p); return *this;
}

TDalitzPlot::TDalitzPlot(const Text_t *name,const Text_t *title,Int_t nbinsx,Axis_t xlow,Axis_t xup
				 ,Int_t nbinsy,Axis_t ylow,Axis_t yup)
				 : THistogram(name,title,nbinsx,xlow,xup,nbinsy,ylow,yup)
{
}

/*
* This function fills a Dalitzplot for the particles name1,name2 and name3 from descriptor p.
* horizontal axis: (name1,name2), vertikal axis: (name2,name3)
* There are three cases:
* 1) Fill the plot for three identical particles (6 entries)
* 2) Fill the plot for two different particles (2 entries)
* 3) Fill the plot for three different particles (1 entry)
*/
void TDalitzPlot::Accumulate(TCandList &pList1,TCandList &pList2,TCandList &pList3)
{
    TCandListIterator i1(pList1);
    TCandListIterator i2(pList2);
    TCandListIterator i3(pList3);
    
    TCandidate *p1 = 0;
    TCandidate *p2 = 0;
    TCandidate *p3 = 0;
    
    while ((p1=i1.Next())!=0)
	while ((p2=i2.Next())!=0)
	    while ((p3=i3.Next())!=0)
		Accumulate(*p1,*p2,*p3);
}

void TDalitzPlot::Accumulate(TCandidate &p1,TCandidate &p2,TCandidate &p3)
{
/*
* Dalitz plot is invariant mass squared [GeV]
*/
    Axis_t mass12,mass21,mass13,mass31,mass23,mass32;
    
    mass12 = mass21 = (p1.P4()+p2.P4()).Mag2();
    mass13 = mass31 = (p1.P4()+p3.P4()).Mag2();
    mass23 = mass32 = (p2.P4()+p3.P4()).Mag2();
    
    Stat_t weight = 1.0;
    
    // One particle
    if (p1.Overlaps(p2) && p2.Overlaps(p3)) {
	Accumulate2(mass12,mass13,weight);
	Accumulate2(mass13,mass12,weight);
	Accumulate2(mass23,mass21,weight);
	Accumulate2(mass21,mass23,weight);
	Accumulate2(mass31,mass32,weight);
	Accumulate2(mass32,mass31,weight);
    }
    // Two particles
    else if (p1.Overlaps(p2) && !p2.Overlaps(p3)) {
	Accumulate2(mass12,mass13,weight);
	Accumulate2(mass12,mass23,weight);
    }
    // Two particles
    else if (p1.Overlaps(p3) && !p2.Overlaps(p3)) {
	Accumulate2(mass12,mass23,weight);
	Accumulate2(mass13,mass23,weight);
    }
    // Two particles
    else if (p2.Overlaps(p3) && !p1.Overlaps(p3)) {
	Accumulate2(mass12,mass23,weight);
	Accumulate2(mass23,mass12,weight);
    }
    // Three particles
    else if (!p1.Overlaps(p2) && !p2.Overlaps(p3)) {
	Accumulate2(mass12,mass23,weight);
    }
}

