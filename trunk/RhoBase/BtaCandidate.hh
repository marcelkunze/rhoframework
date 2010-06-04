#ifndef BtaCandidate_hh
#define BtaCandidate_hh
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaCandidate (derived from TFitParams)				//
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

#include <iostream>

#include "TNtuple.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "RhoBase/BtaConstraint.hh"
#include "RhoBase/BtaFitParams.hh"
#include "RhoBase/BtaCandList.hh"
#include "Pdt/PdtEntry.hh"

class VAbsMicroCandidate;
class BtaAbsVertex;
class BtaMcTruth;
class Pdt;
class Consistency;
class TEventManager;
class TError;
class TConsistency;
class BtaCandListIterator;

class BtaRecoPointers;
class TrkAbsFit;
class TrkRecoTrk;
class TrkCompTrk;
class AbsRecoCalo;
class PidInfoSummary;
class BtaMicroAdapter;
class BtaAbsRecoObject;


//		---------------------
// 		-- Class Interface --
//		---------------------
class BtaCandidate : public BtaFitParams
{
    
    //--------------------
    // Instance Members --
    //--------------------
        
    // Lock : if true, modifications are a fatal error
    Bool_t fLocked;  //! Do not stream
    
    // Fast mode : Do not use error matrix
    Bool_t fFastMode;  //! Do not stream
    
    // The mother
    BtaCandidate* fTheMother;  //! Do not stream
    
protected:
    
    //--------------------
    // Instance Members --
    //--------------------
    
    // Counted reference to the vertex
    BtaAbsVertex* fDecayVtx;	    //! Vertex
    
    // Daughters
    BtaCandList *fDaugList;	    //! List of daughters
    BtaCandidate* fDaughters[5];	    //! Array of daughters
    Short_t nDaug;		    //! Number of daughters

    // Constraints
    BtaConstraint* fConstraints[5];   //! Array of constraints
    Short_t nCons;		    //! Number of constraints
    
    // is this a resonance ?
    Bool_t fIsAResonance;	    //! Rsonance flag
    
    // Monte-Carlo truth 
    BtaMcTruth* fTruth;		    //!  Pointer to MCTruth info          
    
    // Identity
    const PdtEntry* fPdtEntry;  // !Pointer to particle database
    static Pdt*	    fPdt;
    
    // Interface to objects storable in micro database
    VAbsMicroCandidate* fMicroCand; // !Pointer to micro data
    
    UInt_t fTrackNumber;  //! Micro association
    UInt_t fMarker[4];    //! Overlap
    
public:
    
    //--------------------
    // Public interface --
    //--------------------
    
    //
    // Constructors
    //
    
    /** Default ctor. Does not set particle type, etc. */
    BtaCandidate();
    
    /** Ctor from a momentum vector, charge, and origin vertex.
    Does not set the particle type, but uses the mass from the 
    argument 4vector.
    @param v      A THepLorentzVector representing the 4momentum
    @param charge The candidates charge, a Double_t so you can represent quarks
    @param vp     Pointer to the vertex where the candidate was created. The default zero value means to assume the origin
    */
    BtaCandidate( const TLorentzVector &v, 
	Double_t charge = 0, 
	BtaAbsVertex* vp = 0 );
    
	/** Ctor from a 3momentum vector, charge, and origin vertex.
	The particle type (hence mass) is set to pion for charged, photon for neutrals 
	@param v      A TVector3 representing the 3momentum
	@param charge The candidates charge, a Double_t so you can represent quarks
	@param vp     Pointer to the vertex where the candidate was created. The default zero value means to assume the origin
    */
    BtaCandidate( const TVector3 &v, 
	Double_t charge, 
	BtaAbsVertex* vp=0 );
    
	/** Ctor from a momentum vector, particle type, and origin vertex.
	The particle type to pion for charged, photon for neutrals 
	@param v      A TVector3 representing the momentum
	@param pdt    A pointer to a PdtEntry for the charge, type, etc.
	@param vp     Pointer to the vertex where the candidate was created. The default zero value means to assume the origin
    */
    BtaCandidate( const TVector3 &v, 
	const PdtEntry* pdt, 
	BtaAbsVertex* vp=0 );
    
    /** Copy ctor */
    BtaCandidate( const BtaCandidate & );
    
    // Special constructor from MicroCandidate
    BtaCandidate(VAbsMicroCandidate &a, Int_t n, BtaAbsVertex* vp=0, Bool_t fast= kFALSE);
    // Special constructor from McTruth
    BtaCandidate(BtaMcTruth &truth, Int_t n = 0, BtaAbsVertex* vp=0, Bool_t fast=kTRUE );
    
    BtaCandidate( TLorentzVector p4, 
	TError& p4Err,
	BtaCandListIterator& iterDau,
	BtaAbsVertex& theVertex,
	const PdtEntry* hypo = 0);
    //
    // Destructor
    //
    virtual ~BtaCandidate();
    
    //
    // Accessors to contained information
    //
    Double_t             mass() const;
    Double_t             getMass() const { return mass(); }

    const PdtEntry*  pdtEntry() const;
    
    //
    // By convention, the 4-momentum is given :
    //  - at the point of closest approach from the origin for non-composites
    //  - at the decay vertex for composite candidates
    //
    
    // The accessors and modifiers have been noved to TFitParams
    
    //
    // accessors to specific information (reconstruction, Monte-Carlo)
    //
    
    const BtaRecoPointers*  getRecoPointers() const { return 0; }
    const TrkAbsFit*        trkAbsFit()       const { return 0; }
    const TrkRecoTrk*       recoTrk()         const { return 0; }
    const TrkCompTrk*       compTrk()         const { return 0; }
    const AbsRecoCalo*      recoCalo()        const { return 0; }
    const PidInfoSummary*   pidInfoSummary()  const { return 0; }
    
    // information stored in microDB
    const BtaMicroAdapter*  getMicroAdapter() const { return 0; }
    
    //
    // adaptor to the general interface for vertexing
    //
    const BtaAbsRecoObject* recoObject() const { return 0; }
    
    // This method returns the pointer to the Consistency object along with 
    // ownership. The client is responsible for deleting the result.  
    //
    TConsistency* consistency( const PdtEntry* pdt ) const;
    
    //
    //  Modifiers
    //
    // Set type of candidate from a particle data table entry.
    // The setType function sets the type and therefore the mass
    // for any BtaCandidate which has no daughter.
    // Otherwise, i.e. for composite BtaCandidates,
    // the setType function only sets the type, _the mass is not set_.
    void setType( const PdtEntry* pdt );
    void setType( const char* name );
    
    // Set the mass
    // Inactive for non composite candidates
    void setMass( Double_t newMass );
    
    // Set the energy (at constant mass, by default)
    void setEnergy( Double_t newE );
    
    // Set the momentum (at constant mass, by default)
    void setMomentum( Double_t newP );
    
    void setMicroCandidate(VAbsMicroCandidate &micro) { fMicroCand = &micro; }
    // Allow the candidate to fly or not to fly
    // This overrides the default which is to consider the
    // candidate a resonance (a non-flying state) if it
    // has a proper decay length c*tau of less than a nanometer
    // according to the pdt table.
    // This function MUST be called before any call to setVertex,
    // or inconsistencies might occur.  A inconsistent call to 
    // one of these functions when the decay vertex is already set
    // will abort.
    void setFly();
    void setNoFly();
    
    // Set and get TMcTruth
    void setMcTruth( BtaMcTruth* mctruth );
    const BtaMcTruth* mcTruth() const;
    
    //
    //  Genealogy
    //  
    
    // access to the mother
    const BtaCandidate* theMother() const { return fTheMother; }
    BtaCandidate* theMother() { return fTheMother; }
    // access to daughters
    Int_t nDaughters() const;
    BtaCandListIterator daughterIterator() const;
    BtaCandidate* daughter(Int_t n);
    void removeAssociations();
    
    Bool_t isComposite()  const; // true when nDaugthers > 0
    Bool_t isAResonance() const; // true when c*tau is very small ( < 1nm )
    
    //
    //  Access to vertex information
    //
    
    const BtaAbsVertex*    decayVtx() const;
    BtaAbsVertex*          decayVtx();
    const BtaAbsVertex*    productionVtx() const; 
    BtaAbsVertex*          productionVtx(); 
    
    //  recursive function that invalidates the vertex fit.
    //  it sets all the vertices to the UnFitted status.
    //  warning : the function may trig lots of copies and clones.
    void invalidateFit();
    
    //
    // Check for overlap
    //
    
    // the function isCloneOf  returns true if the two 
    // Candidates have shared in the past a common Base.
    // For instance, an identified muon candidate is the
    // clone of the original Candidate it originates from
    // (note that "clone" must be taken in an enlarged sense
    // here. In that example for instance, the two clones have
    // different masses).
    Bool_t isCloneOf( const BtaCandidate&, Bool_t checkType = kFALSE ) const;
    
    // two Candidates are equal if they share the same Base
    Bool_t operator==( const BtaCandidate& ) const;
    
    // and different if they don not
    Bool_t operator!=( const BtaCandidate& ) const;
    
    // this function returns the pointer of the first clone 
    // of a given Candidate found in the decay tree of the
    // present Candidate, if present, else the null pointer.
    const BtaCandidate* cloneInTree( const BtaCandidate& ) const;
    
    //
    // Locks
    //
    // set the flag that prevents a cand from being changed
    
    void lock() { fLocked = kTRUE; }
    
    //
    // Constraints
    //
    
    // modifiers
    BtaConstraint& addConstraint( BtaConstraint& );
    BtaConstraint& addConstraint( BtaConstraint::Type );
    void removeConstraint( const BtaConstraint& );
    void removeConstraint( BtaConstraint::Type );
    
    // access
    Int_t nConstraints() const;
    Int_t numberOfConstraints() const { return nConstraints(); }
    const BtaConstraint* constraint( Int_t i ) ;
    const BtaConstraint* constraint( BtaConstraint::Type ) const;
    
    //
    //  Origin Point
    //    -  The origin point is the position of  the production vertex
    //        when present, else the "origin" (0.,0.,0.)
    //    -  The origin point is not necessarily the point 
    //        where the 4-momentum P4() is defined.  For composite
    //        candidates for instance, P4() is given at the decay point,
    //        not the production point.
    //    -  To get the momentum and covariance at the origin point :
    //
    //          TLorentzVector p4origin = cand->p4( cand->origin() );
    //
    
    TVector3 origin() const;
    
    //
    // Operations
    //
    
    BtaCandidate& operator=( const BtaCandidate & );
    
    //
    // Prints
    //
    void printOn( std::ostream& o=std::cout ) const;

    void setFast(Bool_t yesno) { fFastMode = yesno; }
    Bool_t isFast() const { return fFastMode; }
    
    VAbsMicroCandidate& getMicroCandidate() const { return *fMicroCand; }
    
    BtaCandidate* combine(const BtaCandidate& c);
    
    // two Candidates overlap if they are identical
    // (same pointers), equal (same Base),
    // clones (same Uid), representing a same
    // reconstructed object, or having daughters
    // that are overlapping

    Bool_t overlaps(const BtaCandidate& c) const {
	return ( (fMarker[0] & c.fMarker[0])!=0 || (fMarker[1] & c.fMarker[1])!=0 ||
	    (fMarker[2] & c.fMarker[2])!=0 || (fMarker[3] & c.fMarker[3])!=0 );
    }    	
    
    Bool_t equals(const BtaCandidate& c) const {
	return ( (fMarker[0] == c.fMarker[0]) && (fMarker[1] == c.fMarker[1]) &&
	    (fMarker[2] == c.fMarker[2]) && (fMarker[3] == c.fMarker[3]) );
    }    	
    
    UInt_t getMarker(UInt_t m=0) const {
	if (m<4)
	    return fMarker[m];
	else
	    return 0;
    }
    
    void setMarker(UInt_t l,UInt_t m);    
    void setMarker(UInt_t n);
    
    Int_t getTrackNumber()  const { return fTrackNumber; }
    Int_t uid() const   { return fTrackNumber; }
    void  setUid(UInt_t uid=0);

    // Set Constraints

    // Constrain the mass of the candidate to the given mass
    void setMassConstraint(Double_t mass ) 
    { ::setMassConstraint( *this, mass ); }

    // Constrain the mass of the candidate to its type mass
    void setMassConstraint()
    { ::setMassConstraint( *this ); }

    // Constrain the lifetime of the candidate (c*tau, in centimeters)
    void setLifetimeConstraint( Double_t ctau )
    { ::setLifetimeConstraint( *this, ctau ); }

    // Constrain the lifetime of the candidate to its type lifetime
    void setLifetimeConstraint()
    { ::setLifetimeConstraint( *this ); }

    // Constrain the energy of the candidate
    void setEnergyConstraint( TDoubleErr &energy,const TVector3& boost=TVector3(0,0,0) )
    { ::setEnergyConstraint( *this, energy,boost ); }

    void setEnergyConstraint( TVectorErr& eMinusMom, TVectorErr& ePlusMom, Double_t scale=0.5)
    { ::setEnergyConstraint( *this, eMinusMom, ePlusMom, scale); }

    void setEnergyConstraint( BtaCandidate& cand, const TEventInfo* ev, Double_t scale=0.5)
    { ::setEnergyConstraint( *this, ev, scale); }

    // Constraint the momentum of the candidate
    void setMomentumConstraint()
    { ::setMomentumConstraint( *this ); }

    void setMomentumConstraint( const TVectorErr& momentum, 
			        const TVector3& boost=TVector3(0,0,0) )
    { ::setMomentumConstraint( *this,momentum,boost); }

    // Beam Constrain - 
    // Constrain the head of a decay tree to come from a given vertex
    void setBeamConstraint( const TPointErr& beam )
    { ::setBeamConstraint( *this, beam ); }

    void setBeamConstraint( const TEventInfo* ev)
    { ::setBeamConstraint( *this, ev); }

    void setTrajectory(const TLorentzVector& p4, const TError& p4Err,
	Int_t charge,const PdtEntry* hypo,
	BtaAbsVertex* dVtx );
    
private:
    
    //
    // Private functions (access to friends only)
    //
    Bool_t isLocal() const { return kTRUE; }
    
    // Set the decay vertex - operators can do that 
    void setDecayVtx( BtaAbsVertex*  theVtx ) ;
    
    // Sets the mother link
    void setMotherLink( BtaCandidate* m );
    
    // Drop the mother link
    void dropMotherLink();
    
    // Add a daughter link
    void addDaughterLink( const BtaCandidate* );
    
    // Remove a daughter 
    void removeDaughter( BtaCandidate* );
    
    // fill the list with outgoing candidates (recursive function)
    void addToVertexingList( BtaCandList& );
    
public:
    ClassDef(BtaCandidate,1) // Candidate base class
	
    friend class PAFReader;
    friend class KangaReader;
    friend class TBooster;
    friend class TOperatorBase;
};

// standalone print
std::ostream&  operator << (std::ostream& o, const BtaCandidate&);

#endif

