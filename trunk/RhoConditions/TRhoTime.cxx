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

#include "RhoConditions/TRhoDate.h"
#include "RhoConditions/TRhoTime.h"

ClassImp(TRhoTime)

#include <iostream>
using namespace std;

static const unsigned long SECONDS_IN_DAY  = 86400L;
static const unsigned long SECONDS_IN_HOUR = 3600L;
static const unsigned      SECONDS_IN_MIN  = 60;

/*
* TRhoTime constants
* use julian day numbers (instead of TRhoDates) so as not to depend 
* on static constructor initialization:
*/
const unsigned long refDate_jday = Jul1901;   // jday for TRhoDate(0, 0)
const unsigned long maxDate_jday = 2465095L;  // jday for TRhoDate(49709, 0); 
// ((2**32)-1)/SECONDS_IN_DAY -1

/****************************************************************
*								*
*			CONSTRUCTORS				*
*								*
****************************************************************/

// Specified time and today's date:
TRhoTime::TRhoTime(unsigned long h,unsigned long m,unsigned long s)
{
    TRhoDate today;
    sec = TRhoTime::buildFrom(today, h, m, s);
}

/****************************************************************
*								*
*			PUBLIC FUNCTIONS			*
*								*
****************************************************************/

int TRhoTime::CompareTo(const TRhoTime* t) const
{
    return sec == t->sec ? 0 : (sec > t->sec ? 1 : -1);
}

unsigned TRhoTime::Hour() const 
{
    return (unsigned)((sec % SECONDS_IN_DAY) / SECONDS_IN_HOUR);
} 

TRhoTime TRhoTime::Max(const TRhoTime& t) const 
{
    if ( *this > t ) return *this;
    return t;
}

TRhoTime TRhoTime::Min(const TRhoTime& t) const 
{
    if ( *this < t ) return *this;
    return t;
}

unsigned TRhoTime::Minute() const 
{
    return (unsigned)
        (((sec % SECONDS_IN_DAY) % SECONDS_IN_HOUR) / SECONDS_IN_MIN);
} 

unsigned TRhoTime::Second() const 
{
    return (unsigned)
        (((sec % SECONDS_IN_DAY) % SECONDS_IN_HOUR) % SECONDS_IN_MIN);
}

/*************** public static member functions *******************/


void TRhoTime::Extract(struct tm* tmbuf) const
{
    TRhoDate date((unsigned)(Seconds() / SECONDS_IN_DAY), 0);
    date.Extract(tmbuf);
    tmbuf->tm_hour  = Hour();
    tmbuf->tm_min   = Minute();
    tmbuf->tm_sec   = Second();
    tmbuf->tm_isdst = 0;
}

TRhoTime TRhoTime::Now()
{
    return TRhoTime(TRhoTime::currentTime());
}

void TRhoTime::PrintOn(std::ostream& o) const
{
    struct tm tmbuf;
    Extract(&tmbuf);
    o << tmbuf.tm_mon;
    o << "/";
    o << tmbuf.tm_mday;
    o << "/";
    o << tmbuf.tm_year;
    o << " ";
    o << tmbuf.tm_hour;
    o << ":";
    o << tmbuf.tm_min;
    o << ":";
    o << tmbuf.tm_sec;
    o << " ";
}

// --------------------
// non-member functions
// --------------------

std::ostream&  operator << (std::ostream& o, const TRhoTime& a) {a.PrintOn(o); return o;}

/****************************************************************
*								*
*			PROTECTED FUNCTIONS			*
*								*
****************************************************************/

Bool_t 
TRhoTime::assertDate(const TRhoDate& date)
{
    return date.Between(TRhoDate(refDate_jday), TRhoDate(maxDate_jday));
}   

/*
* Set self from the specified (local) Date, hour, minute, and second.
* Note: this algorithm will fail if DST correction is something other
* than an hour.
* It is complicated by the DST boundary problem: 
* 1) Times in the phantom zone between 2AM and 3AM when DST is invoked
*    are invalid.
* 2) Times in the hour after 1AM when DST ends, are redundant.
*    In this case, standard time should be chosen.
*/


/****************************************************************
*								*
*			PRIVATE FUNCTIONS			*
*								*
****************************************************************/

/* static */  unsigned long
TRhoTime::buildFrom(
                   const TRhoDate& date,
                   unsigned long h, unsigned long m, unsigned long s)
{
    if (!date.IsValid()) return 0;
    if (date < TRhoDate(refDate_jday-1)) return 0;
    
    unsigned long secs = SECONDS_IN_HOUR * h
        + SECONDS_IN_MIN  * m
        +                   s;
    
    if(date < TRhoDate(refDate_jday)) {
        secs -= SECONDS_IN_DAY; // has to be one day exactly
    }
    else {
        secs += SECONDS_IN_DAY  * (date - TRhoDate(refDate_jday));
    }
    return secs;
}

unsigned long
TRhoTime::currentTime()
{
/*
* Non-standard or non-working ANSI time() function.
* Use RWZone::system() time zone to convert system clock time to GMT:
    */
    time_t ltime;
    time(&ltime);
    struct tm t;
    t = *localtime(&ltime);
    
    // Construct the date.  The time struct returns int, so casts are used.
    TRhoDate today((unsigned)t.tm_mday, (unsigned)(t.tm_mon + 1),
	       1900u + (unsigned)t.tm_year);
    
    return buildFrom(today, (unsigned)t.tm_hour, (unsigned)t.tm_min,
        (unsigned)t.tm_sec);
}

Bool_t  operator==(const TRhoTime& t1, const TRhoTime& t2) { return t1.sec== t2.sec; }
Bool_t  operator< (const TRhoTime& t1, const TRhoTime& t2) { return t1.sec<  t2.sec; }    
Bool_t  operator<=(const TRhoTime& t1, const TRhoTime& t2) { return t1.sec<= t2.sec; }    
Bool_t  operator> (const TRhoTime& t1, const TRhoTime& t2) { return t1.sec>  t2.sec; } 
Bool_t  operator>=(const TRhoTime& t1, const TRhoTime& t2) { return t1.sec>= t2.sec; } 
Bool_t  operator!=(const TRhoTime& t1, const TRhoTime& t2) { return t1.sec!= t2.sec; }  
TRhoTime operator+(const TRhoTime& t, unsigned long s) { return TRhoTime(t.Seconds()+s); }
TRhoTime operator-(const TRhoTime& t, unsigned long s) { return TRhoTime(t.Seconds()-s); } 
TRhoTime operator+(unsigned long s, const TRhoTime& t) { return TRhoTime(t.Seconds()+s); }
TRhoTime::TRhoTime(const TRhoDate& date, unsigned long h, unsigned long m, unsigned long s) { sec = buildFrom(date, h, m, s); }

