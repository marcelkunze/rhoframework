//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaFitParamsR_002.cxx,v 1.3 2002-02-01 03:47:41 marcel Exp $
//
// Description:
//      Class BtaFitParamsR_002
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Yury Kolomensky
//      Leif Wilden
//
// Copyright Information:
//	Copyright (C) 1999         Caltech
//
//------------------------------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------
#include "KangaSchema/BtaFitParamsR_002.h"
#include "RhoBase/TFitParams.h"
#include "KangaSchema/ComPackFlatFloat.h"
#include "KangaSchema/ComPackNonFlatFloat.h"
#include "RhoMath/TError.h"

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

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp(BtaFitParamsR_002)

#include <iostream>
using namespace std;

//----------------
// Constructors --
//----------------

BtaFitParamsR_002::BtaFitParamsR_002() : 
  _q( 0 ) {

  int i= 0;
  for( i= 0; i < 5; i++ ) _params[i]= 0;
  for( i= 0; i < 15; i++ ) _diag[i]= 0;
  for( i= 0; i < 10; i++ ) _corr[i]= 0;

}
/*
BtaFitParamsR_002::BtaFitParamsR_002( const BtaCandidate & transient )
  : _q( transient.charge() ) {

  int i= 0;
  for( i= 0; i < 5; i++ ) _params[i]= 0;
  for( i= 0; i < 15; i++ ) _diag[i]= 0;
  for( i= 0; i < 10; i++ ) _corr[i]= 0;

  if(fabs(_q)>0.1){// helices
    TrkExchangePar par(transient.recoTrk()->fitResult()->helix(0));
    storeRotation(par.params(),par.covariance());
    
  }else{ // Neutrals
    
    HepVector v(4);
    v[0]=transient.fitParams().pos().x();
    v[1]=transient.fitParams().pos().y();
    v[2]=transient.fitParams().pos().z();
    v[3]=transient.fitParams().e();
    const HepSymMatrix& cov7=transient.fitParams().cov7();
    HepSymMatrix m(4);
    for(int j=0;j<3;j++){
      for(int k=0;k<=j;k++)m[j][k]=cov7[j][k];
      m[j][3]=cov7[j][6];
    }
    m[3][3]=cov7[6][6];
    store(v,m);
  }

}
*/

//--------------
// Destructor --
//--------------

BtaFitParamsR_002::~BtaFitParamsR_002() {}

//--------------------
// Member Functions --
//--------------------

TFitParams* BtaFitParamsR_002::Transient() const {

  double charge(_q);
  Float_t *par = (Float_t *) _params;
  Float_t *cov = 0;

  if(fabs(charge)<0.1){
      return new TFitParams(charge,par,cov);
  }else{
      cov = LoadRotation();
      return new TFitParams(charge,par,cov);
  }

}
/*  
void BtaFitParamsR_002::print( std::ostream & ost ) const {

  ost << "Charge: " << _q << endl;
  if (_q==0)
    ost << "Cluster Position, Energy: (" << _params[0] << ", " << _params[1] << ", " << _params[2] 
        << ") E= " << _params[3] << endl;
  else
    ost << "Track Parameters: " << _params[0] << ", " << _params[1] << ", " << _params[2] << ", "
        << _params[3] << ", " << _params[4] << endl;

}
*/
// This code reads the schema 4 covariance matrices

class ComFitRangePack{
private:
  double ranges[5][5];
public:
  ComFitRangePack(int chge){
    if(chge == 0){
      for(int j=0;j<4;j++)ranges[j][j]=1e3;
    }else{
      ranges[0][0]=10;
      ranges[1][1]=1e-3;
      ranges[2][2]=1e-4;
      ranges[3][3]=1e3;
      ranges[4][4]=0.1;
    }
  }
  double range(int i,int j){return ranges[i][j];}
};


Float_t*
BtaFitParamsR_002::LoadRotation() const {
    ComPackNonFlatFloat packer(ComPackNonFlatFloat::UNSIG);
    //  ComPackNonFlatFloat packer8(ComPackNonFlatFloat::SIG);
    ComPackFlatFloat packer8(-1,1,15);
    TVectorD eigenValues(5),angles(10);
    for(Int_t j=0;j<nParm();j++){
	if ( ! packer.unpackAndScream (0,10,_diag[j],eigenValues(j),"DiagCov ") ) {
	    cerr << " loadRotation: problems unpacking " << "DiagCov " << j << endl ;
	}
	if(eigenValues(j)<=0)eigenValues(j)=packer.getMinVal();
    }
    Int_t nc = nCorr();
    for(Int_t j2=0;j2<nc;j2++) {
	// if ( ! packer8.unpackAndScream (-1.1,1.1,_diag[nParm()+j2],angles(j2),"Corr") ) {
	//   cerr << " loadRotation: problems unpacking " << "Corr" << j << j2 << endl ;
	// }
	packer8.unpack(_diag[nParm()+j2],angles(j2));
    }
    
    return ComPackedCovariance(eigenValues,angles);
}

Float_t* BtaFitParamsR_002::ComPackedCovariance(const TVectorD& eigenValues,const TVectorD& angles) const
{
    static Float_t _cov[15];
    int nDim=5;
    assert (eigenValues.GetNrows() == nDim);
    assert (angles.GetNrows() == 2*nDim);
    // restore the diagonal part
    TError diag(nDim);
    diag.Zero();
    for(int l=0;l<nDim;l++)diag(l,l)=eigenValues(l);
    // build up the rotation
    TMatrixD rot(nDim,nDim);
    rot = 1.;
    for(int i=nDim-1;i>0;i--){
	for(int j=i+1;j<=nDim;j++){
	    double phi=angles( j*(j-1)/2-i)*TMath::Pi();
	    double co = TMath::Cos(phi);
	    double si = TMath::Sin(phi);
	    for (int k=1;k<=nDim;k++){
		double a1 = co*rot(k-1,j-1)-si*rot(k-1,j-1);
		double a2 = si*rot(k-1,j-1)+co*rot(k-1,j-1);
		rot(k-1,j-2) = a1;
		rot(k-1,j-1) = a2;
	    }
	}
    }

    //HepSymMatrix result(diag.similarity(rot));
    TMatrixD result(diag.Similarity(rot));
    
    int n= 0;
    for( int row= 1; row < 6; row++ ) {
	for( int col= 1; col <= row; col++ ) {
	    _cov[n]= result( row-1, col-1 );
	    n++;
	}
    }

    return _cov;
    
}
