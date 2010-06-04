//////////////////////////////////////////////////////////////////////////
//                                                                      //
// TRhoDate		    						//
//                                                                      //
// Date and time class				    			//
//                                                                      //
// Author: Marcel Kunze, RUB, Feb. 99					//
// Copyright (C) 1999-2001, Ruhr-University Bochum.			//
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "RhoConditions/TRhoDate.h"

ClassImp(TRhoDate)

#include <iostream>
using namespace std;

const unsigned char TRhoDate::daysInMonth[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
const unsigned long TRhoDate::firstDayOfEachMonth[12] = {0,31,59,90,120,151,181,212,243,273,304,334};

/***************************************************************************/

// 	constructors

/***************************************************************************/

/*
* Construct a TRhoDate with a given day of the year and a given year.  The
* base date for this computation is Dec. 31 of the previous year.  If
* year == 0, Construct a TRhoDate with Jan. 1, 1901 as the "day zero".
* i.e., TRhoDate(-1) = Dec. 31, 1900 and TRhoDate(1) = Jan. 2, 1901.
*/

TRhoDate::TRhoDate(unsigned long day, unsigned long year) {
    if (year) julnum = Jday(12, 31, year-1) + (Int_t)day;
    else      julnum = Jul1901              + (Int_t)day;
}

//   Construct a TRhoDate for the given day, month, and year.
TRhoDate::TRhoDate(unsigned long day, unsigned long month, unsigned long year)
{
    julnum = Jday(month, day, year);
}

/****************************************************************
*								*
*			Member functions 			*
*								*
****************************************************************/

// extract to struct tm components
void TRhoDate::Extract(struct tm* tmbuf) const
{
    if (!IsValid()) { clobber(tmbuf); return; }
    unsigned long m; unsigned long d; unsigned long y;
    mdy(m, d, y);
    tmbuf->tm_year = y;
    tmbuf->tm_mon  = m;
    tmbuf->tm_mday = d;
    tmbuf->tm_wday = WeekDay() % 7;
    tmbuf->tm_yday = firstDayOfEachMonth[m - 1] + (d - 1);
    if (LeapYear(y) && m > 2)
        ++tmbuf->tm_yday;
    tmbuf->tm_hour = tmbuf->tm_min = tmbuf->tm_sec = 0;
    tmbuf->tm_isdst = -1;
}

// Compare function:
int TRhoDate::CompareTo(
                   const TRhoDate* d) const
{
    return julnum == d->julnum ? 0 : (julnum > d->julnum ? 1 : -1);
}

unsigned long TRhoDate::Day() const
{
    //Cast is o.k. unless we institute a year of length > 32767 days
    return (unsigned)(julnum - Jday(12, 31, Year()-1 ));
}

// Returns the day of the month of this TRhoDate.
unsigned long  TRhoDate::DayOfMonth() const
{
    unsigned long m; unsigned long d; unsigned long y;
    mdy(m, d, y);
    return d;
}

// Return the number of the first day of a given month
// Return 0 if "month" is outside of the range 1 through 12, inclusive.
unsigned long TRhoDate::FirstDayOfMonth(unsigned long month) const
{
    if ( !assertIndexOfMonth(month) ) return 0;
    unsigned long firstDay = 1 + firstDayOfEachMonth[month-1];
    if (month > 2 && Leap()) firstDay++;
    return firstDay;
}

TRhoDate TRhoDate::Max(const TRhoDate& dt) const
{
    return dt.julnum > julnum ? dt : *this;
}

TRhoDate TRhoDate::Min(const TRhoDate& dt) const 
{
    return dt.julnum < julnum ? dt : *this;
}

// Returns the month of this TRhoDate.
unsigned long TRhoDate::Month() const
{
    unsigned long m; unsigned long d; unsigned long y;
    mdy(m, d, y);
    return m;
}

TRhoDate TRhoDate::Next(unsigned long desiredDayOfWeek) const
{
    unsigned long delta = (desiredDayOfWeek + 6 - WeekDay()) % 7 + 1;
    return TRhoDate(julnum + delta);
}

TRhoDate TRhoDate::Previous(unsigned long desiredDayOfWeek) const
{
    unsigned long delta = (WeekDay() + 6 - desiredDayOfWeek) % 7 + 1;
    return TRhoDate(julnum - delta);
}

unsigned long TRhoDate::WeekDay() const
{
    return (unsigned)(julnum % 7 + 1);
}

// Returns the year of this TRhoDate.
unsigned long TRhoDate::Year() const
{
    unsigned long m; unsigned long d; unsigned long y;
    mdy(m, d, y);
    return y;
}

/***************************************************************************/

// 			static member functions

/***************************************************************************/

// Is a day (1-31) within a given month?
Bool_t TRhoDate::DayWithinMonth(unsigned long month, unsigned long day, unsigned long year)
{
    if (day <= 0 || !assertIndexOfMonth(month) ) return kFALSE;
    unsigned long d = daysInMonth[month-1];
    if (LeapYear(year) && month == 2) d++;
    return day <= d;
}

// How many days in a given month?
unsigned long TRhoDate::DaysInMonthYear(unsigned long month, unsigned long year)
{
    if (!assertIndexOfMonth(month) ) return 0;
    unsigned long d = daysInMonth[month-1];
    if (LeapYear(year) && month == 2) d++;
    return d;
}

// How many days are in the given year?
unsigned long TRhoDate::DaysInYear(unsigned long year)
{
    return LeapYear(year) ? 366 : 365;
}

/*
* Convert Gregorian calendar date to the corresponding Julian day
* number j.  Algorithm 199 from Communications of the ACM, Volume 6, No.
* 8, (Aug. 1963), p. 444.  Gregorian calendar started on Sep. 14, 1752.
* This function not valid before that.
* Returns 0 if the date is invalid.
*/

Int_t TRhoDate::Jday(unsigned long m, unsigned long d, unsigned long y)
{
    if(y < 100 ) y += 1900; // y is unsigned so always >= 0

    unsigned yh = y;
    Int_t c, ya, j;
    if( !DayWithinMonth(m, d, y) ) return (Int_t)0;
    
    if (m > 2) { m -= 3; }  // wash out the leap day
    else       { m += 9;	y--; } 
    c = y / 100;
    ya = y - 100*c;
    j = ((146097*c)>>2) + ((1461*ya)>>2) + (153*m + 2)/5 + d + 1721119;
    if (TRhoDate(j).Year() != yh) return 0;
    return j;
} 

// Return today's date:
TRhoDate TRhoDate::Now()
{
    return TRhoDate(TRhoDate::currentDate());
}

void TRhoDate::PrintOn(std::ostream& o) const
{
    struct tm tmbuf;
    Extract(&tmbuf);
    o << tmbuf.tm_mon;
    o << "/";
    o << tmbuf.tm_mday;
    o << "/";
    o << tmbuf.tm_year;
    o << " ";
}


// --------------------
// non-member functions
// --------------------

std::ostream&  operator << (std::ostream& o, const TRhoDate& a) {a.PrintOn(o); return o;}

/***********************************************************************
*                                                                     *
*                      Private functions                              *
*                                                                     *
***********************************************************************/
/*
* Convert a Julian day number to its corresponding Gregorian calendar
* date.  Algorithm 199 from Communications of the ACM, Volume 6, No. 8,
* (Aug. 1963), p. 444.  Gregorian calendar started on Sep. 14, 1752.
* This function not valid before that.  
*/

void TRhoDate::mdy(unsigned long& m, unsigned long& D, unsigned long& y) const
{
    Int_t d;
    Int_t j = julnum - 1721119;
    y = (unsigned long) (((j<<2) - 1) / 146097);
    j = (j<<2) - 1 - 146097*y;
    d = (j>>2);
    j = ((d<<2) + 3) / 1461;
    d = (d<<2) + 3 - 1461*j;
    d = (d + 4)>>2;
    m = (unsigned long)(5*d - 3)/153;
    d = 5*d - 3 - 153*m;
    D = (unsigned long)((d + 5)/5);
    y = (unsigned long)(100*y + j);
    if (m < 10)
        m += 3;
    else {
        m -= 9;
        y++;
    } 
} 

Int_t TRhoDate::currentDate()
{
    time_t clk = time(0);
    struct tm t;
    t = *localtime(&clk);
    return Jday(t.tm_mon+1, t.tm_mday, t.tm_year+1900);
}

// Algorithm from K & R, "The C Programming Language", 1st ed.
Bool_t TRhoDate::LeapYear(unsigned long year)
{
    return (year&3) == 0 && (year % 100 != 0 || year % 400 == 0);
    
    /* (year&3) is equivalent to (year % 4) */
}

Bool_t TRhoDate::Leap() const { return LeapYear(Year());} 

void TRhoDate::clobber(struct tm* tmbuf)
{
    tmbuf->tm_year = tmbuf->tm_mon = tmbuf->tm_mday = tmbuf->tm_wday =
        tmbuf->tm_yday = tmbuf->tm_hour = tmbuf->tm_min = tmbuf->tm_sec =
        tmbuf->tm_isdst = ~(~0u>>1);
}
