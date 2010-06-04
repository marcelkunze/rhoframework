#ifndef ROOEVTOBJ_CC
#define ROOEVTOBJ_CC
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RooEvtObj.cxx,v 1.1.1.1 2001-05-15 07:01:13 marcel Exp $
//
// Description:
//	Class RooEvtObj
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

//-----------------------
// This Class's Header --
//-----------------------
#include "KangaSchema/RooEvtObj.h"
#include "KangaSchema/RooClonesVectorR.h"

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
#include "TMemberInspector.h"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

ClassImpT(RooEvtObj,T)

//----------------
// Constructors --
//----------------

template <class T>
RooEvtObj<T>::RooEvtObj() 
  : RooPersObj()
{
}


template <class T>
RooEvtObj<T>::RooEvtObj( const RooEvtObj<T>& o )
  : RooPersObj(o)
{
}

//--------------
// Destructor --
//--------------

template <class T>
RooEvtObj<T>::~RooEvtObj() 
{
}

//-------------
// Methods   --
//-------------
/*
template <class T>
bool 
RooEvtObj<T>::fillRefs ( const T* trans, const RooEvtObjLocReg& aRegistry ) 
{
  return true ;
}

template <class T>
bool 
RooEvtObj<T>::fillPointers ( T* trans, const RooEvtObjLocReg& reg ) const 
{
  return true ;
}
*/
//-------------
// Operators --
//-------------

template <class T>
RooEvtObj<T>& 
RooEvtObj<T>::operator= ( const RooEvtObj<T>& o)
{
  RooPersObj::operator= (o) ;
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

//		-------------------------------------------
// 		-- Protected Function Member Definitions --
//		-------------------------------------------

//		-----------------------------------------
// 		-- Private Function Member Definitions --
//		-----------------------------------------

//		-----------------------------------
// 		-- Internal Function Definitions --
//		-----------------------------------

// Stream an object of class RooEvtObj<T>:
template <class T> 
void 
RooEvtObj<T>::Streamer( TBuffer & R__b ) 
{
  if( R__b.IsReading() ) {
    Version_t R__v= R__b.ReadVersion();
    // if (R__v) { }
    RooPersObj::Streamer( R__b );
  } 
  else {
    R__b.WriteVersion( RooEvtObj<T>::IsA() );
    RooPersObj::Streamer( R__b );
  }
}

// Read a pointer to an object of type RooEvtObj<T>:
template <class T> 
TBuffer & 
operator >> ( TBuffer & buf, RooEvtObj<T>* & obj ) 
{
  obj= (RooClonesVectorR<T>*) buf.ReadObject( RooClonesVectorR<T>::Class() );
  return buf;
}

#endif
