//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaAbsVertex								//
//                                                                      //
// Abstract Definition of the vertex					//
//                                                                      //
// Author: Marcel Kunze, RUB, Mar. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#ifndef BtaAbsVertex_hh
#define BtaAbsVertex_hh

#include <iostream>

#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TMatrixD.h"

class BtaCandidate;

//		---------------------
// 		-- Class Interface --
//		---------------------

class BtaAbsVertex : public TObject 
{
    
public:
    
    // status enum RF 8/14/98
    enum VtxStatus{ Success=0, NonConverged, BadInput, Failed, UnFitted };
    
    // type vertex enum FMV 01/24/00
    enum VtxType{ None=-1, Geometric, Kinematic }; 
    
private:
    
    //--------------------
    // Instance Members --
    //--------------------
    
    // status
    VtxStatus       fStatus; //!
    
    // type
    VtxType         fType; //!
    
    //
    // unowned links to incoming and outgoing candidates 
    //
    
    // _THE_ incoming candidate :
    // either  a non-resonance composite candidate
    // or a resonance candidate with no mother
    BtaCandidate* fInComingCand; //!
    
    // Outgoing candidates
    // either non composite candidates
    // or non-resonance composite candidate
    int nOutGoingCands; //!
    BtaCandidate *fOutGoingCands[5]; //! Can not be streamed
    
    // Resonances are produced and are decayed at the vertex
    // and they necessarily have a mother
    int nResonances; //!
    BtaCandidate *fResonances[5]; //!
    
public:
    
    // Constructors expect to be provided by lower classes
    // BtaAbsVertex(const TLorentzVector &v );
    BtaAbsVertex();
    BtaAbsVertex( const BtaAbsVertex& );
    
    // Destructor
    virtual ~BtaAbsVertex( );
    
    // clone

    virtual BtaAbsVertex* clone() const = 0;

    // Operations
    virtual void printOn( std::ostream& o=std::cout ) const;

    void setResonances(BtaCandidate *c) { fResonances[nResonances++] = c; }
    void setOutGoingCands(BtaCandidate *c) { fOutGoingCands[nOutGoingCands++] = c; }
    BtaCandidate& getResonances(Int_t i) { return *fResonances[i]; }
    BtaCandidate& getOutGoingCands(Int_t i) { return *fOutGoingCands[i]; }
    BtaCandidate& getInComingCand() { return *fInComingCand; }
    void setInComingCand(BtaCandidate *cand) { fInComingCand = cand; }
    
    //
    // Pure virtual accessors to contained information
    //        to be defined in subclasses
    //
    
    // position of the vertex in space and time
    virtual TLorentzVector v4()      const =0;
    
    // covariance matrix
    virtual TMatrixD& covariance () const =0;
    
    // chi squared and number of dof of the vertex fit
    virtual double chiSquared()        const =0;
    virtual int nDof()                 const =0;
    
    //
    // non-virtual accessors
    //
    
    // status of the fit
    VtxStatus    status() const;
    
    // type of the fit
    VtxType    type() const;
    
    // vertex position
    TVector3     point()  const;
    
    // 4-momentum of the incoming particle
    TLorentzVector p4() const;
    
    // mass of the incoming particle
    double mass() const;
    
    // momentum of the incoming particle
    double p() const;
    
    // number of out going candidates
    int nOutGoing() const;
    
    // number of resonances (states produced and decayed at that vertex)
    int nResonance() const;
    
    // incoming candidate
    const BtaCandidate* inComingCand() const;
    
    // mother particle
    // that's the incoming particle if not a resonance,
    // or the resonance at the top of the decay tree
    const BtaCandidate* theMother() const;
    
    // Interface to vertexing algorithms (M.Bondioli & M.Carpinelli 7/17/98)
    // accessors to weight matrices defined as the 2nd derivative of   
    // chi2 wrt x and p.
    virtual TMatrixD&  XXWeight() const;
    virtual TMatrixD&  PPWeight() const;
    virtual TMatrixD&  XPWeight() const;
    virtual void       getAllWeights(TVector3& xv,
				     TVector3& pv,
				     TMatrixD& xxWeight,
				     TMatrixD& ppWeight,
				     TMatrixD&    xpWeight) const;
    virtual TMatrixD&  XXCov() const;
    virtual TMatrixD&  PPCov() const;
    virtual TMatrixD&  XPCov() const;
    //
    // Modifiers
    //
    void setStatus( VtxStatus );
    void setType( VtxType );
    
private:
    
    // Operators
    BtaAbsVertex& operator=( const BtaAbsVertex& ) 
    { 
	//    abort(); 
	return *this; 
    }
    
public:
    ClassDef(BtaAbsVertex,1) // Vertex base class
	
};

// standalone print
std::ostream&  operator << (std::ostream& o, const BtaAbsVertex&);

#endif




