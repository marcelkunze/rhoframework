//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: BtaCompositeCandR_000.cxx,v 1.1.1.1 2001-05-15 07:02:01 marcel Exp $
//
// Description:
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Rolf Dubitzky
//
// Copyright Information:
//	Copyright (C) 2000	TU-Dresden
//
//------------------------------------------------------------------------
#include "RhoBase/TRho.h"
#include "RhoBase/TFactory.h"
#include "RhoBase/TCandidate.h"
#include "RhoBase/VAbsVertex.h"
#include "TDatabasePDG.h"

//-----------------------
// This Class's Header --
//-----------------------

#include "KangaSchema/BtaCompositeCandR_000.h"
#include "KangaSchema/BtaAbsVertexR.h"

//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp(BtaCompositeCandR_000)


//		-----------------------------------------------
// 		-- Static Data & Function Member Definitions --
//		-----------------------------------------------

//		----------------------------------------
// 		-- Public Function Member Definitions --
//		----------------------------------------

//----------------
// Constructors --
//----------------

BtaCompositeCandR_000::BtaCompositeCandR_000()
: BtaCandidateR(),
_fitParams(),
_decayVtx( 0 ),
_pdtEntryPdgId( 0 ),
_isAResonance( kFALSE ),
_daughters( )
{
    //    cout << "BtaCompositeCandR_000::BtaCompositeCandR_000()" << endl;
}
/*
BtaCompositeCandR_000::BtaCompositeCandR_000( const BtaCandidate* transCand ,
RooEvtObjLocReg& aRegistry )
: BtaCandidateR(),
_fitParams( transCand->content()->fitParams() ),
_decayVtx( 0 ),
_pdtEntryPdgId( 0 ),
_isAResonance( transCand->isAResonance() ),
_daughters( )
{
// Store the location of this object, and the appropriate BtaCandBase,
//    into the registry.
//    cout << "BtaCompositeCandR_000::BtaCompositeCandR_000(cand,reg)" << endl;
assert( transCand==transCand->thisCandidate() );
registerThis( transCand, aRegistry );
//    cout << "      registered"<< "  RefToMe: "<<refToMe().id() << endl;

  // Even if we have a vertex, we only store it if
  //      i) we are not a resonance
  // OR  ii) we are a resonance, and the mother has NOT been persisted
  
    BtaAbsVertexR *vertex( 0 );
    const BtaAbsVertex *toBePersisted( 0 );
    if ( !transCand->isAResonance() ) 
    {
    toBePersisted = transCand->decayVtx();
    } else {
    //        cout << " CompositeCand is a resonace : " << transCand->pdtEntry()->name()<<endl;
    const BtaCandidate *x= transCand->theMother();
    if ( x==0 || aRegistry.find( x->thisCandidate() )==0 ) 
    {
    toBePersisted=transCand->decayVtx();
    }
    }
    
      if ( toBePersisted!=0 ) 
      {
      RooRef aVtxRef = aRegistry.find( toBePersisted );
      if ( aVtxRef.id() == 0 ) 
      {
      BtaVertexRooPersister p;
      p.setRegistry( aRegistry );
      toBePersisted->persist( p );
      vertex = p.getCreation();
      //            cout << "    persisted Vertex " << vertex << endl;
      } else {
      //            cout << "    Already persisted this Vtx   "<< aVtxRef.ref() <<endl;
      vertex = const_cast<BtaAbsVertexR*>(static_cast<const BtaAbsVertexR*>(  aVtxRef.ref() ) );
      }
      }
      
	_decayVtx = vertex;
	}
*/

//--------------
// Destructor --
//--------------

BtaCompositeCandR_000::~BtaCompositeCandR_000()
{
}

//-------------
// Operators --
//-------------

//-------------
// Selectors --
//-------------

TCandidate*
BtaCompositeCandR_000::Transient() const
{
    const TFitParams *fitParams = _fitParams.Transient();
    TCandidate* transient = TFactory::NewCandidate(*fitParams,refToMe().id());
    delete fitParams;
    if ( _pdtEntryPdgId != 0) {
	static TDatabasePDG *pdt = TRho::Instance()->GetPDG();
	transient->SetType(pdt->GetParticle(_pdtEntryPdgId));
    }

/*
    // Id
    cout << "BtaCompositeCandR_000 RefToMe: "<<refToMe().id() << endl;
    
    // Daughters
    int i;
    for (i=0;i<_daughters.length();++i)
    {
	cout << " daughter["<<i<<"] = "<< _daughters[i]<<endl;
    }

    //Vertex
    if (_decayVtx != 0)
       cout << "BtaCompositeCandR_000 Vertex: "<<_decayVtx->refToMe().id() << endl;
*/    
    return transient;
    
}
/*
bool
BtaCompositeCandR_000::fillPointers( BtaCandidate* transCand,
const RooEvtObjLocReg& aRegistry ) const
{
//    cout << "BtaCompositeCandR_000::fillPointers " << endl;

  //     cout << "    my OID: " << refToMe().id() 
  //          << "  trans UID: " <<  transCand->uid()
  //          << "  pdt: " << (transCand->pdtEntry()?transCand->pdtEntry()->name():" none")
  //          << endl;
  
    if ( ( _daughters.length()>0 ) &&
    ( transCand->nDaughters()==0 ) ) // protect against double filling
    {
    //        cout << "   adding daughters: " << endl;
    // If the persistent object has daughters and the transient object
    //   has none, then set the daughters using the registry.
    int i;
    for (i=0;i<_daughters.length();++i)
    {
    //            cout << " daughter["<<i<<"] = "<< _daughters[i]<<endl;
    RooRef childLocation( _daughters[i] );
    BtaCandidate* transDaughter = const_cast< BtaCandidate* >(
    static_cast<const BtaCandidate*>( aRegistry.find( childLocation ) ));
    if ( transDaughter==0 ) 
    {
    ErrMsg(fatal) << " a persistent BtaCand (oid=" << _daughters[i] 
    << ") doesn't have a transient in registry\n " 
    << " Please make sure all needed lists are read back... " << endmsg;
    } else {
    transCand->addDaughterLink( transDaughter );
    }
    }
    }
    
      //  need to set Vertex _AFTER_ daughtes, so that daughters can become outgoings
      if ( _decayVtx != 0 && 
      transCand->decayVtx()==0 ) // protect against double filling
      {
      BtaAbsVertex* transientVertex = const_cast<BtaAbsVertex*>(
      static_cast< const BtaAbsVertex* >( aRegistry.find( _decayVtx->refToMe() ) ) );
      if ( !transientVertex )
      {
      RooEvtObjLocReg& non_const_Registry = const_cast< RooEvtObjLocReg& >( aRegistry );
      transientVertex = _decayVtx->transient( non_const_Registry );
      }
      transCand->setDecayVtx( transientVertex );
      }
      
	return ( true ) ;
	}
	
	  //-------------
	  // Modifiers --
	  //-------------
	  
	    bool
	    BtaCompositeCandR_000::fillRefs( const BtaCandidate* transCand ,
	    const RooEvtObjLocReg& aRegistry)
	    {
	    //    cout << "BtaCompositeCandR_000::fillRefs"<<endl;
	    
	      //////////////////////////
	      // Particle ID information
	      const PdtEntry* pdtEntry( transCand->pdtEntry() ) ;
	      if ( 0 != pdtEntry ) {
	      
		//         cout << "    my OID: " << refToMe().id() 
		//              << "  trans UID: " <<  transCand->uid()
		//              << "  pdt: " << pdtEntry->name()
		//              << endl;
		// If there is a valid PDT entry set the PGD
		_pdtEntryPdgId = (int)pdtEntry->pdgId() ;
		}
		// !!RD debug  -- exxhange with above
		//  _pdtEntryPdgId = transCand->dtEntry() ? (int)transCand->pdtEntry()->pdgId() : 0;
		
		  /////////////////////////////////////////////////////////
		  // loop over all daughters and add references to the list
		  //      of daughter references
		  if ( _daughters.length() > 0 && _daughters.length() != transCand->nDaughters() )
		  cout << "BtaCompositeCandR_000::fillRefs :: filled already with different "
		  << "Ndaughters ... something is fishy"<<endl;
		  
		    if ( 0 != transCand->nDaughters() &&
		    _daughters.length() != transCand->nDaughters() ) {
		    
		      // If BtaCandCand has daughters the link up BtaCompositeCandR_000 objects
		      HepAListIterator< BtaCandidate > children( transCand->daughterIterator() ) ;
		      const BtaCandidate* child;
		      while ( child = children() ) {
		      
			// For each daughter look up Location.
			RooRef childLocation = aRegistry.find( child->thisCandidate() );
			if( 0 != childLocation.id() ) {
			_daughters.append( childLocation.id() );
			//                 cout << " adding daughter: OID: "<<childLocation.id()
			//                      << "  pdt: " << child->pdtEntry()->name()
			//                      <<endl;
			} else {
			ErrMsg( fatal ) << "  daughter has not been persisted! " << endmsg;
			continue;
			}
			}
			} else {
			//        cout << "    no daughters"<<endl;
			}
			
			  return ( true ) ;
			  }
			  
			    // Operators
			    const BtaCompositeCandR_000& 
			    BtaCompositeCandR_000::operator= ( const BtaCompositeCandR_000& o ) 
			    {
			    //    cout << "BtaCompositeCandR_000::operator=   do I need this?!"<<endl;
			    abort(); // let's see
			    return (*this);
			    }
*/