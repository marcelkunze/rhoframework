//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TPersistenceManager							//
//                                                                      //
// Persistence manager					    		//
//                                                                      //
// Author: Marcel Kunze, Bochum University, Feb. 99			//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include "RhoManager/TPersistenceManager.h"
#include "RhoBase/TRho.h"

ClassImp(TPersistenceManager)

#include <iostream>
using namespace std;

//----------------
// Constructors --
//----------------
TPersistenceManager::TPersistenceManager( const char* file,const char* fmode ) :
fHepTuple(kFALSE)
{
    rtfilep = new TFile( file, (char*)fmode,"Created for you by TPersistenceManager" );
    olist   = new THashList();
    hlist   = new THashList();
    ntlist  = new THashList();
}

//--------------
// Destructor --
//--------------

TPersistenceManager::~TPersistenceManager() 
{
    olist->Delete(); delete olist;// Clear lists and remove objects
    hlist->Delete(); delete hlist;
    ntlist->Delete(); delete ntlist;
    rtfilep->Close();
    delete rtfilep;
    
}

//----------------------
//-- public Functions --
//----------------------

// Set current directory in memory, create if it does not exist:
Bool_t TPersistenceManager::SetDir( const char * path ) 
{    
    Bool_t result= kTRUE;
    checkFile();
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
		cout << "TPersistenceManager::setDir: can't cd to existing " 
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
		result= kFALSE;
		break;
	    }
	}
    }
    return result;
    
}


// Return path of current directory (a la pwd):
const char* TPersistenceManager::GetDir() const 
{   
    return gDirectory->GetPath();  
}

Bool_t TPersistenceManager::SetFileName( const char* file ) 
{   
    // Make sure we are at the top of the current file, then save:
    checkFile();
    rtfilep->cd();
    Save();
    
    // Create the new file and transfer all objects from existing to new
    // file. Function movedir will call itself recursively for directories.
    TFile* filep= new TFile( file, "RECREATE", 
	"Created for you again by TPersistenceManager" );
    movedir( rtfilep, filep );
    
    // Now we can safely get rid of the old file:
    delete rtfilep;
    rtfilep= filep;
    
    // The End:
    return kTRUE;
    
}


// Recursively move contents of olddir to newdir:
void TPersistenceManager::movedir( TDirectory* olddir, TDirectory* newdir ) 
{   
    TList* list1= olddir->GetList();
    TIter iter( list1 );
    TObject* obj= 0;
    while( (obj= iter.Next()) ) {
	if( obj->InheritsFrom( "TH1" ) ) {
	    // TH1::SetDirectory does all the work for us:
	    ((TH1*)obj)->SetDirectory( newdir );
	}
	else if( strcmp( obj->ClassName(), "TDirectory" ) == 0 ) {
	    // Have to do it by hand recursively for TDirectory:
	    list1->Remove( obj );
	    TDirectory* dir= newdir->mkdir( obj->GetName() );
	    movedir( (TDirectory*)obj, dir );
	}
    }    
    return;
    
}

size_t TPersistenceManager::GetNumDir() const 
{ 
    checkFile( "w" );
    TIter iter( gDirectory->GetList() );
    TObject* obj;
    int dircount= 0;
    while( (obj= iter.Next()) ) {
	if( !strcmp( obj->ClassName(), "TDirectory" ) ) dircount++;
    }
    return dircount;
    
}

const char* TPersistenceManager::GetDirName( size_t index ) const 
{
    checkFile( "w" );
    TIter iter( gDirectory->GetList() );
    TObject* obj;
    size_t dircount= 0;
    TString dirName( "Not found" );
    while( (obj= iter.Next()) ) {
	if( !strcmp( obj->ClassName(), "TDirectory" ) ) {
	    dircount++;
	    if( index+1 == dircount ) {
		dirName= obj->GetName();
		break;
	    }
	}
    }
    return dirName;
    
}

Bool_t TPersistenceManager::GetDirIndex( const char* dirname, 
					    size_t & index ) const 
{
    Bool_t result= kTRUE;
    checkFile( "w" );
    TIter iter( gDirectory->GetList() );
    TObject* obj;
    size_t dircount= 0;
    while( (obj= iter.Next()) ) {
	if( !strcmp( obj->ClassName(), "TDirectory" ) ) {
	    dircount++;
	    if( !strcmp( obj->GetName(), &*dirname ) ) break;
	}
    }
    if( dircount == 0 ) {
	result= kFALSE;
    }
    else {
	index= dircount-1;
    }
    return result;
    
}

void TPersistenceManager::PrintOn(std::ostream & o) const 
{
    int i;

    o << endl;
    o << "TPersistenceManager Directory" << endl;
    o << "---------------------------------" << endl;
    o << "THistograms: " << hlist->GetSize() << endl;
    for (i=0;i<hlist->GetSize();i++) {
	TH1F *h = (TH1F*)hlist->At(i);
	if (h==0)
	    o << "Reference to non existent object" << endl;
	else
	    o << h->ClassName() << '\t' << h->GetName() << "\t(" << h->GetTitle() << ")" << endl;
    }

    o << endl;
    o << "TTuples: " << ntlist->GetSize() << endl;
    for (i=0;i<ntlist->GetSize();i++) {
	TTree *h = (TTree*)ntlist->At(i);
	if (h==0)
	    o << "Reference to non existent object" << endl;
	else
	    o << h->ClassName() << '\t' << h->GetName() << "\t(" << h->GetTitle() << ")" << endl;
    }

    o << endl;
    o << "Other TObjects: " << olist->GetSize() << endl;
    for (i=0;i<olist->GetSize();i++) {
	TObject *h = (TObject*)olist->At(i);
	if (h==0)
	    o << "Reference to non existent object" << endl;
	else
	    o << h->ClassName() << '\t' << h->GetName() << "\t(" << h->GetTitle() << ")" << endl;
    }

    o << endl;

    return;
}

// Construct names (keys) for histos and ntuples. When hhid contains 0,
// a key using the prefix and a running counter is created:
const char* TPersistenceManager::makeName( const char* prefix, THistID hhid ) 
{
    TString name( prefix );
    if( hhid.isIDstringSet() ) {
	name= hhid.getIDstring();
    }
    else if( hhid.isIDnumberSet() ) {
	if( hhid.getIDnumber() != 0 ) {
	    name+= TString( hhid.getIDnumber() );
	}
	else {
	    TIter iter( gDirectory->GetList(), kIterBackward );
	    TObject* obj= 0;
	    int number= 0;
	    if( name( 0, 1 ) == "h" ) {
		while( (obj= iter.Next()) ) if( obj->InheritsFrom( "TH1" ) ) break;
		if( obj != 0 ) {
		    TString hkey( obj->GetName() );
		    TString x(hkey( 3, hkey.Length()-3));
		    number = atol(x);
		}
		char tmp[128];
		sprintf(tmp,"%d\0",number+1);
		name+= TString(tmp);
	    }
	    else if( name( 0, 3 ) == "ntp" ) {
		while( (obj= iter.Next()) ) {
		    if( !strcmp( obj->ClassName(), "TTree" ) && 
			!strncmp( obj->GetName(), "ntp", 3 ) ) break;
		}
		if( obj != 0 ) {
		    TString lastname( obj->GetName() );
		    TString x(lastname( 3, lastname.Length()-3 ));
		    number = atol(x);
		}
		char tmp[128];
		sprintf(tmp,"%d\0",number+1);
		name+= TString(tmp);
	    }
	    else {
		cout << "TPersistenceManager::makeName: prefix " << prefix 
		    << " not recognised (h1d, h2d, hpr, ntp)" << endl;
	    }
	}
    }
    return name;
}

// Check if current file belongs to this manager, if not change
// directory to ROOT of correct file:
void TPersistenceManager::checkFile( const char* opt ) const 
{
    if( gDirectory->GetFile() != rtfilep ) {
	if( !strcmp( opt, "" ) ) {
	    rtfilep->cd();
	}
	else {
	    cout << "TPersistenceManager::checkFile: current file "
		<< gDirectory->GetFile()->GetName() 
		<< " does not belong to this manager ("
		<< rtfilep->GetName() << ")" << endl;
	}
    }
    return; 
}


size_t TPersistenceManager::GetNumHist() const 
{
    checkFile( "w" );
    TIter iter( gDirectory->GetList() );
    TObject* obj;
    int histcount= 0;
    while( (obj= iter.Next()) ) {
	if( obj->InheritsFrom( "TH1" ) ) histcount++;
    }
    return histcount;
}

size_t TPersistenceManager::GetNumHist( const char *path ) const 
{
    TDirectory* olddir= gDirectory;
    checkFile();
    gDirectory->cd( path );
    int histcount= GetNumHist();
    olddir->cd();
    return histcount;
    
}

TObject* TPersistenceManager::Add(TObject* item) 
{
    TDirectory* olddir= gDirectory;
    checkFile();
    olist->Add(item);
    olddir->cd();
    return item;
}

TObject* TPersistenceManager::Get(const char *name) 
{
    TObject *item;
    if ((item=olist->FindObject(name)) != 0) return item;
    if ((item=hlist->FindObject(name)) != 0) return item;
    if ((item=ntlist->FindObject(name)) != 0) return item;
    return 0; // Not found
}

void TPersistenceManager::Remove(TObject* item) 
{   
    TDirectory* olddir= gDirectory;
    checkFile();
    olist->Remove(item);
    olddir->cd();
}

void TPersistenceManager::Remove(const char *name) 
{   
    TDirectory* olddir= gDirectory;
    checkFile();
    TObject* item = Get(name);
    if ((item=olist->FindObject(name)) != 0) olist->Remove(item);
    if ((item=hlist->FindObject(name)) != 0) hlist->Remove(item);
    if ((item=ntlist->FindObject(name)) != 0) ntlist->Remove(item);
    olddir->cd();
}

// 1-D histos:
THistogram* TPersistenceManager::Histogram( const char* htitle, int nbin, 
						   float lowX, float highX ) 
{
    return Histogram( htitle, nbin, lowX, highX, THistID( 0 ) );
}

THistogram* TPersistenceManager::Histogram( const char* htitle, int nbin,
						   float lowX, float highX, 
						   THistID hhid ) 
{
    TDirectory* olddir= gDirectory;
    checkFile();
    THistogram* histp= getHistByName( htitle );
    if( histp == 0 ) {
	TString hname;
	if (fHepTuple)
	    hname = makeName( "h1d", hhid );
	else
	    hname = TString(htitle);
	histp=new THistogram( &*hname, htitle, nbin, lowX, highX );
	hlist->Add(histp);
    }
    olddir->cd();
    return histp;
}

TMassHistogram* TPersistenceManager::MassHistogram(const Text_t *hname,const Text_t *htitle,Int_t nbin,Axis_t lowX,Axis_t highX) 
{
    TDirectory* olddir= gDirectory;
    checkFile();
    TMassHistogram* histp= (TMassHistogram*) getHistByName( htitle );
    if( histp == 0 ) {
	histp=new TMassHistogram( &*hname, htitle, nbin, lowX, highX );
	hlist->Add(histp);
    }
    olddir->cd();
    return histp;
}

TEnergyHistogram* TPersistenceManager::EnergyHistogram(const Text_t *hname,const Text_t *htitle,Int_t nbin,Axis_t lowX,Axis_t highX) 
{
    TDirectory* olddir= gDirectory;
    checkFile();
    TEnergyHistogram* histp= (TEnergyHistogram*) getHistByName( htitle );
    if( histp == 0 ) {
	histp=new TEnergyHistogram( &*hname, htitle, nbin, lowX, highX );
	hlist->Add(histp);
    }
    olddir->cd();
    return histp;
}

TMomentumHistogram* TPersistenceManager::MomentumHistogram(const Text_t *hname,const Text_t *htitle,Int_t nbin,Axis_t lowX,Axis_t highX) 
{
    TDirectory* olddir= gDirectory;
    checkFile();
    TMomentumHistogram* histp= (TMomentumHistogram*) getHistByName( htitle );
    if( histp == 0 ) {
	histp=new TMomentumHistogram( &*hname, htitle, nbin, lowX, highX );
	hlist->Add(histp);
    }
    olddir->cd();
    return histp;
}

// 2-D histos:
THistogram* 
TPersistenceManager::Histogram( const char* htitle, 
				   int nbinX, float lowX, float highX,
				   int nbinY, float lowY, float highY ) 
{
    return Histogram( htitle, nbinX, lowX, highX, nbinY, lowY, highY,
	THistID( 0 ) );
}

THistogram* TPersistenceManager::Histogram( const char* htitle, 
						   int nbinX, float lowX, float highX,
						   int nbinY, float lowY, float highY,
						   THistID hhid ) 
{
    TDirectory* olddir= gDirectory;
    checkFile();
    THistogram* histp= getHistByName( htitle );
    if( histp == 0 ) {
	TString hname;
	if (fHepTuple)
	    hname = makeName( "h2d", hhid );
	else
	    hname = TString(htitle);
	histp=new THistogram( &*hname, htitle,nbinX, lowX, highX,nbinY, lowY, highY );
	hlist->Add(histp);
    }
    olddir->cd();
    return histp;    
}

TEoverPHistogram* TPersistenceManager::EoverPHistogram(const Text_t *hname,const Text_t *htitle,Int_t nbinX,Axis_t lowX,Axis_t highX
	,Int_t nbinY,Axis_t lowY,Axis_t highY)
{
    TDirectory* olddir= gDirectory;
    checkFile();
    TEoverPHistogram* histp= (TEoverPHistogram*) getHistByName( htitle );
    if( histp == 0 ) {
	histp=new TEoverPHistogram( &*hname, htitle,nbinX, lowX, highX,nbinY, lowY, highY );
	hlist->Add(histp);
    }
    olddir->cd();
    return histp;
}

TMoverPHistogram* TPersistenceManager::MoverPHistogram(const Text_t *hname,const Text_t *htitle,Int_t nbinX,Axis_t lowX,Axis_t highX
	,Int_t nbinY,Axis_t lowY,Axis_t highY)
{
    TDirectory* olddir= gDirectory;
    checkFile();
    TMoverPHistogram* histp= (TMoverPHistogram*) getHistByName( htitle );
    if( histp == 0 ) {
	histp=new TMoverPHistogram( &*hname, htitle,nbinX, lowX, highX,nbinY, lowY, highY );
	hlist->Add(histp);
    }
    olddir->cd();
    return histp;
}

// Profile histos:
THistogram* TPersistenceManager::Profile( const char* htitle, int nbin, 
						 float lowX, float highX,
						 float lowY, float highY ) 
{
    return Profile( htitle, nbin, lowX, highX, lowY, highY, THistID( 0 ) );  
}

THistogram* 
TPersistenceManager::Profile( const char* htitle, int nbin, 
				 float lowX, float highX,
				 float lowY, float highY, THistID hhid ) 
{
    TDirectory* olddir= gDirectory;
    checkFile();
    THistogram* histp= getHistByName( htitle );
    if( histp == 0 ) {
	TString hname;
	if (fHepTuple)
	    hname = makeName( "hpr", hhid );
	else
	    hname = TString(htitle);
	histp=new THistogram( &*hname, htitle,nbin, lowX, highX,lowY, highY );
	hlist->Add(histp);
    }
    olddir->cd();
    return histp;
    
}


// Ntuples:
TTuple* TPersistenceManager::Ntuple( const char* nttitle ) 
{
    // Map onto two-argument function below:
    return Ntuple( nttitle, THistID( 0 ) );
}

TTuple* TPersistenceManager::Ntuple( const char* nttitle, 
					    THistID hhid ) 
{
    // Try to find ntuple by title in list of managed ntuples:
    TDirectory* olddir= gDirectory;
    checkFile();
    TTuple* ntp= 0;
    for( int i= 0; i < ntlist->GetSize(); ++i ) {
	if( ((TTuple*)ntlist->At(i))->Title() == nttitle ) {
	    ntp= (TTuple*)ntlist->At(i);
	    break;
	}
    }
    // If no ntuple is found create one in the current directory:
    if( ntp == 0 ) {
	TString ntname;
	if (fHepTuple)
	    ntname = makeName( "ntp", hhid );
	else
	    ntname = TString(nttitle);
	ntp=new TTuple( &*ntname, nttitle );
	ntlist->Add( ntp );
    }
    olddir->cd();
    return ntp;
    
}

void TPersistenceManager::DumpAllData() 
{
    for( int i= 0; i < ntlist->GetSize(); ++i ) ((TTuple*)ntlist->At(i))->DumpData();
    return;
}

void TPersistenceManager::ClearAll() 
{
    olist->Delete(); // This removes the objects and clears the lists
    hlist->Delete();
    ntlist->Delete();
    return;
}

int TPersistenceManager::Save() 
{
    checkFile();
    rtfilep->Write( rtfilep->GetName(), TObject::kOverwrite );
    for (int i=0;i<olist->GetSize();i++) {
	olist->At(i)->Write();
    }

    return 0;
}

// Helper function for automatic histo creation or returning.
// First check if a hist with this title already exists in 
// the current directory, if so match it to the corresponding
// THistogram and return its pointer:
THistogram* TPersistenceManager::getHistByName( const char* title ) 
{
    THistogram* histp= 0;
    TIter iter( gDirectory->GetList() );
    TObject* obj;
    while( (obj= iter.Next()) ) {
	if( obj->InheritsFrom( "TH1" ) && 
	    !strcmp( obj->GetName(), title ) ) break;
    }
    for( int i= 0; i < olist->GetSize(); ++i ) {
	if( ((THistogram*)olist->At(i))->PtrIsEqual( obj ) ) {
	    histp= (THistogram*)olist->At(i);
	    break;
	}
    }
    return histp;
}

THistogram* TPersistenceManager::getHistByTitle( const char* title ) 
{
    THistogram* histp= 0;
    TIter iter( gDirectory->GetList() );
    TObject* obj;
    while( (obj= iter.Next()) ) {
	if( obj->InheritsFrom( "TH1" ) && 
	    !strcmp( obj->GetTitle(), title ) ) break;
    }
    for( int i= 0; i < olist->GetSize(); ++i ) {
	if( ((THistogram*)olist->At(i))->PtrIsEqual( obj ) ) {
	    histp= (THistogram*)olist->At(i);
	    break;
	}
    }
    return histp;
}

// --------------------
// non-member functions
// --------------------

std::ostream&  operator << (std::ostream& o, const TPersistenceManager& a) {a.PrintOn(o); return o;}
