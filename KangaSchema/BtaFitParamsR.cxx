//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaFitParamsR.cxx,v 1.3 2002-02-01 03:47:40 marcel Exp $
//
// Description:
//      Class BtaFitParamsR
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Yury Kolomensky
//
// Copyright Information:
//	Copyright (C) 1999         Caltech
//
//------------------------------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------
#include "KangaSchema/BtaFitParamsR.h"

//-------------
// C Headers --
//-------------
#include <assert.h>

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "RhoBase/TFitParams.h"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp(BtaFitParamsR)

#include <iostream>
using namespace std;

//----------------
// Constructors --
//----------------
BtaFitParamsR::BtaFitParamsR() : 
  _q( 0 ) {

  int i= 0;
  for( i= 0; i < 3; i++ ) _pos[i]= 0;
  for( i= 0; i < 4; i++ ) _p4[i]= 0;
  for( i= 0; i < 28; i++ ) _cov7[i]= 0;

}
/*
BtaFitParamsR::BtaFitParamsR( const TFitParams & transient )
  : _q( transient.charge() ) {

  _pos[0]= transient.Pos().X();
  _pos[1]= transient.Pos().Y();
  _pos[2]= transient.Pos().Z();

  _p4[0]= transient.P4().X();
  _p4[1]= transient.P4().Y();
  _p4[2]= transient.P4().Z();
  _p4[3]= transient.P4().T();

  assert( transient.Cov7().GetNrows() == 7 );

  int i= 0;
  for( int row= 0; row < 7; row++ ) {
    for( int col= 0; col < row; col++ ) {
      _cov7[i]= transient.Cov7()( row, col );
      i++;
    }
  }

}
*/

//--------------
// Destructor --
//--------------

BtaFitParamsR::~BtaFitParamsR() {}

//--------------------
// Member Functions --
//--------------------
TFitParams* BtaFitParamsR::Transient() const {

  double charge( _q );
  TVector3 pos( _pos[0], _pos[1], _pos[2] );
  TLorentzVector p4( _p4[0], _p4[1], _p4[2], _p4[3] );

  TMatrixD cov7( 7,7 );
  int row=7;
  int col=7;
  for( int i= 27; i >= 0; i-- ) {
    cov7( row-1, col-1 )=cov7( col-1, row-1 )= _cov7[i];
    col--;
    if( col < 1 ) {
      row--;
      col= row;
    }
  }

  TFitParams* transient = new TFitParams( charge, pos, p4, cov7 );
  return transient;

}
/*  
void BtaFitParamsR::PrintOn( std::ostream & ost ) const {

  ost << "Charge: " << _q << endl;
  ost << "Position: " << _pos[0] << " " << _pos[1] << " " << _pos[2] << endl;
  ost << "4-mom: " << _p4[0] << " " << _p4[1] << " " << _p4[2] << " "
      << _p4[3] << endl;

}
*/