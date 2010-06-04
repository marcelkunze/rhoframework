//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: VtxVertexR.cxx,v 1.1.1.1 2001-05-15 07:01:32 marcel Exp $
//
// Description:
//      see VtxVertexR.h
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Rolf Dubitzky
//
// Copyright Information:
//
//------------------------------------------------------------------------
#include "KangaSchema/VtxVertexR.h"
#include "RhoBase/TSimpleVertex.h"
#include "RhoBase/TFactory.h"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp( VtxVertexR )


//----------------
// Constructors --
//----------------
VtxVertexR::VtxVertexR() 
  : BtaAbsVertexR()
{
//    cout << "VtxVertexR::VtxVertexR()" << endl;
}
/*
VtxVertexR::VtxVertexR( const VtxVertex* aVertex,
                        RooEvtObjLocReg & aRegistry ) 
  : BtaAbsVertexR( ) 
{
//    cout << "VtxVertexR::VtxVertexR(Vtx,Reg)" << endl;
    _chiSquared = (float) aVertex->chiSquared();
    _nDof       = max(0,aVertex->nDof());
    unsigned i,j;
    unsigned k(0),l(0);
    for ( i=0; i<3; i++ ) _xv[i] = (aVertex->v4())[ i ];
    for ( i=0; i<3; i++ ) 
    {
        for( j=0; j<3; j++ )
        {
            _xpCov[k++] = (aVertex->xpCov())[i][j];
            if ( j>=i ) 
                _xxCov[l++] = aVertex->xxCov().fast(j+1,i+1);
        }
    }
    registerThis( aVertex, aRegistry );
}
*/
//--------------
// Destructor --
//--------------
VtxVertexR::~VtxVertexR() 
{}


//--------------
// Operations --
//--------------
VAbsVertex* VtxVertexR::Transient() const 
{
   // Create VtxVertex
/*   HepVector vx(3);
   HepSymMatrix Cxx(3);
   HepMatrix Cxp(3,3);
   unsigned i,j;
   unsigned k(0),l(0);
   for (i=0;i<3;++i)vx[i]=_xv[i];
   for (i=0;i<3;++i) for(int j=0; j<3; ++j){
       Cxp[i][j]=_xpCov[k++];
       if(j>=i) Cxx[j][i]=_xxCov[l++];
   }
*/
   TSimpleVertex* transVtx = 
       TFactory::NewVertex(TSimpleVertex(_xv[0],_xv[1],_xv[2],0.0,_nDof,_chiSquared));

   // return new VtxVertex
   return ( transVtx ) ;
}


//              -------------------------------------------
//              -- Protected Function Member Definitions --
//              -------------------------------------------
