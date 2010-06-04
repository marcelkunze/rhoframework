#ifndef TRHOTIME_H
#define TRHOTIME_H
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRhoTime		    						//
//                                                                      //
// Date and time class							//
//                                                                      //
// Author: Marcel Kunze, RUB, Feb. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <time.h>                /* System time management. */
#include "TObject.h"

class TRhoDate;

class TRhoTime : public TObject
{
    
public:
    
    TRhoTime() : sec(TRhoTime::currentTime()) {;}     // current time
    TRhoTime(unsigned long s) : sec(s) {;}  // Seconds since 00:00:00 Jan 1, 1901
    TRhoTime(unsigned long h, unsigned long m, unsigned long s=0);  // today
    TRhoTime(const TRhoDate&, unsigned long h=0, unsigned long m=0, unsigned long s=0);
    Bool_t   Between(const TRhoTime& a, const TRhoTime& b) const { return *this >= a && *this <= b;}
    int      CompareTo(const TRhoTime*) const;
    unsigned Hour()  const;
    Bool_t   IsValid() const { return sec != 0; }
    TRhoTime  Max(const TRhoTime& t) const;
    TRhoTime  Min(const TRhoTime& t) const;
    unsigned Minute() const;
    unsigned Second() const;         // second: local time or UTC 
    unsigned long Seconds() const {return sec;}
    void Extract(struct tm* tmbuf) const;
    void PrintOn( std::ostream&o=std::cout ) const;

    TRhoTime  operator++() { return TRhoTime(++sec); }
    TRhoTime  operator--() { return TRhoTime(--sec); }
    
    TRhoTime& operator+=(unsigned long s) { sec += s; return *this; }
    TRhoTime& operator-=(unsigned long s) { sec -= s; return *this; }
    
    // Static member functions:
    static TRhoTime Now();          // Return present time
    
    // Logical operators.
    friend Bool_t  operator< (const TRhoTime& t1, const TRhoTime& t2);
    friend Bool_t  operator==(const TRhoTime& t1, const TRhoTime& t2);
    friend Bool_t  operator<=(const TRhoTime& t1, const TRhoTime& t2);
    friend Bool_t  operator> (const TRhoTime& t1, const TRhoTime& t2);
    friend Bool_t  operator>=(const TRhoTime& t1, const TRhoTime& t2);
    friend Bool_t  operator!=(const TRhoTime& t1, const TRhoTime& t2);
    friend TRhoTime operator+(const TRhoTime& t,  unsigned long s );
    friend TRhoTime operator-(const TRhoTime& t,  unsigned long s );
    friend TRhoTime operator+( unsigned long s , const TRhoTime& t);
    
    
protected:
    
    static Bool_t assertDate(const TRhoDate&);
    
private:
    
    static  unsigned long buildFrom(const TRhoDate&, unsigned long h, unsigned long m, unsigned long s);
    static  unsigned long currentTime();
    
    unsigned long         sec;            // Seconds since 1/1/1901.

public:
    ClassDef(TRhoTime,1)  // Time class
};

//standalone print
std::ostream& operator << (std::ostream& o, const TRhoTime& );
/*
inline Bool_t  operator==(const TRhoTime& t1, const TRhoTime& t2) { return t1.sec == t2.sec; }
inline Bool_t  operator< (const TRhoTime& t1, const TRhoTime& t2) { return t1.sec <  t2.sec; }
inline Bool_t  operator<=(const TRhoTime& t1, const TRhoTime& t2) { return t1.sec <= t2.sec; }
inline Bool_t  operator> (const TRhoTime& t1, const TRhoTime& t2) { return t1.sec >  t2.sec; }
inline Bool_t  operator>=(const TRhoTime& t1, const TRhoTime& t2) { return t1.sec >= t2.sec; }
inline Bool_t  operator!=(const TRhoTime& t1, const TRhoTime& t2) { return t1.sec != t2.sec; }
inline TRhoTime operator+(const TRhoTime& t, unsigned long s) { return TRhoTime(t.Seconds()+s); }
inline TRhoTime operator-(const TRhoTime& t, unsigned long s) { return TRhoTime(t.Seconds()-s); }
inline TRhoTime operator+(unsigned long s, const TRhoTime& t) { return TRhoTime(t.Seconds()+s); }
inline TRhoTime::TRhoTime(const TRhoDate& date, unsigned long h, unsigned long m, unsigned long s) { sec = buildFrom(date, h, m, s); }
*/
#endif
