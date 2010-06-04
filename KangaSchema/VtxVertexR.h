//----------*- Mode: c++ -*---------------------------------------------------
// File and Version Information:
// 	$Id: VtxVertexR.h,v 1.1.1.1 2001-05-15 07:01:36 marcel Exp $
//
// Description:
//      peristent parter of VtxVertex
// 
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//     Rolf Dubitzky                     
//
// Copyright Information:
//
//------------------------------------------------------------------------
#if !defined( VtxVertexR_hh )
#define VtxVertexR_hh


//----------------------
// Base Class Headers --
//----------------------
#include "KangaSchema/RooEvtObj.h"

//---------------
// C Headers --
//---------------

//-------------------------------
// Collaborating Class Headers --
//-------------------------------

#include "KangaSchema/BtaAbsVertexR.h"

//------------------------
// Forward declarations --
//------------------------
class RooEvtObjLocReg;
class VtxVertex;


//		---------------------
// 		-- Class Interface --
//		---------------------
 
class VtxVertexR : public BtaAbsVertexR {
//class VtxVertexR : public RooEvtObj<VtxVertex> {
  public:
    //
    // Public interface
    //

    // Constructor:
    VtxVertexR();
    //VtxVertexR( const VtxVertex*, RooEvtObjLocReg & );

    // Destructor:
    virtual ~VtxVertexR();

    // Make transient:
    virtual VAbsVertex* Transient() const;

  private:

    // Data members:
    Float_t          _chiSquared;
    UShort_t         _nDof;
    Float_t          _xxCov[6];
    Float_t          _xpCov[9];
    Float_t          _xv[3];

    // disallowed:
    VtxVertexR( const VtxVertexR & );
    VtxVertexR& operator = ( const VtxVertexR & );

    ClassDef(VtxVertexR,1)

};


#endif
