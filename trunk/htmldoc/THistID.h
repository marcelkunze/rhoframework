#ifndef THISTID_H
#define THISTID_H
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

#include "TString.h"

class THistID {
public: 
    // constructors
    THistID(int theNumber);
    // constructor with number
    THistID(const char * theString);
    // constructor with string
    THistID(int theNumber,const char * theString);
    // constructor with both number and string
    THistID(const THistID & other);
    // copy constructor
    
    // accessors
    inline Bool_t isIDnumberSet() const { return _numSet; };
    // number used in constructor
    inline Bool_t isIDstringSet() const { return _stringSet; };
    // string used in constructor
    inline int getIDnumber() const { return _idNumber; };
    // the id number
    const char* getIDstring() const { return _idString.Data(); };
    // the id string
    
    // operations
    THistID & operator++(); // prefix ++
    const THistID operator++(int); // postfix ++
    
    THistID & operator--(); // prefix -- 
    const THistID operator--(int); // postfix -- 
    
private:
    Bool_t _numSet;
    Bool_t _stringSet;
    int _idNumber;
    TString _idString;
};

#endif
