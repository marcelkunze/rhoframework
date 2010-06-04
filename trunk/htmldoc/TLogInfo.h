#ifndef TLOGINFO_H
#define TLOGINFO_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TLogInfo								//
//                                                                      //
// Log info class							//
//                                                                      //
// Author: Thorsten Brandt, Dresden University, Aug. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "TObject.h"
#include "TObjString.h"
#include "TNamed.h"
#include "TDatime.h"
#include "TString.h"

class TLogReader;

class TLogInfo : public TNamed
{
  public :

    enum Run_Type{unknown=0,colliding=1,cosmics=2,stable=3,other=4,calibration=5};
    enum Shift_Type{none=0,day=1,swing=2,owl=3};

    TLogInfo(Int_t runno);
    TLogInfo() { } ;
    virtual ~TLogInfo();
  
  // Accessors

  Int_t GetRunNumber() { return fRunNumber; }; 
  void SetRunNumber(Int_t no);
  void SetDate(Int_t y,Int_t m, Int_t d);
  void SetStart(Int_t h,Int_t m);
  void SetStop(Int_t h,Int_t m);
  TDatime GetStartTime() { TDatime d; d.Set(fYear,fMonth,fDay,fStartHour,fStartMin,0); return d; };
  TDatime GetStopTime() { TDatime d; d.Set(fYear,fMonth,fDay,fStopHour,fStopMin,0); return d; };
  const char* GetObjyCollection() { return fCollection; }
  const char* GetOprProcspec() { return fProcspec; }
 
  Float_t GetSolenoidCurrent() { return fSolenoid; }
  Float_t GetIntLumi() { return fLumi; }
  Int_t   GetNL1Events() { return 0; } // for bw compatibility
  Int_t   GetNL3Events() { return 0; } // for bw compatibility
  Int_t   GetType() { return fRunType; }
  Int_t   GetRunType() { return fRunType; }
  Int_t   GetShiftType() { return fShiftType; }
  Int_t   GetIndex() { return 0; } 
  Float_t GetEnergyHER() { return fHer; } 					       
  Float_t GetEnergyLER() { return fLer; }
  Bool_t  HasKanga() { return fKanga; }
  Int_t    GetOprCons() { return fCons; }
  Int_t    GetOprVersion() { return fVersion; }
  Int_t   GetDay() { return fDay; }
  Int_t   GetYear() { return fYear; }
  Int_t   GetMonth() { return fMonth; }
  Int_t   GetStartHour() { return fStartHour; }
  Int_t   GetStartMin() { return fStartMin; }
  Int_t   GetStopHour() { return fStopHour; }
  Int_t   GetStopMin() { return fStopMin; }
 
  virtual Int_t Compare(const TObject* obj) const;
  virtual Bool_t IsSortable() const { return kTRUE; }
  // print 
  void PrintOn( std::ostream& o=std::cout ) const;

  private :
    
  // General Data
  Int_t fRunNumber;
  Float_t fLumi;

  // Info from IR2 - db 
  Int_t fYear;
  Byte_t fMonth,fDay;
  Byte_t fStartHour,fStartMin;
  Byte_t fStopHour,fStopMin;
  Byte_t fShiftType;
  Byte_t fRunType;
  Float_t fLer,fHer;
  Float_t fSolenoid;
     
  // Info from OPR - db
  TString fCollection,fProcspec;
  Bool_t fKanga;
  Byte_t fCons,fVersion;     

  friend class TLogReader;

 public:
  ClassDef(TLogInfo,1) // Logbook info
};

#endif
