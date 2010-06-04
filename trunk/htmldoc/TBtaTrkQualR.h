//--------------------------------------------------------------------------
//
// Description:
//	Class TBtaTrkQualR
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
#ifndef TBTATRKQUALPR_HH
#define TBTATRKQUALPR_HH

#include "TObject.h"
#include "KangaSchema/ComPackFlatFloat.h"
#include "KangaSchema/ComPackNonFlatFloat.h"

class TBtaTrkQualR {
    
public:
    
    enum ByteVars  { tLen, nDof, svtPidStatus, nDchHits, firstDch, lastDch, stFound, nByteVars };
    enum ShortVars { chi2, svtPattern, nShortVars };
    
    // Constructors:
    TBtaTrkQualR();
    
    virtual ~TBtaTrkQualR();
    
    virtual void Reset();
    
    // Getters:
    
    UInt_t   GetSvtPattern();
    UShort_t GetNSvtHits();
    UShort_t GetFirstDch();
    UShort_t GetLastDch();
    UShort_t GetNDchHits();
    UShort_t GetPidHypo();
    UShort_t GetFitStatus();
    Float_t  GetChi2();
    Int_t    GetNDof();
    Float_t  GetTLen();
    Float_t  GetStFound();
    
    // Setters:
    
    void     SetSvtPattern(UInt_t pattern) { _svtPattern= pattern; } 
    void     SetNSvtHits(UShort_t hits) { _nSvtHits= hits; }
    void     SetFirstDch(UShort_t first) { _firstDch= first; }
    void     SetLastDch(UShort_t last) { _lastDch= last; }
    void     SetNDchHits(UShort_t hits) { _nDchHits= hits; }
    void     SetPidHypo(UShort_t pid) { _pidHypo= pid; }
    void     SetFitStatus(UShort_t status) { _fitStatus= status; }
    void     SetChi2(Float_t chisq) { _chi2= chisq; }
    void     SetNDof(Int_t ndof) { _nDof= ndof; }
    void     SetTLen(Float_t tlen) { _tLen= tlen; }
    void     SetStFound(Float_t st) { _stFound= st; }
    
private:  
    
    UInt_t _svtPattern;     // SVT hit pattern
    UShort_t _nSvtHits;     // # of SVT hits 
    UShort_t _firstDch;     // first Dch layer
    UShort_t _lastDch;      // last Dch layer
    UShort_t _nDchHits;     // # of Dch hits
    UShort_t _pidHypo;      // Pid hypo
    UShort_t _fitStatus;    // Fit status 
    Float_t _chi2;          // Track fit chi**2
    Int_t _nDof;            // # of Dch hits
    Float_t _tLen;          // track flight length
	Float_t _stFound;       // Start of found range
	   
    Char_t   bytes[nByteVars];
    UShort_t shorts[nShortVars];
    
    static ComPackNonFlatFloat Packer;
    static ComPackNonFlatFloat Packer8;
    static ComPackFlatFloat startPacker;
    
    // Disallowed:
    TBtaTrkQualR( const TBtaTrkQualR & );
    TBtaTrkQualR & operator = ( const TBtaTrkQualR & );
    
    friend class TBtaMicroCandR;
    
    ClassDef(TBtaTrkQualR,2) // Tracking info (Rho Kanga implementation)
	
};

#endif
