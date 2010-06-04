//  	-*- Mode: c++ -*-
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaFitParamsR_002.h,v 1.4 2008-02-25 15:10:31 marcel Exp $
//
// Description:
//	Class BtaFitParamsR_002
//
//      Persistent partner of BtaFitParams; differs from the transient class
//        in a way the data members are packed
// 
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Stefan Kluth
//      Leif Wilden
//
// Copyright Information:
//	Copyright (C) 1999                LBNL
//
//------------------------------------------------------------------------
#ifndef BTAFITPARAMSR_002_HH
#define BTAFITPARAMSR_002_HH

//----------------------
// Base Class Headers --
//----------------------

//---------------
// C++ Headers --
//---------------
#include <iosfwd>

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "Rtypes.h"
#include "TVectorD.h"

class TFitParams;

//		---------------------
// 		-- Class Interface --
//		---------------------

class BtaFitParamsR_002 {
    //--------------------
    // Instance Members --
    //--------------------
public:
    
    BtaFitParamsR_002();
    //BtaFitParamsR_002( const BtaCandidate & );
    
    virtual ~BtaFitParamsR_002();
    
    TFitParams* Transient() const;
    
   //virtual void print( ostream & ) const;

private:
    
    // the params
    Char_t  _q;
    Float_t _params[5];
    
    // the  covariance
    UShort_t _diag[15];
    Char_t   _corr[10];
    
    // functions
    int nParm() const {return (_q==0) ? 4:5;}
    int nCorr() const {int np=nParm();return int( np*(np-1)/2. );}
    int nOct() const{return (_q==0) ? nCorr():0;}
    int nShort() const{return (_q==0) ? nParm():nParm()+nCorr();}
    int indexCorr(int i, int j) const;
    Float_t* LoadRotation() const;   
    Float_t* ComPackedCovariance(const TVectorD& eigenValues,const TVectorD& angles) const;
    
    // disallow
    BtaFitParamsR_002( const BtaFitParamsR_002 & );
    BtaFitParamsR_002 & operator= ( const BtaFitParamsR_002 & );
    
    friend class BtaMicroCandR_004;
    
    ClassDef(BtaFitParamsR_002,1)
	
};

#endif




