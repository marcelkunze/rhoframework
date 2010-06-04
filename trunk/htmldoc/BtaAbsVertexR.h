//----------*- Mode: c++ -*---------------------------------------------------
// File and Version Information:
// 	$Id: BtaAbsVertexR.h,v 1.1.1.1 2002-12-20 15:05:29 marcel Exp $
//
// Description:
//     This is the abstract base class for all ROOT persistent vertices
//     where the transient  Vertex inherits BtaAbsVertex.
//        - transient is implemented empty, so we can have Scribes for 
//          vertices. This is only used for testpurposes at this moment (nov2000)
// 
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     Rolf Dubitzky                     
//
// Copyright Information:
//      Copyright (C) 2000      TU-Dresden
//     
//
//------------------------------------------------------------------------
#if !defined( BtaAbsVertexR_hh )
#define BtaAbsVertexR_hh


//----------------------
// Base Class Headers --
//----------------------
#include "KangaSchema/RooEvtObj.h"

//---------------
// C++ Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

//------------------------
// Forward declarations --
//------------------------
class BtaAbsVertex;
class VAbsVertex;
class RooEvtObjLocReg;


//		---------------------
// 		-- Class Interface --
//		---------------------
 
class BtaAbsVertexR : public RooEvtObj<BtaAbsVertex> {
  public:
    //
    // Public interface
    //

    // Constructor:
    BtaAbsVertexR();
    //BtaAbsVertexR( const BtaAbsVertex*, RooEvtObjLocReg & ) {};

    // Destructor:
    virtual ~BtaAbsVertexR();

    // Selectors (const)
    virtual VAbsVertex* Transient( /*RooEvtObjLocReg& aRegistry*/ ) const ;

  private:

    // Data members:

    // disallowed:
    //BtaAbsVertexR( const BtaAbsVertexR & );
    //BtaAbsVertexR & operator = ( const BtaAbsVertexR & );

    ClassDef(BtaAbsVertexR,1)

};


#endif
