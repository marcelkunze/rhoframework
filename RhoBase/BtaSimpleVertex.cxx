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

#include "RhoBase/BtaSimpleVertex.hh"

ClassImp(BtaSimpleVertex)

#include <iostream>
using namespace std;

BtaSimpleVertex::BtaSimpleVertex() 
  : fV(), fT(0), fNDof(0), fChiSquared(999.)
{}

BtaSimpleVertex::BtaSimpleVertex( const TVector3& v, double t, int ndof, double c) 
  : fV(v), fT(t), fNDof(ndof), fChiSquared(c)
{}

BtaSimpleVertex::BtaSimpleVertex( double x, double y, double z, double t, int ndof, double c )
  : fV( x, y, z ), fT(t), fNDof(ndof), fChiSquared(c)
{}

BtaSimpleVertex::BtaSimpleVertex( const BtaSimpleVertex& o )
  : BtaAbsVertex( o ), fV(o.fV), fT(o.fT), fNDof(o.fNDof), fChiSquared(o.fChiSquared)
{}

BtaSimpleVertex::~BtaSimpleVertex()
{
}

BtaAbsVertex* BtaSimpleVertex::clone() const
{
    return new BtaSimpleVertex();
}

TLorentzVector 
BtaSimpleVertex::v4() const 
{ 
  return TLorentzVector( fV.X(), fV.Y(), fV.Z(), fT ); 
}

TMatrixD& 
BtaSimpleVertex::covariance() const 
{ 
  // set large errors
  static TMatrixD dum(3,3);
  int i,j;
  for (i = dum.GetRowLwb(); i <= dum.GetRowUpb(); i++)
    for(j = dum.GetColLwb(); j <= dum.GetColUpb(); j++)
       dum(i,j) = 1.0;
  dum(1,1)=1.e-04;
  dum(2,2)=1.e-04;
  dum(3,3)=1.e-04;
  return dum; 
}

double 
BtaSimpleVertex::chiSquared() const 
{ 
  return fChiSquared; 
}

int 
BtaSimpleVertex::nDof() const 
{ 
  return fNDof; 
}

		       
