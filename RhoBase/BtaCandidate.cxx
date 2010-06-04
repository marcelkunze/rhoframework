//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaCandidate (derived from BtaFitParams)				//
//                                                                      //
// Contains (polymorphic) contents for BtaCandidate objects		//
// Candidate "Tracks" or "Particles" for analysis use			//
//									//
// Author List:								//
// Sascha Berger, RUB, Feb.99						//
// Marcel Kunze,  RUB, Aug.99						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <assert.h>

#include "PDT/Pdt.hh"
#include "PDT/PdtEntry.hh"
#include "RhoBase/TRho.h"
#include "RhoBase/BtaCandidateFactory.hh"
#include "RhoBase/BtaCandidate.hh"
#include "RhoBase/BtaFitParams.hh"
#include "RhoBase/BtaCandList.hh"
#include "RhoBase/BtaCandListIterator.hh"
#include "RhoBase/BtaAbsVertex.hh"
#include "RhoBase/VAbsMicroCandidate.h"
#include "RhoBase/BtaMcTruth.hh"

ClassImp(BtaCandidate)

#include <iostream>
#include <iomanip>
using namespace std;

BtaCandidate::BtaCandidate() :
fFastMode(kFALSE),
fLocked(kFALSE),
fTheMother(0),
fDecayVtx(0),
fPdtEntry(0),
fIsAResonance(kFALSE),
fTruth(0), 
fMicroCand(0),
fTrackNumber(0),
fDaugList(0),
nDaug(0),
nCons(0)
{
    fMarker[0] = fMarker[1] = fMarker[2] = fMarker[3] = 0;
    setUid();
}

BtaCandidate::BtaCandidate( const TLorentzVector &v, Double_t charge, BtaAbsVertex* vp ) : 
fFastMode(kFALSE),
fLocked(kFALSE), 
fTheMother(0),
fDecayVtx(vp), 
fPdtEntry(0), 
fIsAResonance(kFALSE),
fTruth(0),
fMicroCand(0),
fTrackNumber(0),
fDaugList(0),
nDaug(0),
nCons(0)
{ 
    fMarker[0] = fMarker[1] = fMarker[2] = fMarker[3] = 0;
    setCharge( charge );
    setP7( ( vp!=0 ? vp->point() : TVector3(0.,0.,0.) ), v );
    setUid();
}

BtaCandidate::BtaCandidate( const TVector3 &v, Double_t charge, BtaAbsVertex* vp ) : 
fFastMode(kFALSE),
fLocked(kFALSE), 
fTheMother(0),
fDecayVtx(vp), 
fPdtEntry(0), 
fIsAResonance(kFALSE),
fTruth(0),
fMicroCand(0),
fTrackNumber(0),
fDaugList(0),
nDaug(0),
nCons(0)
{ 
    fMarker[0] = fMarker[1] = fMarker[2] = fMarker[3] = 0;
    if (fPdt==0) fPdt = new Pdt();    // Access particle DB
    PdtEntry* pdt = 0;
    if( fabs(charge)<1.e-06 ) 
	pdt = fPdt->lookup( "gamma" );
    else
	pdt = (charge>0) ? fPdt->lookup( "pi+" ) : fPdt->lookup( "pi-" );
    setPos( vp!=0 ? vp->point() : TVector3(0.,0.,0.) );
    setP3(v);
    setType( pdt );
    setUid();
}

BtaCandidate::BtaCandidate( const TVector3 &v, const PdtEntry* pdt, BtaAbsVertex* vp ) : 
fFastMode(kFALSE),
fLocked(kFALSE), 
fTheMother(0),
fDecayVtx(vp), 
fPdtEntry(0), 
fIsAResonance(kFALSE),
fTruth(0),
fMicroCand(0),
fTrackNumber(0),
fDaugList(0),
nDaug(0),
nCons(0)
{
    fMarker[0] = fMarker[1] = fMarker[2] = fMarker[3] = 0;
    setPos( vp!=0 ? vp->point() : TVector3(0.,0.,0.) );
    setP3(v);
    setType( pdt );
    setUid();
}


BtaCandidate::BtaCandidate( const BtaCandidate& o ) 
{
    fFastMode = o.fFastMode;
    fLocked = kFALSE; 
    fTheMother = 0;
    fDecayVtx = o.fDecayVtx;
    fPdtEntry = o.fPdtEntry; 
    fIsAResonance = o.fIsAResonance;
    fTruth = o.fTruth; 
    fTrackNumber = o.fTrackNumber;
    fMicroCand = o.fMicroCand;

    fMarker[0] = o.fMarker[0];
    fMarker[1] = o.fMarker[1];
    fMarker[2] = o.fMarker[2];
    fMarker[3] = o.fMarker[3];
    
    fCharge    = o.fCharge;
    fXposition = o.fXposition;
    fYposition = o.fYposition;
    fZposition = o.fZposition;
    fXmomentum = o.fXmomentum;
    fYmomentum = o.fYmomentum;
    fZmomentum = o.fZmomentum;
    fEnergy    = o.fEnergy;
    
    if (!fFastMode) {
	int i;
	for (i=0; i<MATRIXSIZE;i++) fErrP7[i] = o.fErrP7[i];
	for (i=0; i<5;i++) fParams[i] = o.fParams[i];
	for (i=0; i<15;i++) fCov[i] = o.fCov[i];
    }
  
    fDaugList = 0;
    nDaug = o.nDaug;
    if (o.nDaug > 0) {
	for (int i=0;i<nDaug;i++) {
	    fDaughters[i] = o.fDaughters[i];
	    fDaughters[i]->fTheMother = this;
	}
    }
    
    nCons = 0;
    if (o.nCons > 0) {
	for (int i=0;i<o.nCons;i++) addConstraint(*o.fConstraints[i]);
    }
}

// This is the special constructor to bring a BtaCandidate into
// life from the MicroCandidate

BtaCandidate::BtaCandidate(VAbsMicroCandidate &a, Int_t n, BtaAbsVertex* vp, Bool_t fast ) : 
fFastMode(fast),
fLocked(kFALSE), 
fTheMother(0),
fDecayVtx(vp),
fPdtEntry(0), 
fIsAResonance(kFALSE),
fTruth(0),
fMicroCand(&a),
fTrackNumber(n),
fDaugList(0),
nDaug(0),
nCons(0)
{
    // set kinematics from MicroCandidate
    
    setP4(a.GetLorentzVector());
    setPos(a.GetPosition());
    setCharge(a.GetCharge());
    
    if (!fFastMode) {
	const Float_t *err = a.GetErrorP7();
	//const Float_t *cov = a.GetCov();
	const Float_t *par = a.GetParams();
	int i;
	if (err!=0) for (i=0; i<MATRIXSIZE;i++) fErrP7[i] = err[i];
	if (par!=0) for (i=0; i<5;i++) fParams[i] = par[i];
	//if (cov!=0) for (i=0; i<15;i++) fCov[i] = cov[i];
    }
    
    setMarker(n);
}


BtaCandidate::BtaCandidate(BtaMcTruth &a, Int_t n, BtaAbsVertex* vp, Bool_t fast ) : 
fFastMode(fast),
fLocked(kFALSE), 
fTheMother(0),
fDecayVtx(vp),
fPdtEntry(0),
fIsAResonance(kFALSE),
fTruth(&a),
fMicroCand(0),
fTrackNumber(n),
fDaugList(0),
nDaug(0),
nCons(0)
{
    fMarker[0] = fMarker[1] = fMarker[2] = fMarker[3] = 0;
    // set kinematics from Truth
    setP4(a.getMomentum());
    setPos(a.getStartVertex());
    fPdtEntry = a.getPdtEntry();
    if (!fPdtEntry) cout << 
        "BtaCandidate(BtaMcTruth&,Int_t,BtaAbsVertex*) : Error: Could not get PdtEntry. No charge set"
	<< endl;
    setCharge( fPdtEntry ? fPdtEntry->charge() : 0 );
    
    // set the markers according to the matched track
    // The match index is -1 in case of no matching track
    if (a.getAodMatchIndex()>=0) setMarker(a.getAodMatchIndex());
    
    setMcTruth(&a); // Note the underlying truth object, MK jan.2k
}

//--------------
// Destructor --
//--------------

BtaCandidate::~BtaCandidate( )
{
    removeAssociations();
}

//--------------
// Operations --
//--------------

// assignment operator
BtaCandidate& 
BtaCandidate::operator = (const BtaCandidate& o) 
{
    fFastMode = o.fFastMode;
    fLocked = kFALSE; 
    fTheMother = 0; 
    fDecayVtx = o.fDecayVtx;
    fPdtEntry = o.fPdtEntry; 
    fIsAResonance = o.fIsAResonance;
    fTruth = o.fTruth; 
    fTrackNumber = o.fTrackNumber;
    fMicroCand = o.fMicroCand;

    fMarker[0] = o.fMarker[0];
    fMarker[1] = o.fMarker[1];
    fMarker[2] = o.fMarker[2];
    fMarker[3] = o.fMarker[3];
    
    fCharge    = o.fCharge;
    fXposition = o.fXposition;
    fYposition = o.fYposition;
    fZposition = o.fZposition;
    fXmomentum = o.fXmomentum;
    fYmomentum = o.fYmomentum;
    fZmomentum = o.fZmomentum;
    fEnergy    = o.fEnergy;
    
    if (!fFastMode) {
	int i;
	for (i=0; i<MATRIXSIZE;i++) fErrP7[i] = o.fErrP7[i];
	for (i=0; i<5;i++) fParams[i] = o.fParams[i];
	for (i=0; i<15;i++) fCov[i] = o.fCov[i];
    }
    
    fDaugList = 0;
    nDaug = o.nDaug;
    if (o.nDaug > 0) {
	for (int i=0;i<nDaug;i++) {
	    fDaughters[i] = o.fDaughters[i];
	    fDaughters[i]->fTheMother = this;
	}
    }
    
    nCons = 0;
    if (o.nCons > 0) {
	for (int i=0;i<o.nCons;i++) addConstraint(*o.fConstraints[i]);
    }

    return *this;
}

void 
BtaCandidate::setMotherLink( BtaCandidate* m )
{
    assert( m!=0 );  // what kind of mother is it ?!
    
    // already a mother ?  not allowed !
    assert( fTheMother==0 );
    
    // should not be called for a local candidate...
    //assert( !isLocal() );
    
    if (m->nDaug>=5) {
	cerr << "BtaCandidate::setMotherLink: Can not add more than 5 daughters." << endl;
	return;
    }

    // set the mother link
    fTheMother = m;
    
    // ... and the mother's daughter link
    fTheMother->fDaughters[fTheMother->nDaug++] = this;
    
    // special for MC trees
    if( fTheMother==0 ) fTheMother=m;
    
    // if it's a resonance and the mother has already a decay vertex,
    // set the decay vertex of the mother to the daughter
    if( isAResonance() && fTheMother->decayVtx()!=0 && fTheMother->decayVtx()!=decayVtx())
    {
	setDecayVtx( fTheMother->decayVtx() );
	// if it is a resonance and the mother has no other vertex, the vertex of the reonance is given to her.  
    }
    
}

void 
BtaCandidate::dropMotherLink()
{
    // no mother
    if( fTheMother==0 ) return;
    
    // case of a non-local candidate
    
    //if( !isLocal() )
    { 
	// the mother looses this as a daughter
	for (int i=0;i<fTheMother->nDaug;i++)
	    if (fTheMother->fDaughters[i] == this) {
		fTheMother->nDaug--;
		fTheMother->fDaughters[i] = fTheMother->fDaughters[fTheMother->nDaug];
	    }
    }
    
    // OEdipus rex : kill the mother link
    fTheMother = (BtaCandidate*)0;
}

TConsistency*
BtaCandidate::consistency( const PdtEntry* pdt ) const
{
    return 0;
}

void 
BtaCandidate::addToVertexingList( BtaCandList& outGoing )
{
    if( isAResonance() )
    {
	for (int i=0;i<nDaug;i++) fDaughters[i]->addToVertexingList( outGoing );
    }
    else
	outGoing.put( *this );
}

const BtaCandidate*
BtaCandidate::cloneInTree( const BtaCandidate& c ) const 
{
    if( isCloneOf( c ) ) return this;
    for (int i=0;i<nDaug;i++) 
    {
	const BtaCandidate* b = fDaughters[i]->cloneInTree( c );
	if( b!=0 ) return b;
    }
    return 0;
}


void
BtaCandidate::invalidateFit() 
{
    // new function (GHM 05/99)
    // this function invalidates the fit to let fitters know
    // that if given this candidate, they have to refit it.
    
    BtaAbsVertex* theDecayVtx = decayVtx();
    if( theDecayVtx!=0 )
    {
	if( theDecayVtx->status() != BtaAbsVertex::UnFitted )
	{
	    // clone the vertex...
	    BtaAbsVertex* newDecayVtx = (BtaAbsVertex*) theDecayVtx->Clone();
	    
	    // ...and invalid the clone
	    newDecayVtx->setStatus( BtaAbsVertex::UnFitted );
	    
	    // now set the new decay vertex
	    setDecayVtx( newDecayVtx );
	}
    } 
    
    // and do that recursively
    for (int i=0;i<nDaug;i++) fDaughters[i]->invalidateFit();
}

std::ostream&  
operator << (std::ostream& o, const BtaCandidate& a) 
{
    a.printOn(o); return o;
}

const BtaAbsVertex* BtaCandidate::productionVtx() const
{
    if( fTheMother!=0 ) return fTheMother->decayVtx();
    else return 0;
}

Bool_t BtaCandidate::operator== (const BtaCandidate& c) const
{ return equals(c) ; }

Bool_t BtaCandidate::operator!= (const BtaCandidate& c) const
{ return (*this!=c) ; }

// This comes from the content

void 
BtaCandidate::setMass( Double_t m ) 
{ 
    // set the mass of a non-composite doesn't make sense
    // assert( IsComposite() );  
    // for the moment, just print an error message and go away
    //if( !IsComposite() && fPdtEntry!=0 )
    //  {
    //    return;
    //  }
    // sets the mass
    BtaFitParams::setMass( m );
    
}

void 
BtaCandidate::setEnergy(Double_t E)
{ 
    if( isComposite() )
    {
	// this implementation leaves P unchanged; subclasses may differ
	BtaFitParams::setEnergy( E );
    }
    else
    {
	setMassAndEnergy( mass(), E );
    }
}

void 
BtaCandidate::setMomentum(Double_t newp) 
{ 
    // this implementation leaves mass unchanged; subclasses may differ
    Double_t pscale = ( p() == 0 ? 0 : newp / p() );
    setP3( pscale*p3() );
}

void 
BtaCandidate::setType( const PdtEntry *pdt ) 
{ 
    if( pdt==fPdtEntry || pdt==0 ) return;
    fPdtEntry = pdt;
    
    //
    // by default :
    //   if the proper lifetime multiplied by light velocity is less
    //    than a nanometer, the candidate is considered a resonance
    //      (a state that does not fly)
    //
    fIsAResonance=kFALSE;
    if( fPdtEntry->width()>1E-15 /*Lifetime()<1e-08*/ ) fIsAResonance=kTRUE;
    
    if( !isComposite() )
    {
	
	// the mass has changed since the type has changed
	setMass( mass() );
	
	// set the charge
	setCharge( pdt->charge() );
    }
    else {
	if ( charge()!=pdt->charge()) {
	    cerr
		<< "ERROR: attempt to call BtaCandidate::setType(\""
		<< "\") for a composite" << endl 
		<< "       BtaCandidate whose daughters have total charge "
		<< charge() << endl;
	    //assert( Charge()==pdt->Charge() );
	}
    }
}



void 
BtaCandidate::setMcTruth( BtaMcTruth * mctruth )
{ 
    fTruth = mctruth;
}

void 
BtaCandidate::setDecayVtx(BtaAbsVertex*  theVtx ) 
{ 
    
    // protection agains null pointers
    if( theVtx==0 ) return;
    
    //
    // Warning : this is a recursive algorithm
    //
    
    // this vertex is not already set
    if( fDecayVtx!=theVtx )
    { 
	// set the new decay vertex reference
	fDecayVtx = theVtx;
	
    }
    else
    {
	cerr << "trying to reset the same vertex ! " << endl;
	// this is used to refresh the vertex links
	//theVtx->_inComingCand=0;
	//theVtx->_outGoingCands.clear();
	//theVtx->_resonances.clear();
    }
    
    // update the vertex back links
    if( isAResonance() ) 
    	theVtx->setResonances(this);
    else
    {
	//assert( theVtx->GetInComingCand()==0 );
    	theVtx->setInComingCand(this);
    }
    // now loop on daughters
    for (int i=0;i<nDaug;i++) 
    {
    	if( fDaughters[i]->isAResonance() ) 
    	    fDaughters[i]->setDecayVtx( theVtx );
    	else
    	    theVtx->setOutGoingCands( fDaughters[i] );
    }
}

// --------------------------------------------------
// Geneology functions, no longer in a separate class
// --------------------------------------------------

Int_t 
BtaCandidate::nDaughters() const 
{
    return nDaug;
}

void 
BtaCandidate::addDaughterLink( const BtaCandidate* cand ) 
{
    //assert( cand!=0 );
    
    // first copy the candidate
    BtaCandidate* d = const_cast<BtaCandidate*>(cand);
    
    // as soon as there are daughters, the charge is
    // given by the sum of the daughter charges
    if( nDaug==0 ) setCharge(0);
    setCharge( charge()+cand->charge() );
    
    // set the daughter's mother link 
    d->setMotherLink(this);
    
    fMarker[0] |= d->getMarker(0);
    fMarker[1] |= d->getMarker(1);
    fMarker[2] |= d->getMarker(2);
    fMarker[3] |= d->getMarker(3);
}

void 
BtaCandidate::removeDaughter( BtaCandidate* d )
{
    // check for consistency
    assert( d!=0 && d->theMother()==this );
    
    // the charge
    setCharge( charge() - d->charge() );
    
    // destroy the daughter
    //delete d;
    
}


//
// Access functions
//

Double_t 
BtaCandidate::mass() const    
{ 
    if( !isComposite() && fPdtEntry!=0 ) 
	return fPdtEntry->mass();
    else 
	return m(); 
}

BtaCandidate* 
BtaCandidate::daughter(Int_t n)
{
    if (n >=0 && n < nDaug ) 
	return fDaughters[n];
    else
	return 0;
}

BtaCandListIterator
BtaCandidate::daughterIterator() const
{
    static BtaCandList emptyList;
    static BtaCandListIterator empty(emptyList);
    if (nDaughters()==0) return empty;

    // return an iterator to the daughter list
    BtaCandList *l = const_cast<BtaCandList *>(fDaugList);
    if (l==0) l = new BtaCandList("DaugList",5);
    l->cleanup();
    for (int i=0;i<nDaug;i++) l->put(*fDaughters[i]);
    return BtaCandListIterator(*l);
}

BtaConstraint& 
BtaCandidate::addConstraint( BtaConstraint& oneMore )
{
    BtaConstraint *newConstraint = new BtaConstraint(oneMore);
    fConstraints[nCons++] = newConstraint;
    return *newConstraint;
}

BtaConstraint& 
BtaCandidate::addConstraint( BtaConstraint::Type  type )
{
    BtaConstraint oneMore( type );
    return addConstraint( oneMore );
}

void 
BtaCandidate::removeConstraint( const BtaConstraint& oneLess )
{ 
}

void 
BtaCandidate::removeConstraint( BtaConstraint::Type type )
{ 
    BtaConstraint oneLess(type);
    removeConstraint( oneLess );
}


Int_t 
BtaCandidate::nConstraints() const
{
    return nCons;
}

const BtaConstraint* 
BtaCandidate::constraint( Int_t i )
{
    if(i>=nCons) return 0;
    return fConstraints[i];
}

const BtaConstraint* 
BtaCandidate::constraint( BtaConstraint::Type type ) const
{
    size_t i;
    for( i=0; i<nCons; i++ )
    {
	if( type==fConstraints[i]->getType() ) return fConstraints[i];
    }
    return 0;
}

// former inline implementations

BtaAbsVertex*    
BtaCandidate::productionVtx()
{
    if( fTheMother!=0 ) return fTheMother->decayVtx();
    else return 0;
}

const BtaAbsVertex*    
BtaCandidate::decayVtx() const 
{ return fDecayVtx; }

BtaAbsVertex*    
BtaCandidate::decayVtx() 
{ return fDecayVtx; }

const PdtEntry*        
BtaCandidate::pdtEntry() const 
{ return fPdtEntry; }

const BtaMcTruth*       
BtaCandidate::mcTruth() const 
{ return fTruth; }

Bool_t
BtaCandidate::isComposite() const 
{
    return nDaug>0;
}

Bool_t
BtaCandidate::isAResonance() const
{
    return fIsAResonance;
}

void 
BtaCandidate::setFly() 
{
    if( !fIsAResonance ) return;
    else
    {
	// can only change to new value is a vertex not already present
	//assert( fDecayVtx==0 );
	fIsAResonance = kFALSE;
    }
}

void 
BtaCandidate::setNoFly()
{
    if( fIsAResonance ) return;
    else
    {
	// can only change to new value is a vertex not already present
	//assert( fDecayVtx==0 ); 
	fIsAResonance = kTRUE;
    }
}

TVector3 
BtaCandidate::origin() const             
{ 
    return pos();
}

void 
BtaCandidate::setType(const char* name) 
{ 
    if (fPdt==0) fPdt = new Pdt();	// Access particle DB
    PdtEntry* pdt;
    if ( pdt=fPdt->lookup( name ) ) setType( pdt );
}

void BtaCandidate::printOn(std::ostream& o) const 
{
    o << "(" << p4().X() << "," << p4().Y() << "," << p4().Z() << ";" << p4().T() <<  ") " << charge() << " " << mass() << " Gev/c2 ";
    o.setf(ios::hex | ios::showbase);
    o << " ("
	<< getMarker(3)
	<< ","
	<< getMarker(2)
	<< ","
	<< getMarker(1)
	<< ","
	<< getMarker(0)
	<< ") ";
    o.unsetf(ios::hex | ios::showbase);
    
    // print daughter links
    o << "d: "<< nDaug;
    for (int i=0;i<nDaug;i++) o << " " << fDaughters[i]->uid();
    if (decayVtx()) o << " dcy: "<<*decayVtx();
}


void BtaCandidate::setMarker(UInt_t n)
{
    
    fMarker[0] = fMarker[1] = fMarker[2] = fMarker[3] = 0;
    
    if (n<32) 
	fMarker[0] = 1<<n;
    else if (n<64)
	fMarker[1] = 1<<(n%32);
    else if (n<96)
	fMarker[2] = 1<<(n%64);
    else if (n<128)
	fMarker[3] = 1<<(n%96);
    else
	cout << "BtaCandidate warning: More than 128 tracks; overlap may occur" << endl;
}

// Constructor from CandBase

BtaCandidate::BtaCandidate( TLorentzVector p4, 
			   TError &p4Err,
			   BtaCandListIterator& iterDau,
			   BtaAbsVertex& theVertex,
			   const PdtEntry* hypo )
			   :
fFastMode(kFALSE),
fLocked(kFALSE), 
fTheMother(0),
fDecayVtx(0), 
fPdtEntry(0), 
fIsAResonance(kFALSE),
fTruth(0),
fMicroCand(0),
fTrackNumber(0),
fDaugList(0),
nDaug(0),
nCons(0)
{
    fMarker[0] = fMarker[1] = fMarker[2] = fMarker[3] = 0;
    
    // create the local candidate
    //createLocalCand();
    
    // first set the mother/daughter links
    BtaCandidate* dau=0;
    iterDau.rewind();
    Int_t nDau=0;
    while( dau=iterDau.next() ) 
    {
	nDau++;
	addDaughterLink( dau );
    }
    
    // a composite cand is not supposed not to have a vertex...
    if( nDau==0 )
	cerr << "A composite cand is supposed to have daughters ! " << endl;
    
    // set the trajector
    setTrajectory(p4,p4Err,charge(),hypo,&theVertex);
}


void BtaCandidate::setTrajectory(const TLorentzVector& p4, const TError& p4Err,
				 Int_t charge,const PdtEntry* hypo,
				 BtaAbsVertex* dVtx )
{
    if(dVtx){
	setP7(dVtx->point(),p4);
	setCov7(dVtx->XXCov(),p4Err,dVtx->XPCov());
	//BbrPointErr pos(dVtx->point(),dVtx->xxCov());
	//_deferCompTrk = true;
	
	//     HepSymMatrix ppc(3);
	//     for(Int_t k=0;k<3;k++)
	//       for (Int_t j=k;j<3;j++)
	// 	ppc[j][k]=p4Err[j][k];
	//     BbrVectorErr mom(p4.vect(),ppc);
	
	//     TrkCompTrk * compTrk=new TrkCompTrk( pos,mom,dVtx->xpCov(),charge,
	// 					 dVtx->chiSquared(),dVtx->nDof());
	//     setTrkCompTrk(compTrk);
	
	setDecayVtx(dVtx);
    }else{
	setP4(p4);
	setCovP4(p4Err);
    }
    
    if (hypo) setType(hypo);
    if (uid()==0) setUid();
}

void BtaCandidate::setUid(UInt_t uid)
{
    static UInt_t u = 10000;
    if (uid!=0)
	u = uid;
    else
	u++;
    
    fTrackNumber = u;
    if ((fMarker[0]|fMarker[1]|fMarker[2]|fMarker[3])==0) setMarker(u%128);
}

Bool_t
BtaCandidate::isCloneOf(const BtaCandidate& o, Bool_t checkType) const    
{
    // Original behaviour of BtaCandidate::IsCloneOf()
    if ( uid() == o.uid() && !checkType) return kTRUE;
    
    if ( (isComposite() && !o.isComposite()) || 
	(!isComposite() && o.isComposite()) ) return kFALSE;
    
    // for single tracks and clusters, it is enough to compare 
    // UIDs and PDT types
    if ( !isComposite() && !o.isComposite() ) {
	return ( uid() == o.uid() && 
	    (!checkType || pdtEntry() == o.pdtEntry() ));
    }

    // if we got here, must be true
    return kTRUE;
}

void BtaCandidate::removeAssociations()
{
    // Clean up asociations to allow new w/ placement
    //if (fTheMother!=0) DropMotherLink(); fTheMother = 0;
    if (fDaugList!=0) { delete fDaugList; fDaugList = 0; }
    //if (fDecayVtx!=0) { delete fDecayVtx; fDecayVtx = 0; }
    for (int i=0;i<nCons;i++) { delete fConstraints[i]; fConstraints[i]=0; }
}

BtaCandidate* BtaCandidate::combine(const BtaCandidate& c) {
    BtaCandidate tmp(p4()+c.p4(),charge()+c.charge());
    BtaCandidate *cand = BtaCandidateFactory::instance()->newCandidate(tmp);
    cand->setMarker(fMarker[0]|c.fMarker[0],0);
    cand->setMarker(fMarker[1]|c.fMarker[1],1);
    cand->setMarker(fMarker[2]|c.fMarker[2],2);
    cand->setMarker(fMarker[3]|c.fMarker[3],3);
    return cand;
}

void BtaCandidate::setMarker(UInt_t l,UInt_t m) {
    if (m<4)
	fMarker[m]=l;
    else
	cout << "BtaCandidate: Trying to set non-existent marker " << m << endl;
}
