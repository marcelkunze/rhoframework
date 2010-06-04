#ifndef TRUNDB_H
#define TRUNDB_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRunDB								//
//                                                                      //
// Run Database class							//
//                                                                      //
// Author: Thorsten Brandt, Dresden University, Aug. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <iostream>

#include <stdlib.h>
#include "TObject.h"
#include "TString.h"

class TObjArray;
class TRunCollection;
class TFile;
class TLogInfo;
class TRunInfo;

class TRunDB : public TObject
{
public:
    
    TRunDB(const char* directory = 0, const char* cluster = "system",
	UInt_t maxFiles = 1000);
    
    virtual ~TRunDB();
    
    void SetCluster(const char* name);
    void AddCluster(const char* indexFilePath);
    void Update(const char* clustername);
    void AddRunsToCollection(TRunCollection *theCol,Int_t runFrom, Int_t runTo=0);
    Bool_t LoadLogbook(const char* filename = "rundb.root");
    void PrintAllRuns(std::ostream &o);
    void PrintMissingRuns(std::ostream &o,Int_t rmin, Int_t rmax);
    TLogInfo* GetLogInfo(Int_t runnumber);
    TRunInfo* GetRunInfo(Int_t runnumber);
    
protected :
    
    void ScanFile(const char *filename,TObjArray *list, Int_t &nevts, Int_t &firstRun);
    void getDirContent(const char* subdir, TObjArray *filenameList);
    
    TObjArray *fFileList; //!
    TObjArray *fRunList; //!
    TString fCurrentCluster; //!
    TFile *fLogbookFile; //!
    Bool_t fKanga; //!
public:
    ClassDef(TRunDB,1) // T run database
};

#endif
