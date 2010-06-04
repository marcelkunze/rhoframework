#ifndef TPARAMETERMANAGER_H
#define TPARAMETERMANAGER_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TParameterManager							//
//                                                                      //
// Parameter manager					    		//
//                                                                      //
// Author: Marcel Kunze, Bochum University, Nov. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"
#include "RhoBase/TParm.h"

class TParameterManager : public TParm {
    
public:
    TParameterManager(const char* file="",const char* fmode= "RECREATE");
    virtual ~TParameterManager();               
    Bool_t SetParmFile(const char* filename,const char* mode);
    Bool_t ReadParm(const char* filename);
    void WriteParm(const char* filename);
    Bool_t SetDir( const char* path= "/" );
    const char* GetDir() const;
    void ClearAllData() {}
    void SetParm(const char* p,Bool_t b,const char *t=0,TObject *x=0) { TParm::Set(p,b,t,x); }
    void SetParm(const char* p,Int_t i,const char *t=0,TObject *x=0) { TParm::Set(p,i,t,x); }
    void SetParm(const char* p,Double_t d,const char *t=0,TObject *x=0) { TParm::Set(p,d,t,x); }
    void SetParm(const char* p,const char* s,const char *t=0,TObject *x=0) { TParm::Set(p,s,t,x); }
    void SetBoolParm(const char* p,Bool_t b,const char *t=0,TObject *x=0) { TParm::Set(p,b,t,x); }
    void SetIntParm(const char* p,Int_t i,const char *t=0,TObject *x=0) { TParm::Set(p,i,t,x); }
    void SetDoubleParm(const char* p,Double_t d,const char *t=0,TObject *x=0) { TParm::Set(p,d,t,x); }
    void SetStringParm(const char* p,const char* s,const char *t=0,TObject *x=0) { TParm::Set(p,s,t,x); }
    Bool_t GetBoolParm(const char* p,TObject *x=0) { return TParm::GetBool(p,x); }
    Int_t GetIntParm(const char* p,TObject *x=0) { return TParm::GetInt(p,x); }
    Double_t GetDoubleParm(const char* p,TObject *x=0) { return TParm::GetDouble(p,x); }
    TString GetStringParm(const char* p,TObject *x=0) { return TParm::GetString(p,x); }
    void AddNewParm(const char* p,Bool_t b,TObject *x=0) { TParm::Set(p,b,"New bool parm",x); }
    void AddNewParm(const char* p,Int_t i,TObject *x=0) { TParm::Set(p,i,"New int parm",x); }
    void AddNewParm(const char* p,Double_t d,TObject *x=0) { TParm::Set(p,d,"New double parm",x); }
    void AddNewParm(const char* p,const char* s,TObject *x=0) { TParm::Set(p,s,"New string parm",x); }
    Double_t GetParmValue(const char* p,TObject *x=0) { return TParm::GetParm(p,x); }
    void ScanCommandLine(int argc, char **argv);
    void SetParameters(int argc, char **argv,const char* module=0);
    void PrintParm(std::ostream &o,TObject *x=0) const;
    void PrintOn( std::ostream& o=std::cout ,TObject *x=0) const;
    void PrintUsage( std::ostream& o=std::cout );
    void SyntaxError(const char* message);
    
private:    
    TString fName;
    TString fMode;
    TFile*  fFile; //! Do not stream
    TParm* ArgSearch(const char *);
    
    TString fProgName;
    
public:
    ClassDef(TParameterManager,1) // Parameter manager
};      

// standalone print
std::ostream&  operator << (std::ostream& o, const TParameterManager&);

#endif

