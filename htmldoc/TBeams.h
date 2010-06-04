#ifndef TBEAMS_H
#define TBEAMS_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TBeams	    							//
// TBeamSpotCal	    							//
// TEnergies	    							//
// TFillPattern	    							//
// TBoostCal								//
//                                                                      //
// Beam calibration classes						//
//                                                                      //
// Author: Marcel Kunze, RUB, Nov. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "RhoConditions/TConditions.h"

#include "RhoMath/TDoubleErr.h"
#include "RhoMath/TVectorErr.h"
#include "RhoMath/TLorentzVectorErr.h"

//------------------------------------------------------------------------
#define HepLorentzVectorSize 4
#define Hep3VectorSize       3

#include <iostream>

class PepBeamsR : public RooCondData {
public:
    PepBeamsR();
    virtual ~PepBeamsR();
    void PrintOn(std::ostream& o=std::cout) const;
protected:
    Short_t _numBunches, _numEmptyBunches;
    Double_t _interactionPoint[HepLorentzVectorSize];
    Double_t _interactionPointErr[HepLorentzVectorSize*HepLorentzVectorSize];
    Double_t _ePlusMomentum[Hep3VectorSize];
    Double_t _ePlusMomentumErr[Hep3VectorSize*Hep3VectorSize];
    Double_t _eMinusMomentum[Hep3VectorSize];
    Double_t _eMinusMomentumErr[Hep3VectorSize*Hep3VectorSize];
    ClassDef(PepBeamsR,1) // persistent PEP-II beam conditions data
};

// standalone print
std::ostream&  operator <<(std::ostream& o, const PepBeamsR&);

// Class TBeams

class TEnergies;
class TBeamSpotCal;

class TBeams : public PepBeamsR {
public:
    TBeams() {}
    TBeams(PepEnergiesR &energies,PepBeamSpotCalR &cal);
    virtual ~TBeams() {}
    
    Int_t NumBunches() const { return _numBunches; }
    Int_t NumEmptyBunches() const { return _numEmptyBunches; }
    TLorentzVector InteractionPoint();
    Double_t	 InteractionPointErr(UInt_t i, UInt_t j) const { return _interactionPointErr[i*4+j]; }
    TVector3       Interaction3Point(); // spatial part
    TVector3       EPlusMomentum();    
    Double_t       EPlusMomentumErr(UInt_t i, UInt_t j) const { return _ePlusMomentumErr[i*3+j]; }    
    TVector3       EMinusMomentum();   
    Double_t       EMinusMomentumErr(UInt_t i, UInt_t j) const { return _eMinusMomentumErr[i*3+j]; }    
    
    Double_t ElectronEnergy(TVector3 momentum);
    Double_t EPlusEnergy();
    Double_t EMinusEnergy();

    // calculated quantities with error matrices:
    TVectorErr        EPlusMomentumWErr();
    TVectorErr        EMinusMomentumWErr();
    TVectorErr        TotalMomentumWErr();
    TLorentzVectorErr Total4MomentumWErr();
    TLorentzVectorErr Total4MomentumCMWErr();
    TDoubleErr        EPlusEnergyWErr();
    TDoubleErr        EMinusEnergyWErr();
    TLorentzVectorErr EPlus4MomentumWErr();
    TLorentzVectorErr EMinus4MomentumWErr();
    TDoubleErr        EnergyCMWErr(); 
    TVectorErr        BoostCMtoLabWErr(); // from CM frame to lab  

    // Same quantities without error matrices for faster access:
    TVector3       TotalMomentum();
    TLorentzVector Total4Momentum();
    TLorentzVector Total4MomentumCM();
    TLorentzVector EPlus4Momentum();
    TLorentzVector EMinus4Momentum();
    Double_t       EnergyCM(); 
    TVector3       BoostCMtoLab(); // from CM frame to lab  
    Double_t       CrossingAngle();
    
    void Update(PepEnergiesR &energies,PepBeamSpotCalR &cal);
    void SetDefaultMomenta(Double_t ePlusEnergy, Double_t eMinusEnergy);
    
    ClassDef(TBeams,1) // PEP-II beams interface
};


//------------------------------------------------------------------------
/** This class is a ROOT persistent version of the transient PepBeamSpotCal
* defined in the PepEnv package, similar to the Objectivity persistent
* PepBeamSpotCalP class in the PepCond package.
*/
class PepBeamSpotCalR : public RooCondData {
public:
/** ROOT I/O requires a default constructor. Create an object based
    * on the defaults specified by the transient default constructor. */
    PepBeamSpotCalR();
    /** Construct a persistent object corresponding to the specified
    * transient object. */
    //PepBeamSpotCalR(const PepBeamSpotCal& transient);
    virtual ~PepBeamSpotCalR();
    /** Create a transient object on the heap initialized with this persistent
    * object's data. */
    //PepBeamSpotCal* transient() const;
    void PrintOn(std::ostream& o=std::cout) const;
    // Accessors (const)
    TVector3 BeamSpot() {return TVector3(_x,_y,_z);}
    Double_t       OrientationXX() const { return _cxx; }
    Double_t       OrientationXY() const { return _cxy; }
    Double_t       OrientationXZ() const { return _cxz; }
    Double_t       OrientationYY() const { return _cyy; }
    Double_t       OrientationYZ() const { return _cyz; }
    Double_t       OrientationZZ() const { return _czz; }
    
    Double_t       Background()  const {return _background;}
    Int_t          NumTracks()   const {return _numTracks;}
protected:
    //void initialize(const PepBeamSpotCal & transient);
    const Double_t& ErrorMatrix(Int_t row, Int_t col) const;
    
    // Data members and how they correspond to PepBeamSpotCal data members:
    // The point of the _beamSpot:
    Double_t _x;
    Double_t _y;
    Double_t _z;
    
    // Size and orientation matrix of the _beamSpot. Of the 3x3 matrix,
    // only the 3/2(3+1) unique elements are kept:
    Double_t _cxx;
    Double_t _cxy;
    Double_t _cxz;
    Double_t _cyy;
    Double_t _cyz;
    Double_t _czz;
    
    // _background:
    Double_t _background;
    
    // _numTracks:
    Short_t _numTracks;
    
    // The 10 columns of the _errorMatrix. Of the 100 elements, only 
    // the 10/2(10+1) unique elements on the lower left part of the matrix
    // are kept. See PepBeamSpotCal.h for the row-datum correspondence:
    enum { ErrMatrixSize = 10 };
    Double_t _errorMatrix[ErrMatrixSize*(ErrMatrixSize+1)/2];
    
    ClassDef(PepBeamSpotCalR,1) // Persistent PEP-II beam conditions data
};

// standalone print
std::ostream&  operator <<(std::ostream& o, const PepBeamSpotCalR&);

class TBeamSpotCal : public PepBeamSpotCalR {
public:
    TBeamSpotCal() {}
    virtual ~TBeamSpotCal() {}
    ClassDef(TBeamSpotCal,1) // Persistent PEP-II beam conditions data
};

//------------------------------------------------------------------------
/** This class is a ROOT persistent version of the transient PepEnergies class
* defined in the PepEnv package, similar to the Objectivity persistent
* PepEnergiesP class in the PepCond package.
*/
class PepEnergiesR : public RooCondData {
public:
/** ROOT I/O requires a default constructor. Create an object based
    * on the defaults specified by the transient default constructor. */
    PepEnergiesR();
    /** Construct a persistent object corresponding to the specified
    * transient object. */
    //PepEnergiesR(const PepEnergies& transient);
    virtual ~PepEnergiesR();
    /** Create a transient object on the heap initialized with this persistent
    * object's data. */
    //PepEnergies* transient() const;
    // Accessors (const)
    Double_t LerEnergy() const {return _lerEnergy;}
    Double_t HerEnergy() const {return _herEnergy;}
    void PrintOn(std::ostream& o=std::cout) const;
protected:
    Double_t _lerEnergy, _herEnergy;
    ClassDef(PepEnergiesR,1) // persistent PEP-II beam conditions data
};

// standalone print
std::ostream&  operator <<(std::ostream& o, const PepEnergiesR&);

class TEnergies : public PepEnergiesR {
public:
    TEnergies() {}
    virtual ~TEnergies() {}
    ClassDef(TEnergies,1) // PEP-II beam conditions data
};

//------------------------------------------------------------------------
/** This class is a ROOT persistent version of the transient PepFillPattern
* defined in the PepEnv package, similar to the Objectivity persistent
* PepFillPatternP class in the PepCond package.
*/
class PepFillPatternR : public RooCondData {
public:
/** ROOT I/O requires a default constructor. Create an object based
    * on the defaults specified by the transient default constructor. */
    PepFillPatternR();
    /** Construct a persistent object corresponding to the specified
    * transient object. */
    //PepFillPatternR(const PepFillPattern& transient);
    virtual ~PepFillPatternR();
    /** Create a transient object on the heap initialized with this persistent
    * object's data. */
    //PepFillPattern* transient() const;
    // Accessors (const)
    Int_t HerFirstBucket() const {return _herFirstBucket;}
    Int_t HerLastBucket() const {return _herLastBucket;}
    Int_t HerStepSize() const {return _herStepSize;}
    Int_t LerFirstBucket() const {return _lerFirstBucket;}
    Int_t LerLastBucket() const {return _lerLastBucket;}
    Int_t LerStepSize() const {return _lerStepSize;}
    void PrintOn(std::ostream& o=std::cout) const;
protected:
    //void initialize(const PepFillPattern & transient);
    Short_t _herFirstBucket;
    Short_t _herLastBucket;
    Short_t _herStepSize;
    Short_t _lerFirstBucket;
    Short_t _lerLastBucket;
    Short_t _lerStepSize;
    ClassDef(PepFillPatternR,1) // persistent PEP-II beam conditions data
};

// standalone print
std::ostream&  operator <<(std::ostream& o, const PepFillPatternR&);

class TFillPattern : public PepFillPatternR {
public:
    TFillPattern() {}
    virtual ~TFillPattern() {}
    ClassDef(TFillPattern,1) // PEP-II beam conditions data
};


//------------------------------------------------------------------------
/** This class is a ROOT persistent version of the transient PepEnergies class
* defined in the PepEnv package, similar to the Objectivity persistent
* PepEnergiesP class in the PepCond package.
*/
class PepBoostCalR : public RooCondData {
public:
/** ROOT I/O requires a default constructor. Create an object based
    * on the defaults specified by the transient default constructor. */
    PepBoostCalR();
    /** Construct a persistent object corresponding to the specified
    * transient object. */
    //PepBoostCalR(const PepBoostCal& transient);
    virtual ~PepBoostCalR();
    /** Create a transient object on the heap initialized with this persistent
    * object's data. */
    //PepBoostCal* transient() const;
    
    // The boost from the CM to the lab and its error matrix:
    TVector3 Beta() const;
    
    // # of events used in the caliubration:
    int NumEvents() const;
    
    void PrintOn(std::ostream& o=std::cout) const;
protected:
    // Elements of the _beta vector and covariance matrix:
    Double_t _betaX,_betaY,_betaZ;
    Double_t _errXX,_errXY,_errXZ,_errYY,_errYZ,_errZZ;
    Short_t _numEvents;
    ClassDef(PepBoostCalR,1) // persistent PEP-II beam conditions data
};

// standalone print
std::ostream&  operator <<(std::ostream& o, const PepBoostCalR&);

class TBoostCal : public PepBoostCalR {
public:
    TBoostCal() {}
    virtual ~TBoostCal() {}
    ClassDef(TBoostCal,1) // PEP-II beam conditions data
};

#endif

