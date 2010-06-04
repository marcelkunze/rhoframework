#if !defined( RooValArray_hh )
#define RooValArray_hh

//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RooValArray.h,v 1.1.1.1 2001-05-15 07:01:37 marcel Exp $
//
// Description:
//	Class :RooValArray
//
//////////////////////////////////////////////////////////////////////
//                                                                  //
//   RooValArray is a temporary fix.                                //
//   BaBar software 8.x.x is based on ROOT 2.23-09. This ROOT       //
//   version does nor support variable size arrays of fundamental   //
//   types in persistent classes. ROOT >= 2.24-01 does support      //
//   thia. So this class might disappear once we have 9.x.x series. //
//   For this reason only rudimentary functionality is implemented. //
//                                                                  //
//////////////////////////////////////////////////////////////////////
//
// Environment:
//	Software developed for the BaBar Detector at the SLAC B-Factory.
//
// Author List:
//      Rolf Dubitzky         
//
// Copyright Information:
//      Copyright (C) 2000      TU-Dresden
//
//------------------------------------------------------------------------

#include "TObject.h"

class RooValArray
{
  public:
    RooValArray();        
    RooValArray( int initial_size, int grow_by=10 );
    RooValArray( const RooValArray & );
    virtual ~RooValArray();

    Int_t length() const { return _nEntries; }       // number of valid entries in container

    UInt_t& operator[]( int i ) { return _entries[i]; }              // !! NO bound checking !!
    const UInt_t& operator[]( int i ) const { return _entries[i]; }  // !! NO bound checking !!

    virtual void append( const UInt_t t );    // appent at end and resize if necessary

  protected:
    virtual void grow();   // grow container if current size exceeded

    const RooValArray& operator=( const RooValArray& );

  private:
    UShort_t _grow_by;     // on resize grow container by this many elements
    UShort_t _size;        // current size of container
    UShort_t _nEntries;    // number of valid entries in container
    UInt_t *_entries;      // container

    // ROOT Interface
    ClassDef( RooValArray, 1 )
};

#endif
