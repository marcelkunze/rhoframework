#ifndef THISTOGRAM_H
#define THISTOGRAM_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// THistogram			    				//
//                                                                      //
// Histogram class							//
//                                                                      //
// Author: Marcel Kunze, RUB, March 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TH1.h"
#include "TH2.h"

class TCandList;
class TCandidate;

class TObject;
class TH1;
class THistID;

class THistogram : public TNamed { 
    
public:
    
    // Constructors:
    // 1-D:
    THistogram( const char*, const char*, Int_t, Axis_t, Axis_t );
    // 2-D:
    THistogram( const char*, const char*, Int_t, Axis_t, Axis_t, Int_t, Axis_t, Axis_t );
    // Profile:
    THistogram( const char*, const char*, Int_t, Axis_t, Axis_t, Axis_t, Axis_t );
    
    // Destructor:
    virtual ~THistogram();
    
    // Functions to fill a histo:
    void Accumulate( Axis_t, Stat_t weight= 1.0 );
    void Accumulate1( Axis_t, Stat_t weight= 1.0 );
    void Accumulate( Axis_t x, Axis_t y, Stat_t weight );
    void Accumulate2( Axis_t, Axis_t, Stat_t weight= 1.0 );
    
    // clear all contents
    void Reset();
    
    // accessors:
    const char* Title() const { return GetTitle(); };
    Float_t GetContents( Int_t, Int_t nbinsY= 0 ) const;
    
    
    // error on a content of a bin
    Float_t GetErrors( Int_t, Int_t nbinsY= 0 ) const;
    
    
    // contents of a bin
    Int_t GetEntries() const;
    // total number of entries
    Float_t GetWtSum() const;
    // weighted sum
    Int_t GetNbins( Int_t ) const;
    // number of bins in a dimension
    Float_t GetLow( Int_t ) const;
    // low edge of a dimension
    Float_t GetHigh( Int_t ) const;
    // high edge of a dimension
    Float_t GetAvg( Int_t ) const; // average X * wtsum
    // average of theDim
    Float_t GetCovar( Int_t, Int_t dim2= 0 ) const;
    // covariance between the two dimensions
    Int_t GetType() const;
    
    // Return the THistID:
    THistID GetHistID() const;
    
    // Test if input pointer matches pointer to ROOT histo:
    Bool_t PtrIsEqual( TObject* ptr ) const;
    
    THistogram& operator<<(Axis_t x);

    // Extra functionality
    void Fill( Axis_t x ) { Accumulate1(x); }
    void Fill( Axis_t x, Axis_t y ) { Accumulate2(x,y); }
    void SetFillColor(Color_t fcolor) { histp->SetFillColor(fcolor); }
    virtual void Draw(Option_t *option="") { histp->Draw(option); }

private:
    
    // Satisfy Scotts weird function
    void setEntries( Int_t ) {}; 
    
    
    // Data membrs:
    TH1* histp;
    
public:
    ClassDef(THistogram,1) //T histogram
};

//----------------Spezialization-----------------

class TMassHistogram : public THistogram {
    
public:
    //Constructor
    TMassHistogram(const Text_t *name,const Text_t *title,Int_t nbins,Axis_t xlow,Axis_t xup); 
    void Accumulate( Axis_t, Stat_t weight= 1.0 );
    void Accumulate(TCandList &l);
    TMassHistogram& operator<<(TCandList &l);
    void Accumulate(TCandidate &p);
    TMassHistogram& operator<<(TCandidate &p);
    //Destructor
    virtual ~TMassHistogram();          
private:
public:
    ClassDef(TMassHistogram,1) //Mass histogram
}; 

class TEnergyHistogram : public THistogram {
    
public:
    //Constructor
    TEnergyHistogram(const Text_t *name,const Text_t *title,Int_t nbins,Axis_t xlow,Axis_t xup);
    void Accumulate( Axis_t, Stat_t weight= 1.0 );
    void Accumulate(TCandList &l);
    TEnergyHistogram& operator<<(TCandList &l);
    void Accumulate(TCandidate &p);
    TEnergyHistogram& operator<<(TCandidate &p);
    //Destructor
    virtual ~TEnergyHistogram();          
private:
public:
    ClassDef(TEnergyHistogram,1) //Energy histogram
}; 

class TMomentumHistogram : public THistogram {
    
public:
    //Constructor
    TMomentumHistogram(const Text_t *name,const Text_t *title,Int_t nbins,Axis_t xlow,Axis_t xup);
    void Accumulate( Axis_t, Stat_t weight= 1.0 );
    void Accumulate(TCandList &l);
    void Accumulate(TCandList &l, Int_t);
    TMomentumHistogram& operator<<(TCandList &l);
    void Accumulate(TCandidate &p);
    TMomentumHistogram& operator<<(TCandidate &p);
    //Destructor
    virtual ~TMomentumHistogram();        
private:
public:
    ClassDef(TMomentumHistogram,1) //Momentum histogram
};

class TEoverPHistogram : public THistogram {
    
public:
    //Constructor
    TEoverPHistogram(const Text_t *name,const Text_t *title,Int_t nbinsx,Axis_t xlow,Axis_t xup
	,Int_t nbinsy,Axis_t ylow,Axis_t yup);
    void Accumulate( Axis_t x, Axis_t y, Stat_t weight=1.0 );
    void Accumulate(TCandList &l);
    TEoverPHistogram& operator<<(TCandList &l);
    void Accumulate(TCandidate &p);
    TEoverPHistogram& operator<<(TCandidate &p);
    //Destructor
    virtual ~TEoverPHistogram();        
private:
public:
    ClassDef(TEoverPHistogram,1) //Energy vs. momentum histogram
};

class TMoverPHistogram : public THistogram {
    
public:
    //Constructor
    TMoverPHistogram(const Text_t *name,const Text_t *title,Int_t nbinsx,Axis_t xlow,Axis_t xup
	,Int_t nbinsy,Axis_t ylow,Axis_t yup);
    void Accumulate( Axis_t x, Axis_t y, Stat_t weight=1.0 );
    void Accumulate(TCandList &l);
    TMoverPHistogram& operator<<(TCandList &l);
    void Accumulate(TCandidate &p);
    TMoverPHistogram& operator<<(TCandidate &p);
    //Destructor
    virtual ~TMoverPHistogram();        
private:
public:
    ClassDef(TMoverPHistogram,1) //Mass vs. momentum histogram
};

class TDalitzPlot : public THistogram {
    
public:
    //Constructor
    TDalitzPlot(const Text_t *name,const Text_t *title,Int_t nbinsx,Axis_t xlow,Axis_t xup
	,Int_t nbinsy,Axis_t ylow,Axis_t yup);
    void Accumulate(TCandList &l1,TCandList &l2,TCandList &l3);
    void Accumulate(TCandidate &p1,TCandidate &p2,TCandidate &p3);
    //Destructor
    virtual ~TDalitzPlot();        
private:
public:
    ClassDef(TDalitzPlot,1) //Generate a Dalitz plot
};

#endif

