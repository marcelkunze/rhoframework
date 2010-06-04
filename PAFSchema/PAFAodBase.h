#ifndef PAFAODBASE_H
#define PAFAODBASE_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFAodBase								//
//                                                                      //
// Definition of the Analysis Object Data database			//
//                                                                      //
// Author: Marcel Kunze, Bochum University, March 1999			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iosfwd>
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#define MATRIXSIZE 28
#define NELEM_AODBASE 14

//  =================================================================
//  ===== PAFAodBase - Base Class for charged and neutral cands =====
//  =================================================================

class PAFAodBase {
    
public :
    
    // Empty Constructor
    PAFAodBase();
    
    // Copy Constructor
    PAFAodBase(PAFAodBase& other);
    
    virtual ~PAFAodBase();
    
    PAFAodBase* GetBase() { return this; }

    // Common member functions

    void SetCharge(Int_t q);
    Int_t GetCharge() const;
    
    // Set Data ( = BtaFitParams)
    
    void    SetPosition(TVector3);
    void    SetMomentum ( TVector3 );
    void    SetEnergy(Float_t);
    void    SetPositionError(Float_t *);
    void    SetMomentumError(Float_t *);
    void    SetErrorP7(Float_t *);
    void    SetMcPid(Int_t mcpid);
    void    SetMcIndex(Int_t no);
    void    SetMcConsistency(Float_t cons);
    
    void SetLorentzVector( TLorentzVector& v ) {
        _xmomentum=(Float_t) v.X();
        _ymomentum=(Float_t) v.Y();
        _zmomentum=(Float_t) v.Z();
        _energy=(Float_t) v.T();
    }
    
    virtual void    Reset();
    
    // GetData
    
    TVector3 GetPosition () const;
    TVector3 GetMomentum () const;
    TLorentzVector GetLorentzVector() const;
    Float_t GetEnergy() const;
    const Float_t* GetParams() const { return 0; }
    const Float_t* GetPositionError() const;
    const Float_t* GetMomentumError() const;
    const Float_t* GetErrorP7() const;
    const Float_t* GetCov() const { return 0; }
    Int_t  GetMcPid() const;    // hypo from MCtruth;			    
    Int_t  GetMcIndex() const; 
    Float_t  GetMcConsistency() const; 
    
    // pure virtual functions
    
    virtual Float_t GetSignificanceLevel(Int_t hypo)=0;
    virtual Float_t GetLikelihood(Int_t hypo)=0;
    
    virtual void SetSignificanceLevel(Int_t hypo, Float_t val)=0;
    virtual void SetLikelihood(Int_t hypo, Float_t val)=0;

    virtual void SetBitPatternOfDetectors(Int_t pat);
    virtual Int_t GetBitPatternOfDetectors();
 
    virtual void PrintOn(ostream &o) const;

protected:
    
    Float_t _xposition,		    // 1
	    _yposition,		    // 2
	    _zposition,		    // 3
	    _xmomentum,		    // 4
	    _ymomentum,		    // 5
	    _zmomentum,		    // 6
	    _energy,		    // 7
	    _errP7[MATRIXSIZE];	    // 8
    
    Float_t _mcCons;		    // 9
    Int_t   _mcHypo,		    // 10
	    _mcIndex;		    // 11
    
    Short_t _DetBitpattern;	    // 12
    Char_t  _charge;		    // 13

public:    
    ClassDef(PAFAodBase,2) //Kinematics info
};

std::ostream&  operator << (std::ostream& o, const PAFAodBase&);

#endif
