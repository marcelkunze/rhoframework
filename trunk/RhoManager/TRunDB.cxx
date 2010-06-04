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

#include <stdio.h>

#include "TBranch.h"
#include "TNetFile.h"
#include "TKey.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TTree.h"
#include "TSystem.h"

#ifdef RUNDBUPDATE
#include "PAFSchema/PAFEventBase.h"
#include "KangaSchema/EidInfoR.h"
#endif

#include "RhoBase/TRho.h"

#include "RhoManager/TRunCollection.h"
#include "RhoManager/TRunInfo.h"
#include "RhoManager/TFileInfo.h"
#include "RhoManager/TLogInfo.h"
#include "RhoManager/TRunDB.h"

ClassImp(TRunDB)

#include <iostream>
#include <iomanip>
using namespace std;

TRunDB::TRunDB(const char* directory, const char* cluster, UInt_t maxFiles)
{
    fFileList = new TObjArray(maxFiles);
    fRunList = new TObjArray(maxFiles);
    fCurrentCluster = cluster;
    fLogbookFile=0;
    fKanga = kFALSE;
}

TRunDB::~TRunDB()
{
    fFileList->Delete();
    fRunList->Delete();
    delete fFileList;
    delete fRunList;
}

void
TRunDB::PrintAllRuns(std::ostream &o)
{
    o << "Contents of Cluster \'" << fCurrentCluster.Data() << "\'" << endl;
    o << "-----------------------------------------------------------------" << endl;
    o << "RUN     NEVENTS    LUM    FILE " << endl;
    Int_t n = fRunList->GetEntries();
    Int_t allevts=0;
    Float_t allLumi=0;
    for (Int_t i=0; i<n; i++) {
	TRunInfo *r = (TRunInfo*) (*fRunList)[i];
	o << r->GetNumber() << "   " << setw(6) << (1+r->GetLast()-r->GetFirst() );
	allevts+=1+r->GetLast()-r->GetFirst();
	TLogInfo *l = GetLogInfo(r->GetNumber());
	Float_t lum=0;
	if (l!=0) { lum = l->GetIntLumi(); }; 
	if (lum<0) { lum=0; }
	if (l!=0) { allLumi+=lum; o << "   " <<  setw(6) << setiosflags(ios::fixed) << setprecision(1) << lum; } else { o << "   ????  "; }
	o << "    " << r->GetFile() << endl;
    } 
    o << "-----------------------------------------------------------------" << endl;
    o << " Total Number of Events : " << allevts << endl;
    o << " Total Luminosity (1/pb) : " << allLumi/1000.0 << endl;
    o << "-----------------------------------------------------------------" << endl;
    
}


void 
TRunDB::PrintMissingRuns(std::ostream &o,Int_t rmin, Int_t rmax)
{
  TLogInfo logBookEntry;
  if (fLogbookFile==0) { LoadLogbook(); }
  TList *dirkeyList = fLogbookFile->GetListOfKeys();
  TListIter dirIter(dirkeyList);
  TKey *dirKey;
  o << "     RUN CONS VER DATE    SHIFT  STARTT  NEVENTS STATUS     PROCSPEC " << endl;
  while ( dirKey=(TKey*) dirIter.Next()  ) {
    TDirectory  *theDir  = (TDirectory*)  dirKey->ReadObj();
    TKey *k;
    TList *keyList = theDir->GetListOfKeys();
    TListIter iter(keyList);
    while ( k=(TKey*) iter.Next()  ) {
      Int_t n;
      sscanf((char*) k->GetName(),"r%i",&n);
      if ((n>=rmin) && (n<=rmax)) {
	TRunInfo *r = GetRunInfo(n);
	if (r==NULL) {  
	  // run is missing
	  k->Read(&logBookEntry);
	  if ( (TString(logBookEntry.GetObjyCollection()).Length()>0) && 
	       (TString(logBookEntry.GetRunType())==TLogInfo::colliding) ) {
	    Int_t day = logBookEntry.GetDay();
	    Int_t month = logBookEntry.GetMonth();
	    Int_t year = logBookEntry.GetYear();
	    Int_t hr =  logBookEntry.GetStartHour();
	    Int_t min = logBookEntry.GetStartMin();
	    o << "    " << n << setw(6) << "    ?   ? ";
	    if (year<10) o << "0"; o << year;
	    if (month<10) o << "0"; o << month;
	    if (day<10) o << "0"; o << day;
	    o << "  ?????  ";
	    if (hr<10) o << "0"; o << hr << ":";
	    if (min<10) o << "0"; o << min << "   ";
	    o << setw(6) << "  ????? done       " << logBookEntry.GetOprProcspec();
	    o << endl << "        /groups/isPhysicsEvents" << logBookEntry.GetObjyCollection() << endl;
	  }
	}
      }
    }
  }
}   



Bool_t
TRunDB::LoadLogbook(const char* filename)
{
    if (fLogbookFile!=0) { delete fLogbookFile; fLogbookFile=NULL; }
    TString t = TString(TRho::Instance()->GetEventStore()) + "/" + filename;
    if (t.Index("root:")>=0) TRho::Instance()->Authorize();
    fLogbookFile = TFile::Open(t); 
    if (!fLogbookFile->IsOpen()) {
	cout << " *** ERROR in Run-DB : Could not find File '" << filename;
	cout << "' for electronic Logbook" << endl;
	delete fLogbookFile;  fLogbookFile=NULL; 
	return kFALSE;
    } 
    return kTRUE;
}

TLogInfo*
TRunDB::GetLogInfo(Int_t runnumber)
{
    TLogInfo *l = 0;
    if (fLogbookFile==0) { LoadLogbook(); }
    if (fLogbookFile!=0) {
	char str[60];
	sprintf(str,"d%i00/r%i",runnumber/100,runnumber);
	l = (TLogInfo*) fLogbookFile->Get(str);
    }
    return l;
}

TRunInfo* 
TRunDB::GetRunInfo(Int_t n)
{
    if (fRunList==0) { return NULL; }
    TRunInfo tmp(n);
    Int_t pos = fRunList->BinarySearch(&tmp);
    if (pos<0) { return NULL; }
    return (TRunInfo*) ((*fRunList)[pos]);
}

void
TRunDB::SetCluster(const char* name)
{
    TString indexPath;
    indexPath.Append(TRho::Instance()->GetEventStore());
    indexPath.Append("/");
    indexPath.Append(name);
    fCurrentCluster = name;
    AddCluster(indexPath);
}

void
TRunDB::AddCluster(const char* indexFilePath)
{
    TString indexFileName;
    indexFileName.Append(indexFilePath);
    indexFileName.Append("/Index.root");
    //static TFile::Open returns either TFile or TNetFile depending on fcurrentFilename
    if (indexFileName.Index("root:")>=0) TRho::Instance()->Authorize();
    TFile* indexFile=TFile::Open(indexFileName.Data());
    if (!indexFile->IsOpen()) {
      cerr << "TRunDB: Index file " << indexFileName << " not found !  " << endl;
      return;
      //cerr << "TRunDB: Trying to update the index..." << endl;
      //Update(indexFilePath);
      //indexFile=TFile::Open(indexFileName.Data());
      //if (!indexFile->IsOpen()) {
      //  cerr << "TRunDB: Update of Index failed !" << endl;
      //  abort();
      //}
    }

    cout << "TRunDB: Reading " << indexFileName.Data() << "; ";
    
    // Read in the run and file list from the index file
    // Merge into fRunList and fFileList
    
    TKey *k1 = indexFile->GetKey("runIndex");
    TKey *k2 = indexFile->GetKey("fileIndex");
    TObjArray *rList = new TObjArray(1000); // This is just a guess...
    TObjArray *fList = new TObjArray(1000);
    k1->Read(rList);
    k2->Read(fList);
    fRunList->AddAll(rList);
    fFileList->AddAll(fList);
    cout << rList->GetEntries() << " runs in " << fFileList->GetEntries() << " files" << endl; 
    delete rList, fList;
    
    indexFile->Close();
    
    // Sort the lists
    
    fRunList->Sort(0);
    fFileList->Sort(0);
    
}

void
TRunDB::Update(const char* clustername)
{
    TObjArray *theCurrentFiles = new TObjArray();
    TObjArray *newList = new TObjArray();
    TString pafroot(TRho::Instance()->GetEventStore());
    TString path(clustername);
    if (path.Index(pafroot)<0) path = pafroot +  "/"  + path;
    getDirContent(path,theCurrentFiles);
    for (Int_t i=0; i<theCurrentFiles->GetEntries(); i++) {
	TObjString *fname = (TObjString*) (*theCurrentFiles)[i];
	TFileInfo tmp(fname->String().Data());
	Int_t pos=-1;
	if (fFileList->GetEntries()>0) { pos = fFileList->BinarySearch(&tmp); }
	if (pos<0) {
	    Int_t nevts;
	    Int_t firstRun;
	    
	    // Change to relative name, Oct.99, MK
	    // A single Kanga file already carries the full name
	    TString s(fname->String());
	    if (s.Index(".root")>=0) {
		fKanga = kTRUE;
		ScanFile(s.Data(),fRunList,nevts,firstRun);
		TFileInfo *f = new TFileInfo(s.Data(),nevts,firstRun);
		newList->Add(f);
	    } else {
		fKanga = kFALSE;
		TString s2 = s + "Tag.root";
		ScanFile(s2.Data(),fRunList,nevts,firstRun);
		TFileInfo *f = new TFileInfo(s.Data(),nevts,firstRun);
		newList->Add(f);
	    }
	}
    }
    
    if (newList->GetEntries()>0) {
	cout << " Updating Index.root" << endl;
	for (Int_t i=0; i<newList->GetEntries(); i++) {
	    TObject *o = (*newList)[i];
	    fFileList->Add(o);
	}
	TString indexFileName(path+"/Index.root");
        if (indexFileName.Index("root:")>=0) TRho::Instance()->Authorize();
	TFile* indexFile=TFile::Open(indexFileName.Data(),"RECREATE");
	fRunList->Sort();
	fFileList->Sort();
	fRunList->Write("runIndex",1,1024*fRunList->GetEntries());
	fFileList->Write("fileIndex",1,100*fFileList->GetEntries());
	indexFile->Close();
    }
    theCurrentFiles->Delete(); delete theCurrentFiles;
    newList->Delete(); delete newList;
}


void
TRunDB::ScanFile(const char *fname, TObjArray *list, Int_t &nevts, Int_t &firstRun)
{
#ifdef RUNDBUPDATE
    cout << " Creating Index for " << fCurrentCluster << " : " << fname << endl;
    
    PAFEventBase *evb = new PAFEventBase();
    EidInfoR *eid = new EidInfoR();
    
    TString fn = TString(TRho::Instance()->GetEventStore()) + "/" + fCurrentCluster +  "/" + TString(fname);
    TFile f1(fn);
    TBranch *theBranch = 0;
    TTree *theTree = 0;
    TBranch *br_maj=NULL;
    TBranch *br_min=NULL;
    TBranch *br_run=NULL;
    if (!fKanga) {
	theTree = (TTree*) f1.Get("TagTree"); // Regular PAF file
	if (theTree != 0) {
	    theBranch = theTree->GetBranch("TagList");
	    theBranch->SetAddress(&evb);
	    theTree->SetBranchStatus("*",1);
	    br_run = theTree->GetBranch("_runno");
	    br_maj = theTree->GetBranch("_majorID");
	    br_min = theTree->GetBranch("_minorID");
	    evb->SetTree(theTree);
	}
    }
    else { // Kanga files
	theTree = (TTree*) f1.Get("Eid"); // 8.2.x format
	if (theTree == 0) theTree = (TTree*) f1.Get("microDST"); // 8.3.x format
	if (theTree!=0) {
	  theTree->SetBranchStatus("*",0);
	  theBranch = theTree->GetBranch("Eid"); // 8.2.x format
	  if (theBranch == 0) theBranch = theTree->GetBranch("BtaEid"); // 8.3.x format
	  if (theBranch != 0) theBranch->SetAddress(&eid); // Success !
	  theBranch->ResetBit(kDoNotProcess);
	}
    }
    
    if (theTree == 0) // Not a PAF readable file
    {
	delete evb, eid;
	cerr << fname << ": Could not read header." << endl; 
	return;
    }
    
    if (!fKanga) {
      if ((br_run==NULL) || (br_min==NULL) || (br_maj==NULL)) {
	cout << "Error while updating RunDB : Tree " << theTree->GetName() 
	     << " in File " << fname << " does not contain correct header " << endl;
	return;
      }
    }

    if ((fKanga) && (theBranch==NULL)) {
      delete evb, eid;
      cerr << fname << ": Could not read header." << endl; 
      return;
    }

    Int_t n = (Int_t) theTree->GetEntries();
    nevts=n;
    
    
    //cout << "Start " << endl;
    // ======= main part =================
    firstRun=999999999;
    Int_t nFirst=0, nLast=0, nRuns=0;
    UInt_t firstId_maj,firstId_min;
    UInt_t lastId_maj,lastId_min;
    UInt_t maj,min;
    Int_t current=0;
    for (Int_t k=0; k<n; k++) {
	
	if (!fKanga) { 
	  evb->SetOtherEventNumber(k);
	} else {
	  theBranch->GetEvent(k);
	}
	
	Int_t r;
	if (!fKanga) {
	    r = evb->GetRunNumber();
	} else
	    r = eid->GetRunNumber();
	
	if (r!=current) {
	    if (k==0) {
		
		nFirst=0;
		if (!fKanga) {
		    firstId_maj = evb->GetMajorID();
		    firstId_min = evb->GetMinorID();
		    lastId_min =  evb->GetMinorID();
		    lastId_maj =  evb->GetMajorID();
		}
		else {
		    firstId_maj = eid->GetMajorID();
		    firstId_min = eid->GetMinorID();
		    lastId_min =  eid->GetMinorID();
		    lastId_maj =  eid->GetMajorID();
		}
		
	    } else {
		nLast=k-1;
		TRunInfo *info = new TRunInfo(current);
		info->SetBegin(nFirst,firstId_min,firstId_maj);
		info->SetEnd(nLast,lastId_min,lastId_maj);
		TString t(fname);
		if (!fKanga) {
		    TString t2(t(0,t.Length()-8));
		    info->SetFilename(t2);
		}
		else {
		    TString t2(t(0,t.Length()-5));
		    info->SetFilename(t2); // Note the full name
		}	  
		info->SetCluster(fCurrentCluster);
		list->Add(info);
		
		nFirst=k;
		if (!fKanga) {
		    firstId_maj = evb->GetMajorID();
		    firstId_min = evb->GetMinorID();
		    lastId_min =  evb->GetMinorID();
		    lastId_maj =  evb->GetMinorID();
		}
		else {
		    firstId_maj = eid->GetMajorID();
		    firstId_min = eid->GetMinorID();
		    lastId_min =  eid->GetMinorID();
		    lastId_maj =  eid->GetMinorID();
		}
		
	    }
	    
	    current=r;
	    nRuns++;
	    if (current<firstRun) { firstRun = current; }
	}
	
	if (!fKanga) {
	    maj =  evb->GetMajorID();
	    min =  evb->GetMinorID();
	}
	else {
	    maj =  eid->GetMajorID();
	    min =  eid->GetMinorID();
	}
	
	if (maj>lastId_maj) {
	    lastId_maj = maj; lastId_min = min;
	} else if ((maj==lastId_maj) && (min>lastId_min)) {
	    lastId_maj = maj; lastId_min = min;
	} else if ((maj==firstId_maj) && (min<firstId_min)) {
	    firstId_maj = maj; firstId_min = min;
	} else if (maj<firstId_maj) {
	    lastId_maj = maj; lastId_min = min;
	}
    }
    
    TRunInfo *info = new TRunInfo(current);
    info->SetBegin(nFirst,firstId_min,firstId_maj);
    info->SetEnd(n-1,lastId_min,lastId_maj);
    TString t(fname);
    if (!fKanga) {
	TString t2(t(0,t.Length()-8));
	info->SetFilename(t2);
    }
    else {
	TString t2(t(0,t.Length()-5));
	info->SetFilename(t2); // Note the full name
    }	  
    info->SetCluster(fCurrentCluster);
    list->Add(info);
    delete evb, eid;
    cout << "OK" << endl;
#else
    cout << "RunDB has not been compiled in update mode !" << endl;
#endif
}


void 
TRunDB::getDirContent(const char* pathName,TObjArray *filenameList)
{
  void* datadir = gSystem->OpenDirectory(pathName);
  if (datadir!=NULL) {
    cout << "TRunDB::getDirContent: Opening " << pathName << endl;
    Int_t ok=-1;
    while (ok) {
      TString t(gSystem->GetDirEntry(datadir));
      if (t.Length()>0) {
	if (t.Length()>9) {
	  TString tlast = TString(t(t.Length()-8,t.Length()));
	  if (tlast == "Tag.root" ) {
	    TString tname(t(0,t.Length()-8));
	    TObjString *newEntry = new TObjString(tname.Data());
	    filenameList->Add(newEntry);
	  }
	  else if (tlast == "cro.root" || tlast == "all.root") { //*****
	    TObjString *newEntry = new TObjString(t.Data());
	    filenameList->Add(newEntry); // This is a single Kanga "micro" file
	  }
	}
	if ((t.Index(".root")<0) && (t!=TString(".")) && (t!=TString(".."))) {
	  TString newPath(TString(pathName)+"/"+t);
	  TObjArray *subList = new TObjArray();
	  getDirContent(newPath,subList);
	  if (subList->GetEntries()>0) {
	    for (Int_t i=0; i<subList->GetEntries(); i++) {
	      TObjString *str = (TObjString*) (*subList)[i];
	      TString newname = TString(t+"/"+str->String().Data());
	      TObjString *newEntry = new TObjString(newname.Data());
	      filenameList->Add(newEntry);
	    }
	  }
	  subList->Delete();
	  delete subList;
	}
      } else {
	ok=0;
      }
    }
    filenameList->Sort();
    gSystem->FreeDirectory(datadir);
  }
  else
      cerr << "TRunDB::getDirContent: Could not open " << pathName << endl;
}  


void TRunDB::AddRunsToCollection(TRunCollection *theCol,Int_t runFrom, Int_t runTo)
{
    Int_t nmax = fRunList->GetEntries()-1;
    
    TRunInfo tmp(runFrom);
    Int_t pos = fRunList->BinarySearch(&tmp);
    if (pos<0) {
	cout << "Run #" << runFrom << " not Found, searching for next higher ... " << endl;
	TRunInfo *last = (TRunInfo*) (*fRunList)[nmax];
	if (runFrom>last->GetNumber()) {
	    return;
	}
	Int_t n=0;
	TRunInfo *current = (TRunInfo*) (*fRunList)[0];
	while ((n<nmax) && (runFrom>current->GetNumber())) {
	    n++;
	    current = (TRunInfo*) (*fRunList)[n];
	}
	if (runFrom<current->GetNumber()) { pos = n; cout << " found " << current->GetNumber() << endl; }
    }
    
    TRunInfo *runToAdd = (TRunInfo*) (*fRunList)[pos];
    theCol->AddRun(runToAdd);
    
    Int_t n=pos;
    while ((n<nmax) && (runToAdd->GetNumber()<runTo)) { 
	n++;
	runToAdd = (TRunInfo*) (*fRunList)[n];
	if (runToAdd->GetNumber()<=runTo) {
	    theCol->AddRun(runToAdd);
	}
    }
} 

