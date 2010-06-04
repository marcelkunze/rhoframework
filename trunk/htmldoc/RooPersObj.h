//						-*- Mode: c++ -*-
#ifndef ROOPERSOBJ_RDL
#define ROOPERSOBJ_RDL

//--------------------------------------------------------------------------
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 1999      LBNL
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

//----------------------
// Base Class Headers --
//----------------------
#include "TObject.h"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "KangaSchema/RooRef.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
//class AbsEvtObj ;
//class RooEvtObjLocReg ;


//		---------------------
// 		-- Class Interface --
//		---------------------


/**
 *  Base class for all ROOT-persistable objects
 *
 *  Copyright (C) 1999   LBNL
 *
 *  @see RooEvtObj
 *
 *  @version $Id: RooPersObj.h,v 1.1.1.1 2002-12-20 15:05:29 marcel Exp $ 
 *
 *  @author A.Salnikov		
 */

class RooPersObj : public TObject {

  //--------------------
  // Declarations     --
  //--------------------

  // Typedefs, consts, and enums

  //--------------------
  // Instance Members --
  //--------------------

public:

  // Constructors
  RooPersObj() ;                            // default constructor
  RooPersObj( const RooPersObj& );            // Copy Constructor

  // Destructor
  virtual ~RooPersObj( );

  // Operators
  RooPersObj& operator= ( const RooPersObj& );    // Assignment op

  //    virtual int operator==( const RooPersObj& ) const;
  //            int operator!=( const RooPersObj& ) const;

  /**
    *  Returns a "ROOT reference" to this object
    *
    *  @return   RooRef object "pointing" to this object
    */
  RooRef        refToMe( ) const ;

  // Selectors (const)

  UInt_t refId() const { return staticId; }

  // Modifiers

protected:

  // Helper functions

  /**
    *  Registers myself, my corresponding transient and RooRef to me in 
    *  the registry object.
    *
    *  @param aTransient - my corresponding transient
    *  @param aRegistry - registry object
    *  @return true if successful, false means already registered
    */
//  void          registerThis ( const AbsEvtObj* aObject ,
//			       RooEvtObjLocReg& aRegistry ) const ;

private:

  // Friends

  // Data members

  // Note: if your class needs a copy constructor or an assignment operator, 
  //  make one of the following public and implement it.

  //------------------
  // Static Members --
  //------------------

public:

  // Selectors (const)

  // Modifiers

private:

  // Data members
  static UInt_t staticId ;

  // This class does not need saving any data (for the moment)
  ClassDef(RooPersObj,0)  // BABAR Kanga

};

#endif // ROOPERSOBJ_RDL
