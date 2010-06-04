#ifndef TCONDITIONS_H
#define TCONDITIONS_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TConditions	    							//
//                                                                      //
// Conditions database class in Rho (Schema of the BABAR experiment)	//
//                                                                      //
// Author: Marcel Kunze, RUB, Nov. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <iostream>
#include "TObject.h"
#include "TFile.h"
#include "RhoConditions/TRhoTime.h"

class TBtree;
class TTree;
class TBeams;
class PepEnergiesR;
class PepBeamSpotCalR;
class PepFillPatternR;
class PepBoostCalR;
class TDchCond;
class TSvtPidCalib;

class TConditions : public TObject
{
    
public:
    
    TConditions(const char *db="", const char *calibration="TwoTracks", const char *bcalibration="TwoTracksPHat");
    virtual ~TConditions();
    
    static void Authorize(const char* user=0, const char* passwd=0);
    Bool_t At(TRhoTime t);
    Bool_t At(ULong_t timestamp);
    TBeams* GetBeams() const { return fBeam; }
    TDchCond* GetDchCond() const { return fDchCond; }
    TSvtPidCalib* GetSvtPidCalib() const { return fSvtPidCalib; }
    void ReadBeams(Bool_t yesNo) { fReadBeams = yesNo; }
    void ReadPepEnergies(Bool_t yesNo) { fReadPepEnergies = yesNo; }
    void ReadPepBeamSpotCal(Bool_t yesNo) { fReadPepBeamSpotCal = yesNo; }
    void ReadPepFillPattern(Bool_t yesNo) { fReadPepFillPattern = yesNo; }
    void ReadPepBoostCal(Bool_t yesNo) { fReadPepBoostCal = yesNo; }
    void ReadDchCond(Bool_t yesNo) { fReadDchCond = yesNo; }
    void ReadSvtPidCalib(Bool_t yesNo) { fReadSvtPidCalib = yesNo; }

    void PrintOn( std::ostream&o=std::cout ) const;
    
protected:
    
private:
    Bool_t  fReadBeams,
	    fReadPepEnergies,
	    fReadPepBeamSpotCal,
	    fReadPepFillPattern,
	    fReadPepBoostCal,
	    fReadDchCond,
	    fReadSvtPidCalib;
    ULong_t fCurrentTime;

    TFile *fFile; //! Do not stream

    TBeams *fBeam; //! Do not stream
    TBtree *fBeamIndex; //! Do not stream
    TTree *fBeamTree; //! Do not stream

    PepEnergiesR *fPepEnergies; //! Do not stream
    TBtree *fPepEnergiesIndex; //! Do not stream
    TTree *fPepEnergiesTree; //! Do not stream

    PepBeamSpotCalR *fPepBeamSpotCal; //! Do not stream
    TBtree *fPepBeamSpotCalIndex; //! Do not stream
    TTree *fPepBeamSpotCalTree; //! Do not stream

    PepFillPatternR *fPepFillPattern; //! Do not stream
    TBtree *fPepFillPatternIndex; //! Do not stream
    TTree *fPepFillPatternTree; //! Do not stream

    PepBoostCalR *fPepBoostCal; //! Do not stream
    TBtree *fPepBoostCalIndex; //! Do not stream
    TTree *fPepBoostCalTree; //! Do not stream

    TDchCond *fDchCond; //! Do not stream
    TBtree *fDchCondIndex; //! Do not stream
    TTree *fDchCondTree; //! Do not stream
    
    TSvtPidCalib *fSvtPidCalib; //! Do not stream
    TBtree *fSvtPidCalibIndex; //! Do not stream
    TTree *fSvtPidCalibTree; //! Do not stream

public:
    ClassDef(TConditions,1)  // Interface to conditions database
};

//standalone print
std::ostream& operator << (std::ostream& o, const TConditions& );

class TCondKey : public TObject {
public:
    TCondKey(ULong_t when) { SetUniqueID(when); }
    virtual ~TCondKey() {}
    Bool_t IsSortable() const { return kTRUE; }
};


class RooCondInterval : public TObject {
public:
    RooCondInterval();
    virtual ~RooCondInterval();
    Bool_t IsSortable() const;
    virtual Int_t Compare(const TObject* other) const;
    int index() const { return fIndex; }
private:
    UInt_t _end;
    UInt_t fIndex;
public:
    ClassDef(RooCondInterval,1) // persistent interval for some conditions data
};

class TBranch;

/** This is the base class for all ROOT persistent conditions data.
* At the moment, it is simply a renaming of the ROOT TObject base
* class, but may be used later for decoupling from TObject.
*/
class RooCondData : public TObject {
public:
    RooCondData();
    virtual ~RooCondData();
    Stat_t Store(TBranch& where) const;
    
    ClassDef(RooCondData,1) // base class for ROOT persistent conditions data
};

#endif
