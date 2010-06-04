#ifndef PAFMCASSOCBASE_H
#define PAFMCASSOCBASE_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFMcAssocBase							//
//                                                                      //
// Definition of the Monte Carlo truth base class in PAF		//
//                                                                      //
// Author: Rolf Dubitzky - TU Dresden, June 99				//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
 
class PAFMcAssocBase : public TObject {
public:
  PAFMcAssocBase();
  PAFMcAssocBase( const PAFMcAssocBase& other );
  virtual ~PAFMcAssocBase();
  Int_t GetMicro() const { return fMicroIndex; }
  Int_t GetMcTru() const { return fMcTruIndex; }
  Float_t GetConsistency() const { return fConsistency; }
  void SetMicro(Int_t i) { fMicroIndex = i; }
  void SetMcTru(Int_t i) { fMcTruIndex = i; }
  void SetConsistency(Float_t f) { fConsistency = f; }

private:

  // Data members:
  Short_t fMicroIndex;
  Short_t fMcTruIndex;
  Float_t fConsistency;

  ClassDef(PAFMcAssocBase,1) // PAF base class to store MC associations
};


#endif
