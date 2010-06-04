//--------------------------------------------------------------------------
//
// Description:
//	Class TBtaPidInfoR
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
#ifndef TBtaPidInfoR_HH
#define TBtaPidInfoR_HH
#include "TObject.h"
#include "KangaSchema/ComPackUFloat.h"
 
#ifndef PidSystem_HH
#define PidSystem_HH

class PidSystem : public TObject
{
public:
  //static const int nSystem;   
  //static const int nChSystem; 
  //static const int nNeuSystem;
  enum System
  {
    bbr = 0,
    ifr,
    emc,
    drc,
    dch,
    svt,
    trk
  };
ClassDef(PidSystem,1) // The BaBar PID
};

#endif

class TBtaPidInfoR {

public:

  enum dims { NCHENT= 30, NNEUENT= 15 };

  // Constructor:
  TBtaPidInfoR();

  // Destructor:
  virtual ~TBtaPidInfoR();

  virtual void Reset();

  // Getters:

  UShort_t GetNent() const;
  void     GetCoValues(Float_t values[NCHENT]) const;
  void     GetCoLhoods(Float_t lhoods[NCHENT]) const;
  void     GetCoBits(UChar_t bits[NCHENT]) const;

  // Setters:

  void     SetNent(UShort_t nent) { _nent= nent; }
  void     SetCoValues(const Float_t values[NCHENT]);
  void     SetCoLhoods(const Float_t lhoods[NCHENT]);
  void     SetCoBits(const UChar_t bits[NCHENT]);

  // Transient interface

  Float_t* GetConsistency(PidSystem::System sys);
  Float_t* GetLikelihood(PidSystem::System sys);
  UChar_t* GetStatus(PidSystem::System sys);

private:  
  // Limits
  enum {
    FRACLEN=6,   // formerly 4   --  but the ComPackUFloat class
    EXPLEN=6,    // formerly 4   --  was buggy and incompatible 
    BIAS=-29     // formerly -5  --  (different unpacked values)
  };

  UShort_t _nent;             // Number of actual entries in arrays
  Float_t _coValues[NCHENT];  // Consistency values
  Float_t _coLhoods[NCHENT];  // Consistency likelihoods
  UChar_t _coBits[NCHENT];    // Consistency sign and status bits

  UShort_t _Cons[25    /* 5*5 */ ];   // Dch/Svt/Drc/Emc/Ifr Consistency
  Char_t   _status[25];

  static ComPackUFloat packer;

  // Disallowed:
  TBtaPidInfoR( const TBtaPidInfoR & );
  TBtaPidInfoR & operator = ( const TBtaPidInfoR & );

  friend class TBtaMicroCandR;

  ClassDef(TBtaPidInfoR,2) // PID info (Rho Kanga implementation)

};

#endif

