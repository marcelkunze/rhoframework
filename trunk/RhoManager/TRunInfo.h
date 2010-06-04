#ifndef TRUNINFO_H
#define TRUNINFO_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRunInfo								//
//                                                                      //
// Run info class in							//
//                                                                      //
// Author: Thorsten Brandt, Dresden University, Aug. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iosfwd>

#include "TObject.h"
#include "TString.h"

class TRunInfo : public TObject {
public :
    TRunInfo() {};
    virtual ~TRunInfo() {};
    TRunInfo(Int_t number);
    virtual Int_t Compare(const TObject *) const;
    virtual Bool_t IsSortable() const { return kTRUE; }

    void PrintOn( std::ostream& o=std::cout ) const;  

    const char* GetFile() { return fFileName; }
    const char* GetCluster() { return fCluster; }
    Int_t GetNumber() { return fNumber; }
    Int_t GetFirst() { return fFirstRun; }
    Int_t GetLast() { return fLastRun; }

    void SetBegin( UInt_t pos,UInt_t startMinor,UInt_t startMajor) {
	fFirstRun = pos; 
	fStartMinor = startMinor;
	fStartMajor = startMajor;
    }

    void SetEnd( UInt_t pos,UInt_t stopMinor,UInt_t stopMajor) {
	fLastRun = pos; 
	fStopMinor = stopMinor;
	fStopMajor = stopMajor;
    }

    void SetFilename(const char* t) { fFileName=t; }

    void SetCluster(const char* t) { fCluster=t; }


private :
    UInt_t fStartMinor,fStartMajor;
    UInt_t fStopMinor,fStopMajor;
    UInt_t fNumber;
    UInt_t fFirstRun,fLastRun;
    TString fFileName;
    TString fCluster;

public:
    ClassDef(TRunInfo,1) // Run info
};

std::ostream&  operator << (std::ostream& o, const TRunInfo&);

#endif
