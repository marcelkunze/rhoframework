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

#include <stdlib.h>
#include "RhoBase/TRho.h"
#include "RhoManager/TParameterManager.h"
#include "RhoManager/TObjectManager.h"
#include "RhoManager/TDirectoryManager.h"
#include "RhoManager/TAnalysis.h"
#include "RhoManager/TModule.h"

ClassImp(TParameterManager)

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//----------------
// Constructors --
//----------------
TParameterManager::TParameterManager(const char* filename,const char* mode ) :
fFile(0), fMode(mode)
{
    // Open the parameter file
    if (TString(filename) != "") SetParmFile(filename,mode);
}

//--------------
// Destructor --
//--------------

TParameterManager::~TParameterManager() 
{
    if (fFile) {
	if (fMode != "READ") fFile->Write();
	fFile->Close(); // Close the current file
    }
}

//----------------------
//-- public Functions --
//----------------------

// Set current directory in memory, create if it does not exist:
Bool_t TParameterManager::SetDir( const char * path ) {
    
    Bool_t result= kTRUE;
    fFile->cd();
    gDirectory->cd( "/" );  
    // Now try to change or create directory. ROOT will
    // create only direct subdirectories without "/" in the name. 
    // Go through the path, extract tokens and cd, creating
    // directories along the way as needed:
    TString ch( path );
    int i1= 0;
    int i2= 0;
    if( ch( 0, 1 ) == "/" ) {
	i1= 1;
	i2= 1;
    }
    while( i2 < ch.Length() ) {
	while( ch( i2, 1 ) != "/" && i2 < ch.Length() ) i2++;
	TString tok( ch( i1, i2-i1 ) );
	i1= ++i2;
	TKey* key= gDirectory->GetKey( &*tok );
	if( key != 0 ) {
	    if( !gDirectory->cd( &*tok ) ) {
		cout << "TParameterManager::setDir: can't cd to existing " 
		    << tok << " in " << gDirectory->GetPath() << endl;
		break;
	    }
	}
	else {
	    TDirectory* newdir= gDirectory->mkdir( &*tok );
	    if( newdir != 0 ) {
		newdir->cd();
	    }
	    else {
		result=kFALSE;
		break;
	    }
	}
    }
    return result;
    
}


// Return path of current directory (a la pwd):
const char* TParameterManager::GetDir() const {
    
    return gDirectory->GetPath();
    
}

Bool_t TParameterManager::SetParmFile(const char* filename, const char* mode) {
    
    if (fFile) {
	if (fMode != "READ") fFile->Write();
	fFile->Close(); // Close the current file
    }
    
    fFile = TFile::Open(filename,mode,"TParameterManager file" );
    fFile->cd();
    
    fName = filename;
    fMode = mode;
    
    return kTRUE;  
}

Bool_t TParameterManager::ReadParm(const char* filename)
{
    const Int_t len(1024);
    Char_t    *argv[2], skip[len];
    string key, value, module, assign;
    
    ifstream  s(filename, ios::in);
    
    if (!s) {
	cout << endl << "TParameterManager: Could not open " << filename << endl;
	return kFALSE;
    }
    else
	cout << endl << "TParameterManager: Reading parameters from " << filename << endl;

    while (!s.eof()) {
	s >> key;  // Get key
	if (key == "") { s.getline(skip,len); continue; } // empty line
	if (key == "sequence") { s.getline(skip,len); continue; } // can not handle yet
	if (key == "module" || key == "mod") { 
	    s >> assign >> module; 
	    s.getline(skip,len);
	    if (assign == "disable") {
		//cout << "Disable from TCL not possible: " << module << endl;
		TObject *object = TRho::Instance()->GetAnalysis()->Get(module.c_str());
		if (object!=0 && object->InheritsFrom("TModule")) {
		    ((TModule *) object)->Disable();
		}
	    }
	    else if (assign == "enable") {
		//cout << "Enable from TCL not possible: " << module << endl;
		TObject *object = TRho::Instance()->GetAnalysis()->Get(module.c_str());
		if (object!=0 && object->InheritsFrom("TModule")) {
		    ((TModule *) object)->Enable();
		}
	    }
	    continue; 
	}
	if (key == "talkto") { s >> module; s.getline(skip,len); continue; }
	if (key == "exit") { module = ""; s.getline(skip,len); continue; }
	if (key.find("#")!=string::npos) { s.getline(skip,len); continue; }
	if (key.find("{")!=string::npos) { s.getline(skip,len); continue; }
	if (key.find("}")!=string::npos) { s.getline(skip,len); continue; }
	s >> assign;    // Get delimiter, "set" etc.
	if (key == "ev") { key = "nev"; s>>assign; }
	s >> value;	// Get value

	// This has to be done first because the directory manager likes
	// to produce absolute path names
	if (key=="store") {
	    ((TRho*)TRho::Instance())->SetEventStore(value.c_str());
	    TRho::Instance()->GetDirectoryManager()->SetDataDirectory(value.c_str());
	}

	key = "-" + key; // Prepend the selector
	argv[0] = (char*) key.c_str();
	argv[1] = (char*) value.c_str();

	if (module == "" || module == "Framework") // Do we talk to a module ?
	  SetParameters(2,argv); // no
	else if (module == "PAFBuildEnv" || module == "RooBuildEnv")
	  SetParameters(2,argv); // These are no modules
	else if (module == "PAFEventInput" || module == "RooEventInput")
	  TRho::Instance()->GetDirectoryManager()->AddFile(value.c_str()); // Add files
	else
	  SetParameters(2,argv,module.c_str()); // Other module
    }

    s.close();
    return kTRUE;
}


void TParameterManager::WriteParm(const char* filename)
{
    TParm *arg=0;

    filebuf buffer;
    std::ostream o(&buffer);
    buffer.open(filename,ios::out);

    if (!buffer.is_open()) {
	cout << endl << "TParameterManager: Could not open " << filename << endl;
    }
    else
	cout << endl << "TParameterManager: Saving parameters to " << filename << endl;
    

    TDatime theTime;
    o << "# Dump of parameters from " << fProgName.Data() << " at " << theTime.AsString() << endl;


    o << endl;
    o << "# Boolean" << endl;
    for (int i=0;i<GetBoolList().GetSize();i++) {
	arg = (TParm *)GetBoolList().At(i);
	if (arg->GetBool(arg->KeywordName()))
	  o << arg->KeywordName() << "\tset\ttrue\t#" << arg->Description() << endl;
	else
	  o << arg->KeywordName() << "\tset\tfalse\t#" << arg->Description() << endl;
    }
    
    o << endl;
    o << "# Integer" << endl;
    for (int j=0;j<GetIntList().GetSize();j++) {
	arg = (TParm *)GetIntList().At(j);
	o << arg->KeywordName() << "\tset\t" << arg->GetInt(arg->KeywordName());
	o << "\t#" << arg->Description() << endl;
    }
    
    o << endl;
    o << "# Double" << endl;
    for (int k=0;k<GetDoubleList().GetSize();k++) {
	arg = (TParm *)GetDoubleList().At(k);
	o << arg->KeywordName() << "\tset\t" << arg->GetDouble(arg->KeywordName());
	o << "\t#" << arg->Description() << endl;
    }
    
    o << endl;
    o << "# String" << endl;
    for (int l=0;l<GetStringList().GetSize();l++) {
	arg = (TParm *)GetStringList().At(l);
	TString value = arg->GetString(arg->KeywordName());
	if (value == "")  // Protect empty strings
	  o << arg->KeywordName() << "\tset\t" << "\"\"";
	else
	  o << arg->KeywordName() << "\tset\t" << value;
	o << "\t#" << arg->Description() << endl;
    }
    
    o << endl;
    o << "# General" << endl;
    for(int m=0;m<gGeneralList.GetSize();m++) {
	TParmGeneral<double> *arg = (TParmGeneral<double> *)gGeneralList.At(m);
	o << arg->KeywordName() << "\tset\t" << arg->Value();
	o << "\t#" << arg->Description() << endl;
    }
    
}

void TParameterManager::ScanCommandLine(int argc, char **argv)
{
    if (argc==0) return;
    
    // Get rid of the program name
    
    char *s = strrchr(argv[0],'/');
    if (s) {
	while (*s == '/') s++;
	fProgName = s;
    }
    else {
	fProgName = argv[0];
    }
    
    int _argc = argc - 1;
    char **_argv = argv + 1;
    
    // Scan the remaining arguments
    
    SetParameters(_argc,_argv);
}

void TParameterManager::SetParameters(int _argc, char** _argv,const char* module)
{
    while (_argc) {
	// first, determine if this is an option
	
	if (_argc <= 1) {
		TString errMsg = TString("Missing value for option ") + TString(*_argv);
		SyntaxError(errMsg);
	}
	
	if (_argv[0][0] != '-') break;
	while (_argv[0][0] == '-') _argv[0]++; // Skip the dashes
	
	// check if it is for usage
	if (strcmp(_argv[0],"help") == 0 || strcmp(_argv[0],"usage") == 0) {
	    PrintUsage(cout); // This stops the program
	    exit(0);
	}
	
	// find the appropriate TParm
	TParm *cmd;
	TString key(_argv[0]);
	if (module != 0) 
	    key = TString(module) + "::" + key; // Search with scope
	else
	    if (key.Index("::")<0) key = "Framework::" + key;

	cmd = ArgSearch(key); // Look for the command

	if (!cmd) {

	    TObject *mod = 0;
	    TString parm(_argv[0]);

	    // Do we have a scope ?
	    if (parm.Index("::")>=0) {
		TString modname = parm(0,parm.Index("::"));
		mod  = TRho::Instance()->GetObjectManager()->Get(modname);
		if (mod!=0)
		    parm = parm(parm.Index("::")+2,parm.Length());
		else
		    cout << "TParameterManager: Module " << modname.Data() << " does not exist. " << endl;
	    }

	    // Check parameter type
	    char *s = strrchr(_argv[1],'.'); // Double type

	    if (strcmp(_argv[1],"true")==0 || strcmp(_argv[1],"false")==0) {
		cout << "TParameterManager: Add new bool parameter " << parm.Data() << endl;
		SetBoolParm(parm,kFALSE,"new bool parameter",mod); // Add new bool parm
	    }
	    else if (s) {
		cout << "TParameterManager: Add new double parameter " << parm.Data() << endl;
		SetDoubleParm(parm,0.0,"new double parameter",mod); // Add new double parm
	    }
	    else if (atoi(_argv[1]) != 0) {
		cout << "TParameterManager: Add new int parameter " << parm.Data() << endl;
		SetIntParm(parm,0,"new int parameter",mod); // Add new int parm
	    }
	    else {
		cout << "TParameterManager: Add new string parameter " << parm.Data() << endl;
		SetStringParm(parm,"","new string parameter",mod); // Add new int parm
	    }

	    if (ArgSearch(key)) 
		_argv[0]--;  // Give it another try...
	    else {
		_argv++; // Skip this...
		_argc--;
		_argv++;
		_argc--;
	    }

	    continue;

	}

	_argv++;                // move to next arg
	_argc--;
	
	// set the value and bump pointers by the number of arguments used
	int rc = cmd->SetValue(0,_argv);
	if (rc < 0) {
	    TString err = TString("Invalid parameter for option ") + TString(*(_argv-1));
	    SyntaxError(err);
	}
	_argc -= rc;
	_argv += rc;
    }
}

void TParameterManager::SyntaxError(const char* message)
{
    cerr << "Syntax error: " << message << endl;
    PrintUsage(cerr);
    abort();
}

void TParameterManager::PrintUsage(std::ostream &o) 
{
    o << endl;
    o << "Usage : " << fProgName.Data();
    PrintParm(o);
}


TParm *TParameterManager::ArgSearch(const char *a) 
{
    TString s(a);
    TParm *arg=0;
    
    for (int i=0;i<GetBoolList().GetSize();i++) {
	arg = (TParm *)GetBoolList().At(i);
	if (arg->KeywordName() == s ) return arg;
    }
    
    for (int j=0;j<GetIntList().GetSize();j++) {
	arg = (TParm *)GetIntList().At(j);
	if (arg->KeywordName() == s ) return arg;
    }
    
    for (int k=0;k<GetDoubleList().GetSize();k++) {
	arg = (TParm *)GetDoubleList().At(k);
	if (arg->KeywordName() == s ) return arg;
    }
    
    for (int l=0;l<GetStringList().GetSize();l++) {
	arg = (TParm *)GetStringList().At(l);
	if (arg->KeywordName() == s ) return arg;
    }
    
    for (int m=0;m<gGeneralList.GetSize();m++) {
	arg = (TParm *)gGeneralList.At(m);
	if (arg->KeywordName() == s ) return arg;
    }
    
    return 0;
}

void TParameterManager::PrintParm(std::ostream &o,TObject *x) const { PrintOn(o,x); }
void TParameterManager::PrintOn(std::ostream &o,TObject *x) const { TParm::PrintAllParm(o,x); }

// non-member functions
// --------------------

std::ostream&  operator << (std::ostream& o, const TParameterManager& a) {a.PrintOn(o); return o;}



