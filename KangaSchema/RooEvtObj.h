#ifndef ROOEVTOBJ_RDL
#define ROOEVTOBJ_RDL
//						-*- Mode: c++ -*-
//--------------------------------------------------------------------------
//
// Environment:
//      This software was developed for the BaBar collaboration.  If you
//      use all or part of it, please give an appropriate acknowledgement.
//
// Copyright Information:
//      Copyright (C) 1998      LBNL
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
#include "KangaSchema/RooPersObj.h"

//-------------------------------
// Collaborating Class Headers --
//-------------------------------
#include "KangaSchema/RooRef.h"

//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
// class AbsEvtObj ;
//class RooEvtObjLocReg ;


//		---------------------
// 		-- Class Interface --
//		---------------------


/**
 *  Abstract base class for all ROOT-persistable objects
 *
 *  Copyright (C) 1999   LBNL
 *
 *  @version $Id: RooEvtObj.h,v 1.1.1.1 2001-05-15 07:00:55 marcel Exp $ 
 *
 *  @author A.Salnikov		
 */

template <class T>
class RooEvtObj : public RooPersObj {

  //--------------------
  // Declarations     --
  //--------------------

  // Typedefs, consts, and enums

  //--------------------
  // Instance Members --
  //--------------------

public:

  // Constructors
  RooEvtObj() ;                            // default constructor
  RooEvtObj( const RooEvtObj<T>& );            // Copy Constructor

  // Destructor
  virtual ~RooEvtObj( );

  // Operators
  RooEvtObj<T>& operator= ( const RooEvtObj<T>& );    // Assignment op

  //    virtual int operator==( const RooEvtObj<T>& ) const;
  //            int operator!=( const RooEvtObj<T>& ) const;

  /**
   *  Create a transient object from this persistent. Pure virtual
   *  method which should be overwritten in any subclass.
   * 
   *  @param aRegistry - registry object
   */
  //virtual T* transient( RooEvtObjLocReg& reg ) const = 0;

  /**
   *  Fill references in this object. Default implementation is
   *  provided which does nothing.
   *
   *  @param trans - transien representation of the object
   *  @param aRegistry - registry object 
   */
  //virtual Bool_t fillRefs ( const T* trans, const RooEvtObjLocReg& aRegistry ) { return true ; }

  /**
   *  Fill pointers in the transient object. Default implementation is
   *  provided which does nothing.
   *
   *  @param trans - transien representation of the object
   *  @param aRegistry - registry object 
   */
  //virtual Bool_t fillPointers ( T* trans, const RooEvtObjLocReg& reg ) const { return true ; }


protected:

  // Helper functions

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


  ClassDefT(RooEvtObj<T>,1) // BABAR Kanga

};

// Even more special ROOT declarations for template classes:
ClassDefT2(RooEvtObj,T)

// Need to protect BaBar template instantiation against rootcint processing
#ifndef __CINT__
#ifdef BABAR_COMP_INST
#include "KangaSchema/RooEvtObj.cxx"
#endif // BABAR_COMP_INST
#endif // __CINT__

#endif // ROOEVTOBJ_RDL
