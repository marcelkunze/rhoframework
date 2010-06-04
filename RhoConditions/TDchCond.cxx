//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TDchCond			    					//
//                                                                      //
// DCH conditions class							//
//                                                                      //
// Author: Marcel Kunze, RUB, Nov. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "RhoConditions/TDchCond.h"
#include "TMath.h"

ClassImp(DchBetheBlochR)

#include <iostream>
using namespace std;

DchBetheBlochR::DchBetheBlochR() { }

DchBetheBlochR::~DchBetheBlochR() { }

void DchBetheBlochR::PrintOn(std::ostream& o) const {
    o <<" Dch PID Bethe-Bloch:\t" << _functionName << endl;
    
    for ( int i=0; i<_nPar; i++ ) {
	o << "\t" << _par[i];
    }
    
    o << endl;
}

std::ostream&  operator << (std::ostream& o, const DchBetheBlochR& a) { a.PrintOn(o); return o; }

ClassImp(TDchCond)

Double_t
TDchCond::Ionization(Double_t momentum, Double_t mass) const 
{
    if (mass<=0.0) return 0.0;
    double betagamma = momentum/mass;
    double beta = betagamma/sqrt(1 + betagamma*betagamma);
    double beta2= pow(beta,_par[4]);
    return  _par[0]*(_par[1]-beta2-log(_par[2]+pow(betagamma,-_par[3])))/beta2;
}

