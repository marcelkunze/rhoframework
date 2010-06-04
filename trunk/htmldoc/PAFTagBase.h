#ifndef PAFTAGBASE_H
#define PAFTAGBASE_H
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

#include <iosfwd>
#include "TObject.h"
#include "TLorentzVector.h"
#include "RhoBase/VAbsBit.h"

#define NELEM_TAGBASE 138

class TBranch;
class TBranchObject;
class THashList;

class PAFTagBase : public VAbsBit {
    
private:
    UInt_t    fEventNumber; //!Actual event
    //#####################################################
    //###########storage data members######################
    //#####################################################
    Float_t _cmf1,_cmf2,_cmf3,_cmf4;
    // Tracking Information
    Float_t _pTotalCosTh,
	_pTotalPhi,
	_pTotalMag,
	_eTotal,
	_pTotalMass;
    Float_t _p1CosTheta,
	_p1Phi,
	_p1Mag,
	_p2CosTheta,
	_p2Phi,
	_p2Mag;
    Float_t _p1EmcCandEnergy,
	_p2EmcCandEnergy;
    Byte_t  _nTracks,
	_nGoodTrkLoose,
	_nGoodTrkTight;
    Char_t  _p1PidHypo,
	_p2PidHypo;
    Char_t  _totChargeLoose,
	_totChargeTight;
    // Lepton information
    Char_t  _leptLoose1hypo,
	_leptLoose2hypo;
    Char_t  _leptLoose1Charge,
	_leptLoose2Charge;
    Char_t  _leptTight1hypo,
	_leptTight2hypo;
    Char_t  _leptTight1Charge,
	_leptTight2Charge;
    Float_t _leptLoose1p,
	_leptLoose2p,
	_leptTight1p,
	_leptTight2p;
    // Calorimeter information
    Float_t _eNeutralCosTh,
	_eNeutralPhi,
	_eNeutralMag,
	_e1CosTheta,
	_e1Phi,
	_e1Mag,
	_e2CosTheta,
	_e2Phi,
	_e2Mag;
    // Vertex and event shape
    Float_t _xPrimaryVtx,
	_yPrimaryVtx,
	_zPrimaryVtx,
	_probPrimaryVtx;
    Float_t _R2;
    Float_t _thrustCosTh,
	_thrustPhi,
	_thrustMag;
    // Particle numbers
    Byte_t  _nLeptonsLoose,
	_nLeptonsTight,
	_neLoose,
	_neTight,
	_nmuLoose,
	_nmuTight,
	_npiLoose,
	_npiTight,
	_nKLoose,
	_nKTight,
	_npLoose,
	_npTight,
	_ngammaLoose,
	_ngammaTight,
	_npi0MergedLoose,
	_npi0MergedTight,
	_npi0Loose,
	_npi0Tight,
	_nKsLoose,
	_nKsTight;
    // Analysis bits
    UInt_t   _analysisbits;
    UInt_t   _analysisbits1;
    // Beam information
    Float_t _cmsEnergy,
	_xBeamSpot,
	_yBeamSpot,
	_zBeamSpot,
	_xBeamErr,
	_yBeamErr,
	_zBeamErr;
    // Trigger and Skim information (8.8.0)
    Float_t _bbqqDiscr, 
	_BGFR2,
	_BunchT0DeltaDircTrk,
	_BunchT0DeltaFF, 
	_BunchT0FinderErr, 
	_BunchT0FitterErr,
	_eCharShow, 
	_elecLoose1cm,
	_elecLoose1l,
	_elecLoose2cm, 
	_elecLoose2l, 
	_elecTight1cm,
	_elecTight1l, 
	_elecTight2cm, 
	_elecTight2l,
	_eNeutShow,
	_L3OutBhabhaFlatOprValue, 
	_L3OutBhabhaFlatValue,
	_muonLoose1cm, 
	_muonLoose1l, 
	_muonLoose2cm, 
	_muonLoose2l, 
	_muonTight1cm, 
	_muonTight1l, 
	_muonTight2cm, 
	_muonTight2l,
	_pTotalScalar,
	_R2All,
	_sphericityAll, 
	_TagLooseProton_maxCmsP, 
	_TagTightProton_maxCmsP,
	_thrustCosThAll, 
	_thrustMagAll, 
	_thrustPhiAll;
    Int_t _D3PS,
	_Dmeson0, 
	_Dsmeson0, 
	_Dstar0,
	_Dstar1, 
	_LumiBhabhaWeight,
	_nNeutralCands,
	_nNeutralCndLoose,
	_nNeutralCndTight,
	_TagEtaPeppTwoBody_maxCmsPmaxCosT,
	_TagEtaPrhogTwoBody_maxCmsPmaxCosT,
	_TagEtaTwoBody_maxCmsPmaxCosT,
	_TagKstpTwoBody_maxCmsPmaxCosT, 
	_TagKstTwoBody_maxCmsPmaxCosT,
	_TagKTwoBody_maxCmsPmaxCosT,
	_TagK_S0TwoBody_maxCmsPmaxCosT,
	_TagOmegaTwoBody_maxCmsPmaxCosT, 
	_TagPhiTwoBody_maxCmsPmaxCosT,
	_TagPi0TwoBody_maxCmsPmaxCosT,
	_TagRhopTwoBody_maxCmsPmaxCosT, 
	_TagRhoTwoBody_maxCmsPmaxCosT;   
    
    TBranch *fBranches[NELEM_TAGBASE];  //! not stored in root file
    THashList *fBranchTable;		//! Branch cache
    
    Float_t GetFloat(TBranch *theBranch, const Float_t &value) const;
    Short_t GetShort(TBranch *theBranch,const Short_t &value) const ;
    Char_t GetChar(TBranch *theBranch, const Char_t &value) const;
    Byte_t GetByte(TBranch *theBranch, const Byte_t &value) const;
    Int_t GetInt(TBranch *theBranch, const Int_t &value) const;					      
    UInt_t GetUInt(TBranch *theBranch, const UInt_t &value) const;	
    
public:
    //Constructor
    PAFTagBase();
    PAFTagBase(const PAFTagBase& );
    //Destructor
    virtual ~PAFTagBase();
    void Init(TBranchObject *);
    void SetEventNumber(UInt_t i);
    
    //##################################################################
    //##################fill & gt operations############################
    //##################################################################
    
    // Beam infromation
    void	SetCMFrame(TLorentzVector);
    void	SetCMSEnergy(Float_t);
    void	SetBeamSpot(TVector3);
    void	SetBeamError(TVector3);
    TLorentzVector GetCMFrame() const;
    Float_t	GetCMSEnergy() const;
    TVector3	GetBeamSpot() const;
    TVector3	GetBeamError() const;
    // Tracking information
    void	SetNumberOfTracks(Int_t);
    void	SetNumberOfGoodTracksLoose(Int_t);
    void	SetNumberOfGoodTracksTight(Int_t);
    void	SetTotalMomentum(TVector3);
    void	SetTotalEnergy(Float_t);
    void	SetTotalMass(Float_t);
    void	SetPrimaryTrackMomentum(TVector3);
    void	SetPrimaryTrackShowerEnergy(Float_t);
    void	SetPrimaryTrackHypothesis(Int_t);
    void	SetSecondaryTrackMomentum(TVector3);
    void	SetSecondaryTrackShowerEnergy(Float_t);
    void	SetSecondaryTrackHypothesis(Int_t);
    void	SetTotalChargeLoose(Int_t);
    void	SetTotalChargeTight(Int_t);
    virtual Int_t	GetNumberOfTracks() const ;
    virtual Int_t	GetNumberOfGoodTracksLoose() const ;
    virtual Int_t	GetNumberOfGoodTracksTight() const ;
    virtual TVector3    GetTotalMomentum() const;
    virtual Float_t	GetTotalEnergy() const ;
    virtual Float_t	GetTotalMass() const ;
    virtual TVector3    GetPrimaryTrackMomentum() const;
    virtual Float_t	GetPrimaryTrackShowerEnergy() const ;
    virtual Int_t	GetPrimaryTrackHypothesis() const ;
    virtual TVector3    GetSecondaryTrackMomentum() const;
    virtual Float_t	GetSecondaryTrackShowerEnergy() const ;
    virtual Int_t	GetSecondaryTrackHypothesis() const ;
    virtual Int_t	GetTotalChargeLoose() const ;
    virtual Int_t	GetTotalChargeTight() const ;
    // Lepton Information
    void	SetPrimaryLeptonHypothesisLoose(Int_t);
    void	SetSecondaryLeptonHypothesisLoose(Int_t);
    void	SetPrimaryLeptonHypothesisTight(Int_t);
    void	SetSecondaryLeptonHypothesisTight(Int_t);
    virtual Int_t	GetPrimaryLeptonHypothesisLoose() const ;
    virtual Int_t	GetSecondaryLeptonHypothesisLoose() const ;
    virtual Int_t	GetPrimaryLeptonHypothesisTight() const ;
    virtual Int_t	GetSecondaryLeptonHypothesisTight() const ;
    void	SetPrimaryLeptonChargeLoose(Int_t);
    void	SetSecondaryLeptonChargeLoose(Int_t);
    void	SetPrimaryLeptonChargeTight(Int_t);
    void	SetSecondaryLeptonChargeTight(Int_t);
    virtual Int_t	GetPrimaryLeptonChargeLoose() const ;
    virtual Int_t	GetSecondaryLeptonChargeLoose() const ;
    virtual Int_t	GetPrimaryLeptonChargeTight() const ;
    virtual Int_t	GetSecondaryLeptonChargeTight() const ;
    void	SetPrimaryLeptonMomentumLoose(Float_t);
    void	SetSecondaryLeptonMomentumLoose(Float_t);
    void	SetPrimaryLeptonMomentumTight(Float_t);
    void	SetSecondaryLeptonMomentumTight(Float_t);
    virtual Float_t	GetPrimaryLeptonMomentumLoose() const ;
    virtual Float_t	GetSecondaryLeptonMomentumLoose() const ;
    virtual Float_t	GetPrimaryLeptonMomentumTight() const ;
    virtual Float_t	GetSecondaryLeptonMomentumTight() const ;
    // Calorimeter information
    void	SetTotalNeutralEnergy(TVector3);
    void	SetHighestEnergyShower(TVector3);
    void	SetSecondHighestEnergyShower(TVector3);
    virtual TVector3    GetTotalNeutralEnergy() const;
    virtual TVector3    GetHighestEnergyShower() const;
    virtual TVector3    GetSecondHighestEnergyShower() const;
    // Vertex and shape
    void	SetPrimaryVertex(TVector3, Float_t p=0.0);
    void	SetPrimaryVertexProbability(Float_t);
    void	SetThrust(TVector3);
    void	SetR2(Float_t); // Ratio of second to zero-th FW moments
    virtual TVector3	GetPrimaryVertex() const;
    virtual Float_t	GetPrimaryVertexProbability() const;
    virtual TVector3	GetThrust() const;
    virtual Float_t	GetR2() const;    
    // Particle numbers
    void	SetLeptonsLoose(Int_t);
    void	SetElectronsLoose(Int_t);
    void	SetMuonsLoose(Int_t);
    void	SetPionsLoose(Int_t);
    void	SetKaonsLoose(Int_t);
    void	SetProtonsLoose(Int_t);
    void	SetGammasLoose(Int_t);
    void	SetMergedPi0Loose(Int_t);
    void	SetPi0Loose(Int_t);
    void	SetKsLoose(Int_t);
    void	SetK0sLoose(Int_t n) { SetKsLoose(n); }
    void	SetLeptonsTight(Int_t);
    void	SetElectronsTight(Int_t);
    void	SetMuonsTight(Int_t);
    void	SetPionsTight(Int_t);
    void	SetKaonsTight(Int_t);
    void	SetProtonsTight(Int_t);
    void	SetGammasTight(Int_t);
    void	SetMergedPi0Tight(Int_t);
    void	SetPi0Tight(Int_t);
    void	SetKsTight(Int_t);
    void	SetK0sTight(Int_t n) { SetKsTight(n); }
    virtual Int_t	GetLeptonsLoose() const ;
    virtual Int_t	GetElectronsLoose() const ;
    virtual Int_t	GetMuonsLoose() const ;
    virtual Int_t	GetPionsLoose() const ;
    virtual Int_t	GetKaonsLoose() const ;
    virtual Int_t	GetProtonsLoose() const ;
    virtual Int_t	GetGammasLoose() const ;
    virtual Int_t	GetMergedPi0Loose() const ;
    virtual Int_t	GetPi0Loose() const ;
    virtual Int_t	GetKsLoose() const ;
    virtual Int_t	GetLeptonsTight() const ;
    virtual Int_t	GetElectronsTight() const ;
    virtual Int_t	GetMuonsTight() const ;
    virtual Int_t	GetPionsTight() const ;
    virtual Int_t	GetKaonsTight() const ;
    virtual Int_t	GetProtonsTight() const ;
    virtual Int_t	GetGammasTight() const ;
    virtual Int_t	GetMergedPi0Tight() const ;
    virtual Int_t	GetPi0Tight() const ;
    virtual Int_t	GetKsTight() const ;
    // Trigger and Skim information
    void	SetBbqqDiscr(Float_t); 
    void	SetBGFR2(Float_t);
    void	SetBunchT0DeltaDircTrk(Float_t);
    void	SetBunchT0DeltaFF(Float_t); 
    void	SetBunchT0FinderErr(Float_t); 
    void	SetBunchT0FitterErr(Float_t);
    void	SetECharShow(Float_t); 
    void	SetElecLoose1cm(Float_t);
    void	SetElecLoose1l(Float_t);
    void	SetElecLoose2cm(Float_t); 
    void	SetElecLoose2l(Float_t); 
    void	SetElecTight1cm(Float_t);
    void	SetElecTight1l(Float_t); 
    void	SetElecTight2cm(Float_t); 
    void	SetElecTight2l(Float_t);
    void	SetENeutShow(Float_t);
    void	SetL3OutBhabhaFlatOprValue(Float_t); 
    void	SetL3OutBhabhaFlatValue(Float_t);
    void	SetMuonLoose1cm(Float_t); 
    void	SetMuonLoose1l(Float_t); 
    void	SetMuonLoose2cm(Float_t); 
    void	SetMuonLoose2l(Float_t); 
    void	SetMuonTight1cm(Float_t); 
    void	SetMuonTight1l(Float_t); 
    void	SetMuonTight2cm(Float_t); 
    void	SetMuonTight2l(Float_t);
    void	SetPTotalScalar(Float_t);
    void	SetR2All(Float_t);
    void	SetSphericityAll(Float_t); 
    void	SetTagLooseProton_maxCmsP(Float_t); 
    void	SetTagTightProton_maxCmsP(Float_t);
    void	SetThrustCosThAll(Float_t); 
    void	SetThrustMagAll(Float_t); 
    void	SetThrustPhiAll(Float_t);
    
    virtual Float_t	GetBbqqDiscr() const; 
    virtual Float_t	GetBGFR2() const;
    virtual Float_t	GetBunchT0DeltaDircTrk() const;
    virtual Float_t	GetBunchT0DeltaFF() const; 
    virtual Float_t	GetBunchT0FinderErr() const; 
    virtual Float_t	GetBunchT0FitterErr() const;
    virtual Float_t	GetECharShow() const; 
    virtual Float_t	GetElecLoose1cm() const;
    virtual Float_t	GetElecLoose1l() const;
    virtual Float_t	GetElecLoose2cm() const; 
    virtual Float_t	GetElecLoose2l() const; 
    virtual Float_t	GetElecTight1cm() const;
    virtual Float_t	GetElecTight1l() const; 
    virtual Float_t	GetElecTight2cm() const; 
    virtual Float_t	GetElecTight2l() const;
    virtual Float_t	GetENeutShow() const;
    virtual Float_t	GetL3OutBhabhaFlatOprValue() const; 
    virtual Float_t	GetL3OutBhabhaFlatValue() const;
    virtual Float_t	GetMuonLoose1cm() const; 
    virtual Float_t	GetMuonLoose1l() const; 
    virtual Float_t	GetMuonLoose2cm() const; 
    virtual Float_t	GetMuonLoose2l() const; 
    virtual Float_t	GetMuonTight1cm() const; 
    virtual Float_t	GetMuonTight1l() const; 
    virtual Float_t	GetMuonTight2cm() const; 
    virtual Float_t	GetMuonTight2l() const;
    virtual Float_t	GetPTotalScalar() const;
    virtual Float_t	GetR2All() const;
    virtual Float_t	GetSphericityAll() const; 
    virtual Float_t	GetTagLooseProton_maxCmsP() const; 
    virtual Float_t	GetTagTightProton_maxCmsP() const;
    virtual Float_t	GetThrustCosThAll() const; 
    virtual Float_t	GetThrustMagAll() const; 
    virtual Float_t	GetThrustPhiAll() const; 
    
    void	SetD3PS(Int_t);
    void	SetDmeson0(Int_t); 
    void	SetDsmeson0(Int_t); 
    void	SetDstar0(Int_t);
    void	SetDstar1(Int_t); 
    void	SetLumiBhabhaWeight(Int_t);
    void	SetNumberOfNeutralCands(Int_t);
    void	SetNumberOfNeutralCndLoose(Int_t);
    void	SetNumberOfNeutralCndTight(Int_t);
    void	SetTagEtaPeppTwoBody_maxCmsPmaxCosT(Int_t);
    void	SetTagEtaPrhogTwoBody_maxCmsPmaxCosT(Int_t);
    void	SetTagEtaTwoBody_maxCmsPmaxCosT(Int_t);
    void	SetTagKstpTwoBody_maxCmsPmaxCosT(Int_t); 
    void	SetTagKstTwoBody_maxCmsPmaxCosT(Int_t);
    void	SetTagKTwoBody_maxCmsPmaxCosT(Int_t);
    void	SetTagK_S0TwoBody_maxCmsPmaxCosT(Int_t);
    void	SetTagOmegaTwoBody_maxCmsPmaxCosT(Int_t); 
    void	SetTagPhiTwoBody_maxCmsPmaxCosT(Int_t);
    void	SetTagPi0TwoBody_maxCmsPmaxCosT(Int_t);
    void	SetTagRhopTwoBody_maxCmsPmaxCosT(Int_t); 
    void	SetTagRhoTwoBody_maxCmsPmaxCosT(Int_t);
    
    virtual Int_t	GetD3PS() const; 
    virtual Int_t	GetDmeson0() const; 
    virtual Int_t	GetDsmeson0() const; 
    virtual Int_t	GetDstar0() const; 
    virtual Int_t	GetDstar1() const; 
    virtual Int_t	GetLumiBhabhaWeight() const; 
    virtual Int_t	GetNumberOfNeutralCands() const; 
    virtual Int_t	GetNumberOfNeutralCndLoose() const; 
    virtual Int_t	GetNumberOfNeutralCndTight() const; 
    virtual Int_t	GetTagEtaPeppTwoBody_maxCmsPmaxCosT() const; 
    virtual Int_t	GetTagEtaPrhogTwoBody_maxCmsPmaxCosT() const; 
    virtual Int_t	GetTagEtaTwoBody_maxCmsPmaxCosT() const; 
    virtual Int_t	GetTagKstpTwoBody_maxCmsPmaxCosT() const; 
    virtual Int_t	GetTagKstTwoBody_maxCmsPmaxCosT() const; 
    virtual Int_t	GetTagKTwoBody_maxCmsPmaxCosT() const; 
    virtual Int_t	GetTagK_S0TwoBody_maxCmsPmaxCosT() const; 
    virtual Int_t	GetTagOmegaTwoBody_maxCmsPmaxCosT() const; 
    virtual Int_t	GetTagPhiTwoBody_maxCmsPmaxCosT() const; 
    virtual Int_t	GetTagPi0TwoBody_maxCmsPmaxCosT() const; 
    virtual Int_t	GetTagRhopTwoBody_maxCmsPmaxCosT() const; 
    virtual Int_t	GetTagRhoTwoBody_maxCmsPmaxCosT() const; 

    // Analysis bits
    void SetAnalysisBits(UInt_t b=0, UInt_t b1=0);
    UInt_t GetAnalysisBits() const ;
    UInt_t GetAnalysisBits1() const ;
    
    // PAF style accessors
    
    virtual Bool_t GetTrigger(const char* key) const { char aBool; getBool(aBool,key); return aBool; }
    virtual Int_t GetInt(const char* key) const { Int_t anInt; getInt(anInt,key); return anInt; }
    virtual Float_t GetFloat(const char* key) const { Float_t aFloat; getFloat(aFloat,key); return aFloat; }
    virtual Bool_t GetBool(const char* key) const { char aBool; getBool(aBool,key); return aBool; }
    
    // BABAR style accessors
    
    virtual char getInt(Int_t &anInt,const char* key) const;
    virtual char getFloat(Float_t &aFloat,const char* key) const;
    virtual char getBool(char &aBool,const char* key) const;
    
    virtual char putBool( char, const char* );
    virtual char putInt( Int_t, const char* );
    virtual char putFloat( Float_t, const char* );
    
    //###################################################################    
    
    void operator =(PAFTagBase&);
    
    ClassDef(PAFTagBase,1) //PAF implementation of the TAG
	
};      

// standalone print
std::ostream&  operator <<(std::ostream& o, const PAFTagBase&);

#endif
