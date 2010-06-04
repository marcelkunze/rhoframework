#ifndef SKIM_H
#define SKIM_H
//--------------------------------------------------------------------------
// Analysis example class in PAF(Pico Analysis Framework)
// class Skim
//
// Write out skim files
//
// Author: M. Kunze, Bochum University, Apr. 00
//
//------------------------------------------------------------------------

#include "RhoManager/TModule.h"

class TEventWriter;

class Skim : public TModule {
    
    //   ---------------------
    //   -- Class Interface --
    //   ---------------------
    
public:
    Skim(const char* const theName, const char* const theDescription);
    virtual ~Skim();        
    virtual Bool_t BeginJob(TEventManager*);        
    virtual Bool_t Event(TEventManager*);
    virtual Bool_t EndJob(TEventManager*);
    
private:
    TEventWriter *fWriter;
};      

#endif
