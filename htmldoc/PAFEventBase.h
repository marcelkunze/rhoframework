#ifndef PAFEVENTBASE_H
#define PAFEVENTBASE_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFEventBase								//
//                                                                      //
// Event header class in PAF(Pico Analysis Framework)			//
//                                                                      //
// Author: Sascha Berger, Bochum University, Feb. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iosfwd>
#include "TObject.h"
#include "TClonesArray.h"
#include "TMath.h"
#include "PAFSchema/PAFTagBase.h"
#include "RhoConditions/TRhoTime.h"

#define NELEM_EVENTBASE 7

class TTree;

//		---------------------
// 		-- Class Interface --
//		---------------------

class PAFEventBase : public TObject {
    
    //--------------------
    // Instance Members --
    //--------------------
    
private:
    UInt_t   _runno;		// 1. The run number
    Int_t    _eventno;		// 2. The event number
    UInt_t   _majorID;		// 3. Number of seconds since 01/01/1901
    UInt_t   _minorID;		// 4. Number of microsecs in the sec
    UInt_t    fEventNumber;	//!   Actual event
    UShort_t _nChargedTracks;	// 5. The number of prongs
    UShort_t _nNeutralTracks;	// 6. The number of neutrals
    
    PAFTagBase	_tag;		// The event TAG info
    
    TBranch *fBranches[NELEM_EVENTBASE];  //! not stored in root file
    
    Int_t  GetInt(TBranch *theBranch, const Int_t &value) const;	
    UInt_t GetUInt(TBranch *theBranch, const UInt_t &value) const;	
    UShort_t GetUShort(TBranch *theBranch, const UShort_t &value) const;	
    
public:
    PAFEventBase();
    virtual ~PAFEventBase();
    void    SetTree(TTree *theTree);
    void    SetEventNumber(Int_t);
    static void Reset();
    
    void	NewEvent();
    void	NewChargedTrack();
    void	NewNeutralTrack();
    void	AddTag(PAFTagBase*);
    void        AddChargedTrack();
    void        AddNeutralTrack();
    void 	SetRunNumber(UInt_t);
    void	SetOtherEventNumber(UInt_t);
    void        SetNumberOfChargedTracks(UInt_t n) ;
    void        SetNumberOfNeutralTracks(UInt_t n) ;
    void	SetTime(UInt_t id1,UInt_t id2=0) {SetMajorID(id1); SetMinorID(id2);}
    void	SetMajorID(UInt_t id) { _majorID = id; }
    void	SetMinorID(UInt_t id) { _minorID = id; }
    
    UInt_t      GetRunNumber() const;
    Int_t       GetEventNumber() const;
    Bool_t      Eof();
    TRhoTime    GetTime() const;
    UInt_t      GetMajorID() const;
    UInt_t      GetMinorID() const;
    UInt_t      GetNumberOfTracks() const;
    UInt_t      GetNumberOfChargedTracks() const;
    UInt_t      GetNumberOfNeutralTracks() const;
    TVector3    GetBoost() const;
    PAFTagBase& GetTag();
    void PrintOn( std::ostream& =std::cout) const;

    ClassDef(PAFEventBase,2)  // Event base class (Header and TAG)
};

//standalone print
std::ostream& operator << (std::ostream& o, const PAFEventBase& );

#endif
