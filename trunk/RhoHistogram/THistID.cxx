//////////////////////////////////////////////////////////////////////////
//                                                                      //
// THistID				    				//
//                                                                      //
// This class allows ID numbers, ID strings or both to be used		//
// to uniquely identify histograms.  The actual use of this		//
// class depends on the concrete package that implements HepTuple	//
//                                                                      //
// Author: Marcel Kunze, RUB, Feb. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "RhoHistogram/THistID.h"

THistID::THistID(int theNumber) : 
_numSet(1), 
_stringSet(0),
_idNumber(theNumber),
_idString() {}

THistID::THistID(const char * theString) :
_numSet(0), 
_stringSet(1),
_idNumber(0),
_idString(theString) {}

THistID::THistID(int theNumber,const char * theString) :
_numSet(1), 
_stringSet(1),
_idNumber(theNumber),
_idString(theString) {}

THistID::THistID(const THistID & other) :
_numSet(other._numSet), 
_stringSet(other._stringSet),
_idNumber(other._idNumber),
_idString(other._idString) {}

// prefix increment number ID
THistID & THistID::operator++() { 
    if ( _numSet ) {
	++_idNumber;
    };
    // return this
    return *this; 
}

// postfix increment number ID
const THistID THistID::operator++(int) { 
    // initialize result
    THistID result(*this);
    // increment this 
    ++(*this);
    // return result
    return result; 
}

// prefix decrement number ID
THistID & THistID::operator--() { 
    if ( _numSet ) {
	--_idNumber;
    };
    // return this
    return *this; 
}

// postfix decrement number ID
const THistID THistID::operator--(int) { 
    // initialize result
    THistID result(*this);
    // increment this 
    --(*this);
    // return result
    return result; 
}
