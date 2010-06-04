
//--------------------------------------------------------------------------
// File and Version Information:
// 	$Id: RooValArray.cxx,v 1.1.1.1 2001-05-15 07:02:05 marcel Exp $
//
// Description:
//	Class :RooRefArray
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
#include "KangaSchema/RooValArray.h"

ClassImp(RooValArray)

RooValArray::RooValArray()
  : _grow_by( 10 ),
    _size( 10 ),
    _nEntries( 0 ),
    _entries( new UInt_t[10] )
{}

RooValArray::RooValArray( int initial_size, int grow_by)
  : _grow_by( grow_by ),
    _size( initial_size ),
    _nEntries( 0 ),
    _entries( new UInt_t[initial_size] )
{}

RooValArray::RooValArray( const RooValArray &o )
  : _grow_by( o._grow_by ),
    _size( o._nEntries ),
    _nEntries( 0 ),
    _entries( new UInt_t[o._nEntries] )
{
    for( int i=0;i<o._nEntries;++i)
    {
        _entries[i] = o[i];
    }
}

RooValArray::~RooValArray()
{
    delete [] _entries;
}


void
RooValArray::append( const UInt_t t )
{
    if ( _nEntries==_size ) grow();
    _entries[_nEntries++] = t;
    return;
}

void
RooValArray::grow()
{
    UInt_t *t = new UInt_t[ _size+_grow_by ];
    for ( int i=0; i<_size;++i)
    {
        t[i] = _entries[i];
    }
    delete [] _entries;
    _entries = t;
    _size += _grow_by;
    return;
}

void 
RooValArray::Streamer( TBuffer & R__b ) 
{
    UInt_t R__s, R__c;
    if( R__b.IsReading() ) {
        Version_t R__v = R__b.ReadVersion(&R__s, &R__c);
        UShort_t nEntries;
        R__b >> nEntries;
        Int_t i;
        UInt_t buf;
        if ( R__v ) {};
        for (i=0;i<nEntries;++i)
        {
            R__b >> buf;
            append( buf );
        }
        R__b.CheckByteCount(R__s, R__c, RooValArray::IsA());
    } 
    else {
        R__c = R__b.WriteVersion(RooValArray::IsA(), kTRUE);
        R__b << _nEntries;
        int i;
        for (i=0;i<_nEntries;++i)
        {
            R__b << _entries[i];
        }

        R__b.SetByteCount(R__c, kTRUE);
    }
}

// Read a pointer to an RooValArray
TBuffer & 
operator >> ( TBuffer & buf, RooValArray* & obj ) 
{
  obj = (RooValArray*) buf.ReadObject( RooValArray::Class() );
  return buf;
}

