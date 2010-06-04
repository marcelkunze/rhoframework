#ifndef TPERSISTENCEMANAGER_H
#define TPERSISTENCEMANAGER_H
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

#include <iostream>

#include "TFile.h"
#include "TDirectory.h"
#include "TString.h"
#include "THashList.h"
#include "TH1.h"
#include "TH2.h"
#include "RhoBase/TCandList.h"
#include "RhoHistogram/THistID.h"
#include "RhoHistogram/THistogram.h"
#include "RhoHistogram/TTuple.h"

class TPersistenceManager : public TObject {
    
public:
    //Constructor
    TPersistenceManager(const char* file= "framework.root",const char* fmode= "RECREATE");
    //Destructor
    virtual ~TPersistenceManager();
    void SetHepTupleMode(Bool_t yesNo=kTRUE) { fHepTuple = yesNo; }
    Bool_t SetFileName( const char* );
    int Save();
    int Store() { return Save(); }
    Bool_t SetDir( const char* path= "/" );
    const char* GetDir() const;
    size_t GetNumDir() const;
    const char* GetDirName( size_t ) const;
    Bool_t GetDirIndex( const char*, size_t & ) const;
    size_t GetNumHist() const;
    size_t GetNumHist( const char* path ) const;
    void PrintOn ( std::ostream& o=std::cout ) const;
    TObject* Add(TObject* item);
    TObject* Put(TObject* item) { return Add(item); } 
    TObject* Get(const char *name);
    void Remove(TObject* item);
    void Remove(const char *name);
    void ClearAll();
    TPersistenceManager(const TPersistenceManager &h) : rtfilep(h.rtfilep), olist(0) {}
    TPersistenceManager& operator=(const TPersistenceManager &v ) { return *this; }
    // 1D histogram without id
    THistogram* Histogram( const char*, int, float, float );
    // Create a 1D histogram. Behaves generally like ntuple().
    THistogram* Histogram( const char*, int, float, float, THistID ); 
    // 2D histogram without id:
    THistogram* Histogram( const char*, int, float, float, int, float, float );
    // 2D histogram with ID:
    THistogram* Histogram( const char*, int, float, float, int, float, float, THistID );
    // Profile histogram without ID:
    THistogram* Profile( const char*, int, float, float, float, float );
    // Profile histogram with ID:
    THistogram* Profile( const char*, int, float, float, float, float, THistID );
    // Ntuple
    TTuple* Ntuple( const char* nttitle );
    TTuple* Ntuple( const char* nttitle,THistID hhid );
    void DumpAllData();
    // Specific histograms
    TMassHistogram* MassHistogram(const Text_t *name,const Text_t *title,Int_t nbins,Axis_t xlow,Axis_t xup);
    TEnergyHistogram* EnergyHistogram(const Text_t *name,const Text_t *title,Int_t nbins,Axis_t xlow,Axis_t xup);
    TMomentumHistogram* MomentumHistogram(const Text_t *name,const Text_t *title,Int_t nbins,Axis_t xlow,Axis_t xup);
    TEoverPHistogram* EoverPHistogram(const Text_t *name,const Text_t *title,Int_t nbinsx,Axis_t xlow,Axis_t xup
	,Int_t nbinsy,Axis_t ylow,Axis_t yup);
    TMoverPHistogram* MoverPHistogram(const Text_t *name,const Text_t *title,Int_t nbinsx,Axis_t xlow,Axis_t xup
	,Int_t nbinsy,Axis_t ylow,Axis_t yup);
protected:        
    TFile* rtfilep;
    THashList *olist;  //! List of external objects
    THashList *hlist;  //! List of internal histograms
    THashList *ntlist; //! List of internal ntuples
    Bool_t fHepTuple;  //! HepTuple mode for names
    
    // Helper function to compose ROOT names:
    const char* makeName( const char*, THistID );
     
    // Helper function to reset current ROOT-file to this manager:
    void checkFile( const char* opt= "" ) const;
    
    // Helper function for setFileName, moves directories and contents
    void movedir( TDirectory* olddir, TDirectory* newdir );
    
    THistogram* getHistByName( const char* );
    THistogram* getHistByTitle( const char* );

public:
    ClassDef(TPersistenceManager,1) // Histogram and ntuple manager
};      

// standalone print
std::ostream&  operator << (std::ostream& o, const TPersistenceManager&);

#endif

