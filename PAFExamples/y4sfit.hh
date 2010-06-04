#ifndef Y4SFIT_H
#define Y4SFIT_H
//--------------------------------------------------------------------------
// Examine the scan run, count number of Bhabha and hadronic events.
// Perform a fit on the Y4S.
//
// Options:
// USETAG           - Use the TAG to preselect interesting events
// USEAOD           - Use the AOD for a more refined selection
// MAXEVENT         - Max. number of events to process
// VERBOSE          - Print conditions info
//
// Author: Marcel Kunze, Bochum University
//         Roland Waldi, TUD, Y4S-fit
//
// June '99     : Initial version (mk)
// November '99 : Inherit from PAFAnalysis, added PAFConditions support (mk)
//
//------------------------------------------------------------------------

// Set options

#define USETAG        1
#define USEAOD        0

#define SPECIAL       0
#define MAXEVENT 1000000
#define MAXRUN      100
#define VERBOSE       0

#include "RhoManager/TModule.h"

class TEventSelector;
class THistogram;


//--------------------------------------------------------------------------

class y4scan : public TModule {
    
    //   ---------------------
    //   -- Class Interface --
    //   ---------------------
    
public:
    y4scan(const char* const theName, const char* const theDescription);
    virtual ~y4scan();        
    virtual Bool_t BeginJob(TEventManager*);        
    virtual Bool_t Event(TEventManager*);
    virtual Bool_t EndJob(TEventManager*);
    virtual Bool_t EndRun(TEventManager*);
    void Count();
    
    double getCMS(Int_t);
    
private:
            
    TEventSelector *bhabhaSelector, *hadronSelector;
    
    THistogram* cms;
    THistogram* bhabhas;
    THistogram* hadrons;
    
    UInt_t  oldrun;
    UInt_t  counter, totalEvents, acceptedBhabhas, acceptedHadrons;
    UInt_t  nbhabhas, nhadrons;
    UInt_t  rNumber, eNumber;
    ULong_t eTime;
    
    Double_t cmsenergy;
};

//--------------------------------------------------------------------------

class y4sfit : public TModule {
    
    //   ---------------------
    //   -- Class Interface --
    //   ---------------------
    
public:
    y4sfit(const char* const theName, const char* const theDescription);
    virtual ~y4sfit();        
    virtual Bool_t BeginJob(TEventManager*) { return kTRUE; }        
    virtual Bool_t Event(TEventManager*) { return kTRUE; }
    virtual Bool_t EndJob(TEventManager*);
    virtual Bool_t EndRun(TEventManager*) { return kTRUE; }
        
private:
            
};

//--------------------------------------------------------------------------

#endif
