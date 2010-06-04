//////////////////////////////////////////////////////////////////////////
//                                                                      //
// PAFCompAodBase							//
//                                                                      //
// Definition of an aod-object which represents a composite particle	//
//                                                                      //
//    * V0 s								//
//    * Clusters with more than on Bump					//
//    * Particles from PID - lists					//
// Author : Thorsten Brandt , TU Dresden				//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//									//
//////////////////////////////////////////////////////////////////////////

#include "PAFSchema/PAFCompAodBase.h"

ClassImp(PAFCompAodBase)

#include <iostream>
using namespace std;

PAFCompAodBase::PAFCompAodBase()
{
  fAodRef=-1;
  fPdtRef=0;
  for (Int_t i=0; i<NDAUGHTERS; i++) { fDaughters[i] = -1; }
}

PAFCompAodBase::PAFCompAodBase(Int_t aodref, Int_t pdtref):
  fAodRef(aodref), fPdtRef(pdtref) 
{
  for (Int_t i=0; i<NDAUGHTERS; i++) { fDaughters[i] = -1; }
}

void
PAFCompAodBase::SetAodRef(Int_t ref)
{
  fAodRef= (Short_t) ref;
}

void
PAFCompAodBase::SetPdtRef(Int_t ref)
{
  fPdtRef= (Short_t) ref;
}

void 
PAFCompAodBase::AddDaughter(Int_t index)
{
  Int_t i=0;
  while ((i<NDAUGHTERS-1) && (fDaughters[i]>=0)) { i++; }
  if (fDaughters[i]>=0) {
    cout << "*** WARNING : PAFCompAodBase cannot have more than " << NDAUGHTERS << " Daughters" << endl;
  } else {
    fDaughters[i] = (Short_t) index;
  }
}

void
PAFCompAodBase::SetDaughters(Int_t n, Int_t *indexList)
{
  if (n>NDAUGHTERS) {
    cout << "*** WARNING : PAFCompAodBase cannot have more than " << NDAUGHTERS << " Daughters" << endl;
  } else {
    Int_t i;
    for (i=0; i<n; i++) {
      fDaughters[i] =  (Short_t) (indexList[i]);
    }
    for (i=n; i<NDAUGHTERS; i++) { fDaughters[i]=-1; }
  }
}

Int_t
PAFCompAodBase::GetDaughter(Int_t which)
{
  if ((which>NDAUGHTERS) || (which<0)) { return -1; }
  return fDaughters[which];
}

void
PAFCompAodBase::Reset()
{
  for (Int_t i=0; i<NDAUGHTERS; i++) { fDaughters[i]= -1; }
  fAodRef=-1;
  fPdtRef=0;
}

void
PAFCompAodBase::PrintOn(std::ostream &o)
{
  o << " Particle type " << fPdtRef << " with mother " << fAodRef << " and Daugthers ";
  for (Int_t i=0; i<NDAUGHTERS; i++) {
    if (fDaughters[i]!=-1) { o << fDaughters[i] << " ; "; }
  }
  o << endl;
}
