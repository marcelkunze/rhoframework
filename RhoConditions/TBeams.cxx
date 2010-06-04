//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TBeams	    							//
// TBeamSpotCal	    							//
// TEnergies	    							//
// TFillPattern	    							//
// TBoostCal								//
//                                                                      //
// Beam calibration classes						//
//                                                                      //
// Author: Marcel Kunze, RUB, Nov. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TLorentzVector.h"
#include "TLorentzRotation.h"

#include "RhoConditions/TBeams.h"


ClassImp(PepBeamsR)

#include <iostream>
using namespace std;

PepBeamsR::PepBeamsR() { }
PepBeamsR::~PepBeamsR() { }

void PepBeamsR::PrintOn(std::ostream& o) const {
    
    // The interaction point:
    o << endl << "Interaction point: " ;
    o << _interactionPoint[0] << '\t' <<
 	 _interactionPoint[1] << '\t' <<
 	 _interactionPoint[2] << '\t' <<
	 _interactionPoint[3] << endl;
/*    
    o << endl << "Error matrix interaction point:";
    for (int i = 0; i < 4; ++i){
	o << endl;
	for (int j = 0; j < 4; ++j){
	    int ij = i * 4 + j;
	    o << _interactionPointErr[ij] << '\t';
	}
    }
*/
    o << endl;
    
    // The e+ momentum:
    
    o << "Positron beam: " ;
    o << _ePlusMomentum[0] << '\t' <<
 	 _ePlusMomentum[1] << '\t' <<
	 _ePlusMomentum[2] << endl;
    o << "Error: " ;
    o << _ePlusMomentumErr[0] << '\t' <<
	 _ePlusMomentumErr[4] << '\t' <<
	 _ePlusMomentumErr[8] << endl;
    
    // The e- momentum:
    
    o << "Electron beam: " ;
    o << _eMinusMomentum[0] << '\t' <<
	 _eMinusMomentum[1] << '\t' <<
	 _eMinusMomentum[2] << endl;
    o << "Error: " ;
    o << _eMinusMomentumErr[0] << '\t' <<
	 _eMinusMomentumErr[4] << '\t' <<
	 _eMinusMomentumErr[8] << endl;
        
    //o << endl << endl << "Number of bunches (empty): ";
    //o << _numBunches << " (" << _numEmptyBunches << ")" << endl;
}

std::ostream&  operator << (std::ostream& o, const PepBeamsR& a) { a.PrintOn(o); return o; }


// Class TBeams

ClassImp(TBeams)

TBeams::TBeams(PepEnergiesR &energies,PepBeamSpotCalR &cal)
{
    Update(energies,cal);
}

void TBeams::Update(PepEnergiesR &energies,PepBeamSpotCalR &cal) 
{
  Double_t ePlusEnergy = energies.LerEnergy();
  Double_t eMinusEnergy = energies.HerEnergy();
  SetDefaultMomenta(ePlusEnergy,eMinusEnergy);

  static Double_t oldIP = 0;

  TVector3 ip = cal.BeamSpot();
  if (ip.Mag() == oldIP) return; // Nothing to do
  oldIP = ip.Mag();

  _interactionPoint[0] = ip.X();
  _interactionPoint[1] = ip.Y();
  _interactionPoint[2] = ip.Z();
  _interactionPoint[3] = 2.0 * sqrt(ePlusEnergy * eMinusEnergy);
  _interactionPointErr[0]  = cal.OrientationXX();
  _interactionPointErr[1]  = cal.OrientationXY();
  _interactionPointErr[2]  = cal.OrientationXZ();
  _interactionPointErr[4]  = cal.OrientationXY();
  _interactionPointErr[5]  = cal.OrientationYY();
  _interactionPointErr[6]  = cal.OrientationYZ();
  _interactionPointErr[8]  = cal.OrientationXZ();
  _interactionPointErr[9]  = cal.OrientationYZ();
  _interactionPointErr[10] = cal.OrientationZZ();
}

TLorentzVector 
TBeams::InteractionPoint() {
    return TLorentzVector(_interactionPoint[0],_interactionPoint[1],_interactionPoint[2],_interactionPoint[3]);
}

TVector3 
TBeams::Interaction3Point() {
    return TVector3(_interactionPoint[0],_interactionPoint[1],_interactionPoint[2]);
}

TVector3
TBeams::EPlusMomentum() {
    return TVector3(_ePlusMomentum[0],_ePlusMomentum[1],_ePlusMomentum[2]); 
}    

TVector3
TBeams::EMinusMomentum() {
    return TVector3(_eMinusMomentum[0],_eMinusMomentum[1],_eMinusMomentum[2]);
}    

Double_t TBeams::EPlusEnergy() { return ElectronEnergy(EPlusMomentum()); }
Double_t TBeams::EMinusEnergy() { return ElectronEnergy(EMinusMomentum()); }

//----------------------------------------------------------------------
// Given a momentum, return the energy of an electron with that momentum:

Double_t 
TBeams::ElectronEnergy(TVector3 momentum) {
    static Double_t electronMass =  0.000510999;
    static const Double_t electronMassSquared = electronMass * electronMass;
    return sqrt(electronMassSquared + momentum.Mag2());
}


//-----------------------------------------------------------------
TLorentzVector 
TBeams::EMinus4Momentum() {
    return TLorentzVector(EMinusMomentum(), EMinusEnergy());
}


//-----------------------------------------------------------------
TLorentzVector 
TBeams::EPlus4Momentum() {
    return TLorentzVector(EPlusMomentum(), EPlusEnergy());
}

//-----------------------------------------------------------------
TLorentzVector
TBeams::Total4Momentum() {
    return EPlus4Momentum() + EMinus4Momentum();
}

//-----------------------------------------------------------------
TLorentzVector
TBeams::Total4MomentumCM() {
    TLorentzVector result = Total4Momentum();
    TLorentzRotation boost(-result.BoostVector());
    result.Transform(boost);
    return result;
}

//--------------------------------------------------------------------------
TVector3 
TBeams::TotalMomentum() {
    return (const TVector3 &)EMinusMomentum() + 
	(const TVector3 &)EPlusMomentum();
}

//-----------------------------------------------------------------
Double_t 
TBeams::CrossingAngle() {
    return EPlusMomentum().Angle(EMinusMomentum());
}

//-----------------------------------------------------------------
TVector3
TBeams::BoostCMtoLab() {  
    return Total4Momentum().BoostVector();
}


//-----------------------------------------------------------------
Double_t
TBeams::EnergyCM() {
    return Total4MomentumCM().T();
}


//-------------------------------------------------------------------
void
TBeams::SetDefaultMomenta(Double_t ePlusEnergy, Double_t eMinusEnergy) {

  // Check cache
  static Double_t oldPlus = 0, oldMinus = 0;
  if (oldPlus==ePlusEnergy && oldMinus==eMinusEnergy) return;

  oldPlus  = ePlusEnergy;
  oldMinus = eMinusEnergy;

  // The rotation in the xz plane:
  const TVector3 yAxis(0.0, 1.0, 0.0);
  TRotation beamRotation;
  beamRotation.Rotate(-0.0188, yAxis);

  // Rotate the momenta:
  TVector3 ePlus(0,0,-ePlusEnergy);
  ePlus.Transform(beamRotation);

  TVector3 eMinus(0,0,eMinusEnergy);
  eMinus.Transform(beamRotation);

  // Set the momenta:
  _ePlusMomentum[0] = ePlus.X();
  _ePlusMomentum[1] = ePlus.Y();
  _ePlusMomentum[2] = ePlus.Z();
  _ePlusMomentumErr[0] = pow(0.0013, 2);
  _ePlusMomentumErr[4] = pow(0.0013, 2);
  _ePlusMomentumErr[8] = pow(0.0024, 2);

  _eMinusMomentum[0] = eMinus.X();
  _eMinusMomentum[1] = eMinus.Y();
  _eMinusMomentum[2] = eMinus.Z();
  _eMinusMomentumErr[0] = pow(0.0028, 2);
  _eMinusMomentumErr[4] = pow(0.0028, 2);
  _eMinusMomentumErr[8] = pow(0.0055, 2);

}

TVectorErr   TBeams::EPlusMomentumWErr()
{
  TError pErr(3);
  int i, j;
  for (i = 0; i < 3; ++i){
    for (j = 0; j < 3; ++j){
      pErr(i,j) = EPlusMomentumErr(i,j);
    }
  }
  return TVectorErr(EPlusMomentum(),pErr);
}

TVectorErr   TBeams::EMinusMomentumWErr()
{
  TError pErr(3);
  int i, j;
  for (i = 0; i < 3; ++i){
    for (j = 0; j < 3; ++j){
      pErr(i,j) = EMinusMomentumErr(i,j);
    }
  }
  return TVectorErr(EMinusMomentum(),pErr);
}

TVectorErr   TBeams::TotalMomentumWErr()
{
  return EMinusMomentumWErr() + EPlusMomentumWErr();
}

TLorentzVectorErr TBeams::Total4MomentumWErr()
{
  return EPlus4MomentumWErr() + EMinus4MomentumWErr();
}

TLorentzVectorErr TBeams::Total4MomentumCMWErr() 
{
  TLorentzVectorErr result = Total4MomentumWErr();
  TLorentzRotation boost(-result.BoostVector());
  result.Transform(boost);
  return result;
}

TDoubleErr        TBeams::EPlusEnergyWErr()
{
  TDoubleErr result;
  const double value = ElectronEnergy(_ePlusMomentum);
  result.SetValue(value);

  if (value != 0){
    double cov = 0.0;
    TError momErr = EPlusMomentumWErr().CovMatrix();
    for (int i = 0; i < 3; ++i){
      for (int j = 0; j < 3; ++j){
	cov += EPlusMomentum()[i] * EPlusMomentum()[j] * momErr(i, j);
      }
    }
    cov /= (value * value);
    result.SetCovariance(cov);
  }
  return result;
}

TDoubleErr        TBeams::EMinusEnergyWErr()
{
  TDoubleErr result;
  const double value = ElectronEnergy(_eMinusMomentum);
  result.SetValue(value);

  if (value != 0){
    double cov = 0.0;
    TError momErr = EMinusMomentumWErr().CovMatrix();
    for (int i = 0; i < 3; ++i){
      for (int j = 0; j < 3; ++j){
	cov += EMinusMomentum()[i] * EMinusMomentum()[j] * momErr(i, j);
      }
    }
    cov /= (value * value);
    result.SetCovariance(cov);
  }
  return result;
}

TLorentzVectorErr TBeams::EPlus4MomentumWErr()
{
  // The 3-vector part of the error, initialized as a 0-matrix, then copied 
  // from p3:
  TError p4Err(4);
  int i, j;
  for (i = 0; i < 3; ++i){
    for (j = 0; j < 3; ++j){
      p4Err(i,j) = EPlusMomentumErr(i,j);
    }
  }
  
  // The energy part of the error:
  p4Err(3,3) = EPlusEnergyWErr().Covariance();
  
  // The E-p correlated part of the error is
  // <(E - <E>) (Pi - <Pi>)> = dE/dPj Vjk dPi/dPk
  // Since dPi/dPk = delta_ik, we get dE/dPj Vji.
  const double ePlusE = EPlusEnergy();
  for (i = 0; i < 3; ++i){
    for (j = 0; j < 3; ++j){
      p4Err(i,3) += EPlusMomentum()[j] / ePlusE * 
	EPlusMomentumErr(j,i);
    }
  }

  return TLorentzVectorErr(EPlus4Momentum(), p4Err);
}


TLorentzVectorErr TBeams::EMinus4MomentumWErr() 
{
  // The 3-vector part of the error, initialized as a 0-matrix, then copied 
  // from p3:
  TError p4Err(4);
  int i, j;
  for (i = 0; i < 3; ++i){
    for (j = 0; j < 3; ++j){
      p4Err(i,j) = EMinusMomentumErr(i,j);
    }
  }
  
  // The energy part of the error:
  p4Err(3,3) = EMinusEnergyWErr().Covariance();
  
  // The E-p correlated part of the error is
  // <(E - <E>) (Pi - <Pi>)> = dE/dPj Vjk dPi/dPk
  // Since dPi/dPk = delta_ik, we get dE/dPj Vji.
  const double eMinusE = EMinusEnergy();
  for (i = 0; i < 3; ++i){
    for (j = 0; j < 3; ++j){
      p4Err(i,3) += EMinusMomentum()[j] / eMinusE * 
	EMinusMomentumErr(j,i);
    }
  }

  return TLorentzVectorErr(EMinus4Momentum(), p4Err);
}

TDoubleErr        TBeams::EnergyCMWErr()
{
  TLorentzVectorErr p4CM = Total4MomentumCMWErr();
  double value = p4CM.T();
  double err = p4CM.CovMatrix()(3,3);

  return TDoubleErr(value, err);
} 

TVectorErr        TBeams::BoostCMtoLabWErr()
{
  TLorentzVectorErr p4 = Total4MomentumWErr();

  // The value part of the result:
  TVector3 value = p4.BoostVector();  // = p/E

  // The error part, given by (B is the boost vector, P is the 4-momentum,
  // i,j = 1,3, m,n = 1,4, Vmn is the 4-momentum error matrix):
  // <(Bi - <Bi>) (Bj - <Bj>)> = sum_{m,n} dBi/dPm Vmn dBj/dPn
  // where dBi/dPk = delta_{ik} / E
  //       dBi/dE = -Pi / E^2
  const double Esq = p4.T() * p4.T();
  const double Ecu = Esq * p4.T();

  TError err(3);
  int i, j;
  for (i = 0; i < 3; ++i){
    for (j = i; j < 3; ++j){    // start from i since (i,j) = (j,i)
      err(i,j) =                // m=n=0:
	(p4(i) / Esq) * (p4(j) / Esq) * p4.CovMatrix()(3,3)
	                        // m=0, n=k leads to delta_{jk}:
	- (p4(i) / Ecu) * p4.CovMatrix()(3,j)
	                        // m=k, n=0 leads to delta_{ik}:
	- (p4(j) / Ecu) * p4.CovMatrix()(i,3)
	                        // m=k, n=l, leads to delta_{ik} delta{jl}:
	+ p4.CovMatrix()(i,j) / Esq;
    }
  }
  
  return TVectorErr(value, err);
} // from CM frame to lab  

//-----------------------------------------------------------------
ClassImp(PepBeamSpotCalR)
ClassImp(TBeamSpotCal)

//static const char rcsid[] =
//"$Id: TBeams.cxx,v 1.2 2001-12-17 17:48:01 Marcel Exp $";

PepBeamSpotCalR::PepBeamSpotCalR() {
}

PepBeamSpotCalR::~PepBeamSpotCalR() { }

void
PepBeamSpotCalR::PrintOn(std::ostream & o) const {
	o << "beamSpot " << _x << " " << _y << " " << _z << endl;
	o << "background " << Background() << " numTracks " << NumTracks() << endl;
	//o << "errorMatrix " << errorMatrix() << endl;
}

const Double_t& PepBeamSpotCalR::ErrorMatrix(int row, int col) const {
    if (row < col){
	int temp = row; row = col; col = temp;
    }
    int index = row * (row - 1) / 2 + col - 1;
    return _errorMatrix[index];
}

std::ostream&  operator << (std::ostream& o, const PepBeamSpotCalR& a) { a.PrintOn(o); return o; }

//-----------------------------------------------------------------
ClassImp(PepEnergiesR)
ClassImp(TEnergies)

//static const char rcsid[] =
//"$Id: TBeams.cxx,v 1.2 2001-12-17 17:48:01 Marcel Exp $";

PepEnergiesR::PepEnergiesR() {
}

PepEnergiesR::~PepEnergiesR() { }

void
PepEnergiesR::PrintOn(std::ostream & stream) const {
    stream << "lerEnergy " << _lerEnergy << " herEnergy " << _herEnergy;
}

std::ostream&  operator << (std::ostream& o, const PepEnergiesR& a) { a.PrintOn(o); return o; }

//-----------------------------------------------------------------

ClassImp(PepFillPatternR)
ClassImp(TFillPattern)

//static const char rcsid[] =
//"$Id: TBeams.cxx,v 1.2 2001-12-17 17:48:01 Marcel Exp $";

PepFillPatternR::PepFillPatternR() {
}

PepFillPatternR::~PepFillPatternR() { }

void
PepFillPatternR::PrintOn(std::ostream & stream) const {
    stream << "herFirstBucket " << HerFirstBucket()
	<< " herLastBucket " << HerLastBucket()
	<< " herStepSize " << HerStepSize() << endl
	<< "lerFirstBucket " << LerFirstBucket()
	<< " lerLastBucket " << LerLastBucket()
	<< " lerStepSize " << LerStepSize();
}

std::ostream&  operator << (std::ostream& o, const PepFillPatternR& a) { a.PrintOn(o); return o; }

//-----------------------------------------------------------------

ClassImp(PepBoostCalR)
ClassImp(TBoostCal)

PepBoostCalR::PepBoostCalR() {
  _numEvents= 2000;
  _betaX= -0.0091;
  _betaY= 0.0;
  _betaZ= 0.48508;
  _errXX= 3.0e-08;
  _errXY= 9.8e-10;
  _errXZ= -2.8e-09;
  _errYY= 2.9e-08;
  _errYZ= 1.7e-09;
  _errZZ= 2.0e-07;
}

// The boost from the CM to the lab and its error matrix:
TVector3 PepBoostCalR::Beta() const {return TVector3(_betaX,_betaY,_betaZ); }

// # of events used in the caliubration:
int PepBoostCalR::NumEvents() const {return _numEvents;}

PepBoostCalR::~PepBoostCalR() { }

void PepBoostCalR::PrintOn(std::ostream &o) const {
  o << " beta " << Beta().X() << '\t' << Beta().Y() << '\t' << Beta().Z() << endl;
  o << " numEvents " << NumEvents() << endl;
}

std::ostream&  operator << (std::ostream& o, const PepBoostCalR& a) { a.PrintOn(o); return o; }

