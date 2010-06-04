//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFTagBase								//
//                                                                      //
// Definition of the TAG database					//
//                                                                      //
// Author: Sascha Berger and Marcel Kunze, RUB, March 1999		//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "RhoBase/VAbsBit.h"
#include "PAFSchema/PAFTagBase.h"

#include "TString.h"
#include "TBranch.h"
#include "TBranchObject.h"
#include "THashList.h"

ClassImp(PAFTagBase)

#include <iostream>
using namespace std;

PAFTagBase::PAFTagBase() :
_cmf1(0),
_cmf2(0),
_cmf3(0),
_cmf4(0),
//####################Analysis bits####################################################
_analysisbits(0),
_analysisbits1(0),
// ################ Beam Information################################################
_cmsEnergy(0),
_xBeamSpot(0),
_yBeamSpot(0),
_zBeamSpot(0),
_xBeamErr(0),
_yBeamErr(0),
_zBeamErr(0),
// ################ Tracking Information################################################
_pTotalCosTh(0),
_pTotalMag(0),
_pTotalPhi(0),
_eTotal(0),
_pTotalMass(0),
_p1CosTheta(0),
_p1Phi(0),
_p1Mag(0),
_p2CosTheta(0),
_p2Phi(0),
_p2Mag(0),
_nTracks(0),
_nGoodTrkLoose(0),
_nGoodTrkTight(0),
_p1EmcCandEnergy(0),
_p2EmcCandEnergy(0),
_p1PidHypo(0),
_p2PidHypo(0),
_totChargeLoose(0),
_totChargeTight(0),
//#####################Lepton information############################################
_leptLoose1hypo(0),
_leptLoose1Charge(0),
_leptLoose1p(0),
_leptLoose2hypo(0),
_leptLoose2Charge(0),
_leptLoose2p(0),
_leptTight1hypo(0),
_leptTight1Charge(0),
_leptTight1p(0),
_leptTight2hypo(0),
_leptTight2Charge(0),
_leptTight2p(0),
//#####################Calorimeter information############################################
_eNeutralCosTh(0),
_eNeutralPhi(0),
_eNeutralMag(0),
_e1CosTheta(0),
_e1Phi(0),
_e1Mag(0),
_e2CosTheta(0),
_e2Phi(0),
_e2Mag(0),
//#####################Vertex and event shape#############################################
_xPrimaryVtx(0),
_yPrimaryVtx(0),
_zPrimaryVtx(0),
_probPrimaryVtx(0),
_thrustCosTh(0),
_thrustPhi(0),
_thrustMag(0),
_R2(0),
//####################Particle numbers####################################################
_nLeptonsLoose(0),
_nLeptonsTight(0),
_neLoose(0),
_neTight(0),
_nmuLoose(0),
_nmuTight(0),
_npiLoose(0),
_npiTight(0),
_nKLoose(0),
_nKTight(0),
_npLoose(0),
_npTight(0),
_ngammaLoose(0),
_ngammaTight(0),
_npi0MergedLoose(0),
_npi0MergedTight(0),
_npi0Loose(0),
_npi0Tight(0),
_nKsLoose(0),
_nKsTight(0),
//######################### Trigger and Skim information ###################################
_bbqqDiscr(0), 
_BGFR2(0),
_BunchT0DeltaDircTrk(0),
_BunchT0DeltaFF(0), 
_BunchT0FinderErr(0), 
_BunchT0FitterErr(0),
_eCharShow(0), 
_elecLoose1cm(0),
_elecLoose1l(0),
_elecLoose2cm(0), 
_elecLoose2l(0), 
_elecTight1cm(0),
_elecTight1l(0), 
_elecTight2cm(0), 
_elecTight2l(0),
_eNeutShow(0),
_L3OutBhabhaFlatOprValue(0), 
_L3OutBhabhaFlatValue(0),
_muonLoose1cm(0), 
_muonLoose1l(0), 
_muonLoose2cm(0), 
_muonLoose2l(0), 
_muonTight1cm(0), 
_muonTight1l(0), 
_muonTight2cm(0), 
_muonTight2l(0),
_pTotalScalar(0),
_R2All(0),
_sphericityAll(0), 
_TagLooseProton_maxCmsP(0), 
_TagTightProton_maxCmsP(0),
_thrustCosThAll(0), 
_thrustMagAll(0), 
_thrustPhiAll(0),
_D3PS(0),
_Dmeson0(0), 
_Dsmeson0(0), 
_Dstar0(0),
_Dstar1(0), 
_LumiBhabhaWeight(0),
_nNeutralCands(0),
_nNeutralCndLoose(0),
_nNeutralCndTight(0),
_TagEtaPeppTwoBody_maxCmsPmaxCosT(0),
_TagEtaPrhogTwoBody_maxCmsPmaxCosT(0),
_TagEtaTwoBody_maxCmsPmaxCosT(0),
_TagKstpTwoBody_maxCmsPmaxCosT(0), 
_TagKstTwoBody_maxCmsPmaxCosT(0),
_TagKTwoBody_maxCmsPmaxCosT(0),
_TagK_S0TwoBody_maxCmsPmaxCosT(0),
_TagOmegaTwoBody_maxCmsPmaxCosT(0), 
_TagPhiTwoBody_maxCmsPmaxCosT(0),
_TagPi0TwoBody_maxCmsPmaxCosT(0),
_TagRhopTwoBody_maxCmsPmaxCosT(0), 
_TagRhoTwoBody_maxCmsPmaxCosT(0)
{
  fBranchTable  = new THashList();
}

PAFTagBase::PAFTagBase( const PAFTagBase& t) :
//####################Analysis bits####################################################
_analysisbits(t._analysisbits),
_analysisbits1(t._analysisbits1),
// ################ Beam Information################################################
_cmf1(t._cmf1),
_cmf2(t._cmf2),
_cmf3(t._cmf3),
_cmf4(t._cmf4),
_cmsEnergy(t._cmsEnergy),
_xBeamSpot(t._xBeamSpot),
_yBeamSpot(t._yBeamSpot),
_zBeamSpot(t._zBeamSpot),
_xBeamErr(t._xBeamErr),
_yBeamErr(t._yBeamErr),
_zBeamErr(t._zBeamErr),
// ################ Tracking Information################################################
_pTotalCosTh(t._pTotalCosTh),
_pTotalMag(t._pTotalMag),
_pTotalPhi(t._pTotalPhi),
_eTotal(t._eTotal),
_pTotalMass(t._pTotalMass),
_p1CosTheta(t._p1CosTheta),
_p1Phi(t._p1Phi),
_p1Mag(t._p1Mag),
_p2CosTheta(t._p2CosTheta),
_p2Phi(t._p2Phi),
_p2Mag(t._p2Mag),
_nTracks(t._nTracks),
_nGoodTrkLoose(t._nGoodTrkLoose),
_nGoodTrkTight(t._nGoodTrkTight),
_p1EmcCandEnergy(t._p1EmcCandEnergy),
_p2EmcCandEnergy(t._p2EmcCandEnergy),
_p1PidHypo(t._p1PidHypo),
_p2PidHypo(t._p2PidHypo),
_totChargeLoose(t._totChargeLoose),
_totChargeTight(t._totChargeTight),
//#####################Lepton information############################################
_leptLoose1hypo(t._leptLoose1hypo),
_leptLoose1Charge(t._leptLoose1Charge),
_leptLoose1p(t._leptLoose1p),
_leptLoose2hypo(t._leptLoose2hypo),
_leptLoose2Charge(t._leptLoose2Charge),
_leptLoose2p(t._leptLoose2p),
_leptTight1hypo(t._leptTight1hypo),
_leptTight1Charge(t._leptTight1Charge),
_leptTight1p(t._leptTight1p),
_leptTight2hypo(t._leptTight2hypo),
_leptTight2Charge(t._leptTight2Charge),
_leptTight2p(t._leptTight2p),
//#####################Calorimeter information############################################
_eNeutralCosTh(t._eNeutralCosTh),
_eNeutralPhi(t._eNeutralPhi),
_eNeutralMag(t._eNeutralMag),
_e1CosTheta(t._e1CosTheta),
_e1Phi(t._e1Phi),
_e1Mag(t._e1Mag),
_e2CosTheta(t._e2CosTheta),
_e2Phi(t._e2Phi),
_e2Mag(t._e2Mag),
//#####################Vertex and event shape#############################################
_xPrimaryVtx(t._xPrimaryVtx),
_yPrimaryVtx(t._yPrimaryVtx),
_zPrimaryVtx(t._zPrimaryVtx),
_probPrimaryVtx(t._probPrimaryVtx),
_thrustCosTh(t._thrustCosTh),
_thrustPhi(t._thrustPhi),
_thrustMag(t._thrustMag),
_R2(t._R2),
//####################Particle numbers####################################################
_nLeptonsLoose(t._nLeptonsLoose),
_nLeptonsTight(t._nLeptonsTight),
_neLoose(t._neLoose),
_neTight(t._neTight),
_nmuLoose(t._nmuLoose),
_nmuTight(t._nmuTight),
_npiLoose(t._npiLoose),
_npiTight(t._npiTight),
_nKLoose(t._nKLoose),
_nKTight(t._nKTight),
_npLoose(t._npLoose),
_npTight(t._npTight),
_ngammaLoose(t._ngammaLoose),
_ngammaTight(t._ngammaTight),
_npi0MergedLoose(t._npi0MergedLoose),
_npi0MergedTight(t._npi0MergedTight),
_npi0Loose(t._npi0Loose),
_npi0Tight(t._npi0Tight),
_nKsLoose(t._nKsLoose),
_nKsTight(t._nKsTight),
//######################### Trigger and Skim information ###################################
_bbqqDiscr(t._bbqqDiscr), 
_BGFR2(t._BGFR2),
_BunchT0DeltaDircTrk(t._BunchT0DeltaDircTrk),
_BunchT0DeltaFF(t._BunchT0DeltaFF), 
_BunchT0FinderErr(t._BunchT0FinderErr), 
_BunchT0FitterErr(t._BunchT0FitterErr),
_eCharShow(t._eCharShow), 
_elecLoose1cm(t._elecLoose1cm),
_elecLoose1l(t._elecLoose1l),
_elecLoose2cm(t._elecLoose2cm), 
_elecLoose2l(t._elecLoose2l), 
_elecTight1cm(t._elecTight1cm),
_elecTight1l(t._elecTight1l), 
_elecTight2cm(t._elecTight2cm), 
_elecTight2l(t._elecTight2l),
_eNeutShow(t._eNeutShow),
_L3OutBhabhaFlatOprValue(t._L3OutBhabhaFlatOprValue), 
_L3OutBhabhaFlatValue(t._L3OutBhabhaFlatValue),
_muonLoose1cm(t._muonLoose1cm), 
_muonLoose1l(t._muonLoose1l), 
_muonLoose2cm(t._muonLoose2cm), 
_muonLoose2l(t._muonLoose2l), 
_muonTight1cm(t._muonTight1cm), 
_muonTight1l(t._muonTight1l), 
_muonTight2cm(t._muonTight2cm), 
_muonTight2l(t._muonTight2l),
_pTotalScalar(t._pTotalScalar),
_R2All(t._R2All),
_sphericityAll(t._sphericityAll), 
_TagLooseProton_maxCmsP(t._TagLooseProton_maxCmsP), 
_TagTightProton_maxCmsP(t._TagTightProton_maxCmsP),
_thrustCosThAll(t._thrustCosThAll), 
_thrustMagAll(t._thrustMagAll), 
_thrustPhiAll(t._thrustPhiAll),
_D3PS(t._D3PS),
_Dmeson0(t._Dmeson0), 
_Dsmeson0(t._Dsmeson0), 
_Dstar0(t._Dstar0),
_Dstar1(t._Dstar1), 
_LumiBhabhaWeight(t._LumiBhabhaWeight),
_nNeutralCands(t._nNeutralCands),
_nNeutralCndLoose(t._nNeutralCndLoose),
_nNeutralCndTight(t._nNeutralCndTight),
_TagEtaPeppTwoBody_maxCmsPmaxCosT(t._TagEtaPeppTwoBody_maxCmsPmaxCosT),
_TagEtaPrhogTwoBody_maxCmsPmaxCosT(t._TagEtaPrhogTwoBody_maxCmsPmaxCosT),
_TagEtaTwoBody_maxCmsPmaxCosT(t._TagEtaTwoBody_maxCmsPmaxCosT),
_TagKstpTwoBody_maxCmsPmaxCosT(t._TagKstpTwoBody_maxCmsPmaxCosT), 
_TagKstTwoBody_maxCmsPmaxCosT(t._TagKstTwoBody_maxCmsPmaxCosT),
_TagKTwoBody_maxCmsPmaxCosT(t._TagKTwoBody_maxCmsPmaxCosT),
_TagK_S0TwoBody_maxCmsPmaxCosT(t._TagK_S0TwoBody_maxCmsPmaxCosT),
_TagOmegaTwoBody_maxCmsPmaxCosT(t._TagOmegaTwoBody_maxCmsPmaxCosT), 
_TagPhiTwoBody_maxCmsPmaxCosT(t._TagPhiTwoBody_maxCmsPmaxCosT),
_TagPi0TwoBody_maxCmsPmaxCosT(t._TagPi0TwoBody_maxCmsPmaxCosT),
_TagRhopTwoBody_maxCmsPmaxCosT(t._TagRhopTwoBody_maxCmsPmaxCosT), 
_TagRhoTwoBody_maxCmsPmaxCosT(t._TagRhoTwoBody_maxCmsPmaxCosT)
{}

//--------------
// Destructor --
//--------------
PAFTagBase::~PAFTagBase() 
{ 
    delete fBranchTable;
}

//----------------------
//-- public Functions --
//----------------------
//fill & get operations

void PAFTagBase::SetCMFrame( TLorentzVector e ) {
    _cmf1 = e.X();
    _cmf2 = e.Y();
    _cmf3 = e.Z();
    _cmf4 = e.T();
}

void PAFTagBase::SetCMSEnergy( Float_t e ) {
    _cmsEnergy = e;
}

void PAFTagBase::SetBeamSpot( TVector3 v ) {
    _xBeamSpot = (Float_t) v.X();
    _yBeamSpot = (Float_t) v.Y();
    _zBeamSpot = (Float_t) v.Z();
}

void PAFTagBase::SetBeamError( TVector3 v ) {
    _xBeamErr = (Float_t) v.X();
    _yBeamErr = (Float_t) v.Y();
    _zBeamErr = (Float_t) v.Z();
}

void PAFTagBase::SetTotalMomentum( TVector3 v ) {
    _pTotalCosTh = (Float_t) v.X();
    _pTotalPhi   = (Float_t) v.Y();
    _pTotalMag   = (Float_t) v.Z();
}

void PAFTagBase::SetTotalEnergy( Float_t e ) {
    _eTotal = e;
}

void PAFTagBase::SetTotalMass( Float_t m ) {
    _pTotalMass = m;
}

void PAFTagBase::SetPrimaryTrackMomentum( TVector3 v ) {
    _p1CosTheta = (Float_t) v.X();
    _p1Phi   = (Float_t) v.Y();
    _p1Mag   = (Float_t) v.Z();
}

void PAFTagBase::SetSecondaryTrackMomentum( TVector3 v ) {
    _p2CosTheta = (Float_t) v.X();
    _p2Phi   = (Float_t) v.Y();
    _p2Mag   = (Float_t) v.Z();
}

void PAFTagBase::SetPrimaryTrackShowerEnergy(Float_t f ) {
    _p1EmcCandEnergy = f; 
}

void PAFTagBase::SetSecondaryTrackShowerEnergy(Float_t f ) {
    _p2EmcCandEnergy = f; 
}

void PAFTagBase::SetNumberOfTracks( Int_t b ) { 
    _nTracks = b; 
}

void PAFTagBase::SetNumberOfGoodTracksLoose( Int_t b ) { 
    _nGoodTrkLoose = b; 
}

void PAFTagBase::SetNumberOfGoodTracksTight( Int_t b ) { 
    _nGoodTrkTight = b; 
}

void PAFTagBase::SetPrimaryTrackHypothesis( Int_t b ) { 
    _p1PidHypo = (Char_t) b; 
}

void PAFTagBase::SetSecondaryTrackHypothesis( Int_t b ) { 
    _p2PidHypo = (Char_t) b; 
}

void PAFTagBase::SetTotalChargeLoose( Int_t b ) { 
    _totChargeLoose = (Char_t) b; 
}

void PAFTagBase::SetTotalChargeTight( Int_t b ) { 
    _totChargeTight = (Char_t) b; 
}

TLorentzVector PAFTagBase::GetCMFrame()  const {
    if (_cmf1!=0.0&&_cmf2!=0.0&&_cmf3!=0.0&&_cmf4!=0.0)
    return TLorentzVector(_cmf1,_cmf2,_cmf3,_cmf4);
    else
    return TLorentzVector(-0.12,0.,5.87,12.1);
}

Float_t PAFTagBase::GetCMSEnergy()  const {
    return _cmsEnergy; 
}

TVector3 PAFTagBase::GetBeamSpot() const {
    return TVector3(_xBeamSpot,_yBeamSpot,_zBeamSpot);
}

TVector3 PAFTagBase::GetBeamError() const {
    return TVector3(_xBeamErr,_yBeamErr,_zBeamErr);
}

TVector3 PAFTagBase::GetTotalMomentum() const {
    return TVector3(_pTotalCosTh,_pTotalPhi,_pTotalMag);
}

Float_t PAFTagBase::GetTotalEnergy()  const {
    return _eTotal; 
}

Float_t PAFTagBase::GetTotalMass()  const {
    return _pTotalMass; 
}

TVector3 PAFTagBase::GetPrimaryTrackMomentum() const {
    return TVector3(_p1CosTheta,_p1Phi,_p1Mag);
}

TVector3 PAFTagBase::GetSecondaryTrackMomentum() const {
  return TVector3(_p2CosTheta,_p2Phi,_p2Mag);
}

Float_t PAFTagBase::GetPrimaryTrackShowerEnergy()  const {
    return _p1EmcCandEnergy; 
}

Float_t PAFTagBase::GetSecondaryTrackShowerEnergy()  const {
    return _p2EmcCandEnergy; 
}

Int_t PAFTagBase::GetNumberOfTracks()  const {
  return _nTracks; 
}

Int_t PAFTagBase::GetNumberOfGoodTracksLoose()  const {
    return _nGoodTrkLoose; 
}

Int_t PAFTagBase::GetNumberOfGoodTracksTight()  const {
    return _nGoodTrkTight; 
}

Int_t PAFTagBase::GetPrimaryTrackHypothesis()  const {
    return _p1PidHypo; 
}

Int_t PAFTagBase::GetSecondaryTrackHypothesis()  const {
    return _p2PidHypo; 
}

Int_t PAFTagBase::GetTotalChargeLoose()  const { 
    return  _totChargeLoose; 
}

Int_t PAFTagBase::GetTotalChargeTight()  const { 
    return  _totChargeTight; 
}

//##################### Lepton information #############################

void PAFTagBase::SetPrimaryLeptonHypothesisLoose( Int_t b ) {
    _leptLoose1hypo = (Char_t) b;
}

void PAFTagBase::SetPrimaryLeptonChargeLoose( Int_t b ) {
    _leptLoose1Charge = (Char_t) b;
}

void PAFTagBase::SetPrimaryLeptonMomentumLoose( Float_t b ) {
    _leptLoose1p = b;
}

void PAFTagBase::SetSecondaryLeptonHypothesisLoose( Int_t b ) {
    _leptLoose2hypo = (Char_t) b;
}

void PAFTagBase::SetSecondaryLeptonChargeLoose( Int_t b ) {
    _leptLoose2Charge = (Char_t) b;
}

void PAFTagBase::SetSecondaryLeptonMomentumLoose( Float_t b ) {
    _leptLoose2p = b;
}

void PAFTagBase::SetPrimaryLeptonHypothesisTight( Int_t b ) {
    _leptTight1hypo = (Char_t) b;
}

void PAFTagBase::SetPrimaryLeptonChargeTight( Int_t b ) {
    _leptTight1Charge = (Char_t) b;
}

void PAFTagBase::SetPrimaryLeptonMomentumTight( Float_t b ) {
    _leptTight1p = b;
}

void PAFTagBase::SetSecondaryLeptonHypothesisTight( Int_t b ) {
    _leptTight2hypo = (Char_t) b;
}

void PAFTagBase::SetSecondaryLeptonChargeTight( Int_t b ) {
    _leptTight2Charge = (Char_t) b;
}

void PAFTagBase::SetSecondaryLeptonMomentumTight( Float_t b ) {
    _leptTight2p = b;
}

Int_t PAFTagBase::GetPrimaryLeptonHypothesisLoose()  const {
    return _leptLoose1hypo;
}

Int_t PAFTagBase::GetPrimaryLeptonChargeLoose()  const {
    return _leptLoose1Charge;
}

Float_t	PAFTagBase::GetPrimaryLeptonMomentumLoose()  const {
    return _leptLoose1p;
}

Int_t PAFTagBase::GetSecondaryLeptonHypothesisLoose()  const {
    return _leptLoose2hypo;
}

Int_t PAFTagBase::GetSecondaryLeptonChargeLoose()  const {
    return _leptLoose2Charge;
}

Float_t	PAFTagBase::GetSecondaryLeptonMomentumLoose()  const {
    return _leptLoose2p;
}

Int_t PAFTagBase::GetPrimaryLeptonHypothesisTight()  const {
    return _leptTight1hypo;
}

Int_t PAFTagBase::GetPrimaryLeptonChargeTight()  const {
    return _leptTight1Charge;
}

Float_t	PAFTagBase::GetPrimaryLeptonMomentumTight()  const {
    return _leptTight1p;
}

Int_t PAFTagBase::GetSecondaryLeptonHypothesisTight()  const {
    return _leptTight2hypo;
}

Int_t PAFTagBase::GetSecondaryLeptonChargeTight()  const {
    return _leptTight2Charge;
}

Float_t	PAFTagBase::GetSecondaryLeptonMomentumTight()  const {
    return _leptTight2p;
}

//##################### Calorimeter information #############################

void PAFTagBase::SetTotalNeutralEnergy( TVector3 v ) { 
    _eNeutralCosTh = (Float_t) v.X(); 
    _eNeutralPhi   = (Float_t) v.Y();
    _eNeutralMag   = (Float_t) v.Z();
}

void PAFTagBase::SetHighestEnergyShower( TVector3 v ) { 
    _e1CosTheta   = (Float_t) v.X(); 
    _e1Phi        = (Float_t) v.Y(); 
    _e1Mag        = (Float_t) v.Z(); 
}

void PAFTagBase::SetSecondHighestEnergyShower( TVector3 v ){ 
    _e2CosTheta  = (Float_t) v.X();
    _e2Phi       = (Float_t) v.Y();
    _e2Mag       = (Float_t) v.Z();
} 

TVector3 PAFTagBase::GetTotalNeutralEnergy() const{ 
  return TVector3(_eNeutralCosTh,_eNeutralPhi,_eNeutralMag);
}

TVector3 PAFTagBase::GetHighestEnergyShower() const{
    return TVector3(_e1CosTheta,_e1Phi,_e1Mag);
}

TVector3 PAFTagBase::GetSecondHighestEnergyShower() const{
  return TVector3(_e2CosTheta,_e2Phi,_e2Mag);
}

//######################## Vertex and shape ####################################

void PAFTagBase::SetPrimaryVertex( TVector3 v, Float_t p ) { 
    _xPrimaryVtx    = (Float_t) v.X();
    _yPrimaryVtx    = (Float_t) v.Y();
    _zPrimaryVtx    = (Float_t) v.Z();
    _probPrimaryVtx = (Float_t) p;
}

void PAFTagBase::SetPrimaryVertexProbability( Float_t p) {
    _probPrimaryVtx = p;
}

void PAFTagBase::SetThrust( TVector3 v ) {
    _thrustCosTh = (Float_t) v.X();
    _thrustPhi   = (Float_t) v.Y();
    _thrustMag   = (Float_t) v.Z();
}

void PAFTagBase::SetR2 ( Float_t r) {
    _R2 = r;
}

TVector3 PAFTagBase::GetPrimaryVertex() const{
    return TVector3(_xPrimaryVtx,_yPrimaryVtx,_zPrimaryVtx); 
}

Float_t PAFTagBase::GetPrimaryVertexProbability() const {
    return _probPrimaryVtx;
}

TVector3 PAFTagBase::GetThrust() const{
    return  TVector3(_thrustCosTh,_thrustPhi,_thrustMag);
}

Float_t PAFTagBase::GetR2 () const {
    return _R2;
}

//######################### Particle numbers ###################################

void PAFTagBase::SetLeptonsLoose( Int_t b ) { _nLeptonsLoose=b; }
void PAFTagBase::SetElectronsLoose( Int_t b ) { _neLoose=b; }
void PAFTagBase::SetMuonsLoose( Int_t b ) { _nmuLoose=b; }
void PAFTagBase::SetPionsLoose( Int_t b ) { _npiLoose=b; }
void PAFTagBase::SetKaonsLoose( Int_t b ) { _nKLoose=b; }
void PAFTagBase::SetProtonsLoose( Int_t b ) { _npLoose=b; }
void PAFTagBase::SetGammasLoose( Int_t b ) { _ngammaLoose=b; }
void PAFTagBase::SetMergedPi0Loose( Int_t b ) { _npi0MergedLoose=b; }
void PAFTagBase::SetPi0Loose( Int_t b ) { _npi0Loose=b; }
void PAFTagBase::SetKsLoose( Int_t b ) { _nKsLoose=b; }

void PAFTagBase::SetLeptonsTight( Int_t b ) { _nLeptonsTight=b; }
void PAFTagBase::SetElectronsTight( Int_t b ) { _neTight=b; }
void PAFTagBase::SetMuonsTight( Int_t b ) { _nmuTight=b; }
void PAFTagBase::SetPionsTight( Int_t b ) { _npiTight=b; }
void PAFTagBase::SetKaonsTight( Int_t b ) { _nKTight=b; }
void PAFTagBase::SetProtonsTight( Int_t b ) { _npTight=b; }
void PAFTagBase::SetGammasTight( Int_t b ) { _ngammaTight=b; }
void PAFTagBase::SetMergedPi0Tight( Int_t b ) { _npi0MergedTight=b; }
void PAFTagBase::SetPi0Tight( Int_t b ) { _npi0Tight=b; }
void PAFTagBase::SetKsTight( Int_t b ) { _nKsTight=b; }

Int_t PAFTagBase::GetLeptonsLoose()  const { return _nLeptonsLoose; }
Int_t PAFTagBase::GetElectronsLoose()  const { return _neLoose; }
Int_t PAFTagBase::GetMuonsLoose()  const { return _nmuLoose; }
Int_t PAFTagBase::GetPionsLoose()  const { return _npiLoose; }
Int_t PAFTagBase::GetKaonsLoose()  const { return _nKLoose; }
Int_t PAFTagBase::GetProtonsLoose()  const { return _npLoose; }
Int_t PAFTagBase::GetGammasLoose()  const { return _ngammaLoose; }
Int_t PAFTagBase::GetMergedPi0Loose()  const { return _npi0MergedLoose; }
Int_t PAFTagBase::GetPi0Loose()  const { return _npi0Loose; }
Int_t PAFTagBase::GetKsLoose()  const { return _nKsLoose; }

Int_t PAFTagBase::GetLeptonsTight()  const { return _nLeptonsTight; }
Int_t PAFTagBase::GetElectronsTight()  const { return _neTight; }
Int_t PAFTagBase::GetMuonsTight()  const { return _nmuTight; }
Int_t PAFTagBase::GetPionsTight()  const { return _npiTight; }
Int_t PAFTagBase::GetKaonsTight()  const { return _nKTight; }
Int_t PAFTagBase::GetProtonsTight()  const { return _npTight; }
Int_t PAFTagBase::GetGammasTight()  const { return _ngammaTight; }
Int_t PAFTagBase::GetMergedPi0Tight()  const { return _npi0MergedTight; }
Int_t PAFTagBase::GetPi0Tight()  const { return _npi0Tight; }
Int_t PAFTagBase::GetKsTight()  const { return _nKsTight; }

//######################### Trigger and Skim information ###################################

void PAFTagBase::SetBbqqDiscr(Float_t f) { _bbqqDiscr = f; } 
void PAFTagBase::SetBGFR2(Float_t f) { _BGFR2 = f; }
void PAFTagBase::SetBunchT0DeltaDircTrk(Float_t f) { _BunchT0DeltaDircTrk = f; }
void PAFTagBase::SetBunchT0DeltaFF(Float_t f) { _BunchT0DeltaFF = f; } 
void PAFTagBase::SetBunchT0FinderErr(Float_t f) { _BunchT0FinderErr = f; } 
void PAFTagBase::SetBunchT0FitterErr(Float_t f) { _BunchT0FitterErr = f; }
void PAFTagBase::SetECharShow(Float_t f) { _eCharShow = f; } 
void PAFTagBase::SetElecLoose1cm(Float_t f) { _elecLoose1cm = f; }
void PAFTagBase::SetElecLoose1l(Float_t f) { _elecLoose1l = f; }
void PAFTagBase::SetElecLoose2cm(Float_t f) { _elecLoose2cm = f; } 
void PAFTagBase::SetElecLoose2l(Float_t f) { _elecLoose2l = f; } 
void PAFTagBase::SetElecTight1cm(Float_t f) { _elecTight1cm = f; }
void PAFTagBase::SetElecTight1l(Float_t f) { _elecTight1l = f; } 
void PAFTagBase::SetElecTight2cm(Float_t f) { _elecTight2cm = f; } 
void PAFTagBase::SetElecTight2l(Float_t f) { _elecTight2l = f; }
void PAFTagBase::SetENeutShow(Float_t f) { _eNeutShow = f; }
void PAFTagBase::SetL3OutBhabhaFlatOprValue(Float_t f) { _L3OutBhabhaFlatOprValue = f; } 
void PAFTagBase::SetL3OutBhabhaFlatValue(Float_t f) { _L3OutBhabhaFlatValue = f; }
void PAFTagBase::SetMuonLoose1cm(Float_t f) { _muonLoose1cm = f; } 
void PAFTagBase::SetMuonLoose1l(Float_t f) { _muonLoose1l = f; } 
void PAFTagBase::SetMuonLoose2cm(Float_t f) { _muonLoose2cm = f; } 
void PAFTagBase::SetMuonLoose2l(Float_t f) { _muonLoose2l = f; } 
void PAFTagBase::SetMuonTight1cm(Float_t f) { _muonTight1cm = f; } 
void PAFTagBase::SetMuonTight1l(Float_t f) { _muonTight1l = f; } 
void PAFTagBase::SetMuonTight2cm(Float_t f) { _muonTight2cm = f; } 
void PAFTagBase::SetMuonTight2l(Float_t f) { _muonTight2l = f; }
void PAFTagBase::SetPTotalScalar(Float_t f) { _pTotalScalar = f; }
void PAFTagBase::SetR2All(Float_t f) { _R2All = f; }
void PAFTagBase::SetSphericityAll(Float_t f) { _sphericityAll = f; } 
void PAFTagBase::SetTagLooseProton_maxCmsP(Float_t f) { _TagLooseProton_maxCmsP = f; } 
void PAFTagBase::SetTagTightProton_maxCmsP(Float_t f) { _TagTightProton_maxCmsP = f; }
void PAFTagBase::SetThrustCosThAll(Float_t f) { _thrustCosThAll = f; } 
void PAFTagBase::SetThrustMagAll(Float_t f) { _thrustMagAll = f; } 
void PAFTagBase::SetThrustPhiAll(Float_t f) { _thrustPhiAll = f; }

Float_t PAFTagBase::GetBbqqDiscr() const { return _bbqqDiscr; }
Float_t PAFTagBase::GetBGFR2() const { return _BGFR2; }
Float_t PAFTagBase::GetBunchT0DeltaDircTrk() const { return _BunchT0DeltaDircTrk; }
Float_t PAFTagBase::GetBunchT0DeltaFF() const { return _BunchT0DeltaFF; } 
Float_t PAFTagBase::GetBunchT0FinderErr() const { return _BunchT0FinderErr; } 
Float_t PAFTagBase::GetBunchT0FitterErr() const { return _BunchT0FitterErr; }
Float_t PAFTagBase::GetECharShow() const { return _eCharShow; } 
Float_t PAFTagBase::GetElecLoose1cm() const { return _elecLoose1cm; }
Float_t PAFTagBase::GetElecLoose1l() const { return _elecLoose1l; }
Float_t PAFTagBase::GetElecLoose2cm() const { return _elecLoose2cm; } 
Float_t PAFTagBase::GetElecLoose2l() const { return _elecLoose2l; } 
Float_t PAFTagBase::GetElecTight1cm() const { return _elecTight1cm; }
Float_t PAFTagBase::GetElecTight1l() const { return _elecTight1l; } 
Float_t PAFTagBase::GetElecTight2cm() const { return _elecTight2cm; } 
Float_t PAFTagBase::GetElecTight2l() const { return _elecTight2l; }
Float_t PAFTagBase::GetENeutShow() const { return _eNeutShow; }
Float_t PAFTagBase::GetL3OutBhabhaFlatOprValue() const { return _L3OutBhabhaFlatOprValue; } 
Float_t PAFTagBase::GetL3OutBhabhaFlatValue() const { return _L3OutBhabhaFlatValue; }
Float_t PAFTagBase::GetMuonLoose1cm() const { return _muonLoose1cm; } 
Float_t PAFTagBase::GetMuonLoose1l() const { return _muonLoose1l; } 
Float_t PAFTagBase::GetMuonLoose2cm() const { return _muonLoose2cm; } 
Float_t PAFTagBase::GetMuonLoose2l() const { return _muonLoose2l; } 
Float_t PAFTagBase::GetMuonTight1cm() const { return _muonTight1cm; } 
Float_t PAFTagBase::GetMuonTight1l() const { return _muonTight1l; } 
Float_t PAFTagBase::GetMuonTight2cm() const { return _muonTight2cm; } 
Float_t PAFTagBase::GetMuonTight2l() const { return _muonTight2l; }
Float_t PAFTagBase::GetPTotalScalar() const { return _pTotalScalar; }
Float_t PAFTagBase::GetR2All() const { return _R2All; }
Float_t PAFTagBase::GetSphericityAll() const { return _sphericityAll; } 
Float_t PAFTagBase::GetTagLooseProton_maxCmsP() const { return _TagLooseProton_maxCmsP; } 
Float_t PAFTagBase::GetTagTightProton_maxCmsP() const { return _TagTightProton_maxCmsP; }
Float_t PAFTagBase::GetThrustCosThAll() const { return _thrustCosThAll; } 
Float_t PAFTagBase::GetThrustMagAll() const { return _thrustMagAll; } 
Float_t PAFTagBase::GetThrustPhiAll() const { return _thrustPhiAll; } 

void	PAFTagBase::SetD3PS(Int_t i)  { _D3PS = i; }
void	PAFTagBase::SetDmeson0(Int_t i)  { _Dmeson0 = i; } 
void	PAFTagBase::SetDsmeson0(Int_t i)  { _Dsmeson0 = i; } 
void	PAFTagBase::SetDstar0(Int_t i)  { _Dstar0 = i; }
void	PAFTagBase::SetDstar1(Int_t i)  { _Dstar1 = i; } 
void	PAFTagBase::SetLumiBhabhaWeight(Int_t i)  { _LumiBhabhaWeight = i; }
void	PAFTagBase::SetNumberOfNeutralCands(Int_t i)  { _nNeutralCands = i; }
void	PAFTagBase::SetNumberOfNeutralCndLoose(Int_t i)  { _nNeutralCndLoose = i; }
void	PAFTagBase::SetNumberOfNeutralCndTight(Int_t i)  { _nNeutralCndTight = i; }
void	PAFTagBase::SetTagEtaPeppTwoBody_maxCmsPmaxCosT(Int_t i)  { _TagEtaPeppTwoBody_maxCmsPmaxCosT = i; }
void	PAFTagBase::SetTagEtaPrhogTwoBody_maxCmsPmaxCosT(Int_t i)  { _TagEtaPrhogTwoBody_maxCmsPmaxCosT = i; }
void	PAFTagBase::SetTagEtaTwoBody_maxCmsPmaxCosT(Int_t i)  { _TagEtaTwoBody_maxCmsPmaxCosT = i; }
void	PAFTagBase::SetTagKstpTwoBody_maxCmsPmaxCosT(Int_t i)  { _TagKstpTwoBody_maxCmsPmaxCosT = i; } 
void	PAFTagBase::SetTagKstTwoBody_maxCmsPmaxCosT(Int_t i)  { _TagKstTwoBody_maxCmsPmaxCosT = i; }
void	PAFTagBase::SetTagKTwoBody_maxCmsPmaxCosT(Int_t i)  { _TagKTwoBody_maxCmsPmaxCosT = i; }
void	PAFTagBase::SetTagK_S0TwoBody_maxCmsPmaxCosT(Int_t i)  { _TagK_S0TwoBody_maxCmsPmaxCosT = i; }
void	PAFTagBase::SetTagOmegaTwoBody_maxCmsPmaxCosT(Int_t i)  { _TagOmegaTwoBody_maxCmsPmaxCosT = i; } 
void	PAFTagBase::SetTagPhiTwoBody_maxCmsPmaxCosT(Int_t i)  { _TagPhiTwoBody_maxCmsPmaxCosT = i; }
void	PAFTagBase::SetTagPi0TwoBody_maxCmsPmaxCosT(Int_t i)  { _TagPi0TwoBody_maxCmsPmaxCosT = i; }
void	PAFTagBase::SetTagRhopTwoBody_maxCmsPmaxCosT(Int_t i)  { _TagRhopTwoBody_maxCmsPmaxCosT = i; } 
void	PAFTagBase::SetTagRhoTwoBody_maxCmsPmaxCosT(Int_t i)  { _TagRhoTwoBody_maxCmsPmaxCosT = i; }

Int_t	PAFTagBase::GetD3PS() const { return _D3PS; } 
Int_t	PAFTagBase::GetDmeson0() const { return _Dmeson0; } 
Int_t	PAFTagBase::GetDsmeson0() const { return _Dsmeson0; } 
Int_t	PAFTagBase::GetDstar0() const { return _Dstar0; } 
Int_t	PAFTagBase::GetDstar1() const { return _Dstar1; } 
Int_t	PAFTagBase::GetLumiBhabhaWeight() const { return _LumiBhabhaWeight; } 
Int_t	PAFTagBase::GetNumberOfNeutralCands() const { return _nNeutralCands; } 
Int_t	PAFTagBase::GetNumberOfNeutralCndLoose() const { return _nNeutralCndLoose; } 
Int_t	PAFTagBase::GetNumberOfNeutralCndTight() const { return _nNeutralCndTight; } 
Int_t	PAFTagBase::GetTagEtaPeppTwoBody_maxCmsPmaxCosT() const { return _TagEtaPeppTwoBody_maxCmsPmaxCosT; } 
Int_t	PAFTagBase::GetTagEtaPrhogTwoBody_maxCmsPmaxCosT() const { return _TagEtaPrhogTwoBody_maxCmsPmaxCosT; } 
Int_t	PAFTagBase::GetTagEtaTwoBody_maxCmsPmaxCosT() const { return _TagEtaTwoBody_maxCmsPmaxCosT; } 
Int_t	PAFTagBase::GetTagKstpTwoBody_maxCmsPmaxCosT() const { return _TagKstpTwoBody_maxCmsPmaxCosT; } 
Int_t	PAFTagBase::GetTagKstTwoBody_maxCmsPmaxCosT() const { return _TagKstTwoBody_maxCmsPmaxCosT; } 
Int_t	PAFTagBase::GetTagKTwoBody_maxCmsPmaxCosT() const { return _TagKTwoBody_maxCmsPmaxCosT; } 
Int_t	PAFTagBase::GetTagK_S0TwoBody_maxCmsPmaxCosT() const { return _TagK_S0TwoBody_maxCmsPmaxCosT; } 
Int_t	PAFTagBase::GetTagOmegaTwoBody_maxCmsPmaxCosT() const { return _TagOmegaTwoBody_maxCmsPmaxCosT; } 
Int_t	PAFTagBase::GetTagPhiTwoBody_maxCmsPmaxCosT() const { return _TagPhiTwoBody_maxCmsPmaxCosT; } 
Int_t	PAFTagBase::GetTagPi0TwoBody_maxCmsPmaxCosT() const { return _TagPi0TwoBody_maxCmsPmaxCosT; } 
Int_t	PAFTagBase::GetTagRhopTwoBody_maxCmsPmaxCosT() const { return _TagRhopTwoBody_maxCmsPmaxCosT; } 
Int_t	PAFTagBase::GetTagRhoTwoBody_maxCmsPmaxCosT() const { return _TagRhoTwoBody_maxCmsPmaxCosT; } 

//######################### Analysis bits ###################################

void PAFTagBase::SetAnalysisBits(UInt_t b, UInt_t b1) { _analysisbits = b; _analysisbits1 = b1; }
UInt_t  PAFTagBase::GetAnalysisBits() const { return _analysisbits; }
UInt_t  PAFTagBase::GetAnalysisBits1() const { return _analysisbits1; }

// Copy operator 

void PAFTagBase::operator =( PAFTagBase& t ) {
    //####################Analysis bits####################################################
    _analysisbits=t._analysisbits;
    _analysisbits1=t._analysisbits1;
    // ################ Beam Information################################################
    _cmf1=t._cmf1;
    _cmf2=t._cmf1;
    _cmf3=t._cmf1;
    _cmf4=t._cmf1;
    _cmsEnergy=t._cmsEnergy;
    _xBeamSpot=t._xBeamSpot;
    _yBeamSpot=t._yBeamSpot;
    _zBeamSpot=t._zBeamSpot;
    _xBeamErr=t._xBeamErr;
    _yBeamErr=t._yBeamErr;
    _zBeamErr=t._zBeamErr;
    // ################ Tracking Information################################################
    _pTotalCosTh=t._pTotalCosTh;
    _pTotalMag=t._pTotalMag;
    _pTotalPhi=t._pTotalPhi;
    _eTotal=t._eTotal;
    _pTotalMass=t._pTotalMass;
    _p1CosTheta=t._p1CosTheta;
    _p1Phi=t._p1Phi;
    _p1Mag=t._p1Mag;
    _p2CosTheta=t._p2CosTheta;
    _p2Phi=t._p2Phi;
    _p2Mag=t._p2Mag;
    _nTracks=t._nTracks;
    _nGoodTrkLoose=t._nGoodTrkLoose;
    _nGoodTrkTight=t._nGoodTrkTight;
    _p1EmcCandEnergy=t._p1EmcCandEnergy;
    _p2EmcCandEnergy=t._p2EmcCandEnergy;
    _p1PidHypo=t._p1PidHypo;
    _p2PidHypo=t._p2PidHypo;
    _totChargeLoose=t._totChargeLoose;
    _totChargeTight=t._totChargeTight;
    //#####################Lepton information############################################
    _leptLoose1hypo=t._leptLoose1hypo;
    _leptLoose1Charge=t._leptLoose1Charge;
    _leptLoose1p=t._leptLoose1p;
    _leptLoose2hypo=t._leptLoose2hypo;
    _leptLoose2Charge=t._leptLoose2Charge;
    _leptLoose2p=t._leptLoose2p;
    _leptTight1hypo=t._leptTight1hypo;
    _leptTight1Charge=t._leptTight1Charge;
    _leptTight1p=t._leptTight1p;
    _leptTight2hypo=t._leptTight2hypo;
    _leptTight2Charge=t._leptTight2Charge;
    _leptTight2p=t._leptTight2p;
    //#####################Calorimeter information############################################
    _eNeutralCosTh=t._eNeutralCosTh;
    _eNeutralPhi=t._eNeutralPhi;
    _eNeutralMag=t._eNeutralMag;
    _e1CosTheta=t._e1CosTheta;
    _e1Phi=t._e1Phi;
    _e1Mag=t._e1Mag;
    _e2CosTheta=t._e2CosTheta;
    _e2Phi=t._e2Phi;
    _e2Mag=t._e2Mag;
    //#####################Vertex and event shape#############################################
    _xPrimaryVtx=t._xPrimaryVtx;
    _yPrimaryVtx=t._yPrimaryVtx;
    _zPrimaryVtx=t._zPrimaryVtx;
    _probPrimaryVtx=t._probPrimaryVtx;
    _thrustCosTh=t._thrustCosTh;
    _thrustPhi=t._thrustPhi;
    _thrustMag=t._thrustMag;
    _R2=t._R2;
    //####################Particle numbers####################################################
    _nLeptonsLoose=t._nLeptonsLoose;
    _nLeptonsTight=t._nLeptonsTight;
    _neLoose=t._neLoose;
    _nmuLoose=t._nmuLoose;
    _npiLoose=t._npiLoose;
    _nKLoose=t._nKLoose;
    _npLoose=t._npLoose;
    _ngammaLoose=t._ngammaLoose;
    _npi0MergedLoose=t._npi0MergedLoose;
    _npi0Loose=t._npi0Loose;
    _nKsLoose=t._nKsLoose;
    _neTight=t._neTight;
    _nmuTight=t._nmuTight;
    _npiTight=t._npiTight;
    _nKTight=t._nKTight;
    _npTight=t._npTight;
    _ngammaTight=t._ngammaTight;
    _npi0MergedTight=t._npi0MergedTight;
    _npi0Tight=t._npi0Tight;
    _nKsTight=t._nKsTight;
//######################### Trigger and Skim information ###################################
    _bbqqDiscr=t._bbqqDiscr; 
    _BGFR2=t._BGFR2;
    _BunchT0DeltaDircTrk=t._BunchT0DeltaDircTrk;
    _BunchT0DeltaFF=t._BunchT0DeltaFF; 
    _BunchT0FinderErr=t._BunchT0FinderErr; 
    _BunchT0FitterErr=t._BunchT0FitterErr;
    _eCharShow=t._eCharShow; 
    _elecLoose1cm=t._elecLoose1cm;
    _elecLoose1l=t._elecLoose1l;
    _elecLoose2cm=t._elecLoose2cm; 
    _elecLoose2l=t._elecLoose2l; 
    _elecTight1cm=t._elecTight1cm;
    _elecTight1l=t._elecTight1l; 
    _elecTight2cm=t._elecTight2cm; 
    _elecTight2l=t._elecTight2l;
    _eNeutShow=t._eNeutShow;
    _L3OutBhabhaFlatOprValue=t._L3OutBhabhaFlatOprValue; 
    _L3OutBhabhaFlatValue=t._L3OutBhabhaFlatValue;
    _muonLoose1cm=t._muonLoose1cm; 
    _muonLoose1l=t._muonLoose1l; 
    _muonLoose2cm=t._muonLoose2cm; 
    _muonLoose2l=t._muonLoose2l; 
    _muonTight1cm=t._muonTight1cm; 
    _muonTight1l=t._muonTight1l; 
    _muonTight2cm=t._muonTight2cm; 
    _muonTight2l=t._muonTight2l;
    _pTotalScalar=t._pTotalScalar;
    _R2All=t._R2All;
    _sphericityAll=t._sphericityAll; 
    _TagLooseProton_maxCmsP=t._TagLooseProton_maxCmsP; 
    _TagTightProton_maxCmsP=t._TagTightProton_maxCmsP;
    _thrustCosThAll=t._thrustCosThAll; 
    _thrustMagAll=t._thrustMagAll; 
    _thrustPhiAll=t._thrustPhiAll; 
    _D3PS=t._D3PS;
    _Dmeson0=t._Dmeson0; 
    _Dsmeson0=t._Dsmeson0; 
    _Dstar0=t._Dstar0;
    _Dstar1=t._Dstar1; 
    _LumiBhabhaWeight=t._LumiBhabhaWeight;
    _nNeutralCands=t._nNeutralCands;
    _nNeutralCndLoose=t._nNeutralCndLoose;
    _nNeutralCndTight=t._nNeutralCndTight;
    _TagEtaPeppTwoBody_maxCmsPmaxCosT=t._TagEtaPeppTwoBody_maxCmsPmaxCosT;
    _TagEtaPrhogTwoBody_maxCmsPmaxCosT=t._TagEtaPrhogTwoBody_maxCmsPmaxCosT;
    _TagEtaTwoBody_maxCmsPmaxCosT=t._TagEtaTwoBody_maxCmsPmaxCosT;
    _TagKstpTwoBody_maxCmsPmaxCosT=t._TagKstpTwoBody_maxCmsPmaxCosT; 
    _TagKstTwoBody_maxCmsPmaxCosT=t._TagKstTwoBody_maxCmsPmaxCosT;
    _TagKTwoBody_maxCmsPmaxCosT=t._TagKTwoBody_maxCmsPmaxCosT;
    _TagK_S0TwoBody_maxCmsPmaxCosT=t._TagK_S0TwoBody_maxCmsPmaxCosT;
    _TagOmegaTwoBody_maxCmsPmaxCosT=t._TagOmegaTwoBody_maxCmsPmaxCosT; 
    _TagPhiTwoBody_maxCmsPmaxCosT=t._TagPhiTwoBody_maxCmsPmaxCosT;
    _TagPi0TwoBody_maxCmsPmaxCosT=t._TagPi0TwoBody_maxCmsPmaxCosT;
    _TagRhopTwoBody_maxCmsPmaxCosT=t._TagRhopTwoBody_maxCmsPmaxCosT; 
    _TagRhoTwoBody_maxCmsPmaxCosT=t._TagRhoTwoBody_maxCmsPmaxCosT;
}

std::ostream&  operator << (std::ostream& o, const PAFTagBase& a) { a.PrintOn(o); return o; }


void PAFTagBase::Init(TBranchObject *objBranch)
{
  TString branchName(objBranch->GetName());
  TBranch *subBranch;
  TObjArray *list = objBranch->GetListOfBranches();
  Int_t listLen = list->GetEntries();
  fBranchTable->Clear("nodelete");
  for (Int_t i=0; i<listLen; i++) {
    subBranch = (TBranch*) (*list)[i];
    TString t(subBranch->GetName());
    if (t.Index("_tag")<0) continue;
    fBranchTable->Add((TObject*)subBranch); // Add the branch to the table
  }
}

void PAFTagBase::SetEventNumber(UInt_t i)
{
  fEventNumber = i;
}

// Here come the BABAR style accessors

char PAFTagBase::getInt(Int_t &anInt,const char* key) const
{
    anInt = 0;
    static const TString prefix("_tag._");
    TString theKey = prefix + key;
    TBranch* branch = (TBranch*) fBranchTable->FindObject(theKey);
    if (branch != 0) {
	branch->GetEntry(fEventNumber);
	anInt = *((Int_t *) branch->GetAddress());
	return kTRUE;
    }
    else {
	cout << "TAG: Unknown integer key " << key << endl;
	return kFALSE;
    }
}

char PAFTagBase::getFloat(Float_t &aFloat,const char* key) const
{
    aFloat = 0.0;
    static const TString prefix("_tag._");
    TString theKey = prefix + key;
    TBranch* branch = (TBranch*) fBranchTable->FindObject(theKey);
    if (branch != 0) {
	branch->GetEntry(fEventNumber);
	aFloat = *((Float_t *) branch->GetAddress());
	return kTRUE;
    }
    else {
	cout << "TAG: Unknown float key " << key << endl;
	return kFALSE;
    }
}


char PAFTagBase::getBool(char &aBool,const char* key) const
{
    cerr << "PAFTagBase::getBool: Function not supported in PAF files." << endl;
    return kFALSE;
    //aBool = kFALSE;
    //VAbsBit &bit = TPico::Instance()->GetEventManager()->GetBit();
    //return bit.getBool(aBool,key);
}

char PAFTagBase::putBool( char aBool, const char* key)
{
    cerr << "PAFTagBase::putBool: Function not supported in PAF files." << endl;
    return kFALSE;
    //VAbsBit &bit = TPico::Instance()->GetEventManager()->GetBit();
    //return bit.putBool(aBool,key);
}

char PAFTagBase::putInt( Int_t aInt, const char* key) 
{
    cerr << "PAFTagBase::putInt: Function not supported in PAF files." << endl;
    return kFALSE;
    //VAbsBit &bit = TPico::Instance()->GetEventManager()->GetBit();
    //return bit.putInt(aInt,key);
}

char PAFTagBase::putFloat( Float_t aFloat, const char* key) 
{
    cerr << "PAFTagBase::putFloat: Function not supported in PAF files." << endl;
    return kFALSE;
    //VAbsBit &bit = TPico::Instance()->GetEventManager()->GetBit();
    //return bit.putFloat(aFloat,key);
}
