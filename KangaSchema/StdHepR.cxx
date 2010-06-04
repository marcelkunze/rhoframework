//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: StdHepR.cxx,v 1.2 2001-12-17 17:46:13 Marcel Exp $
//
// Description:
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Gautier Hamel de Monchenault
//
// Copyright Information:
//	Copyright (C) 1999       Saclay
//
//------------------------------------------------------------------------

//-----------------------
// This Class's Header --
//-----------------------
#include "KangaSchema/StdHepR.h"


//-------------
// C Headers --
//-------------
#include <math.h>

//---------------
// C++ Headers --
//---------------


//-------------------------------
// Collaborating Class Headers --
//-------------------------------
//#include "CLHEP/Alist/AList.h"
#include "KangaSchema/StdHepTrkR.h"
//#include "StdHepData/StdHep.h"
//#include "StdHepData/StdHepTrk.h"
//#include "ErrLogger/ErrLog.h"


//-----------------------------------------------------------------------
// Local Macros, Typedefs, Structures, Unions and Forward Declarations --
//-----------------------------------------------------------------------
ClassImp(StdHepR)

#include <iostream>
using namespace std;

//----------------
// Constructors --
//----------------
StdHepR::StdHepR() : 
  RooClonesClassObj<StdHep>(),
  _eventNumber( 0 )
{

  // Initialise TClonesArray:
  initialiseArray();

}
/*
StdHepR::StdHepR( const StdHep* transient,  RooEvtObjLocReg & reg  ) :  
  RooClonesClassObj<StdHep>(),
  _eventNumber( transient->eventNumber() )
{

  // Initialise TClonesArray:
  initialiseArray();

  // Set number of entries for TClonesArray:
  _nent= transient->noOfTracks();

  // Get the list of StdHepTrk's and store the persistent objects
  // in the TClonesArray:
  const HepAList<StdHepTrk>* trackList= transient->getListOfTracks();
  for( unsigned i= 0; i < _nent; i++ ) {
    const StdHepTrk* aStdHepTrk= (*trackList)[i];
    new( (*_array)[i] ) StdHepTrkR( aStdHepTrk, reg );
  }

  // Register this object and its corresponding transient:
  registerThis( transient, reg );

}
*/
void StdHepR::initialiseArray() {

  // Initialise TClonesArray:
  if( _theArray == 0 ) {
    _array= new TClonesArray( "StdHepTrkR", 100 );
  }
  else {
    _array= _theArray;
    _ownArray= kFALSE;
  }

  _array->BypassStreamer(kFALSE);

}


//--------------
// Destructor --
//--------------
StdHepR::~StdHepR() {

  _array->Clear();
  if( _ownArray ) delete _array;

}
/*
//--------------
// Operations --
//--------------
StdHep* StdHepR::transient( RooEvtObjLocReg & reg ) const {

  // Create the transient object:
  StdHep* theStdHep= new StdHep( _eventNumber );

  // Fill the list of the transient:
  for( unsigned i= 0; i < _nent; i++ ) {
    TObject* obj= _array->At( i );
    if( obj != 0 ) {
      if( obj->IsA() == StdHepTrkR::Class() ) {
	const StdHepTrkR* theStdHepTrkR= (const StdHepTrkR*) obj;
	StdHepTrk* stdTrk= theStdHepTrkR->transient( reg );
	// Set event number for every transient StdHepTrk (what
	// a waste!):
	stdTrk->setEvent( _eventNumber );
	theStdHep->addTrack( stdTrk );
      }
      else {
	ErrMsg(fatal) << " Wrong persistent type: " << obj->ClassName()
		      << endmsg;
      }
    }
    else {
      ErrMsg(fatal) << " _array returned 0 pointer" << endmsg;
    }

  }

  // Register this object and its corresponding transient:
  registerThis( theStdHep, reg );

  // The End:
  return theStdHep;

}
*/

// Printing:
void  StdHepR::print( std::ostream & ost ) const {

  return;

}
void  StdHepR::Print( Option_t* option ) const {

  cout << ClassName() << endl;
  print( cout );
  return;

}

