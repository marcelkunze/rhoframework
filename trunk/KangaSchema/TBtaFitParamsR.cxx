//--------------------------------------------------------------------------
//
// Description:
//      Class TBtaFitParamsR
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Yury Kolomensky
//      Leif Wilden
//      M. Kunze, added support for Kanga schema >=3, Dec. 2k
//
// Copyright Information:
//	Copyright (C) 1999         Caltech, TU Dresden
//
//------------------------------------------------------------------------
//#define SCHEMA3

#include <assert.h>
#include "RhoBase/TFitParams.h"
#include "KangaSchema/TBtaFitParamsR.h"
#include "KangaSchema/kanga.h"
#include "KangaSchema/ComPackFlatFloat.h"
#include "KangaSchema/ComPackNonFlatFloat.h"
#include "RhoMath/TError.h"

#ifdef SCHEMA3
#include "PAFAdapters/PAFConverter.h"
#endif

ClassImp(TBtaFitParamsR)

TBuffer &operator>>(TBuffer &buf, TBtaFitParamsR *&obj)
{
   obj = (TBtaFitParamsR *) buf.ReadObject(TBtaFitParamsR::Class());
   return buf;
}

#include <iostream>
using namespace std;

TBtaFitParamsR::TBtaFitParamsR()
{
    Reset();
}

TBtaFitParamsR::~TBtaFitParamsR() {}

void TBtaFitParamsR::Reset()
{
    _q = _qold = 0;
    Int_t i= 0;
    for( i= 0; i < 3; i++ ) _pos[i]= 0;
    for( i= 0; i < 4; i++ ) _p4[i]= 0;
    for( i= 0; i < 28; i++ ) _cov7[i]= 0;
    for( i= 0; i < 5; i++ ) _params[i]= 0;
    for( i= 0; i < 15; i++ ) _cov[i]= 0;
    for( i= 0; i < 15; i++ ) _diag[i]= 0;
    for( i= 0; i < 10; i++ ) _corr[i]= 0;
}

// Getters:

Int_t TBtaFitParamsR::GetQ() const
{ 
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_q);
    return _q;
}

void TBtaFitParamsR::GetPos(Float_t pos[3]) const
{
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_pos);
    pos[0]= _pos[0];
    pos[1]= _pos[1];
    pos[2]= _pos[2];
}

void TBtaFitParamsR::GetP4(Float_t p4[4]) const
{
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_p4);
    p4[0]= _p4[0];
    p4[1]= _p4[1];
    p4[2]= _p4[2];
    p4[3]= _p4[3];
}

void TBtaFitParamsR::GetCov7(Float_t cov7[28]) const
{
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_cov7);
    
    //change from line to block arrangement
    //position
    cov7[0]=_cov7[0];
    cov7[1]=_cov7[1]; cov7[2]=_cov7[2];
    cov7[3]=_cov7[3]; cov7[4]=_cov7[4]; cov7[5]=_cov7[5];
    
    //momentum-position
    cov7[6]=_cov7[6];    cov7[7]=_cov7[7];   cov7[8]=_cov7[8];
    cov7[9]=_cov7[10];  cov7[10]=_cov7[11]; cov7[11]=_cov7[12];
    cov7[12]=_cov7[15]; cov7[13]=_cov7[16]; cov7[14]=_cov7[17];
    cov7[15]=_cov7[21]; cov7[16]=_cov7[22]; cov7[17]=_cov7[23];
    
    //momentum4
    cov7[18]=_cov7[9];
    cov7[19]=_cov7[13];  cov7[20]=_cov7[14];
    cov7[21]=_cov7[18];  cov7[22]=_cov7[19];  cov7[23]=_cov7[20];
    cov7[24]=_cov7[24];  cov7[25]=_cov7[25];  cov7[26]=_cov7[26];  cov7[27]=_cov7[27];
}

void TBtaFitParamsR::GetParams(Float_t p[5]) const
{
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_params);
    p[0]= _params[0];
    p[1]= _params[1];
    p[2]= _params[2];
    p[3]= _params[3];
    p[4]= _params[4];
}

void TBtaFitParamsR::GetCov(Float_t cov[15]) const
{
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_cov);
    for (Int_t i=0; i<15; i++) cov[i]= _cov[i];
}

// Here is the faster, non-const interface

TVector3 TBtaFitParamsR::GetPos()
{
    if (TBtaMicroCandRDir::GetSchema()<3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_pos);
	return TVector3(_pos[0],_pos[1],_pos[2]);
    }
    
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_q);
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_params);
    
    return GetTFitParams(_q,_params,0)->GetPosition(); // Do not propagate error matrix
}

TVector3 TBtaFitParamsR::GetP()
{
    if (TBtaMicroCandRDir::GetSchema()<3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_p4);
	return TVector3(_p4[0],_p4[1],_p4[2]);
    }
    
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_q);
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_params);
    
    return GetTFitParams(_q,_params,0)->P4().Vect(); // Do not propagate error matrix
}

TLorentzVector TBtaFitParamsR::GetP4()
{
    if (TBtaMicroCandRDir::GetSchema()<3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_p4);
	return TLorentzVector(_p4[0],_p4[1],_p4[2],_p4[3]);
    }
    
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_q);
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_params);
    
    return GetTFitParams(_q,_params,0)->P4(); // Do not propagate error matrix
}

Float_t* TBtaFitParamsR::GetCov7()
{
    if (TBtaMicroCandRDir::GetSchema()<3) {
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_cov7);
	//change from line to block arrangement
	//position
	fCov[0]=_cov7[0];
	fCov[1]=_cov7[1]; fCov[2]=_cov7[2];
	fCov[3]=_cov7[3]; fCov[4]=_cov7[4]; fCov[5]=_cov7[5];
	
	//momentum-position
	fCov[6]=_cov7[6];   fCov[7]=_cov7[7];   fCov[8]=_cov7[8];
	fCov[9]=_cov7[10];  fCov[10]=_cov7[11]; fCov[11]=_cov7[12];
	fCov[12]=_cov7[15]; fCov[13]=_cov7[16]; fCov[14]=_cov7[17];
	fCov[15]=_cov7[21]; fCov[16]=_cov7[22]; fCov[17]=_cov7[23];
	
	//momentum4
	fCov[18]=_cov7[9];
	fCov[19]=_cov7[13];  fCov[20]=_cov7[14];
	fCov[21]=_cov7[18];  fCov[22]=_cov7[19];  fCov[23]=_cov7[20];
	fCov[24]=_cov7[24];  fCov[25]=_cov7[25];  fCov[26]=_cov7[26];  fCov[27]=_cov7[27];
	
	return fCov;
    }
    
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_q);
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_params);
    
    GetCov(); // Prepare the cov. matrix
    
    // Use the BABAR covariance for charged until the problems
    // with the charged covariance matrix are resolved...
#ifdef SCHEMA3
    if (fabs(_q)>.1)
        return PAFConverter::Instance()->KangaToCovariance(_q,_params,_cov);
    else
#endif
        return GetTFitParams(_q,_params,_cov)->GetErrP7(); // Propagate error matrix
}

// Setters:

void TBtaFitParamsR::SetQ(Int_t q) 
{ _q= q; }

void TBtaFitParamsR::SetPos(const Float_t pos[3])
{ _pos[0]= pos[0]; _pos[1]= pos[1]; _pos[2]= pos[2]; }

void TBtaFitParamsR::SetP4(const Float_t p4[4]) 
{ _p4[0]= p4[0]; _p4[1]= p4[1]; _p4[2]= p4[2]; _p4[3]= p4[3]; }

void TBtaFitParamsR::SetCov7(const Float_t cov7[28])
{
    for (Int_t i=0; i<28; i++) _cov7[i]= cov7[i];
}

void TBtaFitParamsR::SetParams(const Float_t p[5]) 
{ _params[0]= p[0]; _params[1]= p[1]; _params[2]= p[2]; _params[3]= p[3]; _params[4]= p[4]; }

Float_t* TBtaFitParamsR::GetParams()
{
    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_params);
    return _params;
}

void TBtaFitParamsR::SetCov(const Float_t cov[15])
{
    for (Int_t i=0; i<15; i++) _cov[i]= cov[i];
}

Float_t* TBtaFitParamsR::GetCov()
{
    // Unpack the data (charged only)
    if (TBtaMicroCandRDir::GetSchema()>3) {
	if (fabs((double)_q)>.1) {
	    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_diag);
	    TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_corr);
	    LoadRotation();
	}
	//Neutrals
	else {
	   TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_diag);
	   Load();
	}
    }
    else
	TBtaMicroCandRDir::Instance()->ReadBranch(TBtaMicroCandRDir::fp_cov);
    
    return _cov;
}

// Parameter cache function
// The expensive covariance is only calculated if an array _cov is passed along

TFitParams* TBtaFitParamsR::GetTFitParams(Int_t _q, Float_t *_params, Float_t *_cov) 
{
    static TFitParams *transient = new TFitParams(); // This is the cache
    static Double_t oldParams = 0.0;
    static Bool_t cachedCov = kFALSE;
    Bool_t needCov = (!cachedCov && _cov!=0);
    
    // Check valididty of cache
    
    Double_t newParams = _params[0]+_params[1]+_params[2]+_params[3]+_params[4];
    if (newParams==oldParams&&!needCov) return transient;
    oldParams = newParams;
    cachedCov = (_cov!=0);
    
    // Generate a new object as requested    
    
    new (transient) TFitParams(_q,_params,_cov);
    
    return transient;
}

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

void
TBtaFitParamsR::Load() {
    TMatrixD covariance(nParm(),nParm());
    ComFitRangePack ranges(_q);
    ComPackNonFlatFloat packer(ComPackNonFlatFloat::UNSIG);
    ComPackNonFlatFloat packer8(ComPackNonFlatFloat::SIG_8);
    for(int j=0;j<nParm();j++){
	if ( ! packer.unpackAndScream (0,ranges.range(j,j),_diag[j],covariance(j,j),"DiagCov ") ) {
	    cerr << " load: problems unpacking " << "DiagCov " << j << endl ;
	}
	double variance1 = covariance(j,j);
	//    assert(covariance[j][j]>1e-30);
	if (variance1<1e-30) variance1 = packer.getMinVal();
	for(int j2=0;j2<j;j2++){
	    double variance2 = covariance(j2,j2);
	    if (variance2<1e-30) variance2 = packer.getMinVal();
	    double corr;
	    if ( ! packer8.unpackAndScream (-1.1,1.1,_corr[indexCorr(j,j2)],corr,"Corr") ) {
		cerr << " load: problems unpacking " << "Corr" << j << j2 << endl ;
	    }
	    //      covariance[j][j2]=corr*sqrt(covariance[j][j]*covariance[j2][j2]);
	    covariance(j,j2)=corr*sqrt(variance1*variance2);
	}
    }
    
    int n= 0;
    for( int row= 1; row < 5; row++ ) {
        for( int col= 1; col <= row; col++ ) {
            _cov[n]= covariance( row-1, col-1 );
            n++;
        }
    }

}

void
TBtaFitParamsR::LoadRotation() {
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
    
    ComPackedCovariance(eigenValues,angles);
}

Int_t
TBtaFitParamsR::indexCorr(Int_t j1,Int_t j2) const {
    return (j2<j1) ? j1*(j1+1)/2-j2-1 :j2*(j2+1)/2-j1-1 ; 
}

void TBtaFitParamsR::ComPackedCovariance(const TVectorD& eigenValues,const TVectorD& angles)
{
    int nDim=5;
    assert (eigenValues.GetNrows() == nDim);
    assert (angles.GetNrows() == 2*nDim);
    // restore the diagonal part
    TError diag(nDim);
    diag.Zero();
    for(int l=0;l<nDim;l++)diag(l,l)=eigenValues(l);
    // build up the rotation
    TMatrixD rot(nDim,nDim);
    rot.UnitMatrix();
    for(int i=nDim-1;i>0;i--){
	for(int j=i+1;j<=nDim;j++){
	    double phi=angles( j*(j-1)/2-i)*TMath::Pi();
	    double co = TMath::Cos(phi);
	    double si = TMath::Sin(phi);
	    for (int k=1;k<=nDim;k++){
		double a1 = co*rot(k-1,j-2)-si*rot(k-1,j-1);
		double a2 = si*rot(k-1,j-2)+co*rot(k-1,j-1);
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
    
}
