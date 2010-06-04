#ifndef TLOGREADER_H
#define TLOGREADER_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLogReader								//
//                                                                      //
// Class to read the output of BABAR's ir2runs, oprruns and lumi	//
// The scanned output is gathered in TLogInfo objects which are stored	//
// in TRunDB								//
//                                                                      //
// Author: Thorsten Brandt, Dresden University, Aug. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <fstream>

#include "TObject.h"
#include "TDatime.h"
#include "TString.h"
#include "TObjString.h"
#include "TObjArray.h"

class TLogInfo;
class TFile;

class TLogReader : public TObject 
{
  public :
    TLogReader();    
    virtual ~TLogReader();
    void ReadFromFiles(const char *source_path="./",const char *dest="rundb.root");
    void CreateNew(const char *dest="rundb.root",const char *source_path=NULL);
    void Update(const char *dest="rundb.root",const char *source_path=NULL);
    void  WriteToFile(TFile  *file);
    TLogInfo* GetLog(Int_t n) const { return (TLogInfo*) (*fLogInfos)[n]; }
    TLogInfo* GetRun(Int_t n) const;

  private :
    TObjArray *fLogInfos;
};

class TOprEntry : public TObject 
{
  public :
    TOprEntry();
  virtual ~TOprEntry() { };
  Int_t GetRun() const { return fRunNumber; }
  const char* GetCollection() const { return fCollection; }
  Bool_t Done() const { return fDone; }
  Bool_t Kanga() const { return fKanga; }
  const char* GetProcspec() const { return fProcspec; }
  Int_t GetCons() const { return fCons; }
  Int_t GetVersion() const { return fVersion; }
  Bool_t ReadFromFile(std::ifstream &in);
  void PrintOn( std::ostream& o=std::cout ) const;

  private :
    Int_t fRunNumber;
    TString fCollection,fProcspec;
    Bool_t fDone,fKanga;
    Byte_t fCons,fVersion;
};
 

class TIr2Entry : public TObject
{
  public :
    TIr2Entry();
    Bool_t SetContent(char *str);
    virtual ~TIr2Entry();
    Int_t GetRun() const { return fRunNumber; }
    Int_t GetShiftType() const { return fShiftType; }
    Float_t GetLer() const { return fLer; }
    Float_t GetHer() const { return fHer; }
    Int_t  GetYear() const { return fYear; }
    Int_t  GetMonth() const { return fMonth; }
    Int_t  GetDay() const { return fDay; }
    Int_t  GetStartHour() const { return fStartHour; }
    Int_t  GetStartMin() const { return fStartMin; }
    Int_t  GetStopHour() const { return fStopHour; }
    Int_t  GetStopMin() const { return fStopMin; }   
    Int_t  GetRunType() const { return fRunType; }
    Float_t GetSolenoid() const { return fSolenoid; }
    virtual Int_t Compare(const TObject *other) const;
    virtual Bool_t IsSortable() const { return kTRUE; }
   void PrintOn( std::ostream& o=std::cout ) const;
    
  private :
      void PrintTime(Int_t t,std::ostream &o) const;
    Int_t fRunNumber;
    Int_t fYear;
    Byte_t fMonth,fDay;
    Byte_t fStartHour,fStartMin;
    Byte_t fStopHour,fStopMin;
    Int_t fShiftType;
    Int_t fRunType;
    Float_t fLer,fHer,fSolenoid;
};


#endif
