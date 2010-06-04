//						-*- Mode: c++ -*-
#ifndef ROOREF_HH
#define ROOREF_HH

//--------------------------------------------------------------------------
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 1999         LBNL
//
//------------------------------------------------------------------------

//-------------
// C Headers --
//-------------
extern "C" {
	   }

//---------------
// C++ Headers --
//---------------
// #include "Rtypes.h"

//----------------------
// Base Class Headers --
//----------------------
#include "TObject.h"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------

//		---------------------
// 		-- Class Interface --
//		---------------------


/**
 *  RooRef class declaration. Presently ROOT reference is simply an
 *  object ID.
 *
 *  Copyright (C) 1999   LBNL
 *
 *  @see RooRef
 *
 *  @version $Id: RooRef.h,v 1.1.1.1 2002-12-20 15:05:29 marcel Exp $ 
 *
 *  @author A.Salnikov		originator
 */

class RooRef : public TObject {

  //--------------------
  // Declarations     --
  //--------------------
  
  // Typedefs, consts, and enums
  
  //--------------------
  // Instance Members --
  //--------------------

public:
  
  // Constructors

  RooRef( UInt_t id ) ;     /** construct ref from object id number */
  RooRef( const RooRef& );  /** copy constructor */
  RooRef() ;                /** default, or null reference */

  // Destructor
  virtual ~RooRef( );
  
  // Operators
  RooRef& operator= ( const RooRef& o ) ;
  Bool_t operator== ( const RooRef& o ) const ;
  Bool_t operator!= ( const RooRef& o ) const ;
  
  /**
    *  Returns the object id to which this ref refers to
    *  @return  the object id
    *
    *  @see RooRef#id
    */
  UInt_t id() const ;
  
  // Selectors (const)
  
  // Modifiers

protected:
  
  // Helper functions

  // Data members
  UInt_t  id_ ;          // Object ID of the referenced object
  
private:
  
  // Friends

  // Data members
  
  //------------------
  // Static Members --
  //------------------
  
public:
  
  // Selectors (const)
  
  // Modifiers
  
private:
  
  // Data members
  

  ClassDef( RooRef, 1 ) // BABAR Kanga 
};

#endif // ROOREF_HH
