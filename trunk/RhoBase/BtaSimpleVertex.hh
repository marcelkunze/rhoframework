#ifndef BtaSimpleVertex_hh
#define BtaSimpleVertex_hh
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// BtaSimpleVertex							//
//                                                                      //
// Simple vertex class							//
//									//
// Author List:								//
// M.Kunze, Bochum University, Feb. 01					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "RhoBase/BtaAbsVertex.hh"

class BtaSimpleVertex : public BtaAbsVertex
{
  TVector3 fV;
  Double_t fT;
  Double_t fChiSquared;
  Int_t    fNDof;

public:
  BtaSimpleVertex(); 
  BtaSimpleVertex( Double_t x, Double_t y, Double_t v, Double_t t=0, Int_t ndof = 0, Double_t c = 999.);
  BtaSimpleVertex( const TVector3& v, Double_t t=0, Int_t ndof = 0, Double_t c = 999.); 
  BtaSimpleVertex( const BtaSimpleVertex& o );
  virtual ~BtaSimpleVertex();
  virtual BtaAbsVertex* clone() const;
  virtual TLorentzVector v4() const; 
  virtual TMatrixD& covariance () const;
  virtual Double_t chiSquared() const;
  virtual Int_t nDof() const;

  ClassDef(BtaSimpleVertex,1) // The simpliest subclass of BtaAbsVertex
};

#endif

