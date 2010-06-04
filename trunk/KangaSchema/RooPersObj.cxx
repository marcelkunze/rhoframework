//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RooPersObj.cxx,v 1.1.1.1 2001-05-15 07:02:06 marcel Exp $
//
// Description:
//	Class RooPersObj
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	A.Salnikov		originator
//
// Copyright Information:
//	Copyright (C) 1999    LBNL
//
//------------------------------------------------------------------------
//#include "BaBar/BaBar.h"

//-----------------------
// This Class's Header --
//-----------------------
#include "KangaSchema/RooPersObj.h"

//-------------
// C Headers --
//-------------
extern "C" {
}

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
//#include "RooUtils/RooEvtObjLocReg.h"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

ClassImp(RooPersObj)

//----------------
// Constructors --
//----------------

RooPersObj::RooPersObj() 
  : TObject()
{
  TObject::SetUniqueID( ++ staticId ) ;
}


RooPersObj::RooPersObj( const RooPersObj& o )
  : TObject(o)
{
  TObject::SetUniqueID( ++ staticId ) ;
}

//--------------
// Destructor --
//--------------

RooPersObj::~RooPersObj() 
{
}

//-------------
// Methods   --
//-------------

RooRef
RooPersObj::refToMe( ) const 
{
  return RooRef( TObject::GetUniqueID() ) ;
}

/*
void
RooPersObj::registerThis ( const AbsEvtObj* aObject ,
			   RooEvtObjLocReg& aRegistry ) const
{
  aRegistry.insert( aObject, refToMe() ) ;
}
*/

//-------------
// Operators --
//-------------

RooPersObj& 
RooPersObj::operator= ( const RooPersObj& o)
{
  TObject::operator= (o) ;
  TObject::SetUniqueID( ++ staticId ) ;
  return *this ;
}


//-------------
// Selectors --
//-------------
    
//-------------
// Modifiers --
//-------------

//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

UInt_t RooPersObj::staticId = 0 ;

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------
