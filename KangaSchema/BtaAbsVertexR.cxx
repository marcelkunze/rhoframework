//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaAbsVertexR.cxx,v 1.1.1.1 2001-05-15 07:01:28 marcel Exp $
//
// Description:
//      see BtaAbsVertexR.h
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Rolf Dubitzky
//
// Copyright Information:
//      Copyright (C) 2000      TU-Dresden
//
//------------------------------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------
#include "KangaSchema/BtaAbsVertexR.h"


//-------------
// C Headers --
//-------------
// extern "C" {}


//---------------
// C++ Headers --
//---------------
#include "RhoBase/VAbsVertex.h"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
//#include "RooUtils/RooEvtObjLocReg.h"


//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp(BtaAbsVertexR);


//----------------
// Constructors --
//----------------
BtaAbsVertexR::BtaAbsVertexR()
  : RooEvtObj<BtaAbsVertex>()
{}

//--------------
// Destructor --
//--------------
BtaAbsVertexR::~BtaAbsVertexR() 
{}


//--------------
// Operations --
//--------------
VAbsVertex* BtaAbsVertexR::Transient( /*RooEvtObjLocReg &*/ ) const 
{
    return( (VAbsVertex*) 0 ) ;
}

//              -------------------------------------------
//              -- Protected Function Member Definitions --
//              -------------------------------------------
