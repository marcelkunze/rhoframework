//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TPidChargedSelector                                                  //
// TPidPlusSelector                                                     //
// TPidMinusSelector                                                    //
// TPidNeutralSelector                                                  //
// TPidMassSelector                                                     //
// TPidEnergySelector                                                   //
// TPidMomentumSelector                                                 //
//                                                                      //
// Selector classes for particle selection                              //
//                                                                      //
// Author List:                                                         //
// Marcel Kunze,  RUB, Feb. 99                                          //
// Copyright (C) 1999-2001, Ruhr-University Bochum.                     //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "TDatabasePDG.h"

#include "RhoSelector/TPidSelector.h"
#include "RhoBase/TRho.h"
#include "RhoBase/TCandidate.h"
#include "RhoBase/VAbsMicroCandidate.h"
#include "RhoManager/TEventManager.h"
#include "RhoManager/TAssociator.h"

//----------------------------------------------------------------

ClassImp(TPidChargedSelector)

TBuffer &operator>>(TBuffer &buf, TPidChargedSelector *&obj)
{
   obj = (TPidChargedSelector *) buf.ReadObject(TPidChargedSelector::Class());
   return buf;
}

TPidChargedSelector::TPidChargedSelector(const char *name, const char *type) : 
VAbsPidSelector(name,type) 
{}

Bool_t TPidChargedSelector::Accept(TCandidate& b) 
{
    if (&b == 0) return kFALSE;
    if (b.GetCharge()==0) return kFALSE;
    SetTypeAndMass(b);
    
    return kTRUE;
}

Bool_t TPidChargedSelector::Accept(VAbsMicroCandidate& b) 
{
    Bool_t decision = kFALSE;
    if (&b == 0) return kFALSE;
    if (b.GetCharge()!=0) decision = kTRUE;
    return decision;
}

//----------------------------------------------------------------

ClassImp(TPidPlusSelector)

TBuffer &operator>>(TBuffer &buf, TPidPlusSelector *&obj)
{
   obj = (TPidPlusSelector *) buf.ReadObject(TPidPlusSelector::Class());
   return buf;
}

TPidPlusSelector::TPidPlusSelector(const char *name, const char *type) : 
VAbsPidSelector(name,type)
{}

Bool_t TPidPlusSelector::Accept(TCandidate& b) 
{ 
    if (&b == 0) return kFALSE;    
    if (b.GetCharge()<0) return kFALSE;
    SetTypeAndMass(b);

    return kTRUE;
}

Bool_t TPidPlusSelector::Accept(VAbsMicroCandidate& b) 
{ 
    Bool_t decision = kFALSE;
    if (&b == 0) return kFALSE;
    if (b.GetCharge()==1) decision = kTRUE;
    return decision;
}

//----------------------------------------------------------------

ClassImp(TPidMinusSelector)

TBuffer &operator>>(TBuffer &buf, TPidMinusSelector *&obj)
{
   obj = (TPidMinusSelector *) buf.ReadObject(TPidMinusSelector::Class());
   return buf;
}

TPidMinusSelector::TPidMinusSelector(const char *name, const char *type) : 
VAbsPidSelector(name,type)
{}

Bool_t TPidMinusSelector::Accept(TCandidate& b) 
{ 
    if (&b == 0) return kFALSE;    
    if (b.GetCharge()>0) return kFALSE;
    SetTypeAndMass(b);

    return kTRUE;
}

Bool_t TPidMinusSelector::Accept(VAbsMicroCandidate& b) 
{ 
    Bool_t decision = kFALSE;
    if (&b == 0) return kFALSE;
    if (b.GetCharge()==-1) decision = kTRUE;
    return decision;
}

//----------------------------------------------------------------

ClassImp(TPidNeutralSelector) 

TBuffer &operator>>(TBuffer &buf, TPidNeutralSelector *&obj)
{
   obj = (TPidNeutralSelector *) buf.ReadObject(TPidNeutralSelector::Class());
   return buf;
}

TPidNeutralSelector::TPidNeutralSelector(const char *name, const char *type) : 
VAbsPidSelector(name,type)
{}

Bool_t TPidNeutralSelector::Accept(TCandidate& b) 
{ 
    if (&b == 0) return kFALSE;   
    if (b.GetCharge()!=0) return kFALSE;
    SetTypeAndMass(b);

    return kTRUE;
}

Bool_t TPidNeutralSelector::Accept(VAbsMicroCandidate& b) 
{ 
    Bool_t decision = kFALSE;
    if (&b == 0) return kFALSE;
    if (b.GetCharge()==0) decision = kTRUE;
    return decision;
}

//----------------------------------------------------------------

ClassImp(TPidMassSelector)

TBuffer &operator>>(TBuffer &buf, TPidMassSelector *&obj)
{
   obj = (TPidMassSelector *) buf.ReadObject(TPidMassSelector::Class());
   return buf;
}

TPidMassSelector::TPidMassSelector(const char *name, Double_t cv,Double_t w,const char *type) : 
VAbsPidSelector(name,type), centralValue(cv), window(0.5*w) 
{}

Bool_t TPidMassSelector::Accept(TCandidate& b) 
{ 
    if (&b == 0) return kFALSE;
    SetTypeAndMass(b);
    return (fabs(b.GetMass()-centralValue)<window);
}

Bool_t TPidMassSelector::Accept(VAbsMicroCandidate& b) 
{ 
    if (&b == 0) return kFALSE;
    return (fabs(b.GetLorentzVector().Mag()-centralValue)<window);
}

//----------------------------------------------------------------

ClassImp(TPidEnergySelector)

TBuffer &operator>>(TBuffer &buf, TPidEnergySelector *&obj)
{
   obj = (TPidEnergySelector *) buf.ReadObject(TPidEnergySelector::Class());
   return buf;
}

TPidEnergySelector::TPidEnergySelector(const char *name, Double_t cv,Double_t w,const char *type) : 
VAbsPidSelector(name,type), centralValue(cv), window(0.5*w) 
{}

Bool_t TPidEnergySelector::Accept(TCandidate& b) 
{ 
    if (&b == 0) return kFALSE;
    SetTypeAndMass(b);
    return (fabs(b.E()-centralValue)<window);
}

Bool_t TPidEnergySelector::Accept(VAbsMicroCandidate& b) 
{ 
    if (&b == 0) return kFALSE;
    return (fabs(b.GetEnergy()-centralValue)<window);
}

//----------------------------------------------------------------

ClassImp(TPidMomentumSelector)

TBuffer &operator>>(TBuffer &buf, TPidMomentumSelector *&obj)
{
   obj = (TPidMomentumSelector *) buf.ReadObject(TPidMomentumSelector::Class());
   return buf;
}

TPidMomentumSelector::TPidMomentumSelector(const char *name, Double_t cv,Double_t w,const char *type) : 
VAbsPidSelector(name,type), centralValue(cv), window(0.5*w) 
{}

Bool_t TPidMomentumSelector::Accept(TCandidate& b) 
{ 
    if (&b == 0) return kFALSE;
    SetTypeAndMass(b);
    return (fabs(b.P()-centralValue)<window);
}

Bool_t TPidMomentumSelector::Accept(VAbsMicroCandidate& b) 
{ 
    if (&b == 0) return kFALSE;
    return (fabs(b.GetMomentum().Mag()-centralValue)<window);
}


