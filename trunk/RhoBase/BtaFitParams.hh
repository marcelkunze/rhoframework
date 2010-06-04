#ifndef BtaFitParams_hh
#define BtaFitParams_hh
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaFitParams								//
//                                                                      //
// Kinematics base class for BtaCandidates				//
//									//
// Author List:								//
// Marcel Kunze,  RUB, Feb. 00						//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "RhoMath/TError.h"
#include "RhoMath/TPointErr.h"
#include "RhoMath/TVectorErr.h"
#include "RhoMath/TLorentzVectorErr.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#define MATRIXSIZE 28

class BtaCandidate;

//		---------------------
// 		-- Class Interface --
//		---------------------

class BtaFitParams : public TObject
{
protected:
    
    // the params
    Char_t  fCharge;		// The electrical charge
    Float_t fXposition,		// The origin in x
	    fYposition,		// The origin in y
	    fZposition,		// The origin in z
	    fXmomentum,		// The momentum in x
	    fYmomentum,		// The momentum in y
	    fZmomentum,		// The momentum in z
	    fEnergy,		// The total energy
	    fErrP7[MATRIXSIZE],	// The symmetric 7*7 error matrix
	    fParams[5],		// The helix fit parameters
	    fCov[15];		// The helix error matrix
    
    // Fitter interface
public:
    enum Status{unlocked=0,locked,nailed,unknown};
private:
    Double_t fChi2;
    Status   fStatus;
    BtaCandidate* fFit; //owned pointer
public:
    
    //
    // Public interface
    //
    
    //  default
    BtaFitParams();
    
    //
    //  from the quantities themselves
    //
    
    // this one takes the full 7X7 covariance
    BtaFitParams( Double_t charge,
	const TVector3& pos,
	const TLorentzVector& p4,
	const TMatrixD& cov7 );
    
    // copy constructor
    BtaFitParams( const BtaFitParams& );
    
    // Use helix parameters (for fitting applications)
    BtaFitParams( Double_t charge, Float_t* par, Float_t* cov );
    
    // destructor
    ~BtaFitParams();

    BtaFitParams&  getFitParams() const { return (BtaFitParams&) *this; }

    // modifiers
    void setCharge( Double_t charge ) { fCharge = (Char_t) charge; }
    void setMass( Double_t mass );
    void setEnergy( Double_t energy ) { fEnergy = (Float_t) energy; }
    void setE( Double_t energy ) { fEnergy = (Float_t) energy; }
    void setMassAndEnergy( Double_t mass, Double_t energy );
    void setPosition( const TVector3& pos );
    void setPos( const TVector3& pos ) { setPosition(pos); }
    void setP3( const TVector3& p3 );
    void setVect( const TVector3& p3 ) { setP3(p3); }
    void setP4( Double_t mass, const TVector3& p3 );
    void setP4( const TLorentzVector& p4 );
    void setP7( const TVector3& pos, const TLorentzVector& p4 );
    void setCovP4( const TMatrixD& covP4 );
    void setCov7( const TMatrixD& cov7 );
    void setCov7( const TMatrixD& covPos, const TMatrixD& covP4 );
    void setCov7( const TMatrixD& covPos, const TMatrixD& covP4,const TMatrixD& covPosP4 );
    void set( const TVector3& pos, const TLorentzVector& p4, const TMatrixD& cov7 );
    void set( Double_t mass,const TPointErr& posErr, const TVectorErr& p3Err, const TMatrixD& xpErr ); 
    void set( Double_t charge, Float_t* par, Float_t* cov=0); // From helix parms
    void setErr(Float_t *err) {if (err!=0) for (int i=0; i<MATRIXSIZE;i++) fErrP7[i] = err[i];}
    void setPar(Float_t *par) {if (par!=0) if (par!=0) for (int i=0; i<5;i++) fParams[i] = par[i];}

    // operators
    Bool_t  operator == (const BtaFitParams& c) const;
    
    // accessors
    Double_t	getCharge()   const { return fCharge; }
    Double_t	charge()   const { return fCharge; }
    TVector3	getPosition()   const { return TVector3(fXposition,fYposition,fZposition); }
    TVector3	pos()   const { return getPosition(); }
    TVector3	getMomentum()   const { return TVector3(fXmomentum,fYmomentum,fZmomentum); }
    TVector3	getVect()   const { return TVector3(fXmomentum,fYmomentum,fZmomentum); }
    Double_t	px() const { return fXmomentum; }
    Double_t	py() const { return fYmomentum; }
    Double_t	pz() const { return fZmomentum; }
    TPointErr	posWCov()   const;
    TError&	posCov()   const;
    TLorentzVector   p4()   const { return TLorentzVector(fXmomentum,fYmomentum,fZmomentum,fEnergy); }
    TError&	p4Cov()   const;
    TError&	p4Err()   const { return p4Cov(); }
    TLorentzVectorErr p4WErr()  const;
    TVector3	p3()   const { return getMomentum(); }
    TError&	p3Cov()   const;
    TVectorErr  p3WErr()  const;
    Double_t	e()   const { return fEnergy; }
    Double_t	energy() const  { return fEnergy; }
    Double_t	getEnergy() const  { return fEnergy; }
    Float_t*	getErrP7() { return fErrP7; }
    Double_t	eVar()   const;
    Double_t	m()   const;
    Double_t	p()   const;
    TMatrixD&	cov7()   const;
    TMatrixD&	xPCov()   const;
    Double_t	pt()  const { return TMath::Sqrt(fXmomentum*fXmomentum+fYmomentum*fYmomentum); }
    
    // Fitter interface (Parameters are stored in TrkExchangePar format)
    
    Double_t chi2() const {return fChi2;}
    void setChi2(Double_t chi2) {fChi2=chi2;}
    Status  getStatus()  const {return fStatus;};
    void setStatus(Status s) {fStatus=s;};
    void setFit(BtaCandidate* b) {fFit=b;}
    const BtaCandidate* getFit() const { return fFit;}
    Float_t *getHelixParams() { return fParams; }
    Float_t *getHelixCov() { return fCov; }
    Double_t getHelixD0() const {return fParams[0];}
    Double_t getHelixPhi0() const    {return fParams[1];}
    Double_t getHelixOmega() const {return fParams[2];}
    Double_t getHelixZ0() const	{return fParams[3];}
    Double_t getHelixTanDip() const	{return fParams[4];}
    void setHelixParms(Float_t *p);
    void setHelixCov(Float_t *p);
    TError& errorMatrixOfEmcClusters(Double_t clEnergy,Double_t clTheta,Double_t clPhi) const;
    const TMatrixD& derivsParWrtCoord(TVector3 mom, TVector3 pos, Int_t charge) const;
    const TMatrixD& derivsCoordWrtPar(Float_t *params, Int_t charge, Double_t fltlen) const;
    
    void boost(Double_t bx, Double_t by, Double_t bz);
    void boost(const TVector3 &p)    { boost(p.X(),p.Y(),p.Z()); }
    
    //
    // Prints
    //
   void printOn( std::ostream& o=std::cout ) const;

private:
    Double_t correlation(Int_t x1,Int_t x2,const TMatrixD& m,const TMatrixD &cov) const;
    
public:
    ClassDef(BtaFitParams,1) // Class to store candidate fit parameters	
};
#endif




