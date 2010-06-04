//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaCandListR_001.cxx,v 1.1.1.1 2001-05-15 07:01:54 marcel Exp $
//
// Description:
//      Stores list of BtaCandPtr for the packing/unpacking of the
//      lists in the microdb
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Stefan Kluth     
//
// Copyright Information:
//	Copyright (C) 1999       LBNL
//
//------------------------------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------
#include "KangaSchema/BtaCandListR_001.h"


//-------------
// C Headers --
//-------------
// extern "C" {}
#include "limits.h"

//---------------
// C++ Headers --
//---------------


//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "KangaSchema/BtaCandPtrR.h"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp(BtaCandListR_001)


//----------------
// Constructors --
//----------------
BtaCandListR_001::BtaCandListR_001() :
RooEvtObj<BtaCandList>(),
_key(),
_indices(),
_lundIds(),
_oneToMany(kFALSE)
{}

//--------------
// Destructor --
//--------------
BtaCandListR_001::~BtaCandListR_001() {}


//--------------
// Operations --
//--------------

BtaCandPtrR* BtaCandListR_001::GetCandPtr(UInt_t i)
{
    if (i>_lundIds.GetSize()) return 0;

    BtaCandPtrR *bcp = new BtaCandPtrR(5);
    bcp->_lundId = _lundIds.At(i);
    if(_oneToMany)
	bcp->_oneToMany = kTRUE;
    else
	bcp->_oneToMany = kFALSE;
    
    int nCand = _lundIds.GetSize();
    int currentInd = 0;
    int n = 0;
    bcp->_indices.Reset();
    for (int j=0;j<nCand;j++){
	if (j>i) break;
	int nInd = _indices.At(currentInd++);
	for (int k=0;k<nInd;k++) {
	    int index = _indices.At(currentInd++);
	    if(index == UCHAR_MAX ) index = -1;
	    if (j==i) bcp->_indices[n++] = index;
	}
    }
    bcp->_indices.Set(n);

    return bcp;
}