#ifndef BtaEventInfo_hh
#define BtaEventInfo_hh
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaEventInfo								//
//                                                                      //
// Contains and provides access to summary information in the event	//
// useful for analysis							//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Dec. 2k						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

//----------------------
// Base Class Headers --
//----------------------
#include "TObject.h"
#include "TVector3.h"
#include "RhoBase/BtaCandidate.hh"
#include "RhoBase/VAbsVertex.h"
#include "RhoMath/TPointErr.h"

class BtaEventShape;

//		---------------------
// 		-- Class Interface --
//		---------------------

class BtaEventInfo : public TObject {
    
    //--------------------
    // Instance Members --
    //--------------------
    
public:
    
    // Constructors
    BtaEventInfo();
    // there should eventually be a constructor that takes all the
    // various data items
    
    // Destructor
    virtual ~BtaEventInfo( );
    
    // Operations
   virtual void printOn( std::ostream& o=std::cout ) const;

    // accessors to contained information
    const BtaCandidate& positronBeam() const   {return fPositronBeam;}
    const BtaCandidate& electronBeam() const   {return fElectronBeam;}
    
    const TLorentzVector& cmFrame() const   {return fCmFrame;}
    
    const TPointErr& beamSpot() const        {return fBeamSpot;}
    const TPointErr& beamSpotBFlight() const        {return fBeamSpotBFlight;}
    const TPointErr primaryVertex() const;
    const VAbsVertex* primaryVtx() const { return fPrimaryVertex;}
    const TVector3* mCImpactPoint() const{ return fMCIP;}
    
    const BtaEventShape* eventShape() const    {return fEventShape;}
    
    // Function to set fPrimaryVertex
    void setPrimaryVertex(VAbsVertex* inVtx) ;
    void setIPTruth(const TLorentzVector& inVtx) ;
    void setIPTruth(const TVector3& inVtx) ;
    void setBeamSpot(const TPointErr& inVtx) {fBeamSpot=inVtx;}
    void setBeamSpotBFlight(const TPointErr& inVtx) {fBeamSpotBFlight=inVtx;}
    void setPepBeams(const BtaCandidate& ele,const BtaCandidate& pos) {fElectronBeam=ele;fPositronBeam=pos;}
    void setCmFrame(const TLorentzVector& cmf){fCmFrame =cmf;}
    
    void setEventShape(BtaEventShape* eventShape);
    
private:
    BtaCandidate fPositronBeam; //!The positron beam
    BtaCandidate fElectronBeam; //!The electron beam
    TLorentzVector fCmFrame; //!The CMS frame
    TVector3* fMCIP; //!The MC interaction point
    TPointErr fBeamSpot; //!The beam spot w/ error matrix
    TPointErr fBeamSpotBFlight; //!
    VAbsVertex* fPrimaryVertex; //!The primary vertex
    
    BtaEventShape* fEventShape; //!The event shape
    
public:
    ClassDef(BtaEventInfo,1) // Summary information in the event
};

// standalone print
std::ostream&  operator << (std::ostream& o, const BtaEventInfo&);

#endif




