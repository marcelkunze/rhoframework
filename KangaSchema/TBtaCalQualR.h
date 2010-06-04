//--------------------------------------------------------------------------
//
// Description:
//	Class TBtaCalQualR
//
//      summarizes the information on the quality of the track reconstruction
// 
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     Stefan Kluth
//     Leif Wilden (adapted as ROOT standalone version)
//
// Copyright Information:
//	Copyright (C) 1999                LBNL, TU Dresden
//
//------------------------------------------------------------------------
#ifndef TBtaCalQualR_HH
#define TBtaCalQualR_HH

#include <math.h>
#include "TObject.h"
#include "KangaSchema/ComPackFlatFloat.h"
#include "KangaSchema/ComPackNonFlatFloat.h"
#include "KangaSchema/ComPackExpFloat.h"

class TBtaCalQualR {
    
public:
    
    enum {nBitsMatch = 8 };
    
    // Constructors:
    TBtaCalQualR();
    
    // Destructor:
    virtual ~TBtaCalQualR();
    
    virtual void Reset();
    
    // Getters:
    
    UShort_t GetPidHypo();
    
    UShort_t GetNBumps();
    UShort_t GetNCrystals();
    
    Float_t  GetLat();
    Float_t  GetAbsZern42();
    Float_t  GetAbsZern20();
    Float_t  GetAbsZern31();
    Float_t  GetStatus();
    Float_t  GetSecondMomentTP();
    Float_t  GetEnergy();
    Float_t  GetRawEnergy();
    
    Float_t  GetCdPhiValue();
    Float_t  GetCdPhiLhood();
    UChar_t  GetCdPhiBits();
    
    Float_t  GetCentroidX();
    Float_t  GetCentroidY();
    Float_t  GetCentroidZ();
    
    void     GetCov(Float_t cov[10]);
    
    Float_t  GetS9S25();
    Float_t  GetS1S9();
    
    // Setters:
    
    void SetPidHypo(UShort_t pid) { _pidHypo= pid; }
    
    void SetNBumps(UShort_t n) { _nBumps= n; }
    void SetNCrystals(UShort_t n) { _nCrystals= n; }
    
    void SetLat(Float_t l) { _lat= l; }
    void SetAbsZern42(Float_t z) { _absZern42= z; }
    void SetAbsZern20(Float_t z) { _absZern20= z; }
    void SetAbsZern31(Float_t z) { _absZern31= z; }
    void SetStatus(Float_t z) { _status= z; }
    void SetSecondMomentTP(Float_t sm) { _secondMomentTP= sm; }
    void SetEnergy(Float_t e) { _energy= e; }
    void SetRawEnergy(Float_t e) { _rawEnergy= e; }
    
    void SetCdPhiValue(Float_t val) { _cdPhiValue= val; }
    void SetCdPhiLhood(Float_t lhood) { _cdPhiLhood= lhood; }
    void SetCdPhiBits(UChar_t bits) { _cdPhiBits= bits; }
    
    void SetCentroidX(Float_t cx) { _centroidX= cx; }
    void SetCentroidY(Float_t cy) { _centroidY= cy; }
    void SetCentroidZ(Float_t cz) { _centroidZ= cz; }
    
    void SetCov(const Float_t cov[10]);
    
    void SetS9S25(Float_t s) { _s9s25= s; }
    void SetS1S9(Float_t s) { _s1s9= s; }
    
private:
    
    enum ByteVars { bumpsPID, nCrystals, lat, absZern42, absZern20, emcStatus, secondMomentTP,
	s9s25, s1s9, cdPhiValue, signStatPhi, nByteVars };
    enum ShortVars { energy, rawEnergy, centroidX, centroidY, centroidZ, cov0, cov1, cov2,
	nShortVars };
    
    double minMatchConsistency() const 
    {return -1./(pow(2.0,nBitsMatch)-1);}
    double maxMatchConsistency() const {return 6.;}
    
    UShort_t _pidHypo;       // PID hypo
    
    UShort_t _nBumps;        // nBumps
    UShort_t _nCrystals;     // Number of crystals
    
    Float_t _lat;            // lateral moment
    Float_t _absZern42;      // Zernike-Moment 4,2
    Float_t _absZern31;      // Zernike-Moment 3,1
    Float_t _absZern20;      // Zernike-Moment 2,0
    
    // Note: AbsZern31 was removed beginnig with 8.6.x.
    // However on the objy persistent side the absZern31 variable
    // was "reused" for the emc status word (see below) "to save
    // one schema evolution"... o-:
    
    Float_t _secondMomentTP; // Second moment
    Float_t _energy;         // energy
    Float_t _rawEnergy;      // raw energy
    Float_t _s9s25;          // sum(9)/sum(25)
    Float_t _s1s9;           // sum(1)/sum(9)
    
    Float_t _cdPhiValue;     // Consistency value
    Float_t _cdPhiLhood;     // Consistency likelihood
    UChar_t _cdPhiBits;      // Consistency sign and status bits
    
    Float_t _centroidX;      // Centroid x
    Float_t _centroidY;      // Centroid y
    Float_t _centroidZ;      // Centroid z
    
    Float_t _cov[10];        // Covariance matrix (assumed 4 parameters)
    
    // new beginning with schema 8.6.x
    Float_t _status;         // emc status flag
    
    Char_t   bytes[nByteVars];
    UShort_t shorts[nShortVars];
    
    // disallowed
    TBtaCalQualR( const TBtaCalQualR & );
    TBtaCalQualR & operator = ( const TBtaCalQualR & );
    
    static ComPackNonFlatFloat Packer;
    static ComPackFlatFloat PackerToOne;
    static ComPackFlatFloat PackerToLess;
    static ComPackFlatFloat cntPack;
    static ComPackExpFloat xpack;
    
    friend class TBtaMicroCandR;
    
    ClassDef(TBtaCalQualR,2) // Calorimetry info (Rho Kanga implementation)
	
};

#endif
