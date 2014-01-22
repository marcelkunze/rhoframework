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

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "RhoManager/TLogReader.h"
#include "RhoManager/TLogInfo.h"
#include "TString.h"
#include "TRegexp.h"
#include "TObjArray.h"

#include "TFile.h"
#include "TSystem.h"
#include "TDirectory.h"

#include <iostream>
#include <strstream>
using namespace std;

TLogReader::TLogReader()
{
  fLogInfos = new TObjArray(1000);
}

TLogReader::~TLogReader()
{
  fLogInfos->Delete();
  delete fLogInfos;
}

TLogInfo*
TLogReader::GetRun(Int_t n) const
{
  TLogInfo dummy(n);
  Int_t pos =  fLogInfos->BinarySearch(&dummy);
  if (pos<0) { return NULL; }
  TLogInfo *l = (TLogInfo*) (*fLogInfos)[pos];
  return l;
}

void
TLogReader::ReadFromFiles(const char *source_path,const char *dest)
{
  // read contents of opr
  TString oprfilename = TString(source_path) + "/" + TString("oprrunsDB.txt");
  TString ir2filename = TString(source_path) + "/" + TString("ir2runsDB.txt");
  TString lumifilename = TString(source_path) + "/" + TString("lumiDB.txt");
  ifstream oprfile(oprfilename.Data());
  ifstream ir2file(ir2filename.Data());
  ifstream lumifile(lumifilename.Data());

  if (!oprfile.good()) {
    cout << " Creation of " << dest << " failed; could not open " << oprfilename.Data() << endl;
    return;
  }
  if (!ir2file.good()) {
    cout << " Creation of " << dest << " failed; could not open " << ir2filename.Data() << endl;
    return;
  }
  if (!lumifile.good()) {
    cout << " Creation of " << dest << " failed; could not open " << lumifilename.Data() << endl;
    return;
  }
  char str[255]; oprfile.getline(str,200);
  Int_t last=-1;
  Int_t lastrun=-1;
  cout << "  Processing  OPR - data ..."  << endl;
  while (oprfile.good()) {
    TOprEntry entry;
    if (entry.ReadFromFile(oprfile)) {
      if (entry.Done()) {
	TLogInfo *theLog;
	if (entry.GetRun()==lastrun) {
	  theLog = (TLogInfo*) (*fLogInfos)[last];
	} else {
	  theLog = new TLogInfo(entry.GetRun());
	}
	theLog->fKanga = entry.Kanga();
	theLog->fCons = entry.GetCons();
	theLog->fVersion = entry.GetVersion();
	theLog->fCollection = entry.GetCollection();
	theLog->fProcspec = entry.GetProcspec();
	if (entry.GetRun()!=lastrun) { fLogInfos->Add(theLog); last++; }
	lastrun = entry.GetRun();
      }
    }
  }
  cout << "   ==> Totally " << fLogInfos->GetEntries() << " runs read  from  OPR-DB" << endl;
  fLogInfos->Sort();
  // Read ir2info and attach to loginfos
  cout << "  Processing  IR2 - data ..."  << endl;
  ir2file.getline(str,200);
  while (ir2file.good()) {
    ir2file.getline(str,200);
    if (ir2file.good()) {
      TIr2Entry ir2; 
      if (ir2.SetContent(str)) {
	TLogInfo *theLog = GetRun(ir2.GetRun());
	if (theLog!=NULL) {
	  theLog->SetDate(ir2.GetYear(),ir2.GetMonth(), ir2.GetDay());
	  theLog->SetStart(ir2.GetStartHour(),ir2.GetStartMin());
	  theLog->SetStop(ir2.GetStopHour(),ir2.GetStopMin());
	  theLog->fShiftType = ir2.GetShiftType();
	  theLog->fRunType = ir2.GetRunType();
	  theLog->fLer = ir2.GetLer();
	  theLog->fHer = ir2.GetHer();
	  theLog->fSolenoid = ir2.GetSolenoid();	
	}
      }
    }
  }
  // Read File with Luminosity list and write to loginfos
  cout << "  Processing luninosities ..." << endl;
  str[0]=0;
  lumifile.getline(str,200);
  while (lumifile.good()) {
    while ( lumifile.good() && (str[0]!='Y') && (str[0]!='N')) {
      lumifile.getline(str,200);
    }
    char s1[50],s2[50];
    istrstream input(str);
    input >> s1 >> s2;
    Int_t run = atoi(s2);
    input >> s1; input >> s1; input >> s1;
    input >> s2;
    TLogInfo *theLog  = GetRun(run);
    if (theLog!=NULL) { theLog->fLumi=atof(s2);}
    str[0]=0;  
  }
}

void TLogReader::WriteToFile(TFile  *file)
{
  Int_t  n  =  fLogInfos->GetEntries();
  cout  << "   Writing  data  to " << file->GetName() << endl;
  for (Int_t i=0; i<n; i++) {
    TLogInfo* current = (TLogInfo*) (*fLogInfos)[i];
    Int_t  blk  =  current->GetRunNumber()/100;
    char dirname[10]; sprintf(dirname,"d%i",blk*100);
    TDirectory  *theDir  = (TDirectory*) file->Get(dirname);
    if (theDir==0) { 
      char dirtitle[50];  sprintf(dirtitle,"runs %i - %i ",blk*100,blk*100+99);
      theDir = file->mkdir(dirname,dirtitle);
    }

    theDir->Append(current);
  }
  file->Write("",TObject::kOverwrite);
}

void
TLogReader::CreateNew(const  char *dest,const char* source)
{
  if (source==NULL) {
    // dump ORACLE-DBs into ascii-file
    char cmd1[255],cmd2[255],cmd3[255];
    cout << "  Reading data  from IR2 - DB ...  " << endl;
    sprintf(cmd1,"ir2runs -f date,starttime,stoptime,shift,solenoid,runtype,ler_e,her_e >  ir2runsDB.txt");
    gSystem->Exec(cmd1);
    cout << "  Reading data  from OPR - DB ...  " << endl;
    sprintf(cmd2,"oprruns -c -f status,procspec,kanga  > oprrunsDB.txt");
    gSystem->Exec(cmd2);
    cout << "  Calling  luminosity script ...  "  <<  endl;
    sprintf(cmd3,"lumi -l -r 0 1000000 >  lumiDB.txt");
    gSystem->Exec(cmd3);
    TFile f1(dest,"RECREATE");
    ReadFromFiles("./",dest);
    WriteToFile(&f1);
    gSystem->Exec("rm ir2runsDB.txt; rm oprrunsDB.txt; rm  lumiDB.txt;");
  } else {
    TFile f1(dest,"RECREATE");
    ReadFromFiles(source,dest);
    WriteToFile(&f1);
  }
  fLogInfos->Delete();
}

void
TLogReader::Update(const  char *dest,const char* source)
{
  if (source==NULL) {
    char cmd1[255],cmd2[255],cmd3[255];
    cout << "  Reading data  from IR2 - DB ...  " << endl;
    sprintf(cmd1,"ir2runs -f date,starttime,stoptime,shift,solenoid,runtype,ler_e,her_e >  ir2runsDB.txt");
    gSystem->Exec(cmd1);
    cout << "  Reading data  from OPR - DB ...  " << endl;
    sprintf(cmd2,"oprruns -c -f status,procspec,kanga  > oprrunsDB.txt");
    gSystem->Exec(cmd2);
    cout << "  Calling  luminosity script ...  "  <<  endl;
    sprintf(cmd3,"lumi -l -r 0 1000000 >  lumiDB.txt");
    gSystem->Exec(cmd3);
    TFile f1(dest,"RECREATE");
    ReadFromFiles("./",dest);
    WriteToFile(&f1);
    gSystem->Exec("rm ir2runsDB.txt; rm oprrunsDB.txt; rm  lumiDB.txt;");
  }  else {
    TFile f1(dest,"UPDATE");
    ReadFromFiles(source,dest);
    WriteToFile(&f1);
  }
  fLogInfos->Delete();
}

TIr2Entry::TIr2Entry() 
  : fRunNumber(0),
    fShiftType(0),
    fRunType(0),
    fYear(-1),
    fMonth(0),fDay(0),
    fStartHour(0),fStartMin(0),
    fStopHour(0),fStopMin(0),
    fLer(0),fHer(0)
{
}

TIr2Entry::~TIr2Entry() 
{ }

Int_t
TIr2Entry::Compare(const TObject *other) const
{
  TIr2Entry *otherI = (TIr2Entry*) other;
  if (fRunNumber < otherI->fRunNumber) { return -1; }
  if (fRunNumber > otherI->fRunNumber) { return 1; }
  return 0;
}

Bool_t
TIr2Entry::SetContent(char *str)
{

  char s1[200];
  s1[0]=0;
  istrstream input(str);
  TString t1;


  // run number
  TRegexp reNoInt("[^0-9,^-]");
  input >> s1; 
  t1 = TString(s1);
  if (t1.Index(reNoInt)>=0) {
    cout << "Error in TIr2Entry::SetContent : Cannot read runnumber from \"" << s1 << "\"" << endl;
    return kFALSE;
  }
  fRunNumber = atoi(s1);
  // Date
  input >> s1;
  TRegexp reDate("[0-9][0-9]-[A-Z][A-Z][A-Z]-[0-9][0-9]");
  t1 = TString(s1);
  if (t1.Index(reDate)<0) {
    cout << "Error in TIr2Entry::SetContent : Cannot read Date from \n" << s1 << endl;
    return kFALSE;
  }
  fDay = (t1[0]-48)*10+(t1[1]-48);
  fYear = (t1[7]-48)*10+(t1[8]-48); if (fYear<99) { fYear+=2000; } else { fYear+=1900; }
  TString mon(t1(3,3));
  if (mon==TString("JAN")) { fMonth=1;}
  if (mon==TString("FEB")) { fMonth=2;}
  if (mon==TString("MAR")) { fMonth=3;}
  if (mon==TString("APR")) { fMonth=4;}
  if (mon==TString("MAY")) { fMonth=5;}
  if (mon==TString("JUN")) { fMonth=6;}
  if (mon==TString("JUL")) { fMonth=7;}
  if (mon==TString("AUG")) { fMonth=8;}
  if (mon==TString("SEP")) { fMonth=9;}
  if (mon==TString("OCT")) { fMonth=10;}
  if (mon==TString("NOV")) { fMonth=11;}
  if (mon==TString("DEC")) { fMonth=12;}

  // Start time, if given
  TRegexp reTime("[0-9][0-9]:[0-9][0-9]");
  s1[0]=0; input >> s1;
  TString t3(s1);
  if (t3.Index(reTime)!=0) {
    fStartHour=255;
  } else {
    fStartHour = (t3[0]-48)*10 + (t3[1]-48);
    fStartMin = (t3[3]-48)*10 + (t3[4]-48);
    s1[0]=0; input >> s1;
  }

  // Stop Time, if given
  TString t4(s1);
  if (t4.Index(reTime)!=0) {
    fStopHour=255;
  } else {
    fStopHour = (t4[0]-48)*10 + (t4[1]-48);
    fStopMin = (t4[3]-48)*10 + (t4[4]-48);
    s1[0]=0; input >> s1;
  }
  // Shift Type
  if (strcmp(s1,"Day")==0) { fShiftType=1; }
  else if (strcmp(s1,"Swing")==0) { fShiftType=2; }
  else if (strcmp(s1,"Owl")==0) { fShiftType=3; }
  else {
    cout << "Error in TIr2Entry::SetContent : Cannot read Shift type from \"" << s1 << "\"" << endl;   
    return kFALSE;
  }
  s1[0]=0; input >> s1;
  // Solenoid current
  TRegexp reNoFlt("[^0-9,^\\.,^-]");  
  TString t5(s1);
  if (t5.Index(reNoFlt)<0) {
    fSolenoid = atof(s1);
    s1[0]=0; input >> s1;
  } else {
    fSolenoid = -1;
  }
  // Run Type
  if (strcmp(s1,"Colliding")==0) { fRunType=1; }
  else if (strcmp(s1,"Cosmics")==0) { fRunType=2; }
  else if (strcmp(s1,"Stable")==0) { fRunType=4; s1[0]=0; input >> s1;}
  else if (strcmp(s1,"Other")==0) { fRunType=8; }
  else if (strcmp(s1,"Calibratio")==0) { fRunType=16; }
  else { 
    fRunType=32; 
    //cout << "Warning in TIr2Entry::SetContent : Cannot read run type from " << str << endl;   
  }
  s1[0]=0; input >> s1;
  // LER, HER if given
  if (strlen(s1)>0) {
    fLer = atof(s1);
  } else {
    fLer = -1;
  }
  s1[0]=0; input >> s1; 
  if (strlen(s1)>0) {
    fHer = atof(s1);
  } else {
    fHer = -1;
  }
  return  kTRUE;
}


void
TIr2Entry::PrintOn(std::ostream &o) const
{
  Int_t dt1,dt2,t1,t2;
  o << "Run# " << fRunNumber << endl;
  o << "-----------" << endl;
  o << " Recorded on "; 
  if (fShiftType==3) { o << "OWL Shift"; }
  if (fShiftType==2) { o << "SWING Shift"; }
  if (fShiftType==1) { o << "DAY Shift"; }
  o << " of " << (Int_t) fDay << "/" << (Int_t) fMonth << "/" << (Int_t) fYear << endl; 
  o << " Run Type : ";
  if (fRunType==1 ) { o << "Colliding Beams"; }
  if (fRunType==2 ) { o << "Cosmics"; }
  if (fRunType==4 ) { o << "Stable Beams"; }
  if (fRunType==8) { o << "Other"; }
  if (fRunType==16 ) { o << "Calibration"; }
  if (fRunType==32) { o << "Unknown"; }

  o << endl;
  if (fStartHour<24) {
    o << " Start : " << (Int_t) fStartHour << ":"  << (Int_t)  fStartMin;
  } else {
    o << " Start : unknown ";  
  }
  if (fStopHour<24) {
    o << " Stop : " <<  (Int_t) fStopHour << ":" << (Int_t) fStopMin;
  } else {
    o << " Stop  : unknown " << endl;
  }
  if (fLer>0 && fHer > 0) {
    o << " Beamenergies : HER = " << fHer << " GeV ,  LER = " << fLer << " GeV" << endl;
  } else {
    o << " Beamenergies : unknown "  << endl;
  }
  o << endl;
}

void TIr2Entry::PrintTime(Int_t t,std::ostream &o) const
{
  Int_t hr = (t/10000)%100;
  Int_t min = (t/100)%100;
  if (hr<10) { o << "0"; }
  o << hr << ":";
  if (min<10) { o << "0"; }
  o << min;
}


// ________________________________________________________________________

TOprEntry::TOprEntry() 
  : fRunNumber(0),
    fDone(kFALSE),
    fKanga(kFALSE),
    fCons(255), fVersion(255)
{
}

void
TOprEntry::PrintOn(std::ostream &o) const
{
  o << "Run # " << fRunNumber << " :  CONS " << GetCons() << " VER " << GetVersion() << endl;
  if (Done()) { o << " done, "; } else { o << " not OK , "; }
  if (Kanga()) { o << " kanga, "; } else { o << " no kanga , "; }
  o << " Procs : " << GetProcspec() << endl;
  o << " Collection : " << GetCollection() << endl;
}

Bool_t
TOprEntry::ReadFromFile(ifstream &in)
{
  char line[255];
  //for (Int_t k=0; k<255; k++) { line[k]='x'; }; line[100]=0;
  char str[200];
  in.getline(line,200);
  //cout << line << endl;
  TString t(line);
  while ( (t.Index("/")>=0) && (in.good()) ) {
    in.getline(line,200);
    t = TString(line);
  }
  if (t.Index("/")<0) {
    // found line with run description
    // Run Number
    istrstream input(line);
    char s1[200]; s1[0]=0;
    input >> s1;
    TString t2(s1);
    TRegexp reNoInt("[^0-9,^-]");
    if (t2.Index(reNoInt)>=0) {
      cout << " ERROR in TOprEntry::ReadFromFile ; cannot read runnumber from line " << endl << line << endl;
      return kFALSE; 
    }
    fRunNumber = atoi(s1); 
    // CONS
    s1[0]=0;
    input >> s1;
    TString t3(s1);
    if (t3.Index(reNoInt)>=0) {
      cout << " ERROR in TOprEntry::ReadFromFile ; cannot read CONS from line " << endl << line << endl;
      return kFALSE;
    }
    fCons = atoi(s1);
    // VER
    s1[0]=0;
    input >> s1;
    TString t4(s1);
    if (t4.Index(reNoInt)>=0) {
      cout << " ERROR in TOprEntry::ReadFromFile ; cannot read VER from line " << endl << line << endl;
      return kFALSE;
    }
    fVersion = atoi(s1);
    // DONE ? 
    s1[0]=0;
    input >> s1;
    if (strcmp(s1,"done")==0) { fDone=kTRUE; } else { fDone=kFALSE; }
    // PROSPEC
    s1[0]=0;
    input >> s1;
    TString t5(s1);
    TRegexp reProcspec("P.*V[0-9][0-9]");
    if (t5.Index(reProcspec)!=0) {
      cout << " ERROR in TOprEntry::ReadFromFile ; cannot read PROCSPEC from line " << endl << line << endl;
      return kFALSE;
    }
    fProcspec = TString(s1);
    // KANGA ?
    s1[0]=0;
    input >> s1;
    TString t6(s1);
    if (t6.Index(reNoInt)>=0) {
      cout << " ERROR in TOprEntry::ReadFromFile ; cannot read KANGA from line " << endl << line << endl;
      return kFALSE;
    }
    fKanga = (atoi(s1)!=0);
    // Read collections
    streampos currentpos = in.tellg();
    in.getline(line,200);
    t = TString(line);
    fCollection = TString("");
  
    while ((t.Index("/")>=0) && (in.good())) {
      Int_t begin = t.Index("/groups/isPhysicsEvents");
      if (begin>0) {
	TString t2(t(begin+23,t.Length()-begin-23));
	fCollection = t2;
      }
      currentpos=in.tellg();
      in.getline(line,200); t = TString(line); 
    }
    if (in.good()) in.seekg(currentpos);
  }
  return kTRUE;
}

