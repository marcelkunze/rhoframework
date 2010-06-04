//--------------------------------------------------------------------------
//
// Description:
//	Class TBtaIfrQualR
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
#ifndef BTAIFRQUALPR_HH
#define BTAIFRQUALPR_HH

#include "TObject.h"
#include "KangaSchema/ComPackFlatFloat.h"
#include "KangaSchema/ComPackNonFlatFloat.h"

class TBtaIfrQualR : public TObject {
    
public:
    
    enum ByteVars { IfrNstrip, IfrBits, expInt, intLenBI, intLen, 
	trkMatchCons, trkMatchChi2, emcMatchCons, rpcProb, ifrDigis, nByteVars };
    
    // Constructors:
    TBtaIfrQualR();
    
    virtual ~TBtaIfrQualR();
    
    virtual void Reset();
    
    // Getters:
    
    void     GetIfrLayStrips(UShort_t laystrips[20]);
    UShort_t GetIfrNstrip();
    UShort_t GetIfrDigis();
    UChar_t  GetIfrBits();
    Float_t  GetInteractionLengthsBeforeIron();
    Float_t  GetExpectedInteractionLengths();
    Float_t  GetMeasuredInteractionLengths();
    Float_t  GetIfrTrkMatch();
    Float_t  GetIfrEmcMatch();
    UShort_t GetLastBarrel();
    Float_t  GetIfrClusterFit();
    Float_t  GetIfrRpcMuonProbability();
    
    // Setters:
    
    void SetIfrLayStrips(UShort_t laystrips[20]);
    void SetIfrNstrip(UShort_t nstrip) { _IfrNstrip= nstrip; }
    void SetIfrBits(UChar_t bits) { _IfrBits= bits; }
    void SetInteractionLengthsBeforeIron(Float_t ilbi) { _interactionLengthsBeforeIron= ilbi; }
    void SetExpectedInteractionLengths(Float_t eil) { _expectedInteractionLengths= eil; }
    void SetMeasueredInteractionLengths(Float_t mil) { _measuredInteractionLengths= mil; }
    void SetIfrTrkMatch(Float_t tm) { _IfrTrkMatch= tm; }
    void SetIfrEmcMatch(Float_t em) { _IfrEmcMatch= em; }
    void SetLastBarrel(UShort_t lb) { _lastBarrel= lb; }
    void SetIfrRpcMuonProbability(Float_t x) { _rpcProb= x; }
    void SetIfrClusterFit(Float_t x) { _IfrTrkMatchChi2= x; }
    
private:  
    
    UShort_t _IfrLayStrips[20];
    UShort_t _IfrNstrip;
    UShort_t _IfrDigis;
    UChar_t  _IfrBits;
    Float_t  _expectedInteractionLengths;
    Float_t  _interactionLengthsBeforeIron;
    Float_t  _measuredInteractionLengths;
    Float_t  _IfrTrkMatch;
    Float_t  _IfrEmcMatch;
    UShort_t _lastBarrel;
    
    // new in 8.6.x schema
    Float_t  _rpcProb;
    Float_t  _IfrTrkMatchChi2;
    
    Char_t   bytes[nByteVars];   
    Char_t   _IfrPattern[11];

    // Disallowed:
    TBtaIfrQualR( const TBtaIfrQualR & );
    TBtaIfrQualR & operator = ( const TBtaIfrQualR & );
    
    static ComPackNonFlatFloat Packer;
    static ComPackFlatFloat intlPack;
    static ComPackFlatFloat intlPack2;
    static ComPackFlatFloat Packer2;
    static ComPackNonFlatFloat Packer8;

    friend class TBtaMicroCandR;
    
    ClassDef(TBtaIfrQualR,2) // IFR info (Rho Kanga implementation)
	
};

#endif
