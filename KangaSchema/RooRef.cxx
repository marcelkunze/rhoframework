//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RooRef.cxx,v 1.1.1.1 2001-05-15 07:00:58 marcel Exp $
//
// Description:
//	Class RooRef
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//	Andy Salnikov           Originator
//
// Copyright Information:
//	Copyright (C) 1997	<Institution>
//
//------------------------------------------------------------------------
//#include "BaBar/BaBar.h"

//-----------------------
// This Class's Header --
//-----------------------
#include "KangaSchema/RooRef.h"

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

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

ClassImp ( RooRef )

//----------------
// Constructors --
//----------------

RooRef::RooRef( UInt_t id )
  : id_(id)
{
}

RooRef::RooRef( const RooRef& o )
  : id_(o.id_)
{
}

RooRef::RooRef()
  : id_(0)
{
}


//--------------
// Destructor --
//--------------
RooRef::~RooRef( )
{
}

//-------------
// Methods   --
//-------------

//-------------
// Operators --
//-------------

RooRef&
RooRef::operator= ( const RooRef& o ) {
  id_ = o.id_ ;
  return *this ;
}
Bool_t
RooRef::operator== ( const RooRef& o ) const {
  return id_ == o.id_ ;
}
Bool_t
RooRef::operator!= ( const RooRef& o ) const {
  return id_ != o.id_ ;
}

UInt_t
RooRef::id() const {
  return id_ ;
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

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------
